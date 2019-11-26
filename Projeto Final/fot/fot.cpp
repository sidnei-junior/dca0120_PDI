#undef _GLIBCXX_DEBUG

#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <fstream>
#include <string>
#include <bits/stdc++.h> 
#include <sys/stat.h> 
#include <sys/types.h> 

//passo do fluxo optico
#define passo 16
//numero de relatorios
#define n_relatorios 2

using namespace std;
using namespace cv;

//Passo de tempos para o relatorio
double step_time = 1;
//variaveis de tempo; tempo_antigo eh a variavel de controle para escreita no relatorio
double tempo = 0.0, tempo_antigo = 0;
//quantidade de passos
double k_final = 0.0;
//numero do relatorio
int n = 1;
//guarfa movimentacoes
float guardaMove[320];
void drawOptFlowMap(const Mat& flow, Mat& cflowmap, int step,double scale, const Scalar& color){
  for(int y = 0; y < cflowmap.rows; y += passo)
    for(int x = 0; x < cflowmap.cols; x += passo){
      const Point2f& fxy = flow.at<Point2f>(y, x);
      line(cflowmap, Point(x,y), Point(cvRound(x+fxy.x), cvRound(y+fxy.y)),color);
      circle(cflowmap, Point(x,y), 2, color, -1);
    }
}


int main(int argc, char** argv){
  VideoCapture cap(0);
  int key;
  
  time_t data_tempo;
  time(&data_tempo);
  struct tm*tempo1;
  struct tm*data;
  tempo1 = localtime(&data_tempo); // executando uma vez pega hora, minuto e segundo
  data = localtime(&data_tempo); // executando a segunda vez pega ano, mes e dia
  
  //separação das variaveis de tempo
  int dia = data->tm_mday;
  int mes = data->tm_mon + 1;
  int ano = data->tm_year + 1900;  
  int segundo = tempo1->tm_sec;
  int minuto = tempo1->tm_min;
  int hora = tempo1->tm_hour;
 
  //nome usado no arquivo que será salvo | precisa ser modificado sempre que for ter uma nova execução
  char dir_file[] = "25-11-2019_relatorio1";
  // Usa String para criar o arquivo txt e o char acima para criar o nome do diretorio devido a limitação do mkdir que só aceitar array de char
  String dir = "25-11-2019_relatorio1";
  mkdir(dir_file, 0777);
  tempo = double(60*hora) + double(minuto); // Deixando todo o tempo em minutos
  k_final =  tempo + n_relatorios*step_time; // Tempo final da execução
  tempo_antigo = tempo + step_time;
  if( !cap.isOpened() ){
	  cout << "camera nao abriu"<<endl;
    return -1;
  }
  cap.set(CV_CAP_PROP_FRAME_WIDTH, 360);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 320);
  Mat prevgray, gray, flow, cflow, frame;
  Mat dst, prev;//dst image
  float fy, ty;
  Point2f fxy;
  namedWindow("flow", 1); 
  ty = 10;
  for(int r = 0; r<cflow.rows; r++) guardaMove[r] = 0; // resetando os valores do array que guarda a movimentação total
  for(;;){
    //refaz a captura de tempo a cada repetição do loop
    time(&data_tempo);
    tempo1 = localtime(&data_tempo);
    data = localtime(&data_tempo);
    dia = data->tm_mday;
    mes = data->tm_mon + 1;
    ano = data->tm_year + 1900;
    segundo = tempo1->tm_sec;
    minuto = tempo1->tm_min;
    hora = tempo1->tm_hour;
    tempo = double(60*hora) + double(minuto);
    
    cap >> frame;
    cvtColor(frame, gray, CV_BGR2GRAY);
    flip(gray,gray,1);
	  Size size(100,100);//the dst image size,e.g.100x100
	  resize(gray,dst,Size(),0.5,0.5);//resize image
    if(prevgray.data){
      calcOpticalFlowFarneback(prevgray, dst, flow, 0.25, 3, passo, 3, 5, 1.2, OPTFLOW_FARNEBACK_GAUSSIAN);
      cvtColor(prevgray, cflow, CV_GRAY2BGR);
      drawOptFlowMap(flow, cflow, passo, 1.5, CV_RGB(0, 255, 0));
      imshow("flow", cflow); 
      
      for(int y = 0; y < cflow.rows; y += passo){
  	     fy = 0;
         for(int x = 0; x < cflow.cols; x += passo){
  	       fxy= flow.at<Point2f>(y, x);
  	       fy = fy + fxy.y;
           if(fabs(fy) > ty) guardaMove[y]++;
  	     }
      }
      if(fabs(fy) > ty){
	       if(fy<0) cout << "sobe - \n";
	       else cout << "desce - \n";
      }else{
           segundo = tempo1->tm_sec;
           minuto = tempo1->tm_min;
           hora = tempo1->tm_hour;
           cout <<ano<<"---"<<mes<<"---"<<dia<<"---"<<hora<<"---"<<minuto<<"---"<<segundo<<"\n";
      }
      
      if(tempo >= tempo_antigo) {
        fy = 0;
        String s = "../fot/"+dir+"/relatorio_"+ to_string(n) +".txt";
        ofstream txtFile;
        txtFile.open(s);
        
        for(int z = 0; z<cflow.rows; z += passo){
          txtFile <<hora<< " ; " <<minuto<<" ; " <<z<<" ; "<< guardaMove[z] << "\n";
        }

        n++;
        tempo_antigo = tempo_antigo + step_time;
        txtFile.close();
      }
      
      if(tempo >= k_final) break;
    }
	  key = waitKey(30);
		if(key <=0)break;
    std::swap(prevgray, dst);
  }

  return 0;
}
