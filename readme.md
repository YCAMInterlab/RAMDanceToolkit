# RAMDanceToolkit

![](https://raw.github.com/wiki/YCAMInterlab/RAMDanceToolkit/Images/Home/ram.png)

RAM Dance Toolkit is a C++ creative coding toolkit to create environments for dancers. This toolkit contains a GUI and functions to access, recognize, and process motion data to support creation of various environmental conditions (called “scene”) and gives realtime feedbacks to dancers using code in an easy way. Toolkit uses openFrameworks, a software development toolkit for artists, which means users can use functions from both RAM Dance Toolkit and openFrameworks. RAM Dance Toolkit will also be published as an application for Windows and Mac. As an application, users also can choreograph or rehearse with previously programmed scenes.

For more details, please see [RAMDanceToolkit wiki](https://github.com/YCAMInterlab/RAMDanceToolkit/wiki)

詳しい情報は[RAMDanceToolkit wiki](https://github.com/YCAMInterlab/RAMDanceToolkit/wiki)を参照してください。



## Licenses

RAMDanceToolkit by YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald is licensed under the [Apache License, Version2.0](http://www.apache.org/licenses/LICENSE-2.0.html)

    Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

<!-- 

## Tracking

Some apps related to tracking can be found in the `dev/` folder.


### CircleTracking

This app integrates data from multiple Kinects in order to track a single bright point in real time. We've tested this system with up to three Kinects, using libfreenect via ofxKinect, on a desktop Mac Pro. The best target for tracking is a retroreflective ball as used in motion capture, but a diffuse infrared LED will also work.

After opening the app, six streams will be displayed from the three Kinects, showing video and depth information. Pressing the "Calibrate" button under the "Background" heading will sample any bright points in the background and ignore them, in order to keep the tracking as robust as possible by avoiding distractions. After the background is calibrated, press "Calibrate" under the "Registration" heading. This will begin the spatial calibration procedure. Slowly wave the tracked point through the space. For every frame in which the tracked point is visible from the first camera and another camera, a note will be made about that relationship. As more relationships are stored, the app will attempt to find a configuration (position and orientation) of each of the sensors that explains the data best. To speed up calibration, you can increase the "Calibration rate" parameter, but this can also mean that the data is recorded from the different sensors at slightly different times, leading to noise in the calibration.

The calibration data is automatically stored in the `data/` folder using a filename that corresponds to the serial number of the Kinect. When the app is opened the next time, these calibration files are loaded automatically. However, the background must be re-calibrated.

If more than one point is visible, the system will not be able to reconstruct the positions. Further work must be done to adapt this system to multiple point tracking, as well as sending data over OSC. However, the primary difficulty is currently that the 3d data from the tracking system needs to be tracked more accurately. Right now the 3d positions are extracted by looking at the area around the tracked point, and averaging those locations. Another technique, such as fitting a plane and ignoring outliers in the averaging, may lead to more robust data.

The primary innovation of the CircleTracking app is the use of OpenCV's `estimateAffine3D()` function, which solves for the position and orientation of the sensors based on an original data set.

-->

## Change log
### v1.2.0

- add **ramMotionExtractor** : It makes you easier to edit node selection. check [wiki](https://github.com/YCAMInterlab/RAMDanceToolkit/wiki/RAM-API-Reference-MotionExtractor_jp) how to use it.
- change default floor pattarn from "GRID" to "NONE".

### v1.1.0

- add **ramReceiverTag & ramOSCManager** : for receiver OSC in scenes & other classes.
- add **ramCommunicationManager** : for control parameters from other devices on OSC.
