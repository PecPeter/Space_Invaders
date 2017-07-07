#/bin/bash

echo "Deleting old files..."
rm -r ./include/mathLib/* ./lib/libmathLib*

echo "Copying new files..."
cp -r ~/Programming/mathLib/include/* ./include
cp -r ~/Programming/mathLib/lib/* ./lib

echo "Finished updating files"
