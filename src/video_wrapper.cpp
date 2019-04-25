//
// Created by zhikun on 18-11-16.
//

#include "video_wrapper.h"


VideoWrapper::VideoWrapper(const std::string &filename0, bool color) {
    video.open(filename0);
    color_mode = color;

}

VideoWrapper::~VideoWrapper() = default;


bool VideoWrapper::init() {
    return video.isOpened();
}

bool VideoWrapper::read(cv::Mat &src_left) {
    if(color_mode)
        return video.read(src_left);

    bool flag = video.read(frame);

    std::vector<cv::Mat> mv(3);
    cv::split(frame, mv);
    src_left = mv.at(0).clone();

    if (CV_8UC1 == src_left.type()) {
        return flag;
    } else {
        return false;
    }




}
