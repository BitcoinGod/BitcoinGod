<h2>Bitcoin God API call list </h2>

Note: up-to-date API reference .

<h3>== Common operations ==</h3>

<h4>=== Listing my bitcoin addresses ===</h4>

Listing the Bitcoin God address|addresses in your wallet is easily done via ''listreceivedbyaddress''. It normally lists only addresses which already have received transactions, however you can list all the addresses by setting the first argument to 0, and the second one to true.

accounts explained|Accounts are used to organize addresses.

<h3>== Full list ==</h3>

Required arguments are denoted inside  and  Optional arguments are inside [ and ].

<table class="wikitable" style="width: 80%">
<tbody><tr>
<th> Command </th>
<th> Parameters </th>
<th> Description </th>
<th> Requires unlocked wallet? 
</th></tr>
<tr>
<td width="25%"> addmultisigaddress </td>
<td> nrequired '["key","key"]' [account] </td>
<td> Add a nrequired-to-sign multisignature address to the wallet. Each key is a bitcoin address or hex-encoded public key. If [account] is specified, assign address to [account]. Returns a string containing the address. </td>
<td> N
</td></tr>
<tr>
<td> addnode </td>
<td> node add/remove/onetry </td>
<td>  Attempts add or remove node from the addnode list or try a connection to node once. </td>
<td> N
</td></tr>
<tr>
<td> backupwallet </td>
<td> destination </td>
<td> Safely copies wallet.dat to destination, which can be a directory or a path with filename. </td>
<td> N
</td></tr>
<tr>
<td> createmultisig </td>
<td> nrequired '["key,"key"]' </td>
<td> Creates a multi-signature address and returns a json object </td>
<td>
</td></tr>
<tr>
<td> createrawtransaction </td>
<td> [{"txid":txid,"vout":n},...] {address:amount,...} </td>
<td>  Creates a raw transaction spending given inputs. </td>
<td> N
</td></tr>
<tr>
<td> decoderawtransaction </td>
<td> hex string </td>
<td>  Produces a human-readable JSON object for a raw transaction. </td>
<td> N
</td></tr>
<tr>
<td> dumpprivkey </td>
<td> bitcoinaddress </td>
<td> Reveals the private key corresponding to bitcoinaddress </td>
<td> Y
</td></tr>
<tr>
<td> dumpwallet </td>
<td> filename </td>
<td>  Exports all wallet private keys to file </td>
<td> Y
</td></tr>
<tr>
<td> encryptwallet </td>
<td> passphrase </td>
<td> Encrypts the wallet with passphrase. </td>
<td> N
</td></tr>
<tr>
<td> getaccount </td>
<td> bitcoinaddress </td>
<td> Returns the account associated with the given address. </td>
<td> N
</td></tr>
<tr>
<td> getaccountaddress </td>
<td> account </td>
<td> Returns the current bitcoin address for receiving payments to this account. If account does not exist, it will be created along with an associated new address that will be returned. </td>
<td> N
</td></tr>
<tr>
<td> getaddednodeinfo </td>
<td> dns [node] </td>
<td> Returns information about the given added node, or all added nodes
<p>(note that onetry addnodes are not listed here)
If dns is false, only a list of added nodes will be provided,
otherwise connected information will also be available.
</p>
</td></tr>
<tr>
<td> getaddressesbyaccount </td>
<td> account </td>
<td> Returns the list of addresses for the given account. </td>
<td> N
</td></tr>
<tr>
<td> getbalance </td>
<td> [account] [minconf=1] </td>
<td> If [account] is not specified, returns the server's total available balance.<br>If [account] is specified, returns the balance in the account. </td>
<td> N
</td></tr>
<tr>
<td> getbestblockhash </td>
<td> </td>
<td> Returns the hash of the best (tip) block in the longest block chain. </td>
<td> N
</td></tr>
<tr>
<td> getblock </td>
<td> hash </td>
<td> Returns information about the block with the given hash. </td>
<td> N
</td></tr>
<tr>
<td> getblockcount </td>
<td> </td>
<td> Returns the number of blocks in the longest block chain. </td>
<td> N
</td></tr>
<tr>
<td> getblockhash </td>
<td> index </td>
<td> Returns hash of block in best-block-chain at index; index 0 is the genesis block </td>
<td> N
</td></tr>
<tr>
<td> getblocknumber </td>
<td> </td>
<td> <b>Deprecated</b>. Use getblockcount. </td>
<td> N
</td></tr>
<tr>
<td> getblocktemplate </td>
<td> [params] </td>
<td> Returns data needed to construct a block to work on. See  BIP_0022 for more info on params.</td>
<td> N
</td></tr>
<tr>
<td> getconnectioncount </td>
<td> </td>
<td> Returns the number of connections to other nodes. </td>
<td> N
</td></tr>
<tr>
<td> getdifficulty </td>
<td> </td>
<td> Returns the proof-of-work difficulty as a multiple of the minimum difficulty. </td>
<td> N
</td></tr>
<tr>
<td> gethashespersec </td>
<td> </td>
<td> Returns a recent hashes per second performance measurement while generating. </td>
<td> N
</td></tr>
<tr>
<td> getinfo </td>
<td> </td>
<td> Returns an object containing various state info. </td>
<td> N
</td></tr>
<tr>
<td> getmemorypool </td>
<td> [data] </td>
<td> <b>Replaced in v0.7.0 with getblocktemplate, submitblock, getrawmempool</b> </td>
<td> N
</td></tr>
<tr>
<td> getmininginfo </td>
<td> </td>
<td> Returns an object containing mining-related information:
<ul><li> blocks</li>
<li> currentblocksize</li>
<li> currentblocktx</li>
<li> difficulty</li>
<li> errors</li>
<li> generate</li>
<li> genproclimit</li>
<li> hashespersec</li>
<li> pooledtx</li>
<li> testnet</li></ul>
</td>
<td> N
</td></tr>
<tr>
<td> getnewaddress </td>
<td> [account] </td>
<td> Returns a new bitcoin address for receiving payments. If [account] is specified payments received with the address will be credited to [account]. </td>
<td> N
</td></tr>
<tr>
<td> getpeerinfo </td>
<td> </td>
<td>  Returns data about each connected node. </td>
<td> N
</td></tr>
<tr>
<td> getrawchangeaddress </td>
<td> [account]</td>
<td>  Returns a new Bitcoin address, for receiving change. This is for use with raw transactions, NOT normal use. </td>
<td> N
</td></tr>
<tr>
<td> getrawmempool </td>
<td> </td>
<td>  Returns all transaction ids in memory pool </td>
<td> N
</td></tr>
<tr>
<td> getrawtransaction </td>
<td> txid [verbose=0] </td>
<td>  Returns  representation for given transaction id. </td>
<td> N
</td></tr>
<tr>
<td> getreceivedbyaccount </td>
<td> [account] [minconf=1] </td>
<td> Returns the total amount received by addresses with [account] in transactions with at least [minconf] confirmations. If [account] not provided return will include all transactions to all accounts.  </td>
<td> N
</td></tr>
<tr>
<td> getreceivedbyaddress </td>
<td> bitcoinaddress [minconf=1] </td>
<td> Returns the amount received by bitcoinaddress in transactions with at least [minconf] confirmations. It correctly handles the case where someone has sent to the address in multiple transactions. Keep in mind that addresses are only ever used for receiving transactions. Works only for addresses in the local wallet, external addresses will always show 0. </td>
<td> N
</td></tr>
<tr>
<td> gettransaction </td>
<td> txid </td>
<td> Returns an object about the given transaction containing:
<ul><li> "amount": total amount of the transaction</li>
<li> "confirmations": number of confirmations of the transaction</li>
<li> "txid": the transaction ID</li>
<li> "time": time associated with the transaction[1]</li>
<li> "details" - An array of objects containing:
<ul><li> "account"</li>
<li> "address"</li>
<li> "category"</li>
<li> "amount"</li>
<li> "fee"</li></ul></li></ul>
</td>
<td> N
</td></tr>
<tr>
<td> gettxout </td>
<td> txid n [includemempool=true] </td>
<td> Returns details about an unspent transaction output (UTXO) </td>
<td> N
</td></tr>
<tr>
<td> gettxoutsetinfo </td>
<td> </td>
<td> Returns statistics about the unspent transaction output (UTXO) set </td>
<td> N
</td></tr>
<tr>
<td> getwork
<td> [data] </td>
<td> If [data] is not specified, returns formatted hash data to work on:
<ul><li> "midstate": precomputed hash state after hashing the first half of the data</li>
<li> "data": block data</li>
<li> "hash1": formatted hash buffer for second hash</li>
<li> "target": little endian hash target</li></ul>
<p>If [data] is specified, tries to solve the block and returns true if it was successful.
</p>
</td>
<td> N
</td></tr>
<tr>
<td> help </td>
<td> [command] </td>
<td> List commands, or get help for a command. </td>
<td> N
</td></tr>
<tr>
<td> importprivkey </td>
<td> bitcoinprivkey [label] [rescan=true]</td>
<td> Adds a private key (as returned by dumpprivkey) to your wallet. This may take a while, as a rescan is done, looking for existing transactions. <b>Optional [rescan] parameter added in 0.8.0.</b> Note: There's no need to import public key, as in ECDSA (unlike RSA) this can be computed from private key. </td>
<td> Y
</td></tr>
<tr>
<td> invalidateblock </td>
<td> hash </td>
<td> Permanently marks a block as invalid, as if it violated a consensus rule.</td>
<td> N
</td></tr>
<tr>
<td> keypoolrefill </td>
<td> </td>
<td> Fills the keypool, requires wallet passphrase to be set. </td>
<td> Y
</td></tr>
<tr>
<td> listaccounts </td>
<td> [minconf=1] </td>
<td> Returns Object that has account names as keys, account balances as values. </td>
<td> N
</td></tr>
<tr>
<td> listaddressgroupings </td>
<td> </td>
<td>  Returns all addresses in the wallet and info used for coincontrol. </td>
<td> N
</td></tr>
<tr>
<td> listreceivedbyaccount </td>
<td> [minconf=1] [includeempty=false] </td>
<td> Returns an array of objects containing:
<ul><li> "account": the account of the receiving addresses</li>
<li> "amount": total amount received by addresses with this account</li>
<li> "confirmations": number of confirmations of the most recent transaction included</li></ul>
</td>
<td> N
</td></tr>
<tr>
<td> listreceivedbyaddress </td>
<td> [minconf=1] [includeempty=false] </td>
<td> Returns an array of objects containing:
<ul><li> "address": receiving address</li>
<li> "account": the account of the receiving address</li>
<li> "amount": total amount received by the address</li>
<li> "confirmations": number of confirmations of the most recent transaction included</li></ul>
<p>To get a list of accounts on the system, execute bitcoind listreceivedbyaddress 0 true
</p>
</td>
<td> N
</td></tr>
<tr>
<td> listsinceblock </td>
<td> [blockhash] [target-confirmations] </td>
<td> Get all transactions in blocks since block [blockhash], or all transactions if omitted. [target-confirmations] intentionally <b>does not</b> affect the list of returned transactions, but only affects the returned "lastblock" value </td>
<td> N
</td></tr>
<tr>
<td> listtransactions </td>
<td> [account] [count=10] [from=0] </td>
<td> Returns up to [count] most recent transactions skipping the first [from] transactions for account [account]. If [account] not provided it'll return recent transactions from all accounts.
</td>
<td> N
</td></tr>
<tr>
<td> listunspent </td>
<td> [minconf=1] [maxconf=999999] </td>
<td>  Returns array of unspent transaction inputs in the wallet. </td>
<td> N
</td></tr>
<tr>
<td> listlockunspent </td>
<td> </td>
<td>  Returns list of temporarily unspendable outputs
</td></tr>
<tr>
<td> lockunspent </td>
<td> unlock? [array-of-objects] </td>
<td>  Updates list of temporarily unspendable outputs
</td></tr>
<tr>
<td> move </td>
<td> fromaccount toaccount amount <br>[minconf=1] [comment] </td>
<td> Move from one account in your wallet to another </td>
<td> N
</td></tr>
<tr>
<td> sendfrom </td>
<td> fromaccount tobitcoinaddress <br> amount [minconf=1] <br> [comment] [comment-to] </td>
<td> amount is a real and is rounded to 8 decimal places. Will send the given amount to the given address, ensuring the account has a valid balance using [minconf] confirmations. Returns the transaction ID if successful (not in JSON object). </td>
<td> Y
</td></tr>
<tr>
<td> sendmany </td>
<td> fromaccount <br> {address:amount,...} <br>[minconf=1] [comment] </td>
<td> amounts are double-precision floating point numbers </td>
<td> Y
</td></tr>
<tr>
<td> sendrawtransaction </td>
<td> hexstring </td>
<td>  Submits  (serialized, hex-encoded) to local node and network. </td>
<td> N
</td></tr>
<tr>
<td> sendtoaddress </td>
<td> bitcoinaddress amount [comment] [comment-to] </td>
<td> amount is a real and is rounded to 8 decimal places. Returns the transaction ID txid if successful. </td>
<td> Y
</td></tr>
<tr>
<td> setaccount </td>
<td> bitcoinaddress account </td>
<td> Sets the account associated with the given address. Assigning address that is already assigned to the same account will create a new address associated with that account. </td>
<td> N
</td></tr>
<tr>
<td> setgenerate </td>
<td> generate [genproclimit] </td>
<td> generate is true or false to turn generation on or off.<br>Generation is limited to [genproclimit] processors, -1 is unlimited. </td>
<td> N
</td></tr>
<tr>
<td> settxfee </td>
<td> amount </td>
<td> amount is a real and is rounded to the nearest 0.00000001 </td>
<td> N
</td></tr>
<tr>
<td> signmessage </td>
<td> bitcoinaddress message </td>
<td> Sign a message with the private key of an address. </td>
<td> Y
</td></tr>
<tr>
<td> signrawtransaction </td>
<td> hexstring [{"txid":txid,"vout":n,<br>"scriptPubKey":hex},...] [privatekey1,...] </td>
<td>  Adds signatures to a raw transaction and returns the resulting raw transaction. </td>
<td> Y/N
</td></tr>
<tr>
<td> stop </td>
<td> </td>
<td> Stop bitcoin server. </td>
<td> N
</td></tr>
<tr>
<td> submitblock </td>
<td> hex data [optional-params-obj] </td>
<td> Attempts to submit new block to network. </td>
<td> N
</td></tr>
<tr>
<td> validateaddress </td>
<td> bitcoinaddress </td>
<td> Return information about bitcoinaddress. </td>
<td> N
</td></tr>
<tr>
<td> verifymessage </td>
<td> bitcoinaddress signature message </td>
<td> Verify a signed message. </td>
<td> N
</td></tr>
<tr>
<td> walletlock </td>
<td> </td>
<td> Removes the wallet encryption key from memory, locking the wallet. After calling this method, you will need to call walletpassphrase again before being able to call any methods which require the wallet to be unlocked. </td>
<td> N
</td></tr>
<tr>
<td> walletpassphrase </td>
<td> passphrase timeout </td>
<td> Stores the wallet decryption key in memory for timeout seconds. </td>
<td> N
</td></tr>
<tr>
<td> generatepos </td>
<td>  </td>
<td> generate posblock :if mining successed ,then return blockhash. if failed,then return nothing. </td>
<td> Y
</td></tr>
</tbody></table>

