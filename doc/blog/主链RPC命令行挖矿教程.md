## 主链 RPC 命令行挖矿教程 

#### 前提：

 - 按照官方教程安装Bitcoin God； 
 - 由于采用POS挖矿，所以需要有GOD币才能挖矿；

#### 开始挖矿：
1. 打开命令行窗口

	启动GOD进程；
    ![此处输入图片的描述][1]
    
    getinfo查看信息；
    
    ![此处输入图片的描述][2]
    
2. 启动挖矿

1）手工挖矿，直接执行命令 

>  bitcoingod-cli generatepos

![此处输入图片的描述][3]
    
2）自动挖矿
    
Win：下载脚本auto_mine.bat，执行截图如下：
    
![此处输入图片的描述][4]
    
Mac/Unix：auto_mine.sh，执行截图如下：
    
![此处输入图片的描述][5]


  [1]: https://github.com/BitcoinGod/static/blob/master/pic/mining/start_cmd.jpg
  [2]: https://github.com/BitcoinGod/static/blob/master/pic/mining/getinfo.png
  [3]: https://github.com/BitcoinGod/static/blob/master/pic/mining/mine.png
  [4]: https://github.com/BitcoinGod/static/blob/master/pic/mining/auto_bat.png
  [5]: https://github.com/BitcoinGod/static/blob/master/pic/mining/auto_sh.png