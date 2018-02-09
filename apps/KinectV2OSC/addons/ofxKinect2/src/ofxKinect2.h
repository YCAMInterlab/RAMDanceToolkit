// @author sadmb
// @date 3,Jan,2014
// modified from ofxNI2.cpp of ofxNI2 by @satoruhiga
#ifndef OFX_KINECT2_H
#define OFX_KINECT2_H

#include "ofMain.h"
#include "ofxKinect2Types.h"
#include "utils/DoubleBuffer.h"

#include <assert.h>

namespace ofxKinect2
{
	void init();
	class Device;
	class Stream;
	class Mapper;

	class ColorStream;
	class DepthStream;
	class IrStream;
	class BodyIndexStream;

	class Body;
	class BodyStream;
	
	class Recorder;

	template<class Interface>
	inline void safe_release(Interface *& p_release)
	{
		if(p_release){
			p_release->Release();
			p_release = NULL;
		}
	}
} // namespace ofxKinect2

// device
class ofxKinect2::Device
{
	friend class ofxKinect2::Stream;

public:
	ofEvent<int> onUpdateStream;

	Device();
	~Device();

	bool setup();
	bool setup(string kinect2_file_path);

	void exit();

	void update();
/*
	bool startRecording(string filename = "");
	void stopRecording();
	bool isRecording() const { return recorder != NULL; }
	*/

	bool isOpen() const
	{
		if (device.kinect2 == NULL) return false;
		bool b = false;
		device.kinect2->get_IsOpen((BOOLEAN*)&b);
		return b;
	}

	DeviceHandle& get() { return device; }
	const DeviceHandle& get() const { return device; }

protected:
	DeviceHandle device;
	vector<ofxKinect2::Stream*> streams;

	Recorder* recorder;
};

class ofxKinect2::Recorder
{
};

// strneam
class ofxKinect2::Stream : public ofThread
{
	friend class ofxKinect2::Device;

public:
	virtual ~Stream() { }

	virtual void exit();

	virtual bool open();
	virtual void close();

	virtual void update();
	virtual bool updateMode();

	bool isOpen() const
	{
		bool b = (stream.p_audio_beam_frame_reader != NULL) || (stream.p_body_frame_reader != NULL) || (stream.p_body_index_frame_reader != NULL) || (stream.p_color_frame_reader != NULL) || (stream.p_depth_frame_reader != NULL)
			|| (stream.p_infrared_frame_reader != NULL) || (stream.p_long_exposure_infrared_frame_reader != NULL);
		return b;
	}

	int getWidth() const;
	virtual bool setWidth(int v);
	int getHeight() const;
	virtual bool setHeight(int v);
	virtual bool setSize(int width, int height);



	ofTexture& getTexture() { return tex; }
	const ofTexture& getTexture() const { return tex; }
	OF_DEPRECATED_MSG("Use getTexture() instead ", ofTexture& getTextureReference() { return tex; });

	int getFps();
	bool setFps(int v);

	void setMirror(bool v = true);
	bool isMirror();

	float getHorizontalFieldOfView();
	float getVerticalFieldOfView();
	float getDiagonalFieldOfView();

	inline bool isFrameNew() const { return is_frame_new; }
	inline uint64_t getFrameTimestamp() const { return kinect2_timestamp; }

	void draw(float x = 0, float y = 0);
	virtual void draw(float x, float y, float w, float h);

	operator StreamHandle&() { return stream; }
	operator const StreamHandle&() const { return stream; }

	StreamHandle& get() { return stream; }
	const StreamHandle& get() const { return stream; }

	CameraSettingsHandle& getCameraSettings() { return camera_settings; }
	const CameraSettingsHandle& getCameraSettings() const { return camera_settings; }

protected:
	Frame frame;
	StreamHandle stream;
	CameraSettingsHandle camera_settings;
	uint64_t kinect2_timestamp, opengl_timestamp;

	bool is_frame_new, texture_needs_update;

	bool is_mirror;

	ofTexture tex;
	Device* device;

	Stream() {}

	void threadedFunction();

	bool setup(Device& device, SensorType sensor_type);
	virtual bool readFrame();
	virtual void setPixels(Frame frame);
};

class ofxKinect2::ColorStream : public ofxKinect2::Stream
{
public:
	ColorStream() : Stream() {}
	~ColorStream() {}

	bool setup(ofxKinect2::Device& device)
	{
		buffer = NULL;
		return Stream::setup(device, SENSOR_COLOR);
	}
	void exit()
	{
		Stream::exit();
		if (buffer)
		{
			delete[] buffer;
			buffer = NULL;
		}
	}
	bool open();
	void close();

	void update();
	bool updateMode();

	bool setWidth(int v);
	bool setHeight(int v);
	bool setSize(int width, int height);
    
    ofColor getColorAt(int x, int y);
    ofColor getColorAt(ofVec2f color_point);
	ofFloatColor getFloatColorAt(int x, int y);
	ofFloatColor getFloatColorAt(ofVec2f color_point);

	ofPixels& getPixels() { return pix.getFrontBuffer(); }
	const ofPixels& getPixels() const { return pix.getFrontBuffer(); }
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofPixels& getPixelsRef() { return pix.getFrontBuffer(); });

	int getExposureTime();
	int getFrameInterval();
	float getGain();
	float getGamma();

	/*
	void setAutoExposureEnabled(bool yn = true) {  }
	bool getAutoExposureEnabled() {  }

	void setAutoWhiteBalanceEnabled(bool yn = true) {  }
	bool getAutoWhiteBalanceEnabled() {  }
	*/

protected:
	DoubleBuffer<ofPixels> pix;
	unsigned char* buffer;

	bool readFrame();
	void setPixels(Frame frame);

};

class ofxKinect2::DepthStream : public ofxKinect2::Stream
{
public:
	DepthStream() : Stream() {}
	~DepthStream() {}

	bool setup(ofxKinect2::Device& device)
	{
		near_value = 50;
		far_value = 10000;
		return Stream::setup(device, SENSOR_DEPTH);
	}

	bool open();
	void close();

	void update();
	bool updateMode();

    unsigned short getDepthAt(int x, int y);
	unsigned short getDepthAt(ofVec2f depth_point);

	ofShortPixels& getPixels() { return pix.getFrontBuffer(); }
	const ofShortPixels& getPixels() const { return pix.getFrontBuffer(); }
	ofShortPixels& getPixels(int _near, int _far, bool invert = false);
	const ofShortPixels& getPixels(int _near, int _far, bool invert = false) const;
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofShortPixels& getPixelsRef() { return pix.getFrontBuffer(); });
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofShortPixels getPixelsRef(int _near, int _far, bool invert = false););

	inline void setNear(float _near) { near_value = _near; }
	inline float getNear() const { return near_value; }

	inline void setFar(float _far) { far_value = _far; }
	inline float getFar() const { return far_value; }

	inline void setInvert(float invert) { is_invert = invert; }
	inline bool getInvert() const { return is_invert; }

protected:
	DoubleBuffer<ofShortPixels> pix;

	float near_value;
	float far_value;
	bool is_invert;

	bool readFrame();
	void setPixels(Frame frame);

};

class ofxKinect2::IrStream : public ofxKinect2::Stream
{
public:
	IrStream() : Stream() {}
	~IrStream() {}

	bool setup(ofxKinect2::Device& device)
	{
		return Stream::setup(device, SENSOR_IR);
	}
	bool open();
	void close();

	void update();
	bool updateMode();

	ofShortPixels& getPixels() { return pix.getFrontBuffer(); }
	const ofShortPixels& getPixels() const { return pix.getFrontBuffer(); }
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofShortPixels& getPixelsRef() { return pix.getFrontBuffer(); });

protected:
	DoubleBuffer<ofShortPixels> pix;

	bool readFrame();
	void setPixels(Frame frame);

};

class ofxKinect2::BodyIndexStream : public ofxKinect2::Stream
{
public:
	BodyIndexStream() : Stream() { colors[0] = ofColor::red; colors[1] = ofColor::green; colors[2] = ofColor::blue; colors[3] = ofColor::cyan; colors[4] = ofColor::magenta; colors[5] = ofColor::yellow; }
	~BodyIndexStream() { }

	bool setup(ofxKinect2::Device& device)
	{
		return Stream::setup(device, SENSOR_BODY_INDEX);
	}

	bool open();
	void close();

	void update();
	bool updateMode();

	ofPixels& getPixels() { return pix.getFrontBuffer(); }
	const ofPixels& getPixels() const { return pix.getFrontBuffer(); }
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofPixels& getPixelsRef() { return pix.getFrontBuffer(); });
protected:
	DoubleBuffer<ofPixels> pix;
	unsigned char* buffer;
	ofColor colors[BODY_COUNT];

	bool readFrame();
	void setPixels(Frame frame);
};

class ofxKinect2::Body
{
	friend class BodyStream;
public:
	typedef ofPtr<Body> Ref;

	Body():left_hand_state(HandState_Unknown), right_hand_state(HandState_Unknown), is_update_scale(false) {
		joints.resize(JointType_Count);
		joint_points.resize(JointType_Count);
	}

	void setup(ofxKinect2::Device& device)
	{
		this->device = &device;
	}

	void update(IBody* body);
	void drawBody(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	void drawBone(JointType joint0, JointType joint1, int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	void drawHands(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	void drawHandLeft(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	void drawHandRight(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	void drawLean(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());

	void setId(UINT64 _id) { id = _id; }
	inline int getId() const { return id;}

	inline HandState getLeftHandState() const { return left_hand_state; }
	inline HandState getRightHandState() const { return left_hand_state; }

	inline size_t getNumJoints() { return JointType_Count; }

	const Joint& getJoint(size_t idx) { return joints[idx]; }

	const ofPoint& getJointPoint(size_t idx) { return joint_points[idx]; }
	const vector<ofPoint> getJointPoints() { return joint_points; }
	 
	const ofVec2f& getLean() { return body_lean; }
private:
	Device* device;
	vector<Joint> joints;
	vector<ofPoint> joint_points;
	UINT64 id;
	TrackingState lean_state;
	ofVec2f body_lean;
	bool is_update_scale;

	HandState left_hand_state;
	HandState right_hand_state;

	ofPoint jointToScreen(const JointType jointType, int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
	ofPoint bodyPointToScreen(const CameraSpacePoint& bodyPoint, int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight());
};

class ofxKinect2::BodyStream : public Stream
{
public:
	BodyStream() : Stream() {}
	~BodyStream() {}

	bool setup(ofxKinect2::Device& device)
	{
		this->device = &device;
		return Stream::setup(device, SENSOR_BODY);
	}
	bool open();
	void close();

	void update();
	bool updateMode();

	void draw(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawBody(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawBone(JointType joint0 = JointType_SpineBase, JointType joint1 = JointType_SpineMid, int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawHands(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawHandLeft(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawHandRight(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);
	void drawLean(int x = 0, int y = 0, int w = ofGetWidth(), int h = ofGetHeight(), size_t idx = BODY_COUNT);

	vector<Body> getBodies() { 
		vector<Body> ret;
		if (lock()) {
			ret = bodies;
			unlock();
		}
	return ret; }

	ofShortPixels& getPixels() { return pix.getFrontBuffer(); }
	const ofShortPixels& getPixels() const { return pix.getFrontBuffer(); }
	ofShortPixels& getPixels(int _near, int _far, bool invert = false);
	const ofShortPixels& getPixels(int _near, int _far, bool invert = false) const;
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofShortPixels& getPixelsRef() { return pix.getFrontBuffer(); });
	OF_DEPRECATED_MSG("Use getPixels() instead ", ofShortPixels getPixelsRef(int _near, int _far, bool invert = false););

	ofPoint righthand_pos_;

protected:
	Device *device;
	DoubleBuffer<ofShortPixels> pix;
	vector<Body> bodies;

	bool readFrame();
	void setPixels(Frame frame);

};

// mapper
class ofxKinect2::Mapper
{
public:
	Mapper(): p_mapper(NULL), depth_space_points(NULL), camera_space_points(NULL), depth_values(NULL), depth_pixels(NULL), color_pixels(NULL){}
	~Mapper() { exit(); }

	bool setup(ofxKinect2::Device& device);
	void exit()
	{
		safe_release(p_mapper);
		if (depth_space_points)
		{
			delete depth_space_points;
			depth_space_points = NULL;
		}
		if (camera_space_points)
		{
			delete camera_space_points;
			camera_space_points = NULL;
		}
		if (depth_values)
		{
			delete depth_values;
			depth_values = NULL;
		}

	}

	bool isReady(bool depth = true, bool color = true);
	void setDepthFromShortPixels(const ofShortPixels* _depth_pixels) { depth_pixels = _depth_pixels; }
	void setDepth(ofxKinect2::DepthStream& _depth_stream) { depth_pixels = &_depth_stream.getPixels(); }
	void setColorFromPixels(const ofPixels* _color_pixels) { color_pixels = _color_pixels; }
	void setColor(ofxKinect2::ColorStream& _color_stream) { color_pixels = &_color_stream.getPixels(); }

	ofVec3f mapDepthToCameraSpace(int x, int y);
	ofVec3f mapDepthToCameraSpace(ofVec2f depth_point);
	vector<ofVec3f> mapDepthToCameraSpace();
	vector<ofVec3f> mapDepthToCameraSpace(vector<ofVec2f> depth_points);
	vector<ofVec3f> mapDepthToCameraSpace(ofRectangle depth_area);

	ofVec2f mapDepthToColorSpace(int x, int y);
	ofVec2f mapDepthToColorSpace(ofVec2f depth_point);
	vector<ofVec2f> mapDepthToColorSpace();
	vector<ofVec2f> mapDepthToColorSpace(vector<ofVec2f> depth_points);
	vector<ofVec2f> mapDepthToColorSpace(ofRectangle depth_area);

	vector<ofFloatColor> getFloatColorsCoordinatesToDepthFrame();
	vector<ofColor> getColorsCoordinatesToDepthFrame();
	ofPixels getColorFrameCoordinatesToDepthFrame();

	vector<ofVec3f> mapColorToCameraSpace();
	vector<ofVec2f> mapColorToDepthSpace();

	ofVec2f mapCameraToDepthSpace(float x, float y, float z);
	ofVec2f mapCameraToDepthSpace(ofVec3f camera_point);
	vector<ofVec2f> mapCameraToDepthSpace(vector<ofVec3f> camera_points);

	ofVec2f mapCameraToColorSpace(float x, float y, float z);
	ofVec2f mapCameraToColorSpace(ofVec3f camera_point);
	vector<ofVec2f> mapCameraToColorSpace(vector<ofVec3f> camera_points);

	ICoordinateMapper* get() { return p_mapper; }
	const ICoordinateMapper* get() const { return p_mapper; }
private:
	Device* device;
	ICoordinateMapper* p_mapper;
	const ofShortPixels* depth_pixels;
	const ofPixels* color_pixels;
	ofPixels coordinate_color_pixels;

	DepthSpacePoint* depth_space_points;
	CameraSpacePoint* camera_space_points;
	UINT16* depth_values;
	vector<ofVec2f> camera_to_depth_points;
	vector<ofVec2f> depth_to_color_points;
	vector<ofVec3f> depth_to_camera_points;
	vector<ofVec2f> color_to_depth_points;
	vector<ofVec3f> color_to_camera_points;

	vector<ofFloatColor> depth_to_float_colors;
	vector<ofColor> depth_to_colors;
};



#endif // OFX_KINECT2_H