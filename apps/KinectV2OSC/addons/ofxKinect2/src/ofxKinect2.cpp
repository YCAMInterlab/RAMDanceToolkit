// @author sadmb
// @date 3,Jan,2014
// modified from ofxNI2.cpp of ofxNI2 by @satoruhiga

#include "ofxKinect2.h"
#include "utils\DepthRemapToRange.h"

namespace ofxKinect2
{
	//----------------------------------------------------------
	void init()
	{
		static bool inited = false;
		if(inited) return;
		inited = true;
		
	}
} // namespace ofxKinect2

using namespace ofxKinect2;

//----------------------------------------------------------
#pragma mark - Device
//----------------------------------------------------------

//----------------------------------------------------------
Device::Device() : recorder(NULL)
{
	device.kinect2 = NULL;
}

//----------------------------------------------------------
Device::~Device()
{
	exit();
}

//----------------------------------------------------------
bool Device::setup()
{
	ofxKinect2::init();

	HRESULT hr;

	hr = GetDefaultKinectSensor(&device.kinect2);

	if (SUCCEEDED(hr))
	{
		if(device.kinect2)
		{
			device.kinect2->Open();
			return true;
		}
	}

	ofLogWarning("ofxKinect2::Device") << " Kinect v2 not found.";
	return false;
}

//----------------------------------------------------------
bool Device::setup(string kinect2_file_path)
{
	ofxKinect2::init();
	kinect2_file_path = ofToDataPath(kinect2_file_path);

	ofLogWarning("ofxKinect2::Device") << " Open from path is not supported yet.";
	return setup();
}

//----------------------------------------------------------
void Device::exit()
{
//	stopRecording();
	if(device.kinect2){
		device.kinect2->Close();
	}
	safe_release(device.kinect2);

	vector<Stream*>::iterator it;
	int counter = 0;
	while (!streams.empty())
	{
		it = streams.begin();
		(*it)->exit();
		counter++;

		// just in case streams array mismatch .
		if (counter > 1000)
		{
			ofLogNotice("ofxKinect2::Device") << "streams array is incorrect.";
			break;
		}
	}

	streams.clear();

}

//----------------------------------------------------------
void Device::update()
{
	if (!isOpen()) return;

	for (int i = 0; i < streams.size(); i++)
	{
		Stream* s = streams[i];
		s->is_frame_new = s->kinect2_timestamp != s->opengl_timestamp;
		s->opengl_timestamp = s->kinect2_timestamp;
	}
	int val = 0;
	ofNotifyEvent(onUpdateStream, val);

}


//----------------------------------------------------------
#pragma mark - Stream
//----------------------------------------------------------

//----------------------------------------------------------
bool Stream::setup(Device& device, SensorType sensor_type)
{
	if (!device.isOpen())
	{
		return false;
	}
	kinect2_timestamp = 0;
	opengl_timestamp = 0;
	frame.sensor_type = sensor_type;
	frame.mode.fps = 0;
	frame.frame_index = 0;
	frame.stride = 0;
	frame.data = NULL;
	frame.data_size = 0;
	is_frame_new = false;
	texture_needs_update = false;

	device.streams.push_back(this);
	this->device = &device;

	setMirror(false);

	return true;
}

//----------------------------------------------------------
void Stream::exit()
{
	for (vector<Stream*>::iterator it = device->streams.begin(); it != device->streams.end(); )
	{
		Stream* s = *it;
		if(s == this)
		{
			it = device->streams.erase(it);
			close();
		}
		else
		{
			++it;
		}
	}
}

//----------------------------------------------------------
bool Stream::open()
{
	startThread();
	return true;
}

//----------------------------------------------------------
void Stream::close()
{
	while(!lock())
	{
	}
	{
		frame.frame_index = 0;
		frame.stride = 0;
		frame.data = NULL;
		frame.data_size = 0;
		stopThread();
		unlock();
	}
}

//----------------------------------------------------------
void Stream::threadedFunction()
{
	while(isThreadRunning() != 0)
	{
		if (lock())
		{
			if (readFrame())
			{
				kinect2_timestamp = frame.timestamp;
				texture_needs_update = true;
			}
			unlock();
		}
		if (frame.mode.fps != 0)
		{
			ofSleepMillis(1000.f / frame.mode.fps);
		}
	}
}

//----------------------------------------------------------
bool Stream::readFrame()
{
	return false;
}

//----------------------------------------------------------
void Stream::setPixels(Frame frame)
{
	kinect2_timestamp = frame.timestamp;
}

//----------------------------------------------------------
void Stream::update()
{
	texture_needs_update = false;
}

//----------------------------------------------------------
bool Stream::updateMode()
{
	return true;
}

//----------------------------------------------------------
bool Stream::setSize(int width, int height)
{
	frame.mode.resolution_x = width;
	frame.mode.resolution_y = height;

	return updateMode();
}

//----------------------------------------------------------
int Stream::getWidth() const
{
	return frame.width;
}

//----------------------------------------------------------
int Stream::getHeight() const
{
	return frame.height;
}

//----------------------------------------------------------
bool Stream::setWidth(int v)
{
	return setSize(v, getHeight());
}

//----------------------------------------------------------
bool Stream::setHeight(int v)
{
	return setSize(getWidth(), v);
}

//----------------------------------------------------------
int Stream::getFps()
{
	return frame.mode.fps;
}

//----------------------------------------------------------
bool Stream::setFps(int v)
{
	frame.mode.fps = v;
	return updateMode();
}

//----------------------------------------------------------
float Stream::getHorizontalFieldOfView()
{
	return frame.horizontal_field_of_view;
}

//----------------------------------------------------------
float Stream::getVerticalFieldOfView()
{
	return frame.vertical_field_of_view;
}

//----------------------------------------------------------
float Stream::getDiagonalFieldOfView()
{
	return frame.diagonal_field_of_view;
}

//----------------------------------------------------------
void Stream::setMirror(bool v)
{
	is_mirror = false;
}

//----------------------------------------------------------
bool Stream::isMirror()
{
	return is_mirror;
}

//----------------------------------------------------------
void Stream::draw(float x, float y)
{
	draw(x, y, getWidth(), getHeight());
}

//----------------------------------------------------------
void Stream::draw(float x, float y, float w, float h)
{
	if (texture_needs_update)
	{
		update();
	}

	if (tex.isAllocated())
	{
		ofSetColor(ofColor::white);
		tex.draw(x, y, w, h);
	}
}

//----------------------------------------------------------
#pragma mark - ColorStream
//----------------------------------------------------------

//----------------------------------------------------------
bool ColorStream::readFrame()
{
	bool readed = false;
	if (!stream.p_color_frame_reader)
	{
		ofLogWarning("ofxKinect2::ColorStream") << "Stream is not open.";
		return readed;
	}
	Stream::readFrame();

	IColorFrame* p_frame = NULL;
	
	HRESULT hr;
	hr = stream.p_color_frame_reader->AcquireLatestFrame(&p_frame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* p_frame_description = NULL;
		ColorImageFormat image_format = ColorImageFormat_None;

		hr = p_frame->get_RelativeTime((INT64*)&frame.timestamp);

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_FrameDescription(&p_frame_description);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Width(&frame.width);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Height(&frame.height);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_HorizontalFieldOfView(&frame.horizontal_field_of_view);
		}
		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_VerticalFieldOfView(&frame.vertical_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_DiagonalFieldOfView(&frame.diagonal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_RawColorImageFormat(&image_format);
		}

		if (SUCCEEDED(hr))
		{
			if (buffer == NULL)
			{
				buffer = new unsigned char[frame.width * frame.height * 4];
			}
			if (image_format == ColorImageFormat_Rgba)
			{
				hr = p_frame->AccessRawUnderlyingBuffer((UINT*)&frame.data_size, reinterpret_cast<BYTE**>(&frame.data));
			}
			else
			{
				frame.data = buffer;
				frame.data_size = frame.width * frame.height * 4 * sizeof(unsigned char);
				hr = p_frame->CopyConvertedFrameDataToArray((UINT)frame.data_size, reinterpret_cast<BYTE*>(frame.data),  ColorImageFormat_Rgba);
			}
		}

		if (SUCCEEDED(hr))
		{
			readed = true;
			setPixels(frame);
		}
		safe_release(p_frame_description);
	}

	safe_release(p_frame);

	return readed;
}

//----------------------------------------------------------
void ColorStream::setPixels(Frame frame)
{
	Stream::setPixels(frame);

	int w = frame.width;
	int h = frame.height;
	int num_pixels = w * h;

	const unsigned char * src = (const unsigned char*)frame.data;
	if(!src)
		return;
	unsigned char *dst = pix.getBackBuffer().getPixels();

	pix.getBackBuffer().setFromPixels(src, w, h, OF_IMAGE_COLOR_ALPHA);
	pix.swap();
}

//----------------------------------------------------------
bool ColorStream::setWidth(int width)
{
	bool ret = Stream::setWidth(width);
	pix.deallocate();
	pix.allocate(frame.width, frame.height, 4);
	return ret;
}

//----------------------------------------------------------
bool ColorStream::setHeight(int height)
{
	bool ret = Stream::setHeight(height);
	pix.allocate(frame.width, frame.height, 4);
	return ret;
}

//----------------------------------------------------------
bool ColorStream::setSize(int width, int height)
{
	bool ret = Stream::setSize(width, height);
	pix.allocate(frame.width, frame.height, 4);
	return ret;
}

//----------------------------------------------------------
void ColorStream::update()
{
	if(!tex.isAllocated()
		|| tex.getWidth() != getWidth()
		|| tex.getHeight() != getHeight())
	{
		tex.allocate(getWidth(), getHeight(), GL_RGB);
	}

	if (lock())
	{
		tex.loadData(pix.getFrontBuffer());
		Stream::update();
		unlock();
	}
}

//----------------------------------------------------------
bool ColorStream::open()
{
	if (!device->isOpen())
	{
		ofLogWarning("ofxKinect2::ColorStream") << "No ready Kinect2 found.";
		return false;
	}
	IColorFrameSource* p_source = NULL;
	HRESULT hr;

	hr = device->get().kinect2->get_ColorFrameSource(&p_source);

	if (SUCCEEDED(hr))
	{
		hr = p_source->OpenReader(&stream.p_color_frame_reader);
	}
	IFrameDescription* p_frame_description = NULL;
	p_source->get_FrameDescription(&p_frame_description);
	if (SUCCEEDED(hr))
	{
		int res_x, res_y = 0;
		hr = p_frame_description->get_Width(&res_x);
		hr = p_frame_description->get_Width(&res_y);
		frame.mode.resolution_x = res_x;
		frame.mode.resolution_y = res_y;
		frame.width = res_x;
		frame.height = res_y;
		pix.allocate(res_x, res_y, 4);

	}
	safe_release(p_frame_description);
	safe_release(p_source);
	if (FAILED(hr))
	{
		ofLogWarning("ofxKinect2::ColorStream") << "Can't open stream.";
		return false;
	}

	return Stream::open();
}

//----------------------------------------------------------
void ColorStream::close()
{
	Stream::close();
	safe_release(stream.p_color_frame_reader);
}

//----------------------------------------------------------
bool ColorStream::updateMode()
{
	ofLogWarning("ofxKinect2::ColorStream") << "Not supported yet.";
	return false;
}

//----------------------------------------------------------
ofColor ColorStream::getColorAt(int x, int y)
{
	if (!pix.getFrontBuffer().isAllocated() || x < 0 || y < 0 || x >= pix.getFrontBuffer().getWidth() || y >= pix.getFrontBuffer().getHeight())
	{
		return ofColor(0, 0, 0, 0);
	}
	int index = (x + y * pix.getFrontBuffer().getWidth()) * 4;
	ofColor color = ofColor(pix.getFrontBuffer()[index], pix.getFrontBuffer()[index + 1], pix.getFrontBuffer()[index + 2], pix.getFrontBuffer()[index + 3]);
	return color;
}

//----------------------------------------------------------
ofColor ColorStream::getColorAt(ofVec2f color_point)
{
    return getColorAt(color_point.x, color_point.y);
}

//----------------------------------------------------------
ofFloatColor ColorStream::getFloatColorAt(int x, int y)
{
	if (!pix.getFrontBuffer().isAllocated() || x < 0 || y < 0 || x >= pix.getFrontBuffer().getWidth() || y >= pix.getFrontBuffer().getHeight())
	{
		return ofFloatColor(0, 0, 0, 0);
	}
	int index = (x + y * pix.getFrontBuffer().getWidth()) * 4;
	ofFloatColor color = ofColor(pix.getFrontBuffer()[index] / 255.f, pix.getFrontBuffer()[index + 1] / 255.f, pix.getFrontBuffer()[index + 2] / 255.f, pix.getFrontBuffer()[index + 3] / 255.f);
	return color;
}

//----------------------------------------------------------
ofFloatColor ColorStream::getFloatColorAt(ofVec2f color_point)
{
	return getFloatColorAt(color_point.x, color_point.y);
}

//----------------------------------------------------------
int ColorStream::getExposureTime()
{
	TIMESPAN exposure_time;
	camera_settings.p_color_camera_settings->get_ExposureTime(&exposure_time);
	return (int)exposure_time;
}

//----------------------------------------------------------
int ColorStream::getFrameInterval()
{
	TIMESPAN frame_interval;
	camera_settings.p_color_camera_settings->get_FrameInterval(&frame_interval);
	return (int)frame_interval;
}

//----------------------------------------------------------
float ColorStream::getGain()
{
	float gain;
	camera_settings.p_color_camera_settings->get_Gain(&gain);
	return gain;
}

//----------------------------------------------------------
float ColorStream::getGamma()
{
	float gamma;
	camera_settings.p_color_camera_settings->get_Gamma(&gamma);
	return gamma;
}

//----------------------------------------------------------
#pragma mark - DepthStream
//----------------------------------------------------------

//----------------------------------------------------------
bool DepthStream::readFrame()
{
	bool readed = false;
	if(!stream.p_depth_frame_reader)
	{
		ofLogWarning("ofxKinect2::DepthStream") << "Stream is not open.";
		return readed;
	}
	Stream::readFrame();

	IDepthFrame* p_frame = NULL;

	HRESULT hr;
	hr = stream.p_depth_frame_reader->AcquireLatestFrame(&p_frame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* p_frame_description = NULL;

		hr = p_frame->get_RelativeTime((INT64*)&frame.timestamp);

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_FrameDescription(&p_frame_description);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Width(&frame.width);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Height(&frame.height);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_HorizontalFieldOfView(&frame.horizontal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_VerticalFieldOfView(&frame.vertical_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_DiagonalFieldOfView(&frame.diagonal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_DepthMinReliableDistance((USHORT*)&near_value);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_DepthMaxReliableDistance((USHORT*)&far_value);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_DepthMinReliableDistance((USHORT*)&near_value);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->AccessUnderlyingBuffer((UINT*)&frame.data_size, reinterpret_cast<UINT16**>(&frame.data));
		}

		if (SUCCEEDED(hr))
		{
			readed = true;
			setPixels(frame);
		}
		safe_release(p_frame_description);
	}

	safe_release(p_frame);

	return readed;
}

//----------------------------------------------------------
void DepthStream::setPixels(Frame frame)
{
	Stream::setPixels(frame);
	
	const unsigned short *pixels = (const unsigned short*)frame.data;
	if(!pixels)
		return;
	int w = frame.width;
	int h = frame.height;
	
	pix.allocate(w, h, 1);
	pix.getBackBuffer().setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
	pix.swap();
}

//----------------------------------------------------------
void DepthStream::update()
{
	if (!tex.isAllocated()
		|| tex.getWidth() != getWidth()
		|| tex.getHeight() != getHeight())
	{
#if OF_VERSION_MINOR <= 7
		static ofTextureData data;
		
		data.pixelType = GL_UNSIGNED_SHORT;
		data.glTypeInternal = GL_LUMINANCE16;
		data.width = getWidth();
		data.height = getHeight();
		
		tex.allocate(data);
#elif OF_VERSION_MINOR > 7
		tex.allocate(getWidth(), getHeight(), GL_RGBA, true, GL_LUMINANCE, GL_UNSIGNED_SHORT);
#endif
	}

	if (lock())
	{
		Stream::update();
		unlock();
	}
}

//----------------------------------------------------------
ofShortPixels& DepthStream::getPixels(int _near, int _far, bool invert)
{
	ofShortPixels _pix;
	depthRemapToRange(getPixels(), _pix, _near, _far, invert);
	return _pix;
}

//----------------------------------------------------------
const ofShortPixels& DepthStream::getPixels(int _near, int _far, bool invert) const
{
	ofShortPixels _pix;
	depthRemapToRange(getPixels(), _pix, _near, _far, invert);
	return _pix;
}

//----------------------------------------------------------
unsigned short DepthStream::getDepthAt(int x, int y)
{
    int index = x + y * pix.getFrontBuffer().getWidth();
	if (pix.getFrontBuffer().isAllocated())
	{
		return pix.getFrontBuffer()[index];
	}
	else
	{
		ofLogNotice("ofKinect2::DepthStream") << "Cannot get depth.";
		return 0;
	}
}

//----------------------------------------------------------
unsigned short DepthStream::getDepthAt(ofVec2f depth_point)
{
	return getDepthAt(depth_point.x, depth_point.y);
}

//----------------------------------------------------------
bool DepthStream::open()
{
	if (!device->isOpen())
	{
		ofLogWarning("ofxKinect2::DepthStream") << "No ready Kinect2 found.";
		return false;
	}
	is_invert = true;
	near_value = 0;
	far_value = 10000;
	IDepthFrameSource* p_source = NULL;
	HRESULT hr;

	hr = device->get().kinect2->get_DepthFrameSource(&p_source);

	if (SUCCEEDED(hr))
	{
		hr = p_source->OpenReader(&stream.p_depth_frame_reader);
	}

	safe_release(p_source);
	if (FAILED(hr))
	{
		ofLogWarning("ofxKinect2::DepthStream") << "Can't open stream.";
		return false;
	}

	return Stream::open();
}

//----------------------------------------------------------
void DepthStream::close()
{
	Stream::close();
	safe_release(stream.p_depth_frame_reader);
}

//----------------------------------------------------------
bool DepthStream::updateMode()
{
	ofLogWarning("ofxKinect2::DepthStream") << "Not supported yet.";
	return false;
}

//----------------------------------------------------------
#pragma mark - IrStream
//----------------------------------------------------------

//----------------------------------------------------------
bool IrStream::readFrame()
{
	bool readed = false;
	if(!stream.p_infrared_frame_reader)
	{
		ofLogWarning("ofxKinect2::IrStream") << "Stream is not open.";
		return readed;
	}
	Stream::readFrame();

	IInfraredFrame* p_frame = NULL;

	HRESULT hr;
	hr = stream.p_infrared_frame_reader->AcquireLatestFrame(&p_frame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* p_frame_description = NULL;

		hr = p_frame->get_RelativeTime((INT64*)&frame.timestamp);

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_FrameDescription(&p_frame_description);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Width(&frame.width);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Height(&frame.height);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_HorizontalFieldOfView(&frame.horizontal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_VerticalFieldOfView(&frame.vertical_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_DiagonalFieldOfView(&frame.diagonal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->AccessUnderlyingBuffer((UINT*)&frame.data_size, reinterpret_cast<UINT16**>(&frame.data));
		}

		if (SUCCEEDED(hr))
		{
			readed = true;
			setPixels(frame);
		}
		safe_release(p_frame_description);
	}

	safe_release(p_frame);

	return readed;
}

//----------------------------------------------------------
void IrStream::setPixels(Frame frame)
{
	Stream::setPixels(frame);

	const unsigned short *pixels = (const unsigned short*)frame.data;
	if(!pixels)
		return;
	int w = frame.width;
	int h = frame.height;
	
	pix.allocate(w, h, 1);
	pix.getBackBuffer().setFromPixels(pixels, w, h, OF_IMAGE_GRAYSCALE);
	pix.swap();
}

//----------------------------------------------------------
void IrStream::update()
{
	if(!tex.isAllocated()
		|| tex.getWidth() != getWidth()
		|| tex.getHeight() != getHeight())
	{
		tex.allocate(getWidth(), getHeight(), GL_LUMINANCE);
	}

	if (lock())
	{
		tex.loadData(pix.getFrontBuffer());
		Stream::update();
		unlock();
	}
}

//----------------------------------------------------------
bool IrStream::open()
{
	if (!device->isOpen())
	{
		ofLogWarning("ofxKinect2::IrStream") << "No ready Kinect2 found.";
		return false;
	}

	IInfraredFrameSource* p_source = NULL;
	HRESULT hr;

	hr = device->get().kinect2->get_InfraredFrameSource(&p_source);

	if (SUCCEEDED(hr))
	{
		hr = p_source->OpenReader(&stream.p_infrared_frame_reader);
	}

	safe_release(p_source);
	if (FAILED(hr))
	{
		ofLogWarning("ofxKinect2::IrStream") << "Can't open stream.";
		return false;
	}

	return Stream::open();
}

//----------------------------------------------------------
void IrStream::close()
{
	Stream::close();
	safe_release(stream.p_infrared_frame_reader);
}

//----------------------------------------------------------
bool IrStream::updateMode()
{
	ofLogWarning("ofxKinect2::IrStream") << "Not supported yet.";
	return false;
}

//----------------------------------------------------------
#pragma mark - BodyIndexStream
//----------------------------------------------------------

//----------------------------------------------------------
bool BodyIndexStream::readFrame()
{
	bool readed = false;
	if(!stream.p_body_index_frame_reader)
	{
		ofLogWarning("ofxKinect2::BodyIndexStream") << "Stream is not open.";
		return readed;
	}
	Stream::readFrame();

	IBodyIndexFrame* p_frame = NULL;

	HRESULT hr;
	hr = stream.p_body_index_frame_reader->AcquireLatestFrame(&p_frame);

	if (SUCCEEDED(hr))
	{
		IFrameDescription* p_frame_description = NULL;

		hr = p_frame->get_RelativeTime((INT64*)&frame.timestamp);

		if (SUCCEEDED(hr))
		{
			hr = p_frame->get_FrameDescription(&p_frame_description);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Width(&frame.width);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_Height(&frame.height);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_HorizontalFieldOfView(&frame.horizontal_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_VerticalFieldOfView(&frame.vertical_field_of_view);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_DiagonalFieldOfView(&frame.diagonal_field_of_view);
		}

		unsigned int bpp = 0;
		if (SUCCEEDED(hr))
		{
			hr = p_frame_description->get_BytesPerPixel(&bpp);
		}

		if (SUCCEEDED(hr))
		{
			hr = p_frame->AccessUnderlyingBuffer((UINT*)&frame.data_size, reinterpret_cast<BYTE**>(&frame.data));
		}

		if (SUCCEEDED(hr))
		{
			readed = true;
			setPixels(frame);
		}
		safe_release(p_frame_description);
	}

	safe_release(p_frame);

	return readed;
}

//----------------------------------------------------------
void BodyIndexStream::setPixels(Frame frame)
{
	Stream::setPixels(frame);

	int w = frame.width;
	int h = frame.height;
	unsigned char *pixels = new unsigned char[w * h * 4];
	for(int i  = 0; i < w * h; i++)
	{
		int index = i * 4;
		unsigned char p = ((unsigned char*)frame.data)[i];
		ofColor color = colors[p];
		if(p != 255)
		{
			pixels[index + 0] = color.r;
			pixels[index + 1] = color.g;
			pixels[index + 2] = color.b;
			pixels[index + 3] = 255;
		}
		else
		{
			pixels[index + 0] = 0;
			pixels[index + 1] = 0;
			pixels[index + 2] = 0;
			pixels[index + 3] = 0;
		}
	}
	
	pix.allocate(w, h, 1);
	pix.getBackBuffer().setFromPixels(pixels, w, h, OF_IMAGE_COLOR_ALPHA);
	pix.swap();
}

//----------------------------------------------------------
void BodyIndexStream::update()
{
	if(!tex.isAllocated()
		|| tex.getWidth() != getWidth()
		|| tex.getHeight() != getHeight())
	{
		tex.allocate(getWidth(), getHeight(), GL_RGB);
	}

	if (lock())
	{
		tex.loadData(pix.getFrontBuffer());
		Stream::update();
		unlock();
	}
}

//----------------------------------------------------------
bool BodyIndexStream::open()
{
	if (!device->isOpen())
	{
		ofLogWarning("ofxKinect2::BodyIndexStream") << "No ready Kinect2 found.";
		return false;
	}

	IBodyIndexFrameSource* p_source = NULL;
	HRESULT hr;

	hr = device->get().kinect2->get_BodyIndexFrameSource(&p_source);

	if (SUCCEEDED(hr))
	{
		hr = p_source->OpenReader(&stream.p_body_index_frame_reader);
	}

	safe_release(p_source);
	if (FAILED(hr))
	{
		ofLogWarning("ofxKinect2::BodyIndexStream") << "Can't open stream.";
		return false;
	}

	return Stream::open();
}

//----------------------------------------------------------
void BodyIndexStream::close()
{
	Stream::close();
	safe_release(stream.p_infrared_frame_reader);
}

//----------------------------------------------------------
bool BodyIndexStream::updateMode()
{
	ofLogWarning("ofxKinect2::BodyIndexStream") << "Not supported yet.";
	return false;
}

//----------------------------------------------------------
#pragma mark - Body
//----------------------------------------------------------

//----------------------------------------------------------
void Body::update(IBody* body)
{
	joints.resize(JointType_Count);
	joint_points.resize(JointType_Count);

	HRESULT hr = body->get_HandLeftState(&left_hand_state);
	hr = body->get_HandRightState(&right_hand_state);

	body->get_LeanTrackingState(&lean_state);
	PointF pnt;
	body->get_Lean(&pnt);
	CameraSpacePoint cpnt;
	cpnt.X = pnt.X;
	cpnt.Y = pnt.Y;
	cpnt.Z = 0;
	body_lean = bodyPointToScreen(cpnt, ofGetWidth(), ofGetHeight());

	hr = body->GetJoints(JointType_Count, &joints.front());

	if(SUCCEEDED(hr))
	{
		is_update_scale = false;
	}
}

//----------------------------------------------------------
ofPoint Body::jointToScreen(const JointType jointType, int x, int y, int w, int h)
{
	CameraSpacePoint bodyPoint = joints[jointType].Position;
	return bodyPointToScreen(bodyPoint, x, y, w, h);
}

//----------------------------------------------------------
ofPoint Body::bodyPointToScreen(const CameraSpacePoint& bodyPoint, int x, int y, int w, int h)
{
	// Calculate the body's position on the screen
    DepthSpacePoint depthPoint = {0};
	HRESULT hr;
	ICoordinateMapper* mapper;
	hr = device->get().kinect2->get_CoordinateMapper(&mapper);
	if (SUCCEEDED(hr))
	{
		mapper->MapCameraPointToDepthSpace(bodyPoint, &depthPoint);
	}
	else
	{
		ofLogError("ofxKinect2::Body") << "can't get Coordinate Mapper.";
		return ofPoint(0, 0);
	}

	// TODO: width/ height
    float screenPointX = static_cast<float>(depthPoint.X * w) / 512 + x;
    float screenPointY = static_cast<float>(depthPoint.Y * h) / 424 + y;

    return ofPoint(screenPointX, screenPointY);
}

//----------------------------------------------------------
void Body::drawBody(int x, int y, int w, int h)
{
		drawBone(JointType_Head, JointType_Neck, x, y, w , h);
		drawBone(JointType_Neck, JointType_SpineShoulder, x, y, w , h);
		drawBone(JointType_SpineShoulder, JointType_SpineMid, x, y, w , h);
		drawBone(JointType_SpineMid, JointType_SpineBase, x, y, w , h);
		drawBone(JointType_SpineShoulder, JointType_ShoulderLeft, x, y, w , h);
		drawBone(JointType_SpineShoulder, JointType_ShoulderRight, x, y, w , h);
		drawBone(JointType_SpineBase, JointType_HipLeft, x, y, w , h);
		drawBone(JointType_SpineBase, JointType_HipRight, x, y, w , h);

		drawBone(JointType_ShoulderLeft, JointType_ElbowLeft, x, y, w , h);
		drawBone(JointType_ElbowLeft, JointType_WristLeft, x, y, w , h);
		drawBone(JointType_WristLeft, JointType_HandLeft, x, y, w , h);
		drawBone(JointType_HandLeft, JointType_HandTipLeft, x, y, w , h);
		drawBone(JointType_WristLeft, JointType_ThumbLeft, x, y, w , h);

		drawBone(JointType_ShoulderRight, JointType_ElbowRight, x, y, w , h);
		drawBone(JointType_ElbowRight, JointType_WristRight, x, y, w , h);
		drawBone(JointType_WristRight, JointType_HandRight, x, y, w , h);
		drawBone(JointType_HandRight, JointType_HandTipRight, x, y, w , h);
		drawBone(JointType_WristRight, JointType_ThumbRight, x, y, w , h);

		drawBone(JointType_HipLeft, JointType_KneeLeft, x, y, w , h);
		drawBone(JointType_KneeLeft, JointType_AnkleLeft, x, y, w , h);
		drawBone(JointType_AnkleLeft, JointType_FootLeft, x, y, w , h);

		drawBone(JointType_HipRight, JointType_KneeRight, x, y, w , h);
		drawBone(JointType_KneeRight, JointType_AnkleRight, x, y, w , h);
		drawBone(JointType_AnkleRight, JointType_FootRight, x, y, w , h);

		ofPushStyle();
		for(int i = 0; i < JointType_Count; ++i)
		{
			if(joints[i].TrackingState == TrackingState_Tracked)
			{
				ofSetColor(50, 200, 50);
				ofEllipse(jointToScreen((JointType)i, x, y, w, h), 3, 3);
			}
		}
		ofSetColor(ofColor::red);
		ofDrawBitmapString(ofToString(id), jointToScreen(JointType_Head, x, y, w, h));
		ofPopStyle();
}

//----------------------------------------------------------
void Body::drawBone(JointType joint0, JointType joint1, int x, int y, int w, int h)
{
		TrackingState state0 = joints[joint0].TrackingState;
		TrackingState state1 = joints[joint1].TrackingState;

		if((state0 == TrackingState_NotTracked) || (state1 == TrackingState_NotTracked))
		{
			return;
		}

		if((state0 == TrackingState_Inferred) && (state1 == TrackingState_Inferred))
		{
			return;
		}

		ofPushStyle();
		if((state0 == TrackingState_Tracked) && (state1 == TrackingState_Tracked))
		{
			ofSetColor(ofColor::green);
		}
		else
		{
			ofSetColor(ofColor::gray);
		}
		ofLine(jointToScreen(joint0, x, y, w, h), jointToScreen(joint1, x, y, w, h));
		ofPopStyle();
}
//----------------------------------------------------------
void Body::drawHands(int x, int y, int w, int h)
{
	drawHandLeft();
	drawHandRight();
}

//----------------------------------------------------------
void Body::drawHandLeft(int x, int y, int w, int h)
{
		ofPushStyle();
		switch(left_hand_state)
		{
		case HandState_Closed:
			ofSetColor(ofColor::red);
			ofEllipse(jointToScreen(JointType_HandLeft, x, y, w, h), 30, 30);
			break;
		case HandState_Open:
			ofSetColor(ofColor::green);
			ofEllipse(jointToScreen(JointType_HandLeft, x, y, w, h), 30, 30);
			break;
		case HandState_Lasso:
			ofSetColor(ofColor::blue);
			ofEllipse(jointToScreen(JointType_HandLeft, x, y, w, h), 30, 30);
			break;
		}
		ofPopStyle();
}

//----------------------------------------------------------
void Body::drawHandRight(int x, int y, int w, int h)
{
		ofPushStyle();
		switch(right_hand_state)
		{
		case HandState_Closed:
			ofSetColor(ofColor::red);
			ofEllipse(jointToScreen(JointType_HandRight, x, y, w, h), 30, 30);
			break;
		case HandState_Open:
			ofSetColor(ofColor::green);
			ofEllipse(jointToScreen(JointType_HandRight, x, y, w, h), 30, 30);
			break;
		case HandState_Lasso:
			ofSetColor(ofColor::blue);
			ofEllipse(jointToScreen(JointType_HandRight, x, y, w, h), 30, 30);
			break;
		}
		ofPopStyle();
}

//----------------------------------------------------------
void Body::drawLean(int x, int y, int w, int h)
{
		if(lean_state == TrackingState_Tracked)
		{
			ofPushStyle();
			ofSetColor(ofColor::magenta);
			ofLine(jointToScreen(JointType_SpineBase, x, y, w, h), jointToScreen(JointType_SpineBase, x, y, w, h) + body_lean);
			ofPopStyle();
		}
}

//----------------------------------------------------------
#pragma mark - BodyStream
//----------------------------------------------------------

//----------------------------------------------------------
bool BodyStream::readFrame()
{
	bool readed = false;
	if(!stream.p_body_frame_reader)
	{
		ofLogWarning("ofxKinect2::BodyStream") << "Stream is not open.";
		return readed;
	}

	IBodyFrame* p_frame = NULL;

	HRESULT hr;
	hr = stream.p_body_frame_reader->AcquireLatestFrame(&p_frame);

	if (SUCCEEDED(hr))
	{
		hr = p_frame->get_RelativeTime((INT64*)&frame.timestamp);

		IBody* ppBodies[BODY_COUNT] = {0};

		if (SUCCEEDED(hr))
		{
			hr = p_frame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);
		}

		if (SUCCEEDED(hr))
		{
			/*
			readed = true;
			righthand_pos_ =  ofPoint(0, 0);
			float nearest = 10000000;
			for(int i = 0; i < _countof(ppBodies); ++i)
			{
				if(ppBodies[i])
				{
					Joint joints[JointType_Count];
					HandState right_hand_state = HandState_Unknown;

					ppBodies[i]->get_HandRightState(&right_hand_state);

					HRESULT hr = ppBodies[i]->GetJoints(_countof(joints), joints);

					if(SUCCEEDED(hr))
					{
						if(joints[JointType_HandRight].TrackingState == TrackingState_Tracked)
						{
							float z_pos = joints[JointType_HandRight].Position.Z;
							if(z_pos < nearest)
							{
								nearest = z_pos;
								// Calculate the body's position on the screen
								DepthSpacePoint depthPoint = {0};
								ICoordinateMapper* mapper = NULL;
								hr = device->get().kinect2->get_CoordinateMapper(&mapper);
								if(SUCCEEDED(hr))
								{
									mapper->MapCameraPointToDepthSpace(joints[JointType_HandRight].Position, &depthPoint);

									// TODO: width/ height
									float screenPointX = static_cast<float>(depthPoint.X * 1920) / 512;
									float screenPointY = static_cast<float>(depthPoint.Y * 1080) / 424;

									righthand_pos_ =  ofPoint(screenPointX, screenPointY);
								}

								safe_release(mapper);
							}
						}
					}
				}
			}

			/**/
			bodies.clear();
			for(int i = 0; i < _countof(ppBodies); ++i)
			{
				BOOLEAN tracked = false;
				if(ppBodies[i])
				{
					ppBodies[i]->get_IsTracked(&tracked);
					if(tracked)
					{
						UINT64 id = -1;
						ppBodies[i]->get_TrackingId(&id);
						bodies.emplace_back(Body());
						Body &body = bodies.back();
						body.setup(*device);
						body.setId(id);
						body.update(ppBodies[i]);
					}
				}
			}
			/**/
			for(int i = 0; i < _countof(ppBodies); ++i)
			{
				safe_release(ppBodies[i]);
			}
			readed = true;
			setPixels(frame);
		}

	}

	safe_release(p_frame);

	return readed;
}

//----------------------------------------------------------
void BodyStream::draw(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawBody(x, y, w, h);
				bodies[i].drawHands(x, y, w, h);
				bodies[i].drawLean(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawBody(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawBody(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawBone(JointType joint0, JointType joint1, int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawBone(joint0, joint1, x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawHands(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawHands(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawHandLeft(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawHandLeft(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawHandRight(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawHandRight(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::drawLean(int x, int y, int w, int h, size_t idx)
{
	if(lock()) {
		for(int i = 0; i < bodies.size(); i++)
		{
			if(idx == BODY_COUNT || idx == i)
			{
				bodies[i].drawLean(x, y, w, h);
			}
		}
		unlock();
	}
}

//----------------------------------------------------------
void BodyStream::setPixels(Frame frame)
{
	Stream::setPixels(frame);
}

//----------------------------------------------------------
void BodyStream::update()
{
	if (lock())
	{
		Stream::update();
		unlock();
	}
}

//----------------------------------------------------------
bool BodyStream::open()
{
	if (!device->isOpen())
	{
		ofLogWarning("ofxKinect2::BodyStream") << "No ready Kinect2 found.";
		return false;
	}

	IBodyFrameSource* p_source = NULL;
	HRESULT hr;

	hr = device->get().kinect2->get_BodyFrameSource(&p_source);

	if (SUCCEEDED(hr))
	{
		hr = p_source->OpenReader(&stream.p_body_frame_reader);
	}

	safe_release(p_source);
	if (FAILED(hr))
	{
		ofLogWarning("ofxKinect2::BodyStream") << "Can't open stream.";
		return false;
	}

	return Stream::open();
}

//----------------------------------------------------------
void BodyStream::close()
{
	Stream::close();
	safe_release(stream.p_body_frame_reader);
}

//----------------------------------------------------------
bool BodyStream::updateMode()
{
	ofLogWarning("ofxKinect2::BodyStream") << "Not supported yet.";
	return false;
}

//----------------------------------------------------------
ofShortPixels& BodyStream::getPixels(int _near, int _far, bool invert)
{
	ofShortPixels _pix;
	depthRemapToRange(getPixels(), _pix, _near, _far, invert);
	return _pix;
}

//----------------------------------------------------------
const ofShortPixels& BodyStream::getPixels(int _near, int _far, bool invert) const
{
	ofShortPixels _pix;
	depthRemapToRange(getPixels(), _pix, _near, _far, invert);
	return _pix;
}

//----------------------------------------------------------
#pragma mark - Mapper
//----------------------------------------------------------

//----------------------------------------------------------
bool Mapper::setup(Device& device)
{
	this->device = &device;
	HRESULT hr;
	hr = device.get().kinect2->get_CoordinateMapper(&p_mapper);
	if (SUCCEEDED(hr))
	{
		return true;
	}
	else
	{
		ofLogWarning("ofxKinect2::Mapper") << "Cannot get Coordinate Mapper.";
		return false;
	}
}

//----------------------------------------------------------
bool Mapper::isReady(bool depth, bool color)
{
	if (depth)
	{
		if (!depth_pixels || !depth_pixels->isAllocated())
		{
			return false;
		}
	}

	if (color)
	{
		if (!color_pixels || !color_pixels->isAllocated())
		{
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------
ofVec3f Mapper::mapDepthToCameraSpace(int x, int y)
{
	if (!isReady(true, false))
	{
		return ofVec3f();
	}
	DepthSpacePoint depth_space_point;
	depth_space_point.X = x;
	depth_space_point.Y = y;
	int index = x + y * depth_pixels->getWidth();
	UINT16 depth = depth_pixels->getData()[index];
	if (depth_to_camera_points.size() == 0)
	{
		depth_to_camera_points.resize(depth_pixels->size());
	}
	p_mapper->MapDepthPointToCameraSpace(depth_space_point, depth, reinterpret_cast<CameraSpacePoint*>(depth_to_camera_points.data()));
	return depth_to_camera_points[0];
}

//----------------------------------------------------------
ofVec3f Mapper::mapDepthToCameraSpace(ofVec2f depth_point)
{
	return mapDepthToCameraSpace(depth_point.x, depth_point.y);
}

//----------------------------------------------------------
vector<ofVec3f> Mapper::mapDepthToCameraSpace()
{
	if (!isReady(true, false))
	{
		return vector<ofVec3f>();
	}
	UINT depth_size = depth_pixels->size();
	if (depth_to_camera_points.size() != depth_size)
	{
		depth_to_camera_points.resize(depth_size);
	}
	p_mapper->MapDepthFrameToCameraSpace(depth_size, depth_pixels->getData(), depth_size, reinterpret_cast<CameraSpacePoint*>(depth_to_camera_points.data()));
	return depth_to_camera_points;
}

//----------------------------------------------------------
vector<ofVec3f> Mapper::mapDepthToCameraSpace(vector<ofVec2f> depth_points)
{
	if (!isReady(true, false))
	{
		return vector<ofVec3f>();
	}
	UINT depth_size = depth_points.size();
	if (!depth_space_points)
	{
		depth_space_points = new DepthSpacePoint[depth_pixels->size()];
	}
	if (!depth_values)
	{
		depth_values = new UINT16[depth_pixels->size()];
	}
	for (int i = 0; i < depth_size; i++)
	{
		depth_space_points[i].X = depth_points[i].x;
		depth_space_points[i].Y = depth_points[i].y;
		int index = depth_points[i].x + depth_points[i].y * depth_pixels->getWidth();
		depth_values[i] = depth_pixels->getData()[index];
	}

	if (depth_to_camera_points.size() != depth_size)
	{
		depth_to_camera_points.resize(depth_size);
	}
	p_mapper->MapDepthPointsToCameraSpace(depth_size, depth_space_points, depth_size, depth_values, depth_size, reinterpret_cast<CameraSpacePoint*>(depth_to_camera_points.data()));
	return depth_to_camera_points;
}

//----------------------------------------------------------
vector<ofVec3f> Mapper::mapDepthToCameraSpace(ofRectangle depth_area)
{
	if (!isReady(true, false))
	{
		return vector<ofVec3f>();
	}
	UINT depth_size = depth_area.getWidth() * depth_area.getHeight();
	if (!depth_space_points)
	{
		depth_space_points = new DepthSpacePoint[depth_pixels->size()];
	}
	if (!depth_values)
	{
		depth_values = new UINT16[depth_pixels->size()];
	}
	const unsigned short* data = depth_pixels->getData();
	int width = depth_area.getWidth();
	int d_width = depth_pixels->getWidth();
	for (int i = 0; i < depth_size; i++)
	{
		depth_space_points[i].X = i % width;
		depth_space_points[i].Y = (int)floor(i / width);
		int index = depth_space_points[i].X + depth_space_points[i].Y * d_width;
		depth_values[i] = data[index];
	}

	if (depth_to_camera_points.size() != depth_size)
	{
		depth_to_camera_points.resize(depth_size);
	}
	p_mapper->MapDepthPointsToCameraSpace(depth_size, depth_space_points, depth_size, depth_values, depth_size, reinterpret_cast<CameraSpacePoint*>(depth_to_camera_points.data()));
	return depth_to_camera_points;
}

//----------------------------------------------------------
ofVec2f Mapper::mapDepthToColorSpace(int x, int y)
{
	if (!isReady(true, false))
	{
		return ofVec2f();
	}
	DepthSpacePoint depth_space_point;
	depth_space_point.X = x;
	depth_space_point.Y = y;
	int index = x + y * depth_pixels->getWidth();
	UINT16 depth = depth_pixels->getData()[index];
	if (depth_to_color_points.size() == 0)
	{
		depth_to_color_points.resize(depth_pixels->size());
	}
	p_mapper->MapDepthPointToColorSpace(depth_space_point, depth, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points[0];
}

//----------------------------------------------------------
ofVec2f Mapper::mapDepthToColorSpace(ofVec2f depth_point)
{
	return mapDepthToColorSpace(depth_point.x, depth_point.y);
}

//----------------------------------------------------------
vector<ofVec2f> Mapper::mapDepthToColorSpace()
{
	if (!isReady(true, false))
	{
		return vector<ofVec2f>();
	}
	UINT depth_size = depth_pixels->size();
	if (depth_to_color_points.size() != depth_size)
	{
		depth_to_color_points.resize(depth_size);
	}
	p_mapper->MapDepthFrameToColorSpace(depth_size, depth_pixels->getData(), depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points;
}

//----------------------------------------------------------
vector<ofVec2f> Mapper::mapDepthToColorSpace(vector<ofVec2f> depth_points)
{
	if (!isReady(true, false))
	{
		return vector<ofVec2f>();
	}
	UINT depth_size = depth_points.size();
	if (!depth_space_points)
	{
		depth_space_points = new DepthSpacePoint[depth_pixels->size()];
	}
	if (!depth_values)
	{
		depth_values = new UINT16[depth_pixels->size()];
	}
	int d_width = depth_pixels->getWidth();
	const unsigned short* data = depth_pixels->getData();
	for (int i = 0; i < depth_size; i++)
	{
		depth_space_points[i].X = depth_points[i].x;
		depth_space_points[i].Y = depth_points[i].y;
		depth_values[i] = data[(int)depth_space_points[i].X + (int)depth_space_points[i].Y * d_width];
	}

	if (depth_to_color_points.size() != depth_size)
	{
		depth_to_color_points.resize(depth_size);
	}
	p_mapper->MapDepthPointsToColorSpace(depth_size, depth_space_points, depth_size, depth_values, depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points;
}
//----------------------------------------------------------
vector<ofVec2f> Mapper::mapDepthToColorSpace(ofRectangle depth_area)
{
	if (!isReady(true, false))
	{
		return vector<ofVec2f>();
	}
	UINT depth_size = depth_area.getWidth() * depth_area.getHeight();
	if (!depth_space_points)
	{
		depth_space_points = new DepthSpacePoint[depth_pixels->size()];
	}
	if (!depth_values)
	{
		depth_values = new UINT16[depth_pixels->size()];
	}
	int width = depth_area.getWidth();
	int d_width = depth_pixels->getWidth();
	const unsigned short* data = depth_pixels->getData();
	for (int i = 0; i < depth_size; i++)
	{
		depth_space_points[i].X = i % width;
		depth_space_points[i].Y = (int)floor(i / width);
		depth_values[i] = data[(int)depth_space_points[i].X + (int)depth_space_points[i].Y * d_width];
	}

	if (depth_to_color_points.size() != depth_size)
	{
		depth_to_color_points.resize(depth_size);
	}
	p_mapper->MapDepthPointsToColorSpace(depth_size, depth_space_points, depth_size, depth_values, depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points;
}

//----------------------------------------------------------
vector<ofVec3f> Mapper::mapColorToCameraSpace()
{
	if (!isReady(true, true))
	{
		return vector<ofVec3f>();
	}
	int depth_size = depth_pixels->size();
	int color_size = color_pixels->size();
	if (color_to_camera_points.size() != color_size)
	{
		color_to_camera_points.resize(color_size);
	}
	p_mapper->MapColorFrameToCameraSpace(depth_size, depth_pixels->getData(), color_size, reinterpret_cast<CameraSpacePoint*>(color_to_camera_points.data()));
	return color_to_camera_points;
}

//----------------------------------------------------------
vector<ofVec2f> Mapper::mapColorToDepthSpace()
{
	if (!isReady(true, true))
	{
		return vector<ofVec2f>();
	}
	UINT depth_size = depth_pixels->size();
	UINT color_size = color_pixels->size();
	if (color_to_depth_points.size() != color_size)
	{
		color_to_depth_points.resize(color_size);
	}
	p_mapper->MapColorFrameToDepthSpace(depth_size, depth_pixels->getData(), color_size, reinterpret_cast<DepthSpacePoint*>(color_to_depth_points.data()));
	return color_to_depth_points;
}

//----------------------------------------------------------
ofVec2f Mapper::mapCameraToDepthSpace(float x, float y, float z)
{
	if (!isReady(true, false))
	{
		return ofVec2f();
	}
	CameraSpacePoint camera_space_point;
	camera_space_point.X = x;
	camera_space_point.Y = y;
	camera_space_point.Z = z;
	if (camera_to_depth_points.size() == 0)
	{
		camera_to_depth_points.resize(depth_pixels->size());
	}
	p_mapper->MapCameraPointToDepthSpace(camera_space_point, reinterpret_cast<DepthSpacePoint*>(camera_to_depth_points.data()));
	return camera_to_depth_points[0];
}

//----------------------------------------------------------
ofVec2f Mapper::mapCameraToDepthSpace(ofVec3f camera_point)
{
	return mapCameraToDepthSpace(camera_point.x, camera_point.y, camera_point.z);
}

//----------------------------------------------------------
vector<ofVec2f> Mapper::mapCameraToDepthSpace(vector<ofVec3f> camera_points)
{
	if (!isReady(true, false))
	{
		return vector<ofVec2f>();
	}
	UINT camera_size = camera_points.size();
	if (!camera_space_points)
	{
		camera_space_points = new CameraSpacePoint[depth_pixels->size()];
	}
	for (int i = 0; i < camera_size; i++)
	{
		camera_space_points[i].X = camera_points[i].x;
		camera_space_points[i].Y = camera_points[i].y;
		camera_space_points[i].Z = camera_points[i].z;
	}

	if (camera_to_depth_points.size() != camera_size)
	{
		camera_to_depth_points.resize(camera_size);
	}
	p_mapper->MapCameraPointsToDepthSpace(camera_size, camera_space_points, camera_size, reinterpret_cast<DepthSpacePoint*>(camera_to_depth_points.data()));
	return camera_to_depth_points;
}

//----------------------------------------------------------
ofVec2f Mapper::mapCameraToColorSpace(float x, float y, float z)
{
	if (!isReady(true, false))
	{
		return ofVec2f();
	}
	CameraSpacePoint camera_space_point;
	camera_space_point.X = x;
	camera_space_point.Y = y;
	camera_space_point.Z = z;
	if (depth_to_color_points.size() == 0)
	{
		depth_to_color_points.resize(depth_pixels->size());
	}
	p_mapper->MapCameraPointToColorSpace(camera_space_point, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points[0];
}

//----------------------------------------------------------
ofVec2f Mapper::mapCameraToColorSpace(ofVec3f camera_point)
{
	return mapCameraToColorSpace(camera_point.x, camera_point.y, camera_point.z);
}

//----------------------------------------------------------
vector<ofVec2f> Mapper::mapCameraToColorSpace(vector<ofVec3f> camera_points)
{
	if (!isReady(true, false))
	{
		return vector<ofVec2f>();
	}
	UINT camera_size = camera_points.size();
	if (!camera_space_points)
	{
		camera_space_points = new CameraSpacePoint[depth_pixels->size()];
	}
	for (int i = 0; i < camera_size; i++)
	{
		camera_space_points[i].X = camera_points[i].x;
		camera_space_points[i].Y = camera_points[i].y;
		camera_space_points[i].Z = camera_points[i].z;
	}

	if (depth_to_color_points.size() != camera_size)
	{
		depth_to_color_points.resize(camera_size);
	}
	p_mapper->MapCameraPointsToColorSpace(camera_size, camera_space_points, camera_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));
	return depth_to_color_points;
}


//----------------------------------------------------------
vector<ofFloatColor> Mapper::getFloatColorsCoordinatesToDepthFrame()
{
	if (!isReady(true, true))
	{
		return vector<ofFloatColor>();
	}
	UINT depth_size = depth_pixels->size();
	const UINT16* depth_pix = depth_pixels->getData();
	if (depth_to_color_points.size() == 0)
	{
		depth_to_color_points.resize(depth_pixels->size());
	}
	if (depth_to_float_colors.size() != depth_size)
	{
		depth_to_float_colors.resize(depth_size);
	}
	p_mapper->MapDepthFrameToColorSpace(depth_size, depth_pix, depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));

	int col_width = color_pixels->getWidth();
	int col_height = color_pixels->getHeight();
	const unsigned char* data = color_pixels->getData();
	for (int i = 0; i < depth_size; i++)
	{
		ofFloatColor &col = depth_to_float_colors[i];
		int index = (int)depth_to_color_points[i].x + (int)depth_to_color_points[i].y * col_width;
		if (depth_to_color_points[i].x >= 0 && depth_to_color_points[i].x < col_width && depth_to_color_points[i].y >= 0 && depth_to_color_points[i].y < col_height)
		{
				col.r = data[index * 4] / 255.f;
				col.g = data[index * 4 + 1] / 255.f;
				col.b = data[index * 4 + 2] / 255.f;
				col.a = data[index * 4 + 3] / 255.f;
		}
		else
		{
			col = ofFloatColor(0, 0, 0, 0);
		}
	}
	return depth_to_float_colors;
}

//----------------------------------------------------------
vector<ofColor> Mapper::getColorsCoordinatesToDepthFrame()
{
	if (!isReady(true, true))
	{
		return vector<ofColor>();
	}
	UINT depth_size = depth_pixels->size();
	const UINT16* depth_pix = depth_pixels->getData();
	if (depth_to_color_points.size() == 0)
	{
		depth_to_color_points.resize(depth_size);
	}
	if (depth_to_colors.size() != depth_size)
	{
		depth_to_colors.resize(depth_size);
	}
	p_mapper->MapDepthFrameToColorSpace(depth_size, depth_pix, depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));

	int col_width = color_pixels->getWidth();
	int col_height = color_pixels->getHeight();
	const unsigned char* data = color_pixels->getData();
	for (int i = 0; i < depth_size; i++)
	{
		ofColor &col = depth_to_colors[i];
		int index = (int)depth_to_color_points[i].x + (int)depth_to_color_points[i].y * col_width;
		if (depth_to_color_points[i].x >= 0 && depth_to_color_points[i].x < col_width && depth_to_color_points[i].y >= 0 && depth_to_color_points[i].y < col_height)
		{
			col.r = data[index * 4];
			col.g = data[index * 4 + 1];
			col.b = data[index * 4 + 2];
			col.a = data[index * 4 + 3];
		}
		else
		{
			col = ofColor(0, 0, 0, 0);
		}
	}
	return depth_to_colors;
}

//----------------------------------------------------------
ofPixels Mapper::getColorFrameCoordinatesToDepthFrame()
{
	if (!isReady(true, true))
	{
		return ofPixels();
	}
	UINT depth_size = depth_pixels->size();
	const UINT16* depth_pix = depth_pixels->getData();
	if (depth_to_color_points.size() == 0)
	{
		depth_to_color_points.resize(depth_size);
	}
	if (depth_to_colors.size() != depth_size)
	{
		depth_to_colors.resize(depth_size);
	}
	p_mapper->MapDepthFrameToColorSpace(depth_size, depth_pix, depth_size, reinterpret_cast<ColorSpacePoint*>(depth_to_color_points.data()));

	int col_width = color_pixels->getWidth();
	int col_height = color_pixels->getHeight();

	if (!coordinate_color_pixels.isAllocated())
	{
		coordinate_color_pixels.allocate(depth_pixels->getWidth(), depth_pixels->getHeight(), OF_PIXELS_RGBA);
	}
	const unsigned char* data = color_pixels->getData();
	for (int i = 0; i < depth_size; i++)
	{
		int index = (int)depth_to_color_points[i].x + (int)depth_to_color_points[i].y * col_width;
		if (depth_to_color_points[i].x >= 0 && depth_to_color_points[i].x < col_width && depth_to_color_points[i].y >= 0 && depth_to_color_points[i].y < col_height)
		{
			coordinate_color_pixels[i * 4] = data[index * 4];
			coordinate_color_pixels[i * 4 + 1] = data[index * 4 + 1];
			coordinate_color_pixels[i * 4 + 2] = data[index * 4 + 2];
			coordinate_color_pixels[i * 4 + 3] = data[index * 4 + 3];
		}
		else
		{
			coordinate_color_pixels[i * 4] = 0;
			coordinate_color_pixels[i * 4 + 1] = 0;
			coordinate_color_pixels[i * 4 + 2] = 0;
			coordinate_color_pixels[i * 4 + 3] = 0;
		}
	}
	return coordinate_color_pixels;
}
/**/
