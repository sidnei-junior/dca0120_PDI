#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;

  image= imread("chesterton.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data) cout << "nao abriu biel.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);
  imshow("janela", image);   
  waitKey();
  

  int coluna = image.cols/2;
  int linha = image.rows/2;

  Rect ROI[4] = {
    Rect(0, 0, linha, coluna),
    Rect(0, coluna, linha, coluna),
    Rect(linha, 0, linha, coluna),
    Rect(linha, coluna, linha, coluna)
  };

  Mat image2;
  image.copyTo(image2);

  Mat A (image, ROI[0]);
  Mat B (image, ROI[1]);
  Mat C (image, ROI[2]);
  Mat D (image, ROI[3]);

  A.copyTo(image2(ROI[2]));
  B.copyTo(image2(ROI[0]));
  C.copyTo(image2(ROI[3]));
  D.copyTo(image2(ROI[1]));
  
  //cout << "x: " << image.rows << " y: " << image.cols << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);
  imshow("janela", image2);  
  imwrite("chesterton_troca.png", image2);
  waitKey();

  return 0;
}
