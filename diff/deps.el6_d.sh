#!/bin/bash

## Run this script to build proxygen and run the tests. If you want to
## install proxygen to use in another C++ project on this machine, run
## the sibling file `reinstall.sh`.



#. /opt/rh/devtoolset-3/enable
#scl enable devtoolset-3 bash

PREFIX=`pwd`
BOOST_DIR=/home/ihersht/lib/boost
INC_DIR="${PREFIX}/include"
LIB_DIR="${PREFIX}/lib"
export LD_LIBRARY_PATH=${LIB_DIR}:${BOOST_DIR}/lib:${LD_LIBRARY_PATH}




# Parse args
JOBS=4
USAGE="./deps.sh [-j num_jobs]"
while [ "$1" != "" ]; do
  case $1 in
    -j | --jobs ) shift
                  JOBS=$1
                  ;;
    * )           echo $USAGE
                  exit 1
esac
shift
done

set -e


# Must execute from the directory containing this script
cd "$(dirname "$0")"

cd build/proxygen

    cd google-gflags
#    autoreconf -ivf
    ./configure --prefix=${PREFIX} 
	make clean
    make  CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread   -std=gnu++0x -g3 -O0"
     make install
    cd ..
    

# WORKAROUND:
# Need to exec ldconfig here or wangle build fails

  
    echo "fetching glog from svn (apt-get failed)"
                cd google-glog
#                autoreconf -ivf
                ./configure --prefix=${PREFIX} 
				make clean
                make CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread   -std=gnu++0x -g3 -O0"
                 make install
                cd ..
      

      
    echo "Fetching double-conversion from git (apt-get failed)"
                cd double-conversion
                cmake -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX} . -DBUILD_SHARED_LIBS=ON
                 make install
                cd ..
        


# Build folly
cd folly/folly
autoreconf --install
#autoreconf -ivf
./configure --prefix=${PREFIX} 
make clean
make -j$JOBS CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_DIR}/lib -std=gnu++0x -g3 -O0"
 make install
make check -j$JOBS CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_DIR}/lib -std=gnu++0x -g3 -O0"

if test $? -ne 0; then
        echo "fatal: folly build failed"
        exit -1
fi
cd ../..


# Get wangle

cd wangle/wangle

# wangle build fails if we don't add -pthread here
CPPFLAGS='-lz -pthread -g3 -O0 '
LDFLAGS='-pthread'
export LDFLAGS CPPFLAGS
cmake -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX} -DCMAKE_VERBOSE_MAKEFILE=ON  .
make clean
make -j$JOBS
make install
#ctest

if test $? -ne 0; then
        echo "fatal: wangle build failed"
        exit -1
fi
cd ../..
# UNDO WORKAROUND:
unset LDFLAGS CPPFLAGS
cd ./proxygen


# Build proxygen
autoreconf -ivf
./configure --prefix=${PREFIX} 
make clean
make -j$JOBS CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_DIR}/lib -std=gnu++0x -g3 -O0"

# Run tests
make check -j$JOBS CXXFLAGS="-lz -pthread  -std=gnu++0x -g3 -O0 -I${INC_DIR} -I${BOOST_DIR}/include " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_DIR}/lib -std=gnu++0x -g3 -O0"

# Install the libs
 make install
