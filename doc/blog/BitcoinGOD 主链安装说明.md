## BitcoinGOD ������װ˵��
&#160; &#160; &#160; &#160;Bitcoin God �ͻ����ǹ������������֧������������Ĭ�ϴ洢���رҽ��׵�������ʷ��Ŀǰ����160 G����ͬ���Ĺ��̽��Ứ����Сʱ�����ʱ�䣬��ȡ���ڼ������Ӳ����д�ٶȺ����١� 
### ����
#### UNIX

```
sudo apt-get update  
sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils 
sudo apt-get install libboost-all-dev 
sudo apt-get install software-properties-common 
sudo add-apt-repository ppa:bitcoin/bitcoin 
sudo apt-get update 
sudo apt-get install libdb4.8-dev libdb4.8++-dev 
sudo apt-get install libminiupnpc-dev
```

#### MAC
��װOS X�����й��ߣ�**xcode-select �Cinstall**������������ʱ�򣬵��**Install**
��װ **[Homebrew][1]**.

```
brew install automake berkeley-db4 libtool boost --c++11 miniupnpc openssl pkg-config protobuf libevent
```

#### WINDOWS
��ѹ[�ļ�][2]��Ŀ¼����Ŀ¼����Ӳ�̿��ÿռ���Ҫ����160G��
### ����
#### Unix & Mac

1. ����bitcoingodԴ�벢����bitcoingodĿ¼

```
git clone https://github.com/BitcoinGod/BitcoinGod.git 
cd BitcoinGod
```

2. ����Bitcoin God: ���ò�����Bitcoin God

```
binaries �Cwithout-gui 
./autogen.sh   
./configure --without-gui   
make  
make install
```

#### Windows
����Ҫ���롣
### ����
����֮ǰ�������Ƽ�����RPC�����ļ�����OSX�´��������ļ����£�

```
echo -e "rpcuser=default@\nrpcpassword=default@"  > "/Users/${USER}/Library/Application Sup port/BitcoinGod/bitcoin.conf"
chmod 600 "/Users/${USER}/Library/Application Support/BitcoinGod/bitcoin.conf"
```

��Linux����Unix���洴�������ļ�

```
echo -e "rpcuser=default@\nrpcpassword=default@"  > "/${USER}/.BitcoinGod/bitcoin.conf"
chmod 600 "/${USER}/.BitcoinGod/bitcoin.conf"
```
��Windows���洴�������ļ�

```
echo rpcuser=default@ >%USERPROFILE%\AppData\Roaming\BitcoinGod\bitcoin.conf	
echo rpcpassword=default@  >%USERPROFILE%\AppData\Roaming\BitcoinGod\bitcoin.conf
```

### ���ٿ�ʼ
#### windows
����Bitcoin God

```
bitcoingodd.exe
```

ִ��Rpc����

```
bitcoingod-cli.exe help
```

#### linux
����Bitcoin God

```
bitcoingodd -datadir={/BitcoinGod/datadir} -rpcuser={username} -rpcpassword={passwd} -daemon
```

ִ��Rpc����

```
bitcoingod-cli -rpcuser={username} -rpcpassword={passwd} help
```

����bitcoingodd daemon

```
bitcoingodd -daemon
```

�����һ�������������������У�bitcoingodd�ῪʼУ��blockchain�����ء�����Ҫ����������������Ӳ�̶�д������ϼ���Сʱ�����ʱ�䡣
����ͨ����������鿴��־��

```
tail -f debug.log
```

�������
1.	��ʼ BitcoinGOD

```
bitcoingodd -daemon -debug=0
```

2.	ֹͣBitcoinGOD

```
bitcoingod-cli stop
```

3.	�鿴���

```
bitcoingod-cli getbalance <account[optional]>
```

4.	�鿴fullnode��Ϣ

```
bitcoingod-cli getinfo
```
5.	ת�˵�ĳ����ַ

```
bitcoingod-cli sendtoaddress <address> <amount>
```
6.	�鿴������ʷ

```
bitcoingod-cli listtransactions <account> <count=10> <from=0>
```
7.	�鿴UTXOS

```
bitcoingod-cli listunspent <minconf=1> <maxconf=999999>
```
8.	�ڿ�

```
bitcoingod-cli generatepos
```
9.	����blockheight��ȡblockhash

```
bitcoingod-cli getblockhash <height>
```
10.	��ȡBlock��Ϣ

```
bitcoingod-cli getblock <blockhash>
```
11.	��ȡ������Ϣ

```
bitcoingod-cli gettransaction <txid>
```


  [1]: https://brew.sh/
  [2]: https://github.com/BitcoinGod/BitcoinGod/releases/download/v0.1.5.0/bitcoingod-v0.1.5.0-binary-win64.zip