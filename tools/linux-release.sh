#!/bin/sh
# -= stand-alone release builder for Linux =-


# Go to the root directory of the git repository
PATH_TO_ROOT=$(git rev-parse --show-cdup)
if [ $PATH_TO_ROOT ]; then
	cd $PATH_TO_ROOT
fi

# Get project name from directory and ensure lowercase
PROJECT_NAME=$(basename $(pwd) | tr '[A-Z]' '[a-z]')

# Ask which version to build (will be included in filename)
read -p "Enter version number: " VERSION

# Name of the directory which will contain the release
TARGET_NAME="$PROJECT_NAME"_"$VERSION"-linux
echo Packaging  $TARGET_NAME.tar.gz...

# Copy SFML libraries
mkdir lib32
cp /usr/local/lib/libsfml* lib32

# Copy launcher script
cp tools/kroniax-launcher.sh .

# Create tarball from the bin directory and rename it as TARGET_NAME
ARCHIVE_CONTENT="./Kroniax ./README.md ./LICENSE.txt ./kroniax-launcher.sh ./config ./data ./levels ./lib32"
tar -czf $TARGET_NAME.tar.gz $ARCHIVE_CONTENT --transform=s/\./$TARGET_NAME/ 

# Clean up the mess 
rm -r lib32
rm kroniax-launcher.sh

echo Done!

