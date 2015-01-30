#Presentor


##起動
RDTK内のシーン”Presentor”をオンにする

##箱庭選択
###camearaUnitを介した正規の手続きで切り替え
- 画面右側の一覧から指定の箱庭を選択
- TargetHakoniwaテキストボックスがシーン名になっている事を確認
- Enableチェックを有効化、ターゲットのディスプレイA/Bを選択
- Sendボタン押下で切り替え

###RDTKに直接シーン選択の信号を送る
画面左のGUIから、動揺の手順でシーン選択、Send押下で切り替え。**但しCameraUnitを介さないので切り替わるのはRDTKのみ**

##motionExtractorの連携
###基本操作
連携には、Extractor項目を使用

**Syncボタン**：現在のExtractorデータをRDTKに送信する。

**Getボタン**：RDTKのExtractorデータを取得する。

**ViewSimpleボタン**：コンソールを一部非表示にして編集しやすいようにする

###RemotePresetingオフ時
切り替えと同時にRDTKからExtractorをゲットする。

###RemotePresetingオン時
切り替えと同時にローカルのプリセットを読み込み、RDTKにExtractor情報を送信する。