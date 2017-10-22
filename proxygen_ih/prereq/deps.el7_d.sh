#!/bin/bash


#. /opt/rh/devtoolset-3/enable
#scl enable devtoolset-3 bash

PREFIX=`pwd`
BOOST_DIR=/home/ihersht/lib/boost
INC_DIR="${PREFIX}/include"
LIB_DIR="${PREFIX}/lib"
LIB_DIR_STATIC="${PREFIX}/lib_s"
BOOST_INC_DIR=${BOOST_DIR}/include
BOOST_LIB_DIR=${BOOST_DIR}/lib_d
export LD_LIBRARY_PATH=${LIB_DIR}:${BOOST_LIB_DIR}:${LD_LIBRARY_PATH}


# Parse args
JOBS=6



# Must execute from the directory containing this script
cd "$(dirname "$0")"
pwd
cd build/proxygen

# Build folly
cd folly/folly
autoreconf --install
#autoreconf -ivf
./configure  --libdir=${LIB_DIR} --includedir=${INC_DIR}
make clean
make -j$JOBS CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++1y -g3 -O0  -I${BOOST_INC_DIR} " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_LIB_DIR} "
 make install
make check -j$JOBS CXXFLAGS="-lz -pthread  -std=gnu++1y -g3 -O0 -I${INC_DIR} -I${BOOST_INC_DIR} " LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_LIB_DIR} -std=gnu++1y -g3 -O0"
cd ../..



cd wangle/wangle

FOLLY_INCLUDE_DIR=${INC_DIR}
export LDFLAGS CPPFLAGS
cmake  -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS_DEBUG:STRING="-O0 -g3"  -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX}  .

make clean
make -j$JOBS
make install
make test
cd ../..



# Build proxygen
cd proxygen
cp -r external ${INC_DIR}/proxygen
export CXXFLAGS=" -std=gnu++1y -g3 -O0 $CXXFLAGS"
#autoreconf -ivf
autoreconf --install
./configure --libdir=${LIB_DIR} --includedir=${INC_DIR}
make clean
make -j$JOBS CFLAGS="-g3 -O0 " CXXFLAGS="-lz -pthread  -std=gnu++1y -g3 -O0 -I${INC_DIR} -I${BOOST_INC_DIR}" LDFLAGS=" -pthread -L${LIB_DIR} -L${BOOST_LIB_DIR}"
 make install
 mkdir ${LIB_DIR_STATIC}
cp ${LIB_DIR}/*.a ${LIB_DIR_STATIC}
# Run tests
make check -j$JOBS CXXFLAGS="-lz -lwangle -pthread  -std=gnu++1y -g3 -O0 -I${INC_DIR} -I${BOOST_INC_DIR} " LDFLAGS=" -lwangle -pthread -L${LIB_DIR} -L${BOOST_LIB_DIR} "



