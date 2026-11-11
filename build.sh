#!/bin/bash
wget https://github.com
sudo tar -xzf ps3dev-ubuntu-20.04.tar.gz -C /
export PS3DEV=/usr/local/ps3dev
export PSL1GHT=$PS3DEV/ppu
export PATH=$PATH:$PS3DEV/bin:$PS3DEV/ppu/bin
make clean
make
