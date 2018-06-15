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
#include "chainparams.h"

//godcoin:superblock
static const uint256 SUPER_BLOCK_HASH1 = uint256S("0fd7ab36246daf61802e10a0316cc2b0f17a1d6404bfdd53878ba34e158b4680");
static const uint256 SUPER_BLOCK_HASH2 = uint256S("1bfe6e1dc86f504a0a059f624d97e53772a767ea950b43d57665a37383b16536");

static const uint256 TESTNET_SUPER_BLOCK_HASH1 = uint256S("35724d3b0d72504bde551381f6751342e5a693a129a92a0bde9db87d901bea03");
static const uint256 TESTNET_SUPER_BLOCK_HASH2 = uint256S("3c1526d933b9f0b483cf209751b7f9ac3ed75ae7ddd02c3ffca971dfb6ef5088");
static const uint256 TESTNET_SUPER_BLOCK_HASH3 = uint256S("7c54f3e13c9e8543ff0230d9cad6d13795f0cfde898c782a8151417625227dd5");

bool isSuperBlock(const CBlock& block) {
    return isSuperBlockHash(block.GetHash());
}

bool isSuperBlockHash(const uint256 hash) {
    if (Params().NetworkIDString().compare(CBaseChainParams::TESTNET) == 0){
        return hash ==  TESTNET_SUPER_BLOCK_HASH1 || hash ==  TESTNET_SUPER_BLOCK_HASH2 || hash ==  TESTNET_SUPER_BLOCK_HASH3;
    }else if (Params().NetworkIDString().compare(CBaseChainParams::MAIN) == 0){
         return hash ==  SUPER_BLOCK_HASH1 || hash ==  SUPER_BLOCK_HASH2;
    }else{
        //when REGTEST have not superblock
        return false;
    }
    
}

//godcoin:superblock
static const char* superBlocks[] = {"00000020553669484d6f06633dbde60c04c093f775d627ba6b7412000000000000000000918713e96a16f6bf229d7cf15b3406f76e6b75d5275c8bfd0722198258c74ac0cbacc15affff7f20000000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1103e9a507000b2f503253482f627463642fffffffff0100801e086b4701001976a9149e2ee6d80fcb41bef7da5cbbe142bc52925a182b88ac00000000", 
    "0000002080468b154ea38b8753ddbf04641d7af1b0c26c31a0102e8061af6d2436abd70f60454c8955969db0b86fe45044f0808973fcc6d9295fe039b42c804889092c8ecbacc15affff7f20000000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff1103eaa507000b2f503253482f627463642fffffffff0100e87648170000001976a9142e12023a49c3d81a98a1d8b4f6947681f5bf1e5688ac00000000"};

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
            (pindexLast->nHeight + 1) >= Params().GetConsensus().nSuperBlockHeight && 
            (pindexLast->nHeight + 1) < Params().GetConsensus().nLastPOWBlock) { 
        nProofOfWorkLimit =  UintToArith256(uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff")).GetCompact();
        return true;
    }
    return false;
}
