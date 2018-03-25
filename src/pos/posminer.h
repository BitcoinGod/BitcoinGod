// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POS_MINER_H
#define BITCOIN_POS_MINER_H

#include <univalue.h>
#include "wallet/wallet.h"
#include "primitives/block.h"

static const int POS_MINING_TIMES=100;
//Will not attempt to add more transactions when GetAdjustedTime() >= nTimeLimit
//And nTimeLimit = StakeExpirationTime - STAKE_TIME_BUFFER
static const int32_t STAKE_TIME_BUFFER = 2;

UniValue minePosBlock(CWallet *pwallet);
bool CheckStake(const std::shared_ptr<const CBlock> pblock, CWallet& wallet);

#endif // BITCOIN_POS_MINER_H
