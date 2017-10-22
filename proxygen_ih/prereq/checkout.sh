
mkdir build
cd ./build
git clone https://github.com/facebook/proxygen.git proxygen
cd proxygen
#git clone https://github.com/google/glog.git google-glog
#git clone https://github.com/gflags/gflags.git google-gflags
#git clone https://github.com/floitsch/double-conversion.git double-conversion
		

# Get folly
if [ ! -e folly/folly ]; then
    echo "Cloning folly"
    git clone https://github.com/facebook/folly
fi
cd folly/folly
git fetch
git checkout master
cd ../..

# Get wangle
if [ ! -e wangle/wangle ]; then
    echo "Cloning wangle"
    git clone https://github.com/facebook/wangle
fi
cd wangle/wangle
git fetch
git checkout master

cd ../..


