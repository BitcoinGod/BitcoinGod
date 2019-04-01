## BitcoinGOD首次接入快速区块同步
				
&#160; &#160; &#160;&#160;因为BitcoinGOD是比特币的分叉，分叉高度在501225，并且向上兼容；所以首次接入BitcoinGOD用户涉及到501225前的BTC网络区块同步（大约150G）。为加快用户进行完整区块区块同步，用户可以选择在分叉高度501225前通过BTC网络节点来同步，到达501225后恢复到BitcoinGOD网络继续后续同步即可。（注：如当前连接只有BTC节点没有GOD节点，则桌面钱包只能同步到501225，待恢复到GOD网络节点后才能继续同步）

#### 具体操作方法：

使用BitcoinGOD core 桌面钱包将BTC节点IP地址添加为同步节点：(BTC 节点可以通过附录提供的方式获得)

1)	打开钱包，点击 帮助-调试窗口 选择控制台
 ![此处输入图片的描述][1]
 
2)	运行命令addnode将BTC节点IP添加到BitcoinGOD同步节点列表（BTC 节点IP获取方法见附录）

例: 添加节点IP为 1.2.3.4则在控制台中输入命令 addnode 1.2.3.4:8333 add
 ![此处输入链接的描述][2]

<font color='red'>**Warning**</font>: 

&#160; &#160; &#160;&#160;同步达到分叉高度501225后，关闭BitcoinGOD桌面钱包，进入BitcoinGOD同步数据目录，找到并删除‘peers.dat’文件并重新启动钱包（钱包将自动寻找BitcoinGOD网络节点并继续同步）

TIPs:

-   8333为BTC节点端口号，需要在所有BTC节点IP后添加
-   IP需要逐个添加
-   建议选择与自己地域接近的IP节点（可以通过一些提供IP位置查询服务的网站进行IP所在区域查询）
-   使用getaddednodeinfo来获知节点是否成功添加
 ![此处输入链接的描述][3]

#### 附录：Bitcoin 节点查询方法

1.	打开命令行窗口（windows 快捷键’win+r’ 中输入’cmd’ 进入命令行窗口/Mac启动Terminal）
2.	输入nslookup命令获取BTC节点IP列表:（Win/MAC相同），运行如下任意一个命令即可，每个nslookup命令会查询到多个不同的IP列表，用户可根据需要选择。
	-	nslookup seed.bitcoin.sipa.be
	- 	nslookup dnsseed.bluematt.me
	-	nslookup dnsseed.bitcoin.dashjr.org
	-	nslookup seed.bitcoinstats.com
	-	nslookup seed.bitcoin.jonasschnelli.ch
	-	nslookup seed.btc.petertodd.org
3.	获得节点IP列表，添加至BitcoinGOD连接节点中（建议添加3-5个即可）


  [1]: https://github.com/BitcoinGod/static/blob/master/pic/sync/menu_cn.png
  [2]: https://github.com/BitcoinGod/static/blob/master/pic/sync/eg.png
  [3]: https://github.com/BitcoinGod/static/blob/master/pic/sync/result.png