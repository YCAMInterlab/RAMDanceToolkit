# MagPendulum_Hakoniwa_Hakovis

The example of RDTK to "MagPendulum" and visualization of "MagPendulum"

##  Arduino
**/Arduino/MagPendulum**

## HakoniwaMagPendulum

When the distance of two "ramNode" is below threshold, an electromagnet turn on.
The pendulum is attracted to an electromagnet.

### GUI

**ON/OFF_MAGNET1** toggle of electromagnet1

**ON/OFF_MAGNET2** toggle of electromagnet2

**Distance Threshold** threshold

### OSC Output
**/dp/hakoniwa/MagPendulum**

## HakoVisMagPendulum

Record the movement of the pendulum in buffers. The buffers make multi circles move and multi circles makes quad meshes.

### OSC Input

**port**: 10000

#### from CameraUnit
**/dp/cameraUnit/MagPendulum/contour/boundingRect**: see the reference of CameraUnit


### GUI

**scale** scale for xy coordinate of the pendulum. 

