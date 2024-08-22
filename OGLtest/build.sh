#!/bin/bash

# Build the CMake configuration
cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "MinGW Makefiles"

cd build
PROJ_NAME=$(grep "^CMAKE_PROJECT_NAME:STATIC=" CMakeCache.txt | sed 's/CMAKE_PROJECT_NAME:STATIC=//')

# Delete the old executable
rm -f $PROJ_NAME

# Build the executable
cmake --build .

# Run the program if it exists
if [ -f $PROJ_NAME ]; then
	echo ' ~ Successfully found the '$PROJ_NAME' executable'
else
	echo ' ~ Build ERROR: Could not locate executable: '$PROJ_NAME''
fi

# Exit terminal on key press
echo " ~ Press any key to exit"
while [ true ] ; do
	read -t 3 -n 1
if [ $? = 0 ] ; then
	exit ;
fi
done