// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"

uint256 CBlockHeader::GetHash() const
{
    return SerializeHash(*this);
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}

//godcoin:pos
bool CBlock::IsProofOfStake() const{
    //coinbase needed
    if(this->vtx.empty() || !this->vtx[0]->IsCoinBase()){
        return false;
    }

    //coinbase is emtpy
    if (this->vtx[0]->vout[0].nValue != 0)
        return false;

    // Second transaction must be coinstake
    if (this->vtx.size() < 2 )
       return false;

    //coinstake vin shouldn't be empty
    //first vout is empty
    if(this->vtx[1]->vin.size()>0
            && (!this->vtx[1]->vin[0].prevout.IsNull())
            && this->vtx[1]->vout.size() >= 2
            && this->vtx[1]->vout[0].IsEmpty()){
        return true;
    }

    return false;
}
