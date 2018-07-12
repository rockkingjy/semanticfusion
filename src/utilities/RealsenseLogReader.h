
#ifndef REALSENSELOGREADER_H_
#define REALSENSELOGREADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <signal.h>
#include <iostream>

#include "LogReader.h"

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API

class RealsenseLogReader : public LogReader
{
  public:
    RealsenseLogReader(std::string file, bool flipColors);

    virtual ~RealsenseLogReader();

    void getNext();

    int getNumFrames();

    bool hasMore();

    bool rewound()
    {
        return false;
    }

    bool is_valid()
    {
        return valid;
    }

    void getBack()
    {
    }

    void fastForward(int frame)
    {
    }

    void setAuto(bool value);

    const std::string getFile()
    {
        return file;
    };

  private:
    int64_t lastFrameTime;
    int lastGot;
    bool valid;

    // Declare depth colorizer for pretty visualization of depth data
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
};

#endif /* LIVELOGREADER_H_ */
