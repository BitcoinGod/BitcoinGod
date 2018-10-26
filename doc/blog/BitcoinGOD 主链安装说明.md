## BitcoinGOD 主链安装说明
&#160; &#160; &#160; &#160;Bitcoin God 客户端是构建整个网络的支柱。下载它并默认存储比特币交易的所有历史（目前超过160 G）；同步的过程将会花费数小时或更多时间，它取决于计算机的硬件读写速度和网速。 
### 依赖
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
安装OS X命令行工具：**xcode-select –install**，当弹出窗口时候，点击**Install**
安装 **[Homebrew][1]**.

```
brew install automake berkeley-db4 libtool boost --c++11 miniupnpc openssl pkg-config protobuf libevent
```

#### WINDOWS
解压[文件][2]到目录，该目录所在硬盘可用空间需要大于160G。
### 编译
#### Unix & Mac

1. 下载bitcoingod源码并进入bitcoingod目录

```
git clone https://github.com/BitcoinGod/BitcoinGod.git 
cd BitcoinGod
```

2. 编译Bitcoin God: 配置并编译Bitcoin God

```
binaries –without-gui 
./autogen.sh   
./configure --without-gui   
make  
make install
```

#### Windows
不需要编译。
### 运行
运行之前，我们推荐创建RPC配置文件，在OSX下创建配置文件如下：

```
echo -e "rpcuser=default@\nrpcpassword=default@"  > "/Users/${USER}/Library/Application Sup port/BitcoinGod/bitcoin.conf"
chmod 600 "/Users/${USER}/Library/Application Support/BitcoinGod/bitcoin.conf"
```

在Linux或者Unix下面创建配置文件

```
echo -e "rpcuser=default@\nrpcpassword=default@"  > "/${USER}/.BitcoinGod/bitcoin.conf"
chmod 600 "/${USER}/.BitcoinGod/bitcoin.conf"
```
在Windows下面创建配置文件

```
echo rpcuser=default@ >%USERPROFILE%\AppData\Roaming\BitcoinGod\bitcoin.conf	
echo rpcpassword=default@  >%USERPROFILE%\AppData\Roaming\BitcoinGod\bitcoin.conf
```

### 快速开始
#### windows
启动Bitcoin God

```
bitcoingodd.exe
```

执行Rpc命令

```
bitcoingod-cli.exe help
```

#### linux
启动Bitcoin God

```
bitcoingodd -datadir={/BitcoinGod/datadir} -rpcuser={username} -rpcpassword={passwd} -daemon
```

执行Rpc命令

```
bitcoingod-cli -rpcuser={username} -rpcpassword={passwd} help
```

启动bitcoingodd daemon

```
bitcoingodd -daemon
```

当你第一次启动并连接在网络中，bitcoingodd会开始校验blockchain并下载。这需要根据你的网络情况和硬盘读写情况花上几个小时或更多时间。
你能通过下面命令查看日志：

```
tail -f debug.log
```

其他命令：
1.	开始 BitcoinGOD

```
bitcoingodd -daemon -debug=0
```

2.	停止BitcoinGOD

```
bitcoingod-cli stop
```

3.	查看余额

```
bitcoingod-cli getbalance <account[optional]>
```

4.	查看fullnode信息

```
bitcoingod-cli getinfo
```
5.	转账到某个地址

```
bitcoingod-cli sendtoaddress <address> <amount>
```
6.	查看交易历史

```
bitcoingod-cli listtransactions <account> <count=10> <from=0>
```
7.	查看UTXOS

```
bitcoingod-cli listunspent <minconf=1> <maxconf=999999>
```
8.	挖矿

```
bitcoingod-cli generatepos
```
9.	根据blockheight获取blockhash

```
bitcoingod-cli getblockhash <height>
```
10.	获取Block信息

```
bitcoingod-cli getblock <blockhash>
```
11.	获取交易信息

```
bitcoingod-cli gettransaction <txid>
```


  [1]: https://brew.sh/
  [2]: https://github.com/BitcoinGod/BitcoinGod/releases/download/v0.1.5.0/bitcoingod-v0.1.5.0-binary-win64.zip