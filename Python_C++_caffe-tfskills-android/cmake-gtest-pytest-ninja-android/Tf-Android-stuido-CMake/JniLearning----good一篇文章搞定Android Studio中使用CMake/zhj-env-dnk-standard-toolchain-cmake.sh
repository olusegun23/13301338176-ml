

#http://blog.csdn.net/manshilingkai/article/details/22619855    Android NDK 工具链的使用方法(Standalone Toolchain)
#~/android-sdk-linux-1/ndk-bundle/build/tools$ ./make-standalone-toolchain.sh  --platform=android-27  --install-dir=/home/haijunz/studio-cmake-gtest-prj/ndk-standard-tools-api27

export PATH=/home/haijunz/studio-cmake-gtest-prj/ndk-standard-tools-api27/bin:$PATH
export CC=arm-linux-androideabi-gcc
export CXX=arm-linux-androideabi-g++
export CXXFLAGS="-lstdc++"
