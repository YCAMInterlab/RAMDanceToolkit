#CameraUnit
CameraUnit is an application for analyzing Hakoniwa images.

##Outline
This application gets images from video and/or a web camera, applies fx for analysis and send analyzed data via OSC messages.

##CvEffect
The CvEffect UI applies fx to image sources.

**Background**：This applies a background difference effect. Pressing the UI button uses the current frame as the base for the background difference effect. 

**Warp**：This applies an unwarp perspective effect.  A yellow wireframe is applied on an image source, and you can drag around on the frame to apply unwarp perspective.

**Blur**：This applies a blur effect.

**Invert**：This inverts the pixel colors.

**FrameDiff**：This applies a frame difference effect. The result is any difference from the previous frame.

**Threshold**：This returns a threholded image.

**AdaptiveThreshold**：This returns an adaptive thresholded image.

**AccumelateWeight**：This returns an accumlated image.

**Erode**：This erodes the white area (makes it smaller).

**Dilate**：This dilates the white area (makes it bigger).

**Canny**：This picks out outlines and edges.


##Analysis
This section documents how to analyze an image source from the CvEffects unit, and send the analyzed data via OSC messages.

**Hakoniwa Name**：Hakoniwa's name. This name is used as the OSC address and the preset names.

**Load Preset**：Load the data for the preset Hakoniwa.

**Save Preset**：Save presets of CvEffects and analysis options.

**Send OSC**：Enable the sending of analyzed data via OSC messages.

##OSC format
####ContourFinder
***/dp/cameraUnit/hakoniwaName/contour/boundingRect***

**[Int] numRects, [Int]Label, [Float]rects-X, [Float]rects-Y, [Float]rects-W, [Float]rects-H**

Num of args --- 5 * n + 1

Type：Rectangle from blobs.

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

Type：Average color of image source.

---
####optFlow(pyrLK)

***/dp/cameraUnit/hakoniwaName/features***

**[Int] numPt, [Float]X, [Float]Y,[Float]vec_X,[Float]vec_Y,,,,**

***/dp/cameraUnit/hakoniwaName/vector***

**[Float] vec_X, [Float]vec_Y ,,,**

Num of args --- 2 * 10

Type：Average movement.

***/dp/cameraUnit/hakoniwaName/vector/length***

**[Float] Length1 ,,, [Float] Length10**

Num of args --- 10

Type：Length of average movement.

***/dp/cameraUnit/hakoniwaName/vector/total***

**[Float] VecX, [Float] VecY**

Num of args --- 2

Type：Total movement.

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
