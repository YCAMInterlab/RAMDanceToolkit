# ServoPendulum_Hakoniwa_Hakovis

The example of RDTK to "ServoPendulum" and visualization of "ServoPendulum"

## HakoniwaServoPendulum

The pendulum moves automatically.

The distance of two "ramNode" is mapped to the movable range of the pendulum.

### GUI

**speed** seconds of 1 round trip.

**lengthMin** minimum distance of two "ramNode"

**lengthMax** maximum distance of two "ramNode"

**range** movable range of the pendulum(0 ~ 90)

**angle** current angle of the pendulum(-90 ~ 90)

### OSC Output
**/dp/hakoniwa/servoPendulum**

## HakoVisServoPendulum

Tracking vector of the pendulum's tip. The history of angle and length of vector makes 3D computer graphics.

### OSC Input

**port**: 10000

#### from CameraUnit
**/dp/cameraUnit/ServoPendulum/vector/total**: see the reference of CameraUnit


### GUI

**scale** /dp/cameraUnit/ServoPendulum/vector/total scale for the vector length

## Reference Movie

under construction...