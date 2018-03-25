## Installation

It is the Bitcoin God client and it builds the backbone of the network. It downloads and, by default, stores the entire history of Bitcoin transactions (which is currently more than 150 GBs); depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more. ### Dependencies#### Unix		sudo apt-get update 	sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils	sudo apt-get install libboost-all-dev	sudo apt-get install software-properties-common	sudo add-apt-repository ppa:bitcoin/bitcoin	sudo apt-get update	sudo apt-get install libdb4.8-dev libdb4.8++-dev	sudo apt-get install libminiupnpc-dev#### MacInstall the OS X command line tools:***xcode-select --install***  When the popup appears, click Install . Then install [Homebrew](https://brew.sh/).			brew install automake berkeley-db4 libtool boost --c++11 miniupnpc openssl pkg-config protobuf libevent#### WindowsUnpack the [files](https://github.com/tokengod/godcoind/releases/download/v1.0/bitcoingod__x86_v0.1.zip) into a directory.### Building1.Clone the bitcoingod source code and cd into bitcoingod		git clone https://github.com/tokengod/godcoind.git 	cd godcoind 2.build Bitcoind God: Configure and build the headless Bitcoind God binaries --without-gui 		./autogen.sh 	./configure --without-gui 	make 	make install### RunningBitCoind God is now available at ./src/bitcoind

Before running, it's recommended you create an RPC configuration file,create an RPC configuration file on OSX	echo -e "rpcuser=default@\nrpcpassword=default@" > "/Users/${USER}/Library/Application Sup port/Bitcoin/bitcoin.conf"
	chmod 600 "/Users/${USER}/Library/Application Support/Bitcoin/bitcoin.conf"
create an RPC configuration file on linux or unix	echo -e "rpcuser=default@\nrpcpassword=default@" > "/${USER}/.bitcoin/bitcoin.conf"	chmod 600 "/${USER}/.bitcoin/bitcoin.conf"
create an RPC configuration file on Windows	echo rpcuser=default@ >%USERPROFILE%\AppData\Roaming\Bitcoin\bitcoin.conf		echo rpcpassword=default@ >>%USERPROFILE%\AppData\Roaming\Bitcoin\bitcoin.conf	Starts the bitcoin daemon		bitcoingodd -daemonThe first time you run bitcoind, it will start downloading the blockchain. This process could take several hours.

You can monitor the download process by looking at the debug.log file:		tail -f debug.log
	
	## Other commands:	bitcoingodd -daemon # Starts the bitcoin daemon.
	bitcoingod-cli --help # Outputs a list of command-line options.
	bitcoingod-cli help # Outputs a list of RPC commands when the daemon is running.	bitcoingod-cli generatepos # POS mining.
	
See more [RPC commands List](https://github.com/tokengod/godcoind/blob/master/doc/RPC_LIST.md).