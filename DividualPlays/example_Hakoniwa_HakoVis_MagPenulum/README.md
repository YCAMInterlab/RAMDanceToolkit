# Example_Hakoniwa_Hakovis

MagPendulumへの送りとビジュアライズのexample

## HakoniwaMagPendulum

ramNode２点間の距離が一定以上近づいた際に、電磁石がオンになり振り子が吸い寄せられる。

### GUI

**speed** 振り子が指定の幅を１往復する秒数

**lengthMin** ramNode２点間の距離の最小値。

**lengthMax** ramNode２点間の距離の最大値

**range** 振り子の可動範囲(0 ~ 90)

**angle** 現在の振り子の角度

### OSC出力
**/dp/hakoniwa/servoPendulum**

## HakoVisMagPendulum

磁石に吸い寄せられる振り子の位置をトラッキングして記録。その履歴で動く複数の円が四角メッシュを作る。

### OSC入力

**port**: 10000

#### CameraUnitから
**/dp/cameraUnit/MagPendulum/contour/boundingRect**: フォーマットはCameraUnit参照。


### GUI

**scale** /dp/cameraUnit/MagPendulum/contour/boundingRect のXY座標をスケーリング

