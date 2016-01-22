# MagPendulum_Hakoniwa_Hakovis

This example demonstates the MagPendulum Hakoniwa, how it works with the RDTK and its visualization.

##  Arduino
**/Arduino/MagPendulum**

## HakoniwaMagPendulum

When the distance between two "ramNode" objects is below a threshold, an electromagnet is turned on. 

This causes the metal pendulum to be attracted to the electromagnet.

### GUI

**ON/OFF_MAGNET1** Toggle electromagnet1

**ON/OFF_MAGNET2** Toggle electromagnet2

**Distance Threshold** Distance threshold value for toggling electromagnet

### OSC Output
**/dp/hakoniwa/MagPendulum**

## HakoVisMagPendulum

The movement of the pendulum is recorded in buffers. The data of the movement in the buffers results in multi-circles. The multi-circles result in quad meshes.

### OSC Input

**port**: 10000

#### from CameraUnit
**/dp/cameraUnit/MagPendulum/contour/boundingRect**: See the [CameraUnit](https://github.com/YCAMInterlab/RAMDanceToolkit/tree/master/apps/CameraUnit) reference for more information.


### GUI

**scale** Scale for the x and y coordinates of the pendulum. 

