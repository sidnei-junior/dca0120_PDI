#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>
using namespace std;
using namespace cv;

#define RAIO_MAX 10
#define JITTER_MAX 10
#define STEP_MAX 20

int top_slider = 50;
int top_slider_max = 200;
int raio_slider = 3;
int jitter = 3;
int step = 5;
int width, height;

char TrackbarName[50];

Mat image, border, points;

void function_aux(int, void*);

void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);
  function_aux(top_slider, 0);

}

int main(int argc, char**argv){
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  width=image.size().width;
  height=image.size().height;

  sprintf( TrackbarName, "canny");

  namedWindow("cannypoints",1);
  createTrackbar( TrackbarName, "cannypoints",
                &top_slider,
                top_slider_max,
                on_trackbar_canny );

  sprintf( TrackbarName, "raio do ponto");
  createTrackbar( TrackbarName, "cannypoints",
                  &raio_slider,
                  RAIO_MAX,
                  function_aux );

  sprintf( TrackbarName, "Jitter");
  createTrackbar( TrackbarName, "cannypoints",
                  &jitter,
                  JITTER_MAX,
                  function_aux );

  sprintf( TrackbarName, "passo");
  createTrackbar( TrackbarName, "cannypoints",
                  &step,
                  STEP_MAX,
                  function_aux );

  on_trackbar_canny(top_slider, 0 );

  waitKey();
  return 0;
}

void function_aux(int, void*){
    vector<int> yrange;
    vector<int> xrange;
    int width, height;
    uchar gray;
    int x, y;

    width=image.size().width;
    height=image.size().height;

    xrange.resize(height/step);
    yrange.resize(width/step);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for(uint i=0; i<xrange.size(); i++){
      xrange[i]= xrange[i]*step+step/2;
    }

    for(uint i=0; i<yrange.size(); i++){
      yrange[i]= yrange[i]*step+step/2;
    }


    points = Mat(height, width, CV_8U, Scalar(255));

    random_shuffle(xrange.begin(), xrange.end());


    for(auto i : xrange){
      random_shuffle(yrange.begin(), yrange.end());
      for(auto j : yrange){
        x = i+rand()%(2*jitter)-jitter+1;
        y = j+rand()%(2*jitter)-jitter+1;
        gray = image.at<uchar>(x,y);
        circle(points,
               cv::Point(y,x),
               raio_slider,
               CV_RGB(gray,gray,gray),
               -1,
               CV_AA);
      }
    }

    vector<vector<Point>> contornos;
    vector<Vec4i> hierarchy;
    for (int i = 0; i < int(contornos.size()); i++){
      for (int j = 0; j < int(contornos[i].size()); j++) {
        gray = image.at<uchar>(contornos[i][j].y, contornos[i][j].x);
        circle(points, cv::Point(contornos[i][j].x, contornos[i][j].y),
        1,
        CV_RGB(gray, gray, gray),
        -1,
        CV_AA);
      }
    }

    imshow("cannypoints", points);
    imwrite("bokuP.jpg", points);
    imshow("canny", border);
    imwrite("bokuB.jpg", border);

}