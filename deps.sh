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

sudo apt install git
sudo apt install cmake
sudo apt install libboost-dev-all
sudo apt install proto-c-compiler proto-compiler libprotobuf-c-dev libprotoc-dev
sudo apt install libre2-dev