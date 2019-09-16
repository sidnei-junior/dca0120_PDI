#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  Mat image;

  image= imread("chesterton.png",CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data)
    cout << "nao abriu bolhas.png" << endl;

  namedWindow("janela",WINDOW_AUTOSIZE);
  imshow("janela", image); 
  
  cout << "x: " << image.rows << " y: " << image.cols << endl;

  cout << "Digite o ponto P1 e P2 determinando uma região para ter suas cores negativas." << endl;
  cout << "x de P1: ";
  int P1x;
  cin >> P1x;
  cout << "y de P1: ";
  int P1y;
  cin >> P1y;

  cout << "x de P2: ";
  int P2x;
  cin >> P2x;
  cout << "y de P2: ";
  int P2y;
  cin >> P2y;

  // ordenando coordenadas
  int aux;
  //ordenando em x

  if (P1x > P2x) {
    aux = P1x;
    P1x = P2x;
    P2x = aux;
  }

  //ordenando em y

  if (P1y > P2y) {
    aux = P1y;
    P1y = P2y;
    P2y = aux;
  }


  image= imread("chesterton.png",CV_LOAD_IMAGE_GRAYSCALE);

  
  for(int i=P1x;i<P2x;i++){
    for(int j=P1y;j<P2y;j++){
      image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
    }
  }

  imshow("janela", image);  
  imwrite("chesterton_region.png", image);
  waitKey();
  return 0;
}
