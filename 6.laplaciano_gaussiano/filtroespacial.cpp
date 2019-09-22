#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m){
  for(int i=0; i<m.size().height; i++){
    for(int j=0; j<m.size().width; j++){
      cout << m.at<float>(i,j) << ",";
    }
    cout << endl;
  }
}

void menu(){
  cout << "\npressione a tecla para ativar o filtro: \n"
	"a - calcular modulo\n"
    "m - media\n"
    "g - gauss\n"
    "v - vertical\n"
	"h - horizontal\n"
    "l - laplaciano\n"
    "d - gauss seguido de laplaciano\n"
	"esc - sair\n";
}

int main(int argvc, char** argv){
  VideoCapture video;
  float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};
  float gauss[] = {1,2,1,
				   2,4,2,
				   1,2,1};
  float horizontal[]={-1,0,1,
					  -2,0,2,
					  -1,0,1};
  float vertical[]={-1,-2,-1,
					0,0,0,
					1,2,1};
  float laplacian[]={0,-1,0,
					 -1,4,-1,
					 0,-1,0};

  Mat cap, frame, frame32f, frameFiltered;
  Mat mask(3,3,CV_32F), mask1;
  Mat result, result1;
  double width, height;
  int absolut;
  char key;
  //criacao da flag command para saber quando iremos aplicar gauss seguido do filtro laplaciano
  int command = 0;
  video.open(0); 
  if(!video.isOpened()) 
    return -1;
  width=video.get(CV_CAP_PROP_FRAME_WIDTH);
  height=video.get(CV_CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";;
  std::cout << "altura =" << height<< "\n";;

  namedWindow("filtroespacial",1);

  mask = Mat(3, 3, CV_32F, media); 
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  swap(mask, mask1);
  absolut=1; // calcs abs of the image

  menu();
  for(;;){
    if(command == 0){
      video >> cap; 
      cvtColor(cap, frame, CV_BGR2GRAY);
      flip(frame, frame, 1);
      imshow("original", frame);
      frame.convertTo(frame32f, CV_32F);
      filter2D(frame32f, frameFiltered,
  			 frame32f.depth(), mask, Point(1,1), 0);
      if(absolut){
        frameFiltered=abs(frameFiltered);
      }
      frameFiltered.convertTo(result, CV_8U);
      imshow("filtroespacial", result);
      key = (char) waitKey(10);
      if( key == 27 ) break; // esc pressed!
    }else{
      //se command for 1 entao devo aplicar o filtro gauciano
      //quando o algoritmo executar essa condicional, mask estara com o filtro gauciano
      video >> cap; 
      cvtColor(cap, frame, CV_BGR2GRAY);
      flip(frame, frame, 1);
      imshow("original", frame);
      //aplicando o filtro gauciano e armazenando em result
      frame.convertTo(frame32f, CV_32F);
      filter2D(frame32f, frameFiltered,
         frame32f.depth(), mask, Point(1,1), 0);
      frameFiltered.convertTo(result, CV_8U);
      //imshow("filtroespacial", result);
      //recuperando o filtro laplaciano
      mask = Mat(3, 3, CV_32F, laplacian);
      //agora devemos pegar a matriz result, que ja esta com o filtro gauciano
      //e aplicar o filtro laplaciano
      result.convertTo(frame32f, CV_32F);
      filter2D(frame32f, frameFiltered,
         frame32f.depth(), mask, Point(1,1), 0);
      frameFiltered.convertTo(result, CV_8U);
      imshow("filtroespacial", result);
      key = (char) waitKey(10);
      if( key == 27 ) break; // esc pressed!
    }
    switch(key){
    case 'a':
      command = 0;
      absolut=!absolut;
      break;
    case 'm': 
      command = 0;
      mask = Mat(3, 3, CV_32F, media);
      scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      menu();
      break;
    case 'g':
      command = 0;
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      menu();
      break;
    case 'h':
      command = 0;
      mask = Mat(3, 3, CV_32F, horizontal);
      printmask(mask);
      menu();
      break;
    case 'v':
      command = 0;
      mask = Mat(3, 3, CV_32F, vertical);
      printmask(mask);
      menu();
      break;
    case 'l':
      command = 0;
      mask = Mat(3, 3, CV_32F, laplacian);
      printmask(mask);
      menu();
      break;
    case 'd':
      //recuperando o filtro gauciano
      command = 1;
      mask = Mat(3, 3, CV_32F, gauss);
      scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
      mask = mask1;
      printmask(mask);
      menu();
      break;
    case 's':
      
      imwrite("original.png", result);
      break;
    default:
      break;
    }
  }
  return 0;
}