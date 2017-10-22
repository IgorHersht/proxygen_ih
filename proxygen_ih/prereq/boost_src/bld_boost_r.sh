INST_DIR=`pwd` 
echo $INST_DIR
#tar -xzvf boost_1_64_0-snapshot.tar.gz
ln -s boost_1_63_0 boost
cd boost


./bootstrap.sh --prefix=${INST_DIR}

sudo ./b2  -d+2 install --with=all --includedir=${INST_DIR}/include --libdir=${INST_DIR}/lib_r --build-dir=${INST_DIR}/bld_r cxxflags="-std=gnu++1y -g3 -pthread -fPIC " variant=release threading=multi  -j 6 



