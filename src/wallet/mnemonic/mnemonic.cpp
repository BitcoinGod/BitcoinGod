// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "mnemonic.h"

#include <iostream>
#include <cstring>

#include <openssl/ecdsa.h>
#include <openssl/rand.h>
#include <openssl/obj_mac.h>
#include <openssl/evp.h>
#include "util.h"

void Mnemonic::setMnemonic(string &mnemoics){
    SplitStringToVector(mnemoics,data,"-");

    //check mnemonic size
    if((data.size() % 3) != 0 || (data.size() / 3) < 4 || (data.size() / 3) > 8)
        data.clear();
}

unsigned char *Mnemonic::MnemonicToSeed(){
    unsigned char *out;
    const char *mnemoichar;
    string mnemoics;
    unsigned char salt_value[] = {'m','n','e','m','o','n','i','c'};

    for(string str : data){
        mnemoics.append(str);
        mnemoics.append(" ");
    }

    mnemoics.erase((mnemoics.length()-1),1);
    mnemoichar = mnemoics.c_str();

    out = (unsigned char *) malloc(sizeof(unsigned char) * SEED_KEY_SIZE);
    
    LogPrintf("\n");
    if(PKCS5_PBKDF2_HMAC(mnemoichar, mnemoics.length(), salt_value, sizeof(salt_value), PBKDF2_ROUNDS,EVP_sha512() ,SEED_KEY_SIZE, out) == 0 )
    {
        LogPrintf("PKCS5_PBKDF2_HMAC_SHA1 failed\n");
        free(out);
        return nullptr;
    }
    
    return out;
}

vector<unsigned char> Mnemonic::toVector(){
    vector<unsigned char> seedVector;
    unsigned char *out = MnemonicToSeed();
    for(int i = 0; i < SEED_KEY_SIZE; i++){
        seedVector.push_back(out[i]);
    }
    return seedVector;
}

void SplitStringToVector(const string& s, vector<string>& v, const string& c)  
{  
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        string str = s.substr(pos1, pos2-pos1);
        if(!str.empty())
            v.push_back(s.substr(pos1, pos2-pos1));
         
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));    
}

