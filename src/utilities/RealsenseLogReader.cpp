
#include "RealsenseLogReader.h"

RealsenseLogReader::RealsenseLogReader(std::string file, bool flipColors)
    : LogReader(file, flipColors),
      lastFrameTime(-1),
      lastGot(-1),
      valid(false)
{
    std::cout << "Creating Real sense capture... " << std::endl;
    std::cout.flush();
    
	decompressionBufferDepth = new Bytef[Resolution::getInstance().numPixels() * 2];

	decompressionBufferImage = new Bytef[Resolution::getInstance().numPixels() * 3];

    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);

    // Start streaming with default recommended configuration
    pipe.start();

    valid = true;

    std::cout << "Initialize real sense." << std::endl;
}

RealsenseLogReader::~RealsenseLogReader()
{
    delete[] decompressionBufferDepth;

    delete[] decompressionBufferImage;

    //	delete asus;
}

void RealsenseLogReader::getNext()
{
    //std::cout << "getNext real sense." << std::endl;
    rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
    rs2::frame dp = data.get_depth_frame();//color_map(data.get_depth_frame()); // Find and colorize the depth data
    rs2::frame color = data.get_color_frame();            // Find the color data

    // For cameras that don't have RGB sensor, we'll render infrared frames instead of color
    if (!color)
        color = data.get_infrared_frame();

    memcpy(&decompressionBufferDepth[0], dp.get_data(), Resolution::getInstance().numPixels() * 2);
    memcpy(&decompressionBufferImage[0], color.get_data(), Resolution::getInstance().numPixels() * 3);

    //std::cout << "flag" << std::endl;
    rgb = (unsigned char *)&decompressionBufferImage[0];
    depth = (unsigned short *)&decompressionBufferDepth[0];

    imageReadBuffer = 0;
    depthReadBuffer = 0;

    imageSize = Resolution::getInstance().numPixels() * 3;
    depthSize = Resolution::getInstance().numPixels() * 2;

    if (flipColors)
    {
        for (int i = 0; i < Resolution::getInstance().numPixels() * 3; i += 3)
        {
            std::swap(rgb[i + 0], rgb[i + 2]);
        }
    }
}

int RealsenseLogReader::getNumFrames()
{
    return std::numeric_limits<int>::max();
}

bool RealsenseLogReader::hasMore()
{
    return true;
}

void RealsenseLogReader::setAuto(bool value)
{
}
