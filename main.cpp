#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "tracker/kcftracker.hpp"
#include "tracker/tracker.h"
#include "video_wrapper.h"

cv::Point center;


void on_mouse(int EVENT, int x, int y, int flags, void *userdata){
    cv::Mat frame;
    frame = *(cv::Mat*) userdata;
    center.x = x;
    center.y = y;
    switch(EVENT){
        case CV_EVENT_LBUTTONDOWN:{
            //cv::Rect2i box(center.x-18, center.y-24, 36, 48);
            //cv::rectangle(frame, box, cv::Scalar(255,0,0), 1);
            std::cout<<x<<", "<<y<<std::endl;

        }
            break;
    }
}


int getCNT(){
    int cnt = 0;
    std::ifstream in("cnt.txt");
    if (in.is_open())
    {
        in>>cnt;
        in.close();
    }

    cnt++;

    std::ofstream out("cnt.txt");
    if(out.is_open())
    {
        out<<cnt<<std::endl;
        out.close();

    }
    return cnt;
}



int main() {

    int cnt = getCNT();
    int isPositiveLabel = 1;
    std::cout<<"input 1 to label positive target, input 0 to label negative target"<<std::endl;
    std::cin>>isPositiveLabel;
    std::string labelPrefix;
    if(isPositiveLabel){
        int armor_id;
        std::cout<<"input the armor id you are labeling (check the video)"<<std::endl;
        std::cin>>armor_id;
        labelPrefix = "positive/id"+std::to_string(armor_id)+"_";
    }

    else labelPrefix = "negative/";

    VideoWrapper video("video.mp4");

    if(!video.init()){
        std::cout<<"open video file failed"<<std::endl;
        return 0;
    }


    cv::Mat frame;
    int  width = 48, height = 36;

    for(int i = 0; i < 5; i++)
        video.read(frame);
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
                       "press 'r' to label next frame, press digit to change armor id"<<std::endl;
            else
                std::cout<<"click an area and press space"<<std::endl;
            q=cv::waitKey(0);
            if(q -'0' > 0 && q-'0' <=9)
            {
                labelPrefix = "positive/id"+std::to_string(q-'0')+"_";
                std::cout<<"change to label armor id "<< q-'0'<<std::endl;
            }

            if(q == ' ') break;

        }
        std::cout<<center.x<<", "<<center.y<<std::endl;

        cv::Rect2i box(center.x-width/2, center.y-height/2, width, height);      //  48*36
        KCFTracker kcf_tracker(true, true, false, false);
        kcf_tracker.init(box, frame);
        while(video.read(frame))
        {
            box = kcf_tracker.update(frame);
            cv::Mat roi = frame.clone();
            cv::rectangle(roi, box, cv::Scalar(255, 0, 0), 1);
            cv::imshow("frame", roi);
            std::cout<<"press space to save current box, press 'r' to relabel the wrong box.\n"
                       "use 'wsad' to move box, use 'q/e' to turn the box smaller/bigger"<<std::endl;
            q = cv::waitKey(0);


            if(q == 'r'){
                break;
            } else{
                cnt = getCNT();
                bool save_state = cv::imwrite(labelPrefix+std::to_string(cnt)+".jpg", frame(box));
                if(isPositiveLabel)
                {
                    cv::imwrite(labelPrefix+std::to_string(cnt)+"_full.jpg", frame);
                    std::ofstream out(labelPrefix+std::to_string(cnt)+ "_full.txt");
                    if(out.is_open())
                    {
                        out<<box.x<<" "<<box.y<<" "<<box.width<<" "<<box.height<<std::endl;
                        out.close();
                    }
                }

                if(save_state)
                    std::cout<<"save "+labelPrefix<<cnt<<".jpg successfully."<<std::endl;
                else std::cout<<"save failed."<<std::endl;

                if(q == 'd' || q == 'a' || q=='w' || q=='s'|| q=='q' || q=='e'){
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
                    }
                    new_box.width = width; new_box.height = height;


                    if((cv::Rect2i(0,0,639,479) & new_box) != new_box)
                        break;
                    kcf_tracker.init(new_box, frame);
                }



            }
        }


    }



    return 0;
}