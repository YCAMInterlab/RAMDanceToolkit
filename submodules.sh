#!/bin/bash

# usage: $ ./submodules.sh

echo "[start submodule init]"
git submodule init
echo "end submodule init."


echo "[start submodule update]"
git submodule update
echo "end submodule update."
