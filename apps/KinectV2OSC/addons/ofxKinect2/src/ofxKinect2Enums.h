// @author sadmb
// @date 10,Jan,2014
#ifndef _OFX_KINECT2_ENUMS_H_
#define _OFX_KINECT2_ENUMS_H_

namespace ofxKinect2 {
	typedef enum
	{
		STATUS_OK = 0,
		STATUS_ERROR = 1,
		STATUS_NOT_IMPLEMENTED = 2,
		STATUS_NOT_SUPPORTED = 3,
		STATUS_NO_DEVICE = 6,
		STATUS_TIME_OUT = 102,
	} Status;

	typedef enum
	{
		SENSOR_NONE = 0,
		SENSOR_COLOR = 0x1,
		SENSOR_IR = 0x2,
		SENSOR_LONG_EXPOSURE_IR = 0x4,
		SENSOR_DEPTH = 0x8,
		SENSOR_BODY_INDEX = 0x10,
		SENSOR_BODY = 0x20,
		SENSOR_AUDIO = 0x40,
	} SensorType;

	typedef enum
	{
		PIXEL_FORMAT_NONE = 0,
		PIXEL_FORMAT_RGBA = 1,
		PIXEL_FORMAT_YUV = 2,
		PIXEL_FORMAT_BGRA = 3,
		PIXEL_FORMAT_BAYER = 4,
		PIXEL_FORMAT_YUY2 = 5
	} PixelFormat;

	typedef enum
	{
		DEVICE_STATE_OK = 0,
		DEVICE_STATE_ERROR = 1,
		DEVICE_STATE_NOT_READY = 2
	} DeviceState;
} // namespace ofxKinect2

#endif // _OFX_KINECT2_ENUMS_H_