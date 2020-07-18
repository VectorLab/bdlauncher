#!/bin/bash

if [ -f ./build/preload.so ]; then
echo -n > /dev/null
else
echo "not compile"
echo "current path: " $(pwd)
exit 1
fi

if [ "$PREFIX" == "" ]; then
PREFIX="";
PREFIX=$1;
fi

if [ "$PREFIX" == "" ]; then
echo "prefix not define"
echo 'example: ./install.sh ${PREFIX}'
exit 1
fi

echo "install to: " $PREFIX;

# install preload.so
cp -f ./build/preload.so $PREFIX/preload.so

# install s
echo '#!/bin/bash' > $PREFIX/s
echo 'LD_LIBRARY_PATH=. LD_PRELOAD=preload.so ./bedrock_server' >> $PREFIX/s
chmod 755 $PREFIX/s

# install mods
if [ -d "$PREFIX/mods" ]; then
echo -n > /dev/null
else
cp -r ./mods $PREFIX/mods
fi

# install bdl
if [ -d "$PREFIX/mlr" ]; then
echo -n > /dev/null
else
mkdir $PREFIX/mlr
fi

if [ -d "$PREFIX/mlr/include" ]; then
rm -rf $PREFIX/mlr/include
fi
cp -rf ./include $PREFIX/mlr/include
cp -rf ./build/zydis/usr/include/* $PREFIX/mlr/include

if [ -d "$PREFIX/mlr/config" ]; then
echo -n > /dev/null
else
mkdir $PREFIX/mlr/config
fi

rm -f $PREFIX/mlr/config/cc
rm -f $PREFIX/mlr/config/cxx
cp ./build/cc $PREFIX/mlr/config/cc
cp ./build/cxx $PREFIX/mlr/config/cxx
chmod 755 $PREFIX/mlr/config/cc
chmod 755 $PREFIX/mlr/config/cxx

