#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image, imgTonsDeCinzas, imgEqualize;
  int width, height;
  VideoCapture cap;

  Mat hist1, hist2;
  int nbins = 128;
  float range[] = {0, 256};
  const float *histrange = {range};

  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if (!cap.isOpened())
  {
    cout << "cameras indisponiveis";
    return -1;
  }

  width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_8UC1, Scalar(0, 0, 0));
  Mat histImg2(histh, histw, CV_8UC1, Scalar(0, 0, 0));
  
  while (1)
  {
    cap >> image;

    cvtColor(image, imgTonsDeCinzas, CV_BGR2GRAY);
    equalizeHist(imgTonsDeCinzas, imgEqualize);

    calcHist(&imgTonsDeCinzas, 1, 0, Mat(), hist1, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&imgEqualize, 1, 0, Mat(), hist2, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(hist1, hist1, 0, histImg.rows, NORM_MINMAX, -1, Mat());
    normalize(hist2, hist2, 0, histImg2.rows, NORM_MINMAX, -1, Mat());

    histImg.setTo(Scalar(0));
    histImg2.setTo(Scalar(0));

    for (int i = 0; i < nbins; i++)
    {
      line(histImg,
           Point(i, histh),
           Point(i, histh - cvRound(hist1.at<float>(i))),
           Scalar(128), 1, 8, 0);
    }
    for (int i = 0; i < nbins; i++)
    {
      line(histImg2,
           Point(i, histh),
           Point(i, histh - cvRound(hist2.at<float>(i))),
           Scalar(128), 1, 8, 0);
    }
    histImg.copyTo(imgTonsDeCinzas(Rect(0, 0, nbins, histh)));
    histImg2.copyTo(imgEqualize(Rect(0, 0, nbins, histh)));
    //namedWindow("image");
    //namedWindow("equa");
    imshow("image", imgTonsDeCinzas);
    imshow("equa", imgEqualize);

    //imshow("hist1", histImg);
    //imshow("hist2", histImg2);
    if (waitKey(30) >= 0) break;
  }
  imwrite("habito.png", imgTonsDeCinzas);
  imwrite("habito_eq.png", imgEqualize);
  waitKey();
  return 0;
}
