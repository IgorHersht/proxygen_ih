INST_DIR=`pwd`
echo $INST_DIR
ln -s boost_1_63_0 boost
cd boost
./bootstrap.sh 
sudo ./b2  -d+2 install --with=all --includedir=${INST_DIR}/include --libdir=${INST_DIR}/lib_d --build-dir=${INST_DIR}/bld_d cxxflags="-std=gnu++1y -g3 -pthread -fPIC " variant=debug threading=multi  -j 6 


