#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "tracker/kcftracker.hpp"
#include "tracker/tracker.h"
#include "video_wrapper.h"
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

cv::Point center;


void on_mouse(int EVENT, int x, int y, int flags, void *userdata){
    cv::Mat frame;
    frame = *(cv::Mat*) userdata;

    switch(EVENT){
        case CV_EVENT_LBUTTONDOWN:{
            //cv::Rect2i box(center.x-18, center.y-24, 36, 48);
            //cv::rectangle(frame, box, cv::Scalar(255,0,0), 1);
            center.x = x;
            center.y = y;
            std::cout<<x<<", "<<y<<std::endl;

        }
            break;
    }
}



std::string getPicFilename(){
    return std::to_string(time(NULL)) + std::to_string(clock());
}



int main() {
    //4,5,10,39
    std::string video_filename = "/home/zhikun/Videos/armor_id/11.avi";
    bool color_mode = true;
    VideoWrapper video(video_filename, color_mode);
    cv::Mat frame;


    int isPositiveLabel = 1;
    std::cout<<"input 1 to label positive target, input 0 to label negative target"<<std::endl;
    std::cin>>isPositiveLabel;
    std::string labelPrefix;
    int armor_id=0, armor_color;

    if(isPositiveLabel){   // set label id, be ready to make training set later
        std::cout<<"input the armor id you are labeling (check the video)"<<std::endl;
        std::cin>>armor_id;
        if(armor_id >= 7) armor_id -= 1;

        std::cout<<"choose armor color, 1 for blue, 2 for red"<<std::endl;
        std::cin>>armor_color;
        if(armor_color == 2) armor_id += 7;
        std::string pathname = "id"+std::to_string(armor_id);
        if(access(pathname.data(), F_OK) == -1)
            mkdir(pathname.data(), 0777);
        labelPrefix = pathname + "/id_"+std::to_string(armor_id)+"_";
    }
    else labelPrefix = "negative/id_neg_";


    int cnt = 0;

    if(!video.init()){
        std::cout<<"open video file failed"<<std::endl;
        return 0;
    }



    int  width = 48, height = 36;


    //std::cout<<frame.type()<<std::endl;
    int q;
    while(video.read(frame))
    {
        cv::namedWindow("frame");
        cv::setMouseCallback("frame", on_mouse, &frame);
        while(video.read(frame)){

            cv::imshow("frame", frame);
            if(isPositiveLabel)
                std::cout<<"click the center of armor with ID. \n then press space to confirm, "
                       "press 'r' to label next frame"<<std::endl;
            else
                std::cout<<"click an area and press space"<<std::endl;
            q=cv::waitKey(0);

            if(q == ' ') break;

        }
        std::cout<<center.x<<", "<<center.y<<std::endl;

        cv::Rect2i box(center.x-width/2, center.y-height/2, width, height);      //  48*36
        KCFTracker kcf_tracker(true, true, false, false);
        kcf_tracker.init(box, frame);
        std::cout<<"press space to save current box, press 'r' to relabel the wrong box.\n"
                   "use 'wsad' to move box, use 'q/e' to turn the box smaller/bigger\n"
                   "use 'jlik' to turn the box smaller/bigger just by one side"<<std::endl;
        while(true)
        {
            box = kcf_tracker.update(frame);
            cv::Mat roi = frame.clone();
            cv::rectangle(roi, box, cv::Scalar(255, 0, 0), 1);
            cv::imshow("frame", roi);

            q = cv::waitKey(0);


            if(q == 'r'){
                break;
            } else{
                std::string file_id = getPicFilename();

                bool save_state = true;
                //save_state = cv::imwrite(labelPrefix+file_id+".jpg", frame(box));

                if(q != ' '){
                    cv::Rect2i new_box;
                    switch (q){
                        case 'q':
                            width -= 4; height -= 4;
                            new_box.x = box.x+2; new_box.y = box.y+2;
                            break;
                        case 'e':
                            width +=4; height +=4;
                            new_box.x = box.x-2; new_box.y = box.y-2;
                            break;
                        case 'w':
                            new_box.x = box.x; new_box.y = box.y-2;
                            break;
                        case 's':
                            new_box.x = box.x; new_box.y = box.y+2;
                            break;
                        case 'a':
                            new_box.x = box.x-2; new_box.y = box.y;
                            break;
                        case 'd':
                            new_box.x = box.x+2; new_box.y = box.y;
                            break;
                        case 'j':
                            width -= 2; new_box.x = box.x; new_box.y = box.y;
                            break;
                        case 'l':
                            width += 2; new_box.x = box.x; new_box.y = box.y;
                            break;
                        case 'k':
                            height -=2; new_box.x = box.x; new_box.y = box.y;
                            break;
                        case 'i':
                            height += 2; new_box.x = box.x; new_box.y = box.y;
                            break;

                    }
                    new_box.width = width; new_box.height = height;


                    if((cv::Rect2i(0,0,639,479) & new_box) != new_box)
                        break;
                    kcf_tracker.init(new_box, frame);

                } else {
                    if((cv::Rect2i(0,0,640,480) & box) != box)
                        break;
                    if(cnt % 5 == 0)
                    {
                        save_state = save_state && cv::imwrite(labelPrefix+file_id+".jpg", frame);
                        std::ofstream out(labelPrefix+file_id + ".txt");
                        if(out.is_open())
                        {
                            out<<box.x<<","<<box.y<<","<<box.x+box.width<<","<<box.y+box.height<<","<<armor_id<<std::endl;
                            out.close();
                        }
                        if(save_state)
                            std::cout<<cnt/5<<std::endl;
                        else std::cout<<"save failed."<<std::endl;
                        video.read(frame);
                    }
                    cnt++;
                }
            }
        }
    }



    return 0;
}