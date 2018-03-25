// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "pos/poswallet.h"

#include "base58.h"
#include "checkpoints.h"
#include "chain.h"
//#include "wallet/coincontrol.h"
#include "consensus/consensus.h"
#include "consensus/validation.h"
#include "key.h"
#include "keystore.h"
#include "validation.h"
#include "net.h"
#include "policy/policy.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "script/sign.h"
#include "timedata.h"
#include "txmempool.h"
#include "util.h"
#include "ui_interface.h"
#include "utilmoneystr.h"
#include "wallet/coincontrol.h"
#include "policy/rbf.h"
#include "scheduler.h"
#include "policy/fees.h"
#include "fs.h"
#include "init.h"
#include "pos/pos.h"
#include "script/interpreter.h"
#include "wallet/wallet.h"

#include <assert.h>

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>
#include <miner.h>

typedef std::vector<unsigned char> valtype;

using namespace std;
CAmount nReserveBalance = 0;
unsigned int GetStakeMaxCombineInputs() { return STAKE_MAX_INPUT; }

int64_t GetStakeCombineThreshold() { return 100 * COIN; }

unsigned int GetStakeSplitOutputs() { return 2; }

/**check whether wallet have utxo **/
bool HaveAvailableCoinsForStaking(CWallet *pwallet){
    std::vector<COutput> vCoins;
    AvailableCoinsForStaking(pwallet,vCoins);
    return vCoins.size() > 0;
}

/**Get a list of UTXO form wallet**/
void AvailableCoinsForStaking(CWallet *pwallet,std::vector<COutput>& vCoins){
    vCoins.clear();
    {
        LOCK2(cs_main, pwallet->cs_wallet);
        //lfmodify
        for (std::map<uint256, CWalletTx>::const_iterator it = pwallet->mapWallet.begin(); it != pwallet->mapWallet.end(); ++it){
            const uint256& wtxid = it->first;
            const CWalletTx* pcoin = &(*it).second;
            int nDepth = pcoin->GetDepthInMainChain();

            //this utxo is low depth
            if (nDepth < COINBASE_MATURITY)
                continue;

            //check depth again
            if (pcoin->GetBlocksToMaturity() > 0)
                continue;

            //tranverse coin vout
            for (unsigned int i = 0; i < pcoin->tx->vout.size(); i++) {
                // We should not consider coins which aren't at least in our mempool
                // It's possible for these to be conflicted via ancestors which we may never be able to detect
                if (nDepth == 0 && !pcoin->InMempool())
                    continue;

                //should be mine
                isminetype mine = pwallet->IsMine(pcoin->tx->vout[i]);
                //lfmodify
                if (!(pwallet->IsSpent(wtxid, i)) && mine != ISMINE_NO &&
                    !pwallet->IsLockedCoin(it->first, i) && (pcoin->tx->vout[i].nValue > 0)){
                        vCoins.push_back(COutput(pcoin, i, nDepth,
                                                 ((mine & ISMINE_SPENDABLE) != ISMINE_NO) ||
                                                 (mine & ISMINE_WATCH_SOLVABLE) != ISMINE_NO,
                                                 (mine & (ISMINE_SPENDABLE | ISMINE_WATCH_SOLVABLE)) != ISMINE_NO,true));
                }
            }
        }
    }
}

/**Get a list of UTXO form wallet**/
bool SelectCoinsForStaking(CWallet *pwallet,CAmount& nTargetValue, std::set<std::pair<const CWalletTx*,unsigned int> >& setCoinsRet, CAmount& nValueRet){
    std::vector<COutput> vCoins;
    //get available coins
    AvailableCoinsForStaking(pwallet,vCoins);
    setCoinsRet.clear();
    nValueRet = 0;

    //find money to meet target
    for(const COutput& output: vCoins){
        const CWalletTx *pcoin = output.tx;
        //get vout index
        int i = output.i;
        // Stop if we've chosen enough inputs
        if (nValueRet >= nTargetValue)
            break;

        int64_t n = pcoin->tx->vout[i].nValue;

        pair<int64_t,pair<const CWalletTx*,unsigned int> > coin = make_pair(n,make_pair(pcoin, i));

        if (n >= nTargetValue){
            // If input value is greater or equal to target then simply insert
            // it into the current subset and exit
            setCoinsRet.insert(coin.second);
            nValueRet += coin.first;
            break;
        } else if (n < nTargetValue + CENT){
            setCoinsRet.insert(coin.second);
            nValueRet += coin.first;
        }
    }
    return true;
}

/**construct a coinstake tx based on tx**/ //lfmodify Ckeystore
bool CreateCoinStake(CWallet *pwallet,unsigned int nBits, const CAmount& nTotalFees, uint32_t nTimeBlock, CMutableTransaction& tx, CKey& key){
    //godcoin:pos TODO chainActive
    CBlockIndex* pindexPrev = chainActive.Tip();
    arith_uint256 bnTargetPerCoinDay;
    bnTargetPerCoinDay.SetCompact(nBits);

    //int a new empty stake
    struct CMutableTransaction txNew(tx);
    txNew.vin.clear();
    txNew.vout.clear();

    // Mark coin stake transaction
    CScript scriptEmpty;
    scriptEmpty.clear();
    //first vout is empty
    txNew.vout.push_back(CTxOut(0, scriptEmpty));

    // Choose coins to use
    //get all balance
    CAmount nBalance = pwallet->GetBalance();//lfmodify
    //blance should be gt 0
    if (nBalance <= nReserveBalance)
        return false;

    std::vector<const CWalletTx*> vwtxPrev;

    std::set<pair<const CWalletTx*,unsigned int> > setCoins;
    CAmount nValueIn = 0;

    // Select coins with suitable depth
    //get target money
    CAmount nTargetValue = nBalance - nReserveBalance;
    //choose money for stake
    if (!SelectCoinsForStaking(pwallet,nTargetValue, setCoins, nValueIn)) 
        return false;
    

    if (setCoins.empty()) 
        return false;
    

    static std::map<COutPoint, CStakeCache> stakeCache;
    if(stakeCache.size() > setCoins.size() + 100){
        //Determining if the cache is still valid is harder than just clearing it when it gets too big, so instead just clear it
        //when it has more than 100 entries more than the actual setCoins.
        stakeCache.clear();
    }
    if(gArgs.GetBoolArg("-stakecache", DEFAULT_STAKE_CACHE)) {
        for(const std::pair<const CWalletTx *, unsigned int> &pcoin:setCoins){
            boost::this_thread::interruption_point();
            COutPoint prevoutStake = COutPoint(pcoin.first->GetHash(), pcoin.second);
            CacheKernel(stakeCache, prevoutStake, pindexPrev, *pcoinsTip); //this will do a 2 disk loads per op
        }
    }
    int64_t nCredit = 0;
    CScript scriptPubKeyKernel;
    //iterate eligible coin, construct the prevout(vin)
    for(const std::pair<const CWalletTx*, unsigned int>& pcoin:setCoins) {
        bool fKernelFound = false;
        boost::this_thread::interruption_point();
        // Search backward in time from the given txNew timestamp
        // Search nSearchInterval seconds back up to nMaxStakeSearchInterval
        //get prevout Tx
        COutPoint prevoutStake = COutPoint(pcoin.first->GetHash(), pcoin.second);
        //check kernel Hash
        if (CheckKernel(pindexPrev, nBits, nTimeBlock, prevoutStake, *pcoinsTip, stakeCache)){
            // Found a kernel
            std::vector<valtype> vSolutions;
            txnouttype whichType;
            CScript scriptPubKeyOut;
            //get output pk(owner pk)
            scriptPubKeyKernel = pcoin.first->tx->vout[pcoin.second].scriptPubKey;
            //analysis pubkey
            if (!Solver(scriptPubKeyKernel, whichType, vSolutions))
            {
                LogPrintf("coinstake", "CreateCoinStake : failed to parse kernel\n");
                break;
            }
            LogPrintf("coinstake", "CreateCoinStake : parsed kernel type=%d\n", whichType);
            //type should be p2pk,or p2pkh
            if (whichType != TX_PUBKEY && whichType != TX_PUBKEYHASH)
            {
                LogPrintf("CreateCoinStake : no support for kernel type=%d\n", whichType);
                break;  // only support pay to public key and pay to address
            }
            //convert p2pkh to p2pk, save it into scriptPubKeyOut
            if (whichType == TX_PUBKEYHASH) // pay to address type
            {
                // convert to pay to public key type
                if (!pwallet->GetKey(uint160(vSolutions[0]), key))//lfmodify
                {
                    LogPrintf("CreateCoinStake : failed to get key for kernel type=%d\n", whichType);
                    break;  // unable to find corresponding public key
                }
                scriptPubKeyOut << key.GetPubKey().getvch() << OP_CHECKSIG;
            }
            //if p2pk, directly save into scriptPubKeyOut
            if (whichType == TX_PUBKEY)
            {
                valtype& vchPubKey = vSolutions[0];
                if (!pwallet->GetKey(Hash160(vchPubKey), key))//lfmodify
                {
                    LogPrintf("CreateCoinStake : failed to get key for kernel type=%d\n", whichType);
                    break;  // unable to find corresponding public key
                }

                if (key.GetPubKey() != vchPubKey)
                {
                    LogPrintf("CreateCoinStake : invalid key for kernel type=%d\n", whichType);
                    break; // keys mismatch
                }

                scriptPubKeyOut = scriptPubKeyKernel;
            }

            //set vin
            txNew.vin.push_back(CTxIn(pcoin.first->GetHash(), pcoin.second));
            //get prveout amount
            nCredit += pcoin.first->tx->vout[pcoin.second].nValue;
            vwtxPrev.push_back(pcoin.first);
            //set vout
            txNew.vout.push_back(CTxOut(0, scriptPubKeyOut));

            LogPrintf("CreateCoinStake : added kernel type=%d\n", whichType);
            fKernelFound = true;
            break;
        }

        //success
        if (fKernelFound)
            break; // if kernel is found stop searching
    }

    //check money again
    if (nCredit == 0 || nCredit > nBalance - nReserveBalance) 
        return false;

    //put the remaint money into tx
    for(const std::pair<const CWalletTx*, unsigned int>& pcoin:setCoins){
        // Attempt to add more inputs
        // Only add coins of the same key/address as kernel
        if (txNew.vout.size() == 2 && ((pcoin.first->tx->vout[pcoin.second].scriptPubKey == scriptPubKeyKernel || pcoin.first->tx->vout[pcoin.second].scriptPubKey == txNew.vout[1].scriptPubKey))
                && pcoin.first->GetHash() != txNew.vin[0].prevout.hash) {
            // Stop adding more inputs if already too many inputs
            if (txNew.vin.size() >= GetStakeMaxCombineInputs())
                break;
            // Stop adding inputs if reached reserve limit
            if (nCredit + pcoin.first->tx->vout[pcoin.second].nValue > nBalance - nReserveBalance)
                break;
            // Do not add additional significant input
            if (pcoin.first->tx->vout[pcoin.second].nValue >= GetStakeCombineThreshold())
                continue;

            txNew.vin.push_back(CTxIn(pcoin.first->GetHash(), pcoin.second));
            nCredit += pcoin.first->tx->vout[pcoin.second].nValue;
            vwtxPrev.push_back(pcoin.first);
        }
    }

    const Consensus::Params& consensusParams = Params().GetConsensus();

    //get final money
    int64_t nReward = nTotalFees + GetBlockSubsidy(pindexPrev->nHeight + 1, consensusParams);
    if (nReward < 0) {
        return false;
    }

    nCredit += nReward;


    //put reward into vout[1]
    txNew.vout[1].nValue = nCredit;

    // Append the Refunds To Sender to the transaction outputs
    for(unsigned int i = 2; i < tx.vout.size(); i++)
    {
        txNew.vout.push_back(tx.vout[i]);
    }

    // Sign the input coins
    int nIn = 0;
    for(const CWalletTx* pcoin:vwtxPrev){
        //godcoin: pos-> 2way-protection required
        if (!SignSignature(*pwallet, *pcoin, txNew, nIn++, SIGHASH_ALL|SIGHASH_FORKID))
            return error("CreateCoinStake : failed to sign coinstake");
        ////
    }

    // Successfully generated coinstake
    tx = CTransaction(txNew);
     return true;
 }