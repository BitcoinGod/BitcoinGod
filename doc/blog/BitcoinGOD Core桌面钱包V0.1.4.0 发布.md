## BitcoinGOD V0.1.4.0 版本发布

&#160; &#160; &#160; &#160;V0.1.4.0版本发布了，本版本进一步优化网络节点健壮性，根据BitcoinGOD社群用户反馈意见完善了用户使用体验等，主要包括，

1. 优化网络协议，增强网络节点健壮性

2. 优化debug日志输出，不再记录挖矿日志，使debug日志文件会大幅削减

3. 优化POS挖矿Block时间校验，增强POS验证机制提高主链公平性

4. 添加checkpoint，以提高网络安全性防止恶意攻击

5. 增加测试网络


测试网络运行命令：

```
bitcoingodd -testnet -datadir=<path> -rpcuser=<username> -rpcpassword=<password>

例：bitcoingodd -testnet -datadir=D:\datadir -rpcuser=zhangsan -rpcpassword=123456

```

测试网络执行rpc命令：

```
bitcoingod-cli -rpcport=18886 -rpcuser= <username> -rpcpassword=<password> getinfo

例：bitcoingod-cli.exe -rpcport=18886 -rpcuser=zhangsan -rpcpassword=123456 getinfo

```