#!/bin/bash


cd build
if [ -d "zydis" ];then
	echo "zydis rebuilding"
	rm -rf zydis
else
	echo "zydis building"
fi
mkdir zydis
cd zydis
cp -rf ../../lib/zydis src
echo "set(CMAKE_C_FLAGS  \"${CMAKE_C_FLAGS} -s -O3 -fPIC\")" >> src/CMakeLists.txt
mkdir build
cd build
CC=$1
CXX=$2
echo "zydis CC=" ${CC} " , CXX=" ${CXX}
export CC=${CC}
export CXX=${CXX}
cmake ../src
make -j4
if [ -f "libZydis.a" ] && [ -f "ZydisExportConfig.h" ];then
	echo "zydis build finished"
else
	echo "zydis build failed"
	exit 1
fi
cd ..
mkdir -p ./usr/lib
mv build/libZydis.a usr/lib/libZydis.a
mv src/include usr/include
mv build/ZydisExportConfig.h usr/include/ZydisExportConfig.h
echo "zydis finished"

