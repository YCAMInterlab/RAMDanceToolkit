#CameraUnit
CameraUnit is an application for analysis hakoniwa images.

##outline
This application get images from video and web camera, apply fx for analysis and send analyzed data with OSC messages.

##CvEffect
CvEffect UI apply fx to image sources.

**Background**：It takes background difference.You push the button,then background image will be taken from current frame.

**Warp**：Apply unwarp perspective.You can pick Yellow wire frame on image source, and drag frame to apply unwarp perspective.

**Blur**：Apply blur effect.

**Invert**：Make pixel colos inverse.

**FrameDiff**：Apply frame difference effect. It returns difference from previous frame.

**Threshold**：Returns thresholded image.

**AdaptiveThreshold**：Returns adaptive thresholded image.

**AccumelateWeight**：Returns accumlated image.

**Erode**：Erode white area.

**Dilate**：Dilate white area.

**Canny**：Picks outlines.


##Analysis
Analyze image source from CvEffects unit, and send analyzed datas with OSC messages.

**Hakoniwa Name**：Hakoniwa's name. This name is used as OSC address and preset names.

**Load Preset**：Load preset Hakoniwa name's data.

**Save Preset**：Save preset cvEffects and analysis options.

**Send OSC**：Enable OSC Sending.

##Analyze way and OSC format
####ContourFinder
***/dp/cameraUnit/hakoniwaName/contour/boundingRect***

**[Int] numRects, [Int]Label, [Float]rects-X, [Float]rects-Y, [Float]rects-W, [Float]rects-H**

Num of args --- 5 * n + 1

概要：Rectangle from blobs.

***/dp/cameraUnit/hakoniwaName/contour/blob***

**[Int] numBlob, [Int]numPts, [Float]pt-X, [Float]pt-Y, .....**

Type：Vertex positions of outlines.

***/dp/cameraUnit/hakoniwaName/contour/area***

**[Int] numAreas, [float]area1, [float]area2 ,,,**

Type：Area of outlines.

---
####mean
***/dp/cameraUnit/hakoniwaName/mean***

**[Int] mean_R, [Int]mean_G, [Int]mean_B, [Int]mean_Brightness**

Type：Avarage color of image source.

---
####optFlow(pyrLK)

***/dp/cameraUnit/hakoniwaName/features***

**[Int] numPt, [Float]X, [Float]Y,[Float]vec_X,[Float]vec_Y,,,,**

***/dp/cameraUnit/hakoniwaName/vector***

**[Float] vec_X, [Float]vec_Y ,,,**

Num of args --- 2 * 10

Type：Avarage of movement.

***/dp/cameraUnit/hakoniwaName/vector/length***

**[Float] Length1 ,,, [Float] Length10**

Num of args --- 10

Type：Avarage of movement's length.

***/dp/cameraUnit/hakoniwaName/vector/total***

**[Float] VecX, [Float] VecY**

Num of args --- 2

Type：Total of movement.

---
####Pixelate
***/dp/cameraUnit/hakoniwaName/pixelate***

***/dp/cameraUnit/hakoniwaName/pixelate/r***

***/dp/cameraUnit/hakoniwaName/pixelate/g***

***/dp/cameraUnit/hakoniwaName/pixelate/b***

**[Int32]width, [Int32]height, [Int64]pixelBit,,,,,**

Num of args --- 2 + width * height / 64

Type：mosaic data of image source.

Sample:

	vector<bool> pixels;

	pix_w = m.getArgAsInt32(0);
	pix_h = m.getArgAsInt32(1);

	pixels.clear();
	pixels.assign(m.getArgAsInt32(0) * m.getArgAsInt32(1), false);

	for (int64_t i = 0;i < pixels.size();i++){
		int64_t bt = m.getArgAsInt64(2 + i / 64);
		int64_t targetBit = int64_t(1) << (i % 64);
		pixels[i] = (bt & targetBit) > 0;
	}
