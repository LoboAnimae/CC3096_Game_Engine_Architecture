#!/bin/sh

cd src;
mkdir $1;
cd $1;
touch $1.cpp;
echo "#include \"$1.h\"" >> $1.cpp;

touch $1.h;
echo "#ifndef $1_PROJECT_H" >> $1.h;
echo "#define $1_PROJECT_H" >> $1.h;
echo "" >> $1.h;
echo "" >> $1.h;
echo "" >> $1.h;
echo "#endif" >> $1.h;
touch CMakeLists.txt;
echo "add_library($1 $1.cpp $1.h)" >> CMakeLists.txt;
cd ..;
cd ..;
echo "add_subdirectory(src/$1)" >> CMakeLists.txt;
echo "target_include_directories(\${PROJECT_NAME} PUBLIC src/$1)" >> CMakeLists.txt;
echo "target_link_directories(\${PROJECT_NAME} PRIVATE src/$1)" >> CMakeLists.txt;
echo "target_link_libraries(\${PROJECT_NAME} $1)" >> CMakeLists.txt;
