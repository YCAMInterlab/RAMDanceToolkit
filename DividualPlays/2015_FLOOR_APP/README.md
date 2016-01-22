# 2015_FLOOR_APP

床打ち用のRDTK。
シーンは神田マップと呼ばれる解像度可変のグリッドのみ(dpVisIce)

## Max patchとの連動
**DividualPlays/maxpat/icePlayer.maxpat**
録画したIceのムービーを再生して解析結果をOSCで出力。
**/dp/cameraUnit/Ice/mean** フォーマットはCameraUnit参照。

## OSC入力

**port**: 10000

#### CameraUnitから
**/dp/cameraUnit/Ice/mean**: フォーマットはCameraUnit参照。

**/dp/cameraUnit/sceneState/**: フォーマットはCameraUnit参照。

## GUI

### floorWarper

**Save** warpの保存

**Load** warpの読み込み

**Reset** warpの設定をクリア

**Debug** warpの調整用の線を表示。warperにキーとマウスがバインドされる


### dpVisIce

**div**: マップの解像度

**all**: onでマップが全て表示。offでランダマイズを無視してマップが非表示

**lineWidth**: ラインの太さ

**extendThresh**: /dp/cameraUnit/Ice/mean　の値と比較して表示されるマップのスレッショルド

**extendNum**: /dp/cameraUnit/Ice/mean で渡される値を手動で設定 

**randomize**: extendNumに応じてマップがランダマイズ

**externalOsc**: /dp/cameraUnit/Ice/mean の影響のon/off


## キーバインド

**escape**: 終了

**f**: フルスクリーン

**Spcae**: ウィンドウをメインディスプレイの下に持って行ってフルスクリーン

## キーバインド(dpHakoVisIceMap)

**w**: マップを消す。モードをクリア

**e**: allとバインド

**r**: divを５に

**t**: divを13に

**y**: /dp/cameraUnit/Ice/mean を有効にするか否か


## キーとマウスバインド(ofxQuadWarp)

既存のaddonと同じ。debugモード時のみバインド

**https://github.com/julapy/ofxQuadWarp**



## 関連箱庭
**Tornado** Tornado時には自動的にマップが消える

**Ice** /dp/cameraUnit/Ice/mean の値でマップがランダマイズ