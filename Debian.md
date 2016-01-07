Installing from Packages
========================

There are packages available (more to come) via our debian mirror.  You can
access it with the following commands.

    sudo apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 75FE259AA8AC8032
    echo "deb http://gplug.in/debian jessie main" | sudo tee /etc/apt/sources.list.d/gplugin.list
	sudo apt-get update
	sudo apt-get install libgplugin0 libgplugin-loaders

You can use `apt-cache search gplugin` to see the other packages that are available


