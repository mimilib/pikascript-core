#!/bin/bash

SRC_FOLDER=src
SUB_CMAKE_DIR=setup/subCmake/CMakeLists.txt

# copy sub CMakeLists.txt to sub sorce folder
for dir in $(ls $SRC_FOLDER)
do
        echo 'copy '$SUB_CMAKE_DIR' to '$SRC_FOLDER'/'$dir
        cp $SUB_CMAKE_DIR $SRC_FOLDER/$dir/CMakeLists.txt
done

# make dir build to build project
if [ ! -d build ];then
	mkdir build
else
   echo dir build exist
fi

# clear the include and lib folder
rm include -rf
rm lib -rf
rm bin -rf

# build project
cd build && cmake .. && make && cd .. 

cp subMake/make_bin.sh .
sh make_bin.sh
rm make_bin.sh
