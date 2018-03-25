// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pos/posvalidation.h"

#include "arith_uint256.h"
#include "chain.h"
#include "chainparams.h"
#include "checkpoints.h"
#include "checkqueue.h"
#include "consensus/consensus.h"
#include "consensus/merkle.h"
#include "consensus/tx_verify.h"
#include "consensus/validation.h"
#include "cuckoocache.h"
#include "fs.h"
#include "hash.h"
#include "init.h"
#include "policy/fees.h"
#include "policy/policy.h"
#include "policy/rbf.h"
#include "pow.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "random.h"
#include "reverse_iterator.h"
#include "script/script.h"
#include "script/sigcache.h"
#include "script/standard.h"
#include "timedata.h"
#include "tinyformat.h"
#include "txdb.h"
#include "txmempool.h"
#include "ui_interface.h"
#include "undo.h"
#include "util.h"
#include "utilmoneystr.h"
#include "utilstrencodings.h"
#include "validationinterface.h"
#include "versionbits.h"
#include "warnings.h"

#include "superblock.h"
#include "pos/pos.h"
#include "poswallet.h"
#include "script/interpreter.h"

#include <atomic>
#include <sstream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/thread.hpp>

#if defined(NDEBUG)
# error "Bitcoin cannot be compiled without assertions."
#endif

typedef std::vector<unsigned char> valtype;

bool CheckKernelAndUpdateHashProof(const CBlock& block, CValidationState& state, const Consensus::Params& consensusParams,
                                          CBlockIndex* pindex, CCoinsViewCache& view, uint256& hashProofOfStake){
    uint256 hash = block.GetHash();

    // Check coinstake timestamp
    if (block.IsProofOfStake() && !CheckCoinStakeTimestamp(block.GetBlockTime()))
        return state.DoS(50, error("UpdateHashProof() : coinstake timestamp violation nTimeBlock=%d", block.GetBlockTime()));

    // Check proof-of-work or proof-of-stake
    if(block.IsProofOfStake()){
        if (block.nBits != GetNextPosWorkRequired(pindex->pprev, &block, consensusParams))
            return state.DoS(100, false, REJECT_INVALID, "bad-diffbits", false, "incorrect proof of work");
    } else if (!isSuperBlock(block)) {
        /**
         * If block is GenesisBlock when bitcoind -reindex,GetNextWorkRequired will push a exception in Assert(pindexLast != nullptr).
         * Because,GenesisBlock haven't prevBlock。So,add check if the block is GenesisBlock
         * */
        if (hash != consensusParams.hashGenesisBlock && block.nBits != GetNextWorkRequired(pindex->pprev, &block, consensusParams))
            return state.DoS(100, false, REJECT_INVALID, "bad-diffbits", false, "incorrect proof of work");
    }

    // Verify hash target and signature of coinstake tx
    if (block.IsProofOfStake()){
        uint256 targetProofOfStake;
        if (!CheckProofOfStake(pindex->pprev, state, *block.vtx[1], block.nBits, block.nTime, hashProofOfStake, targetProofOfStake, view))
            return error("UpdateHashProof() : check proof-of-stake failed for block %s", hash.ToString());

        if(hashProofOfStake.IsNull()){
            return error("CheckProofOfStake get hash proof is null");
        }
    }

    return true;
}

/**compute and check pos block important parameter**/
static bool UpdateHashProof(const CBlock& block, CValidationState& state, const Consensus::Params& consensusParams, CBlockIndex* pindex, CCoinsViewCache& view){
    uint256 hash = block.GetHash();

    // Check coinstake timestamp
    if (block.IsProofOfStake() && !CheckCoinStakeTimestamp(block.GetBlockTime()))
        return state.DoS(50, error("UpdateHashProof() : coinstake timestamp violation nTimeBlock=%d", block.GetBlockTime()));

    // Check proof-of-work or proof-of-stake
    if(block.IsProofOfStake()){
        if (block.nBits != GetNextPosWorkRequired(pindex->pprev, &block, consensusParams))
            return state.DoS(100, false, REJECT_INVALID, "bad-diffbits", false, "incorrect proof of work");
    } else if (!isSuperBlock(block)) {
        /**
         * If block is GenesisBlock when bitcoind -reindex,GetNextWorkRequired will push a exception in Assert(pindexLast != nullptr).
         * Because,GenesisBlock haven't prevBlock。So,add check if the block is GenesisBlock
         * */
        if (hash != consensusParams.hashGenesisBlock && block.nBits != GetNextWorkRequired(pindex->pprev, &block, consensusParams))
            return state.DoS(100, false, REJECT_INVALID, "bad-diffbits", false, "incorrect proof of work");
    }

    // PoW is checked in CheckBlock()
    if (block.IsProofOfWork()){
        // Record proof hash value
        pindex->hashProof = block.GetHash();
    }

    return true;
}

// Compute at which vout of the block's coinbase transaction the witness
// commitment occurs, or -1 if not found.
//the function same as GetWitnessCommitmentIndex in validation.cpp
static int GetWitnessCommitmentIndexPos(const CBlock& block)
{
    int commitpos = -1;
    if (!block.vtx.empty()) {
        for (size_t o = 0; o < block.vtx[0]->vout.size(); o++) {
            if (block.vtx[0]->vout[o].scriptPubKey.size() >= 38 && block.vtx[0]->vout[o].scriptPubKey[0] == OP_RETURN && block.vtx[0]->vout[o].scriptPubKey[1] == 0x24 && block.vtx[0]->vout[o].scriptPubKey[2] == 0xaa && block.vtx[0]->vout[o].scriptPubKey[3] == 0x21 && block.vtx[0]->vout[o].scriptPubKey[4] == 0xa9 && block.vtx[0]->vout[o].scriptPubKey[5] == 0xed) {
                commitpos = o;
            }
        }
    }
    return commitpos;
}
//the function same as IsCompressedOrUncompressedPubKey in interpreter.cpp
bool static IsCompressedOrUncompressedPubKeyPos(const valtype &vchPubKey) {
    if (vchPubKey.size() < 33)
        return false;//  Non-canonical public key: too short
    
    if (vchPubKey[0] == 0x04) {
        if (vchPubKey.size() != 65) {
            //  Non-canonical public key: invalid length for uncompressed key
            return false;
        }
    } else if (vchPubKey[0] == 0x02 || vchPubKey[0] == 0x03) {
        if (vchPubKey.size() != 33) {
            //  Non-canonical public key: invalid length for compressed key
            return false;
        }
    } else {
        //  Non-canonical public key: neither compressed nor uncompressed
        return false;
    }
    return true;
}

/**Push coinstake and find a useful UTXO to push coinstake ，then make a useful pos block**/
bool SignBlock(std::shared_ptr<CBlock> pblock, CWallet *pwallet, const CAmount& nTotalFees, uint32_t nTime){
    if (!CheckFirstCoinstakeOutput(*pblock))
        return false;
    
    CKey key;
    CMutableTransaction txCoinStake(*pblock->vtx[1]);
    uint32_t nTimeBlock = nTime;
    nTimeBlock &= ~STAKE_TIMESTAMP_MASK;
    //original line:
    //IsProtocolV2 mean POS 2 or higher, so the modified line is:
    if (CreateCoinStake(pwallet, pblock->nBits, nTotalFees, nTimeBlock, txCoinStake, key)){
        if (nTimeBlock >= chainActive.Tip()->GetMedianTimePast()+1){
            // make sure coinstake would meet timestamp protocol
            //    as it would be the same as the block timestamp
            pblock->nTime = nTimeBlock;
            pblock->vtx[1] = MakeTransactionRef(std::move(txCoinStake));
            pblock->hashMerkleRoot = BlockMerkleRoot(*pblock);
            
            //compute stake modifider
            CBlockIndex* pindexPrev = chainActive.Tip();
            uint256 prevoutHash = pblock->vtx[1]->vin[0].prevout.hash;
            uint256 nStakeModifier = ComputeStakeModifier(pindexPrev, prevoutHash);
            pblock->nNonce = UintToArith256(nStakeModifier).GetCompact();
            // Check timestamp against prev
            if(pblock->GetBlockTime() <= chainActive.Tip()->GetBlockTime() || FutureDrift(pblock->GetBlockTime()) < chainActive.Tip()->GetBlockTime())
                return false;

            // append a signature to our block and ensure that is LowS
            return key.Sign(pblock->GetHash(), pblock->vchBlockSig) &&
                       EnsureLowS(pblock->vchBlockSig);
        }
    }

    return false;
}

bool CheckFirstCoinstakeOutput(const CBlock& block){
    // Coinbase output should be empty if proof-of-stake block
    int commitpos = GetWitnessCommitmentIndexPos(block);
    if(commitpos < 0){
        if (block.vtx[0]->vout.size() != 1 || !block.vtx[0]->vout[0].IsEmpty())
            return false;
    }
    else{
        if (block.vtx[0]->vout.size() != 2 || !block.vtx[0]->vout[0].IsEmpty() || block.vtx[0]->vout[1].nValue)
            return false;
    }

    return true;
}

/**check pos block simply**/
bool CheckPosBlock(const CBlock& block, CValidationState& state){
    //check pos time within 16s 
    if (block.GetBlockTime() > FutureDrift(GetAdjustedTime()))
        return error("CheckBlock() : block timestamp too far in the future");

    // Coinbase output should be empty if proof-of-stake block
    if (!CheckFirstCoinstakeOutput(block))
        return state.DoS(100, false, REJECT_INVALID, "bad-cb-missing", false, "coinbase output not empty for proof-of-stake block");

        // Second transaction must be coinstake
    if (block.vtx.empty() || block.vtx.size() < 2 || !block.vtx[1]->IsCoinStake())
        return state.DoS(100, false, REJECT_INVALID, "bad-cs-missing", false, "second tx is not coinstake");

        //the rest of the transactions must not be coinstake
    for (unsigned int i = 2; i < block.vtx.size(); i++) {
        if (block.vtx[i]->IsCoinStake())
            return state.DoS(100, false, REJECT_INVALID, "bad-cs-multiple", false, "more than one coinstake");
    }
    // Check proof-of-stake block signature
    if (!CheckBlockSignature(block))
        return state.DoS(100, false, REJECT_INVALID, "bad-blk-signature", false, "bad proof-of-stake block signature");   
    
    return true;
}
/**Accept POS block**/
bool AcceptPosBlock(const std::shared_ptr<const CBlock>& pblock, CValidationState& state, const CChainParams& chainparams, CBlockIndex** ppindex){
    const CBlock& block = *pblock;

    CBlockIndex *pindexDummy = nullptr;
    CBlockIndex *&pindex = ppindex ? *ppindex : pindexDummy;

    // Get prev block index
    CBlockIndex* pindexPrev = nullptr;
    if(pindex->nHeight > 0){
        BlockMap::iterator mi = mapBlockIndex.find(block.hashPrevBlock);
        if (mi == mapBlockIndex.end())
            return state.DoS(10, error("AcceptBlock() : prev block not found"));
        pindexPrev = (*mi).second;
    }

    //validate stakemodifier
    if(block.IsProofOfStake()){
        uint256 prevoutHash = block.vtx[1]->vin[0].prevout.hash;
        pindex->prevoutStake = block.vtx[1]->vin[0].prevout;
        uint256 nStakeModifier = ComputeStakeModifier(pindexPrev, prevoutHash);
        if(pindex->nNonce != UintToArith256(nStakeModifier).GetCompact()){
            return error("AcceptBlock() : stake modifider doesn't match");
        }
     }

    if (!UpdateHashProof(block, state, chainparams.GetConsensus(), pindex, *pcoinsTip))
        return error("%s: AcceptBlock(): %s", __func__, state.GetRejectReason().c_str());
    
    // Check timestamp against prev
    if (pindexPrev && block.IsProofOfStake() && (block.GetBlockTime() <= pindexPrev->GetBlockTime() || FutureDrift(block.GetBlockTime()) < pindexPrev->GetBlockTime()))
        return error("AcceptBlock() : block's timestamp is too early");

    // Check timestamp
    if (block.IsProofOfStake() &&  block.GetBlockTime() > FutureDrift(GetAdjustedTime()))
        return error("AcceptBlock() : block timestamp too far in the future");

    return true;
}
/**Accept POS blockheader**/
bool AcceptPosBlockHeader(const CBlockHeader& block, CValidationState& state, CBlockIndex* pindexPrev){
    if((pindexPrev->nHeight >= LAST_POW_BLOCK_HEIGHT)&&block.nNonce == 0)
        return state.DoS(10, error("pos block nNonce is zero, hash:%s, prevHeight:%d, prevHash:%s",block.GetHash().GetHex(),pindexPrev->nHeight, pindexPrev->GetBlockHash().GetHex()),REJECT_INVALID, "bad-pos-block-nNonce");
    
    return true;
}
/**Check POS sign**/
bool CheckBlockSignature(const CBlock& block){
    if (block.IsProofOfWork())
        return block.vchBlockSig.empty();

    std::vector<unsigned char> vchPubKey;
    if(!GetBlockPublicKey(block, vchPubKey))
        return false;
    
    return CPubKey(vchPubKey).Verify(block.GetHash(), block.vchBlockSig);
}
/**Check POS block subsidy**/
bool GetPosBlockSubsidy(int nHeight, const Consensus::Params& consensusParams,CAmount& nSubsidy){
    if(nHeight > consensusParams.nLastPOWBlock){
        //average policy
        int posHalvings
                = (nHeight - consensusParams.nLastPOWBlock - 1) / consensusParams.nPosSubsidyHalvingInterval;
        // Force block reward to zero when right shift is undefined.
        if (posHalvings >= 10)
            return 0;

        nSubsidy = 0.19 * COIN;
        return true;
    }
    return false;
}

/**Check block amount**/
bool CheckBlockAmount(const CBlock& block,CBlockIndex* pindex,CValidationState& state,const CChainParams& chainparams,CAmount& blockReward,CAmount& coinstakeIn){
    if(block.IsProofOfWork()){
        if(pindex->nHeight > chainparams.GetConsensus().nLastPOWBlock){
            return state.DoS(100,
                             error("ConnectBlock(): The expeted height should be pos block (height=%d vs lastpowblock=%d)",
                                   pindex->nHeight, chainparams.GetConsensus().nLastPOWBlock),
                                   REJECT_INVALID, "bad-pos-block");
        }
    } else {
        if (coinstakeIn + blockReward < block.vtx[1]->GetValueOut())
            return state.DoS(100,
                             error("ConnectBlock(): coinstake pays too much (actual=%d vs vin=%d,fee=%d)",
                                   block.vtx[1]->GetValueOut(), coinstakeIn, blockReward ),
                                   REJECT_INVALID, "bad-cb-amount");
    }
    return true;
}

/**Getblock**/
bool GetBlockPublicKey(const CBlock& block, std::vector<unsigned char>& vchPubKey){
    if (block.IsProofOfWork())
        return false;

    if (block.vchBlockSig.empty())
        return false;

    std::vector<valtype> vSolutions;
    txnouttype whichType;

    const CTxOut& txout = block.vtx[1]->vout[1];

    if (!Solver(txout.scriptPubKey, whichType, vSolutions))
        return false;

    if (whichType == TX_PUBKEY) {
        vchPubKey = vSolutions[0];
        return true;
    }else {
        // Block signing key also can be encoded in the nonspendable output
        // This allows to not pollute UTXO set with useless outputs e.g. in case of multisig staking

        const CScript& script = txout.scriptPubKey;
        CScript::const_iterator pc = script.begin();
        opcodetype opcode;
        valtype vchPushValue;

        if (!script.GetOp(pc, opcode, vchPubKey))
            return false;
        if (opcode != OP_RETURN)
            return false;
        if (!script.GetOp(pc, opcode, vchPubKey))
            return false;
        if (!IsCompressedOrUncompressedPubKeyPos(vchPubKey))
            return false;
        return true;
    }

    return false;
}

