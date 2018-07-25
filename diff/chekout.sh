#!/bin/bash

## Run this script to build proxygen and run the tests. If you want to
## install proxygen to use in another C++ project on this machine, run
## the sibling file `reinstall.sh`.


set -e
FOLLY_VERSION='v0.54.0'
WANGLE_VERSION='v0.12.0'


# Must execute from the directory containing this script
cd "$(dirname "$0")"

cd build/proxygen
svn checkout https://google-gflags.googlecode.com/svn/trunk/ google-gflags
svn checkout https://google-glog.googlecode.com/svn/trunk/ google-glog
git clone https://github.com/floitsch/double-conversion.git double-conversion
git clone https://github.com/facebook/folly
cd folly/folly
git fetch
git checkout $FOLLY_VERSION
cd ../..

git clone https://github.com/facebook/wangle
cd wangle/wangle
git fetch
git checkout $WANGLE_VERSION
cd ../..


