# RamDanceToolkit

This repository is the primary source for the "[Reactor for Awareness in Motion](http://www.ycam.jp/en/performingarts/2013/02/ram-workshop.html)" Joint Research Project with Yoko Ando at [Yamaguchi Center for Arts and Media](http://www.ycam.jp). The other source is the [Motioner](https://github.com/YCAMInterlab/Motioner) application for accessing data from the custom inertial motion sensor system.

The toolkit itself is built using a project file in `libs/`. This toolkit is used by applications in `apps/` such as `AllScenes` which demonstrates a number of scenes built with the RAM Toolkit. `addons/` contains submodules that refer to addons which are not shipped with openFrameworks.

`dev/` contains work in progress, while `experiment/` is unused but interesting code.

`track/` contains an absolute positioning app and OpenNI tracking code.

## Tracking

Apps related to tracking can be found in the `track/` folder.

### CircleTracking

This app integrates data from multiple Kinects in order to track a single bright point in real time. We've tested this system with up to three Kinects, using libfreenect via ofxKinect, on a desktop Mac Pro. The best target for tracking is a retroreflective ball as used in motion capture, but a diffuse infrared LED will also work.

After opening the app, six streams will be displayed from the three Kinects, showing video and depth information. Pressing the "Calibrate" button under the "Background" heading will sample any bright points in the background and ignore them, in order to keep the tracking as robust as possible by avoiding distractions. After the background is calibrated, press "Calibrate" under the "Registration" heading. This will begin the spatial calibration procedure. Slowly wave the tracked point through the space. For every frame in which the tracked point is visible from the first camera and another camera, a note will be made about that relationship. As more relationships are stored, the app will attempt to find a configuration (position and orientation) of each of the sensors that explains the data best. To speed up calibration, you can increase the "Calibration rate" parameter, but this can also mean that the data is recorded from the different sensors at slightly different times, leading to noise in the calibration.

The calibration data is automatically stored in the `data/` folder using a filename that corresponds to the serial number of the Kinect. When the app is opened the next time, these calibration files are loaded automatically. However, the background must be re-calibrated.

If more than one point is visible, the system will not be able to reconstruct the positions. Further work must be done to adapt this system to multiple point tracking, as well as sending data over OSC. However, the primary difficulty is currently that the 3d data from the tracking system needs to be tracked more accurately. Right now the 3d positions are extracted by looking at the area around the tracked point, and averaging those locations. Another technique, such as fitting a plane and ignoring outliers in the averaging, may lead to more robust data.

The primary innovation of the CircleTracking app is the use of OpenCV's `estimateAffine3D()` function, which solves for the position and orientation of the sensors based on an original data set.