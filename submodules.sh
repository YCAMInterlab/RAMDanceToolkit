#!/bin/bash

# usage: $ ./submodules.sh


if [ -r .git ]; then

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

	echo "[couldn't found ./.gitmodule. start clone five addons from github repos instead of git submodule update]"

	# ofxBt
	git clone git@github.com:motoishmz/ofxBt.git
	cd ofxBt
	cd ../

	# ofxUI
	git clone git@github.com:rezaali/ofxUI.git
	cd ofxUI
	git checkout 1832f2a
	cd ../

	# ofxInteractivePrimitives
	git clone git@github.com:satoruhiga/ofxInteractivePrimitives.git
	cd ofxInteractivePrimitives
	git checkout 987205e
	cd ../

	# ofxCv
	git clone git@github.com:kylemcdonald/ofxCv.git
	cd ofxCv
	git checkout 115deaf
	cd ../

	# ofxNodeArray
	git clone git@github.com:YCAMInterlab/ofxNodeArray.git
	cd ofxNodeArray
	git checkout b21dcd5
	cd ../

	# ofxQuadWarp
	git clone git@github.com:julapy/ofxQuadWarp.git
	cd ofxQuadWarp
	git checkout a45ff5e
	cd ../

	# ofxDelaunay
	git clone git@github.com:obviousjim/ofxDelaunay.git
	cd ofxDelaunay
	git checkout 957f902
	cd ../

	echo "end clone addons"
	cd ../
fi