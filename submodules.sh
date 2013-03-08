#!/bin/bash

# usage: $ ./submodules.sh


if [ -r .gitmodules ]; then

	# submodule init & update
	echo "[start submodule init]"
	git submodule init
	echo "end submodule init."

	echo "[start submodule update]"
	git submodule update
	echo "end submodule update."

else

	# clone from repo
	cd ./addons

	echo "[start clone five addons from github repos]"
	git clone git@github.com:satoruhiga/ofxBt.git
	git clone git@github.com:rezaali/ofxUI.git
	git clone git@github.com:satoruhiga/ofxInteractivePrimitives.git
	git clone git@github.com:kylemcdonald/ofxCv.git
	git clone git@github.com:YCAMInterlab/ofxNodeArray.git

	echo "end clone addons"
	cd ../
fi


# apply ofxUI patch

echo "[start running patch for modify ofxUI]"
cd addons/ofxUI
patch -p0 < ../ofxUI.patch
cd ../../
echo "end apply patch."