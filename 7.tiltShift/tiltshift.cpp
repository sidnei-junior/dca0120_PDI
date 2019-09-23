#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
/*
Para podermos criar o efeito de tiltshift temos que utilizar de tres matrizes distintas combinadas. 
Primeiro precisamos borrar a imagem original, e para isso iremos utilizar um filtro de media
pois esse borramento eh importante para o ajuste do efeito. Posteriormente temos que criar uma matriz 
ponderada por uma funcao ja pre modelada para que possamos multiplicar elemento a elemento com a nossa
matriz borrada de acordo com a regiao de interesse para dar o efeito nao borrado nessa regiao. 
E por ultimo precisamos aplicar o ponderamento inverso nas regiões de borda da faixa escolhida
para dar um efeito suave na visualização da região não borrada para a região.
*/
int height;
int maximo = 100; 
//referencia decaimento do tutorial
double decaimento = 6;
int decaimento_slider = 0;

int top_slider = 0;
int vertical = 0;

int faixa = 20;
int altura_slider = 0;


Mat image1, image2, alfa, beta;

char TrackbarName[50];
//funcao de ponderamento seguindo o modelo apresentado no tutorial
//a(x) = (1/2)*( tanh((x-l1)/d) - tanh((x-l2)/d) ) 
void desfoqueVertical(){
	Mat tiltshift, pondera1, pondera2;
	int l1 = - faixa/2;
    int l2 = faixa/2;
    //cria duas matrizes de zeros nas dimensoes de image1
    alfa = Mat::zeros(image1.rows, image1.cols, CV_32F);
    beta = Mat::zeros(image1.rows, image1.cols, CV_32F);
    for (int i = 0; i < alfa.rows; i++) {
        int x = i - (vertical + faixa/2);
        float alfaValue = 0.5*float(tanh((x - l1)/decaimento) - tanh((x - l2)/decaimento));
        for (int j = 0; j < alfa.cols; j++) {
            //preenchendo as matrizes alfa e beta com os valores de ponderacao
            alfa.at<float>(i, j) = alfaValue;
            beta.at<float>(i, j) = 1 - alfaValue;
        }
    }
    //vetor de matrizes que sera mesclado usando merge()
    Mat vet1[] = {alfa, alfa, alfa};
    Mat vet2[] = {beta, beta, beta};
    merge(vet1, 3, alfa);
    merge(vet2, 3, beta);
    //normaliza 
	image1.convertTo(pondera1, CV_32FC3);
    image2.convertTo(pondera2, CV_32FC3);
    //multiplicacao elemento a elemento 
    //matriz recebe matriz multiplicanda pela matriz de ponderada
    multiply(pondera1, alfa, pondera1);
    multiply(pondera2, beta, pondera2);
    // junta as ponderacoes na imagem final borrada de acordo com os ajustes dos sliders
    add(pondera1, pondera2, tiltshift);
    // converte para escala de 0 a 255
    tiltshift.convertTo(tiltshift, CV_8UC3);
    //exibe as imagens
    imshow("tiltshift", tiltshift);
    imshow("Pondera imagem original", alfa);
	imshow("Pondera imagem borradda", beta);
}

void on_trackbar_decaimento(int, void*){
    decaimento = (double) decaimento_slider;
    // slider de decaimento para o decaimento que fara o ajuste do 
    // ponderamento para a imagem borrada
    if (decaimento < 1) {
        decaimento = 1;
    }
    desfoqueVertical();
}
void on_trackbar_vertical(int, void *){
    // slider para o deslocamento da faixa com altura variavel ajustada em on_trackbar_vertical
    vertical = top_slider*height/maximo;
    desfoqueVertical();
}

void on_trackbar_altura(int, void*) {
    // slider para scanear as linhas
    faixa = altura_slider*height/maximo;
    if (faixa == 0) {
        faixa = 1;
    }

    if (faixa > height) {
        faixa = height;
    }
    desfoqueVertical();
}

int main(int argvc, char** argv){
  image1 = imread("nt5.jpeg");
  height = image1.size().height;
  // o clone eh necessario para que possamos alter image2 sem alterar image1
  // se fizessemos image2 receber image1 teriamos problemas e alterariamos image1 tambem  
  image2 = image1.clone();

	Mat aux, mask, mask1;
  	float media[] = {1,1,1,
                     1,1,1,
                     1,1,1};
    //filtro de media para dar o efeito de borramento
    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    image2.convertTo(aux, CV_32F);
    for (int i = 0; i < 10; i++) {
        filter2D(aux, aux, aux.depth(), mask, Point(1, 1), 0);
    }
    aux.convertTo(image2, CV_8UC3);

    namedWindow("tiltshift", 1);
    // cria os sliders e mostra  o nome de cada um
    sprintf( TrackbarName, "ScanLine");
    createTrackbar( TrackbarName, "tiltshift",
                    &altura_slider,
                    maximo,
                    on_trackbar_altura);

    sprintf( TrackbarName, "Decaimento");
    createTrackbar( TrackbarName, "tiltshift",
                    &decaimento_slider,
                    maximo,
                    on_trackbar_decaimento);

    sprintf( TrackbarName, "deslocamento da faixa");
    createTrackbar( TrackbarName, "tiltshift",
                    &top_slider,
                    maximo,
                    on_trackbar_vertical );
    desfoqueVertical();

  waitKey(0);
  return 0;
}