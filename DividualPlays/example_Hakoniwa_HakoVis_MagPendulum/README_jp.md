# MagPendulum_Hakoniwa_Hakovis

MagPendulumへの送りとビジュアライズのexample。

##  Arduino
**/Arduino/MagPendulum**

## HakoniwaMagPendulum

ramNode２点間の距離が閾値以下になった際に、電磁石がオンになり振り子が吸い寄せられる。

### GUI

**ON/OFF_MAGNET1** 電磁石1のon/off

**ON/OFF_MAGNET2** 電磁石2のon/off

**Distance Threshold** 距離の閾値。

### OSC出力
**/dp/hakoniwa/MagPendulum**

## HakoVisMagPendulum

磁石に吸い寄せられる振り子の位置をトラッキングして記録。その履歴で動く複数の円が四角メッシュを作る。

### OSC入力

**port**: 10000

#### CameraUnitから
**/dp/cameraUnit/MagPendulum/contour/boundingRect**: フォーマットはCameraUnit参照。


### GUI

**scale** /dp/cameraUnit/MagPendulum/contour/boundingRect のXY座標をスケーリング

## Reference Movie

under construction...