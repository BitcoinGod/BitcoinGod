// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pos/posminer.h"
#include "amount.h"
#include "chain.h"
#include "chainparams.h"
#include "coins.h"
#include "consensus/consensus.h"
#include "consensus/tx_verify.h"
#include "consensus/merkle.h"
#include "consensus/validation.h"
#include "hash.h"
#include "validation.h"
#include "primitives/transaction.h"
#include "script/standard.h"
#include "timedata.h"
#include "txmempool.h"
#include "util.h"
#include "utilmoneystr.h"
#include "validationinterface.h"
#include "pos/pos.h"
#include "miner.h"
#include "wallet/wallet.h"
#include "pos/poswallet.h"
#include "posvalidation.h"
#include "pos/posvalidation.h"

#include <algorithm>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include <utility>

int64_t nLastCoinStakeSearchInterval = 0;

static bool isMining = false;
//gocoin:pos
UniValue minePosBlock(CWallet *pwallet) {

    if (pwallet->IsLocked())
        return NullUniValue;
    
    UniValue blockHashes(UniValue::VOBJ);
    CReserveKey reservekey(pwallet);
    //godcoin:pos
    if(!HaveAvailableCoinsForStaking(pwallet)) {
        LogPrintf("wallet doesn't have available coins for staking\n");
        return blockHashes;
    }

    int64_t nTotalFees = 0;
    std::unique_ptr<CBlockTemplate> pblocktemplate(BlockAssembler(Params())
            .CreateEmptyBlock(reservekey.reserveScript, &nTotalFees));
    if (!pblocktemplate.get())
        return NullUniValue;

    CBlockIndex* pindexPrev =  chainActive.Tip();
    uint32_t beginningTime=GetAdjustedTime();
    beginningTime &= ~STAKE_TIMESTAMP_MASK;

    for(uint32_t i=beginningTime;i<beginningTime + POS_MINING_TIMES; i+=STAKE_TIMESTAMP_MASK+1) {
        static int64_t nLastCoinStakeSearchTime = GetAdjustedTime(); // startup timestamp
        nLastCoinStakeSearchInterval = i - nLastCoinStakeSearchTime;

        pblocktemplate->block.nTime = i;
        std::shared_ptr<CBlock> pblock = std::make_shared<CBlock>(pblocktemplate->block);

        //godcoin:pos
        if (!SignBlock(pblock, pwallet, nTotalFees, i)) {
            continue;
        }

        if (chainActive.Tip()->GetBlockHash() != pblock->hashPrevBlock) {
            LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
            break;
        }

        std::unique_ptr<CBlockTemplate> pblocktemplatefilled(
                BlockAssembler(Params()).CreateNewPosBlock(pblock->vtx[1]->vout[1].scriptPubKey, true, &nTotalFees,
                    i, FutureDrift(GetAdjustedTime()) - STAKE_TIME_BUFFER));
        if (!pblocktemplatefilled.get()){
            return NullUniValue;
        }

        if (chainActive.Tip()->GetBlockHash() != pblock->hashPrevBlock) {
            //another block was received while building ours, scrap progress
            LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
            break;
        }

        std::shared_ptr<CBlock> pblockfilled = std::make_shared<CBlock>(pblocktemplatefilled->block);
        if (!SignBlock(pblockfilled, pwallet, nTotalFees, i)) {
            break;
        }

        bool validBlock = false;
        while(!validBlock) {
            if (chainActive.Tip()->GetBlockHash() != pblockfilled->hashPrevBlock) {
                LogPrintf("minePosBlock: Valid future PoS block was orphaned before becoming valid");
                break;
            }

            if (pblockfilled->GetBlockTime() <= pindexPrev->GetBlockTime() ||
                    FutureDrift(pblockfilled->GetBlockTime()) < pindexPrev->GetBlockTime()) {
                LogPrintf("minePosBlock: Valid PoS block took too long to create and has expired");
                break; 
            }
            //godcoin:pos
            if (pblockfilled->GetBlockTime() > FutureDrift(GetAdjustedTime())) {
                MilliSleep(3000);
                continue;
            }
            validBlock = true;
        }

        if(validBlock) {
            CheckStake(pblockfilled, *pwallet);
            nLastCoinStakeSearchTime = pblockfilled->GetBlockTime();
            blockHashes.push_back(Pair("blockhash",pblockfilled->GetHash().GetHex()));
            break;
        }
    }

    return blockHashes;
}

/**check generate posblock by myself**/
bool CheckStake(const std::shared_ptr<const CBlock> pblock, CWallet& wallet){
    uint256 proofHash, hashTarget;
    uint256 hashBlock = pblock->GetHash();

    // verify hash target and signature of coinstake tx
    CValidationState state;
    //godcoin:pos
    if (!CheckProofOfStake(mapBlockIndex[pblock->hashPrevBlock], state, *pblock->vtx[1], pblock->nBits, pblock->nTime, proofHash, hashTarget, *pcoinsTip))
        return error("CheckStake() : proof-of-stake checking failed");

    //// debug print
    LogPrintf("CheckStake() : new proof-of-stake block found  \n  hash: %s \nproofhash: %s  \ntarget: %s\n", hashBlock.GetHex(), proofHash.GetHex(), hashTarget.GetHex());
    LogPrintf("CheckStake(), block:%s\n", pblock->ToString());
    LogPrintf("CheckStake(), stake money out %s\n", FormatMoney(pblock->vtx[1]->GetValueOut()));

    // Found a solution
    {
        LOCK(cs_main);
        //check block is the latest one
        if (pblock->hashPrevBlock != chainActive.Tip()->GetBlockHash())
            return error("CheckStake() : generated block is stale");

        //check vin
        for(const CTxIn& vin : pblock->vtx[1]->vin) {
            if (wallet.IsSpent(vin.prevout.hash, vin.prevout.n)) {
                return error("CheckStake() : generated block became invalid due to stake UTXO being spent");
            }
        }

        // Process this block the same as if we had received it from another node
        bool fNewBlock = false;
        //
        if (!ProcessNewBlock(Params(), pblock, true, &fNewBlock))
            return error("CheckStake() : ProcessBlock, block not accepted");
    }

    return true;
}
