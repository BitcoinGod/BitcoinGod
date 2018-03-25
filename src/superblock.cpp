// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <iostream>

#include "superblock.h"
#include "streams.h"
#include "utilstrencodings.h"
#include "version.h"
#include "chain.h"
#include "consensus/consensus.h"

//godcoin:superblock
//static const uint256 SUPER_BLOCK_HASH1 = uint256S("1d7119dc891c5cf48f1be3b9d484690d8ad04c5798a6936d64196702e6ff883e");
static const uint256 SUPER_BLOCK_HASH1 = uint256S("1bae65c725942294a7df7476e47f7d7fb3c78a2665916f760c3f027a6fc4593d");
static const uint256 SUPER_BLOCK_HASH2 = uint256S("c6da539f99872b645b70e691849318b23092d56f47ce10c00f63ab61a4d4d8a1");
//static const uint256 SUPER_BLOCK_HASH2 = uint256S("b9f47c8fbea6efe96b4d65828cd2c93754f26366199638a7bfad63f61234b6d0");
static const uint256 SUPER_BLOCK_HASH3 = uint256S("4c50599de0a18b69a84ab1339153c6afac15e733dc197c6bbba071b864d6a7f7");

bool isSuperBlock(const CBlock& block) {
    return isSuperBlockHash(block.GetHash());
}

bool isSuperBlockHash(const uint256 hash) {
    return hash ==  SUPER_BLOCK_HASH1||
        hash ==  SUPER_BLOCK_HASH2 ||
        hash ==  SUPER_BLOCK_HASH3;
}

//godcoin:superblock
static const char* superBlocks[] = {"00000020d7901582a10f1d35f102eb138f21adaf82055c4c8ccd06000000000000000000e08fe1dfacf4a4dce56913a3d490cc00da07b3cd3962882452beeddaccdead654e225f5affff7f20000000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1103c89c07000b2f503253482f627463642fffffffff010040e59c301200001976a9149e2ee6d80fcb41bef7da5cbbe142bc52925a182b88ac00000000", 
    "000000203e88ffe6026719646d93a698574cd08a0d6984d4b9e31b8ff45c1c89dc19711d0714805d9bc6d4d59a75a8d054cb8f7d31e002b917abd68301f3bd2f45d795b14e225f5affff7f20000000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1103c99c07000b2f503253482f627463642fffffffff010088526a740000001976a9142e12023a49c3d81a98a1d8b4f6947681f5bf1e5688ac00000000",
    "00000020d0b63412f663adbfa73896196663f25437c9d28c82654d6be9efa6be8f7cf4b9fa08020834314223ceb6a6e2398a8f0db28e15575efc03c0ba09d93efbc63cb64e225f5affff7f20000000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1103ca9c07000b2f503253482f627463642fffffffff010088526a740000001976a91435591e81072a8f2fde1617259dab02e10cc807cb88ac00000000"};

//godcoin:superblock
std::shared_ptr<CBlock> getSuperBlock(int i) {
    std::shared_ptr<CBlock> pblock = std::make_shared<CBlock>();
    CDataStream stream(ParseHex(superBlocks[i]),SER_NETWORK, PROTOCOL_VERSION);
    pblock = std::make_shared<CBlock>();
    stream >> *pblock;
    return pblock;
}

bool getSuperBlockWorkLimit(const CBlockIndex* pindexLast,unsigned int& nProofOfWorkLimit) {
    if (pindexLast != nullptr && 
            (pindexLast->nHeight + 1) >= SUPER_BLOCK_HEIGHT && 
            (pindexLast->nHeight + 1) < LAST_POW_BLOCK_HEIGHT) { 
        nProofOfWorkLimit =  UintToArith256(uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")).GetCompact();
        return true;
    }
    return false;
}
