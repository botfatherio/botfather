#!/bin/bash

##
# Configuration
##

SOURCE_DIR_PATH=$HOME/Source
CEF_DOWNLOAD_URL="http://opensource.spotify.com/cefbuilds/cef_binary_3.3578.1861.g1992780_linux64.tar.bz2"
CEF_TAR_BZ2_FILE=$(echo $CEF_DOWNLOAD_URL | cut -d "/" -f 5)
CEF_SRC_DIR_NAME=$(echo $CEF_TAR_BZ2_FILE | rev | cut -f 3- -d '.' | rev)
CEF_SRC_DIR_PATH=$SOURCE_DIR_PATH/$CEF_SRC_DIR_NAME
CEF_DEBUG_BUILD_DIR_PATH=$CEF_SRC_DIR_PATH/DebugBuild
CEF_RELEASE_BUILD_DIR_PATH=$CEF_SRC_DIR_PATH/ReleaseBuild
NUMBER_OF_MAKE_BUILD_JOBS=$(nproc)

##
# Functions
##

function continue_y_or_no {
	read -p "Continue? [y/N] " -n 1 -r
	echo
	if [[ ! $REPLY =~ ^[Yy]$ ]]
	then
		echo "Okay, bye bye"
		echo
	    [[ "$0" = "$BASH_SOURCE" ]] && exit 1 || return 1 # handle exits from shell or function but don't exit interactive shell
	fi
}

function build_CEF {
	BUILD_DIR_PATH=$1
	CMAKE_BUILD_TYPE=$2

	echo
	echo "Building " $CMAKE_BUILD_TYPE " version..."
	echo

	mkdir -p $BUILD_DIR_PATH
	cd $BUILD_DIR_PATH
	cmake -G "Unix Makefiles" -D USE_SANDBOX=NO -D CMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE .. 
	make -j $NUMBER_OF_MAKE_BUILD_JOBS libcef_dll_wrapper 
}

##
# Script | Config check prompt
##

echo
echo "## Your Configuration:## "
echo
echo "SOURCE_DIR_PATH:            " $SOURCE_DIR_PATH
echo "CEF_DOWNLOAD_URL:           " $CEF_DOWNLOAD_URL
echo "CEF_TAR_BZ2_FILE:           " $CEF_TAR_BZ2_FILE
echo "CEF_SRC_DIR_NAME:           " $CEF_SRC_DIR_NAME
echo "CEF_SRC_DIR_PATH:           " $CEF_SRC_DIR_PATH
echo "CEF_DEBUG_BUILD_DIR_PATH:   " $CEF_DEBUG_BUILD_DIR_PATH
echo "CEF_RELEASE_BUILD_DIR_PATH: " $CEF_RELEASE_BUILD_DIR_PATH
echo "NUMBER_OF_MAKE_BUILD_JOBS:  " $NUMBER_OF_MAKE_BUILD_JOBS
echo

continue_y_or_no

##
# Script | Preperation, CEF download and modification
##

mkdir -p $SOURCE_DIR_PATH

cd $SOURCE_DIR_PATH

wget -O $CEF_TAR_BZ2_FILE $CEF_DOWNLOAD_URL

tar xvfj $CEF_TAR_BZ2_FILE

cd $CEF_SRC_DIR_PATH

# Turn off that warnings are threaded as errors
sed -i 's/-Werror/#-Werror/g' cmake/cef_variables.cmake

##
# Script | Build CEF
##

build_CEF $CEF_DEBUG_BUILD_DIR_PATH "Debug"
build_CEF $CEF_RELEASE_BUILD_DIR_PATH "Release"

##
# Script | Done, inform user
##

echo
echo "#############################################################"
echo 
echo "Done. Don't forget to set the following environment variable:"
echo
echo "export CEF_ROOT="$SOURCE_DIR_PATH
echo
echo "#############################################################"
echo