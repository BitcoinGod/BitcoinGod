// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_MNEMONIC_H
#define BITCOIN_MNEMONIC_H

#include <vector>
#include <string> 

#define SEED_KEY_SIZE 64
#define PBKDF2_ROUNDS 2048
using namespace std;

class Mnemonic {
    private:
        vector<string> data;
    public:
        void setMnemonic(string &mnemoics);

        vector<string> getMnemonic(){return data;};
        vector<unsigned char> toVector();
        unsigned char *MnemonicToSeed();

        Mnemonic(){
        };

        Mnemonic(string mnemoics){
            setMnemonic(mnemoics);
        };

        ~Mnemonic(){

        }
        
};

void SplitStringToVector(const string& source, vector<string>& v, const string& c);  



#endif // BITCOIN_MNEMONIC_H
