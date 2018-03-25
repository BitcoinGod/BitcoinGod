// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POS_VALIDATION_H
#define BITCOIN_POS_VALIDATION_H

#include "chainparams.h"
#include "amount.h"
#include "coins.h"
#include "fs.h"
#include "protocol.h" // For CMessageHeader::MessageStartChars
#include "policy/feerate.h"
#include "script/script_error.h"
#include "sync.h"
#include "versionbits.h"

#include "consensus/consensus.h"
#include "wallet/wallet.h"

#include <algorithm>
#include <exception>
#include <map>
#include <set>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

#include <atomic>

bool SignBlock(std::shared_ptr<CBlock> pblock, CWallet *pwallet, const CAmount& nTotalFees, uint32_t nTime);
bool CheckFirstCoinstakeOutput(const CBlock& block);
bool CheckPosBlock(const CBlock& block, CValidationState& state);
bool AcceptPosBlock(const std::shared_ptr<const CBlock>& pblock, CValidationState& state, const CChainParams& chainparams, CBlockIndex** ppindex);
bool AcceptPosBlockHeader(const CBlockHeader& block, CValidationState& state, CBlockIndex* pindexPrev);
bool CheckBlockSignature(const CBlock& block);
bool GetPosBlockSubsidy(int nHeight, const Consensus::Params& consensusParams,CAmount& nSubsidy);
bool CheckBlockAmount(const CBlock& block,CBlockIndex* pindex,CValidationState& state,const CChainParams& chainparams,CAmount& blockReward,CAmount& coinstakeIn);
bool GetBlockPublicKey(const CBlock& block, std::vector<unsigned char>& vchPubKey);
bool CheckKernelAndUpdateHashProof(const CBlock& block, CValidationState& state, const Consensus::Params& consensusParams,CBlockIndex* pindex, CCoinsViewCache& view, uint256& hashProofOfStake);
inline int64_t FutureDrift(uint32_t nTime) { return nTime + 15; }

#endif // BITCOIN_POS_VALIDATION_H