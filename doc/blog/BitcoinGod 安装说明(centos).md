
# Centos 安装文档
本文档用于引导用户在Centos环境安装GOD客户端。客户端的其他操作请参照BitcoinGod[教程](https://github.com/BitcoinGod/BitcoinGod/tree/master/doc)
## 依赖

	yum install -y boost-devel qt-devel protobuf-devel qrencode-devel libevent-devel libtool openssl-devel

	wget http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz 
	tar -xzvf db-4.8.30.NC.tar.gz 
	cd db-4.8.30.NC/build_unix 
	../dist/configure –enable-cxx 
	make 
	sudo make install 


## 编译

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

## 问题
在安装过程中可能出现以下问题。

1. configure过程出现的问题.
	+ 问题描述
	
	```
	configure: error: C++ preprocessor “/lib/cpp” fails sanity 
	 check See `config.log’ for more details
	```
	+ 解决方案	

	```
	yum install glibc-headers
	yum install gcc-c++ 
	```	
