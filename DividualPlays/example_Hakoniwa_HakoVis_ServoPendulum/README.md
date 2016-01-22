# ServoPendulum_Hakoniwa_Hakovis

This example demonstates the ServoPendulum Hakoniwa, how it works with the RDTK and its visualization.

## HakoniwaServoPendulum

Currently this pendulum moves on its own.

The distance betwee two "ramNode" objects is mapped to the movable range of the pendulum.

### GUI

**speed** Seconds of 1 round trip of the pendulum.

**lengthMin** Minimum distance of two "ramNode" objects

**lengthMax** Maximum distance of two "ramNode" objects

**range** Movable range of the pendulum(0 ~ 90)

**angle** Current angle of the pendulum(-90 ~ 90)

### OSC Output
**/dp/hakoniwa/servoPendulum**

## HakoVisServoPendulum

This visualization tracks the vector of the pendulum's tip. 

Previous recordings of the angle and length of the vector are used to create a 3D visualization.

### OSC Input

**port**: 10000

#### from CameraUnit

**/dp/cameraUnit/ServoPendulum/vector/total**: See the [CameraUnit](https://github.com/YCAMInterlab/RAMDanceToolkit/tree/master/apps/CameraUnit) reference for more information.

### GUI

**scale** /dp/cameraUnit/ServoPendulum/vector/total Scale of the vector length

## Reference Movie

Coming soon! :)
