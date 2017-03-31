#!/bin/bash

if ! sudo apt-get install -y libgoogle-glog-dev;
then
    if [ ! -e deps/google-glog ]; then
        echo "failed to get google-glog"
    fi
fi

if ! sudo apt-get install -y libgflags-dev;
then
    if [ ! -e deps/google-gflags ]; then
        echo "failed to get google-gflags"
    fi
fi

sudo apt install -y git
sudo apt install -y cmake
sudo apt install -y g++
sudo apt install -y libboost-all-dev
sudo apt install -y protobuf-c-compiler protobuf-compiler libprotobuf-c-dev libprotoc-dev
sudo apt install -y libre2-dev

[[ ! -d deps/googletest ]] && mkdir -p deps && git clone https://github.com/google/googletest deps/googletest
