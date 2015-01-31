# ServoPendulum_Hakoniwa_Hakovis

箱庭へのデータの送りとビジュアライズのexample。ここではServoPendulumを取り上げる。

## HakoniwaServoPendulum

ramNode２点間の距離が、振り子が一定時間内に動く角度の範囲(-90 ~ 90)を決定。

lengthMinとlengthMaxで設定した距離が、0~90にマッピングされる。

### GUI

**speed** 振り子が指定の幅を１往復する秒数

**lengthMin** ramNode２点間の距離の最小値。

**lengthMax** ramNode２点間の距離の最大値

**range** 振り子の可動範囲(0 ~ 90)

**angle** 現在の振り子の角度

### OSC出力
**/dp/hakoniwa/servoPendulum**

## HakoVisServoPendulum

ServoPendulumの先端をカメラでトラッキング。取得したvectorの角度と強さの履歴が3DCGに変換される。

### OSC入力

**port**: 10000

#### CameraUnitから
**/dp/cameraUnit/ServoPendulum/vector/total**: フォーマットはCameraUnit参照。


### GUI

**scale** /dp/cameraUnit/ServoPendulum/vector/total のlength()をスケーリング

