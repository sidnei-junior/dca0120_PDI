#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m)
{
  for (int i = 0; i < m.size().height; i++)
  {
    for (int j = 0; j < m.size().width; j++)
    {
      cout << m.at<float>(i, j) << ",";
    }
    cout << endl;
  }
}

void menu()
{
  cout << "\npressione a tecla para ativar o filtro: \n"
          "a - calcular modulo\n"
          "m - media\n"
          "g - gauss\n"
          "v - vertical\n"
          "h - horizontal\n"
          "l - laplaciano\n"
          "d - gauss + laplaciano\n"
          "esc - sair\n";
}

int main(int argvc, char **argv)
{

  VideoCapture video;
  float media[] = {1, 1, 1,
                   1, 1, 1,
                   1, 1, 1};
  float gauss[] = {1, 2, 1,
                   2, 4, 2,
                   1, 2, 1};
  float horizontal[] = {-1, 0, 1,
                        -2, 0, 2,
                        -1, 0, 1};
  float vertical[] = {-1, -2, -1,
                      0, 0, 0,
                      1, 2, 1};
  float laplacian[] = {0, -1, 0,
                       -1, 4, -1,
                       0, -1, 0};

  Mat cap, frame, frame32f, frameFiltered, frame32f2, frameFiltered2;
  Mat mask(3, 3, CV_32F), mask1, maskGL;
  Mat result, result1, result2;
  double width, height /*, min, max*/;
  int absolut, command = 0;
  char key;

  video.open(0);
  if (!video.isOpened())
    return -1;
  width = video.get(CV_CAP_PROP_FRAME_WIDTH);
  height = video.get(CV_CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;

  namedWindow("filtroespacial", 1);

  mask = Mat(3, 3, CV_32F, media);
  scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
  swap(mask, mask1);
  absolut = 1; // calcs abs of the image

  menu();
  for (;;)
  {

    video >> cap;
    cvtColor(cap, frame, CV_BGR2GRAY);
    flip(frame, frame, 1);
    imshow("original", frame);
    imwrite("orig.png", frame);

    frame.convertTo(frame32f, CV_32F);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1, 1), 0);

    if (absolut)
    {
      frameFiltered = abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    //imshow("filtroespacial2", result);

    if (command == 1)
    {
      result.convertTo(frame32f2, CV_32F);
      filter2D(frameFiltered, frameFiltered2, frameFiltered.depth(), maskGL, Point(1, 1), 0);
      frameFiltered2.convertTo(result2, CV_8U);
      imshow("filtroespacial", result2);
      imwrite("lapGal.png", result2);
    }else{
      imshow("filtroespacial", result);
      imwrite("lap.png", result);
    }

    

    key = (char)waitKey(10);
    if (key == 27)
      break; // esc pressed!

    switch (key)
    {
    case 'a':
      
      absolut = !absolut;
      command = 0;

      menu();
      break;
    case 'm':
      
      mask = Mat(3, 3, CV_32F, media);
      scaleAdd(mask, 1 / 9.0, Mat::zeros(3, 3, CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      command = 0;

      menu();
      break;
    case 'g':
      
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1 / 16.0, Mat::zeros(3, 3, CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      command = 0;

      menu();
      break;
    case 'h':
      
      mask = Mat(3, 3, CV_32F, horizontal);
      printmask(mask);
      command = 0;

      menu();
      break;
    case 'v':
      
      mask = Mat(3, 3, CV_32F, vertical);
      printmask(mask);
      command = 0;

      menu();
      break;
    case 'l':
      
      mask = Mat(3, 3, CV_32F, laplacian);
      printmask(mask);
      command = 0;

      menu();
      break;

    case 'd':
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1 / 16.0, Mat::zeros(3, 3, CV_32F), mask1);
      mask = mask1;

      maskGL = Mat(3, 3, CV_32F, laplacian);

      command = 1;
      cout << "Command agora é: " << command << endl;
      printmask(mask);
      printmask(maskGL);

      menu();
      break;
    default:
      break;
    }
  }
  return 0;
}
