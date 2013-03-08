#!/bin/bash

# usage: $ ./submodules.sh


# submodule init & update
echo "[start submodule init]"
git submodule init
echo "end submodule init."

echo "[start submodule update]"
git submodule update
echo "end submodule update."


# apply ofxUI patch

echo "[start running patch for modify ofxUI]"
cd addons/ofxUI
patch -p0 < ../ofxUI.patch
cd ../../
echo "end apply patch."