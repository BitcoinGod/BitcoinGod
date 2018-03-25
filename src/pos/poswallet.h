#ifndef BITCOIN_POS_WALLET_H
#define BITCOIN_POS_WALLET_H

#include "amount.h"
#include "streams.h"
#include "policy/feerate.h"
#include "tinyformat.h"
#include "ui_interface.h"
#include "utilstrencodings.h"
#include "validationinterface.h"
#include "script/ismine.h"
#include "script/sign.h"
#include "wallet/crypter.h"
#include "wallet/walletdb.h"
#include "wallet/rpcwallet.h"
#include "consensus/params.h"
#include "wallet/wallet.h"
#include <algorithm>
#include <atomic>
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

static const unsigned int STAKE_MAX_INPUT = 5000000;
static const unsigned int STAKE_SPLIT_OUTPUT = 2;
static const bool DEFAULT_STAKE_CACHE = true;

unsigned int GetStakeMaxCombineInputs();
int64_t GetStakeCombineThreshold();
unsigned int GetStakeSplitOutputs();
bool HaveAvailableCoinsForStaking(CWallet *pwallet);
void AvailableCoinsForStaking(CWallet *pwallet,std::vector<COutput>& Coins);
bool SelectCoinsForStaking(CWallet *pwallet,CAmount& nTargetValue, std::set<std::pair<const CWalletTx*,unsigned int> >& setCoinsRet, CAmount& nValueRet);
bool CreateCoinStake(CWallet *pwallet, unsigned int nBits, const CAmount& nTotalFees, uint32_t nTimeBlock, CMutableTransaction& tx, CKey& key);

#endif // BITCOIN_POS_WALLET_H