// @author sadmb
// @date 10,Jan,2014
#ifndef _OFX_KINECT2_TYPES_H_
#define _OFX_KINECT2_TYPES_H_

#include "ofxKinect2Enums.h"
#include "Kinect.h"

namespace ofxKinect2 {
#define MAX_STR 256
#define MAX_SENSORS 10
	typedef union
	{
		IColorFrameReader* p_color_frame_reader;
		IDepthFrameReader* p_depth_frame_reader;
		IBodyFrameReader* p_body_frame_reader;
		IBodyIndexFrameReader* p_body_index_frame_reader;
		IAudioBeamFrameReader* p_audio_beam_frame_reader;
		IInfraredFrameReader* p_infrared_frame_reader;
		ILongExposureInfraredFrameReader* p_long_exposure_infrared_frame_reader;
	} StreamHandle;

	typedef union
	{
		IColorCameraSettings* p_color_camera_settings;
	} CameraSettingsHandle;

	typedef union
	{
		IKinectSensor* kinect2;
	} DeviceHandle;

	typedef struct
	{
		PixelFormat pixel_format;
		int resolution_x;
		int resolution_y;
		float fps;
	} Mode;

	typedef struct
	{
		int data_size;
		void* data;

		SensorType sensor_type;
		UINT64 timestamp;
		int frame_index;

		int width;
		int height;

		float horizontal_field_of_view;
		float vertical_field_of_view;
		float diagonal_field_of_view;

		Mode mode;
		int stride;
	} Frame;

	typedef struct
	{
		SensorType sensor_type;
	} SensorInfo;

	typedef struct
	{
		char uri[MAX_STR];
		char vendor[MAX_STR];
		char name[MAX_STR];
	} DeviceInfo;

} // namespace ofxKinect2

#endif // _OFX_KINECT2_TYPES_H_