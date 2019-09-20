#include <iostream>
#include <opencv2/opencv.hpp>



using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, mask;
  int width, height, nobjects, nobjectsBuraco, corEscolhida, corObjct;

  CvPoint p;
  image = imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);

  if(!image.data){
    cout << "imagem nao carregou corretamente\n";
    return(-1);
  }
  width=image.size().width;
  height=image.size().height;

  p.x=0;
  p.y=0;

  //Nao conseguimos categorizar os objetos que estao nas bordas, assim, devemos elimina-los
  //inicialmente vamos eliminar os objetos que estiverem nas extremidades verticais
    for(int i=0; i<height; i++){
		if(image.at<uchar>(i,0)==255){
			p.x=0;
			p.y=i;
			floodFill(image,p,0);
		}

		if(image.at<uchar>(i,width-1)==255){
			p.x=width-1;
			p.y=i;
			floodFill(image,p,0);
		}
	}

	//Agora vamos eliminar os que estiverem nas extremidades horizontais
	    for(int j=1; j<width-1; j++){
		if(image.at<uchar>(0,j)==255){
			p.x=j;
			p.y=0;
			floodFill(image,p,0);
		}

		if(image.at<uchar>(height-1,j)==255){
			p.x=j;
			p.y=height-1;
			floodFill(image,p,0);
		}
	}

  //Precisamos mudar o fundo da imagem para poder reconhecer os buracos nos objetos
  //Vamos aplicar floodFill a partir do pixel (0,0) com tom de cinza 1
  p.x=0;
  p.y=0;
  floodFill(image,p,1);

  nobjectsBuraco=0;
  nobjects=0;

  //Agora vamos percorrer a imagem procurando por objetos com buracos
  corObjct=100;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
		//Se um objeto possuir o tom de cinza 0, isso significa que ele tem um buraco
		if(image.at<uchar>(i,j-1)==255){
			//Se o pixel anterior ao tom de cinza 0 for 255
			//Isso significa que o objeto com buraco nao foi pintado
			//Assim, devemos pinta-lo e contabilizar
			corEscolhida=corObjct;
			nobjectsBuraco++;
			p.y=i;
			p.x=j-1;
			floodFill(image,p,corEscolhida);
		}
	  }
	}
  }

  //Por fim devemos proceder como no algoritmo de labeling, percorrendo a matriz e identificando os objetos
  // com tom de cinza 255, pintando esses objetos com tom de cinza 200, incrementando o contador
  // de forma a contabilizar os objetos na imagem que nao tem buracos
  corObjct=200;
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
		nobjects++;
		p.x=j;
		p.y=i;
		floodFill(image,p,corObjct);
	  }
	}
  }
  //Exibindo o numero de objetos com e sem buracos, e tambem o total de objetos na imagem
  cout<<"Objetos com buracos: "<<nobjectsBuraco<<endl;
  cout<<"Objetos sem buracos: "<<nobjects<<endl;
  cout<<"Total de objetos:"<<(nobjectsBuraco+nobjects)<<endl;

  imshow("image", image);
  imwrite("labeling.png", image);
  waitKey();
  return 0;
}