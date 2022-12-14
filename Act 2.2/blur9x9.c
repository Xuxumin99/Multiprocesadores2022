#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#define NUM_THREADS 1

int main()
{
      omp_set_num_threads(NUM_THREADS);
      FILE *image, *outputImage, *lecturas;
      image = fopen("toad.bmp","rb");          //Imagen original a transformar
      outputImage = fopen("toadout9.bmp","wb");    //Imagen transformada
      long ancho;
      long alto;
      unsigned char r, g, b;               //Pixel
      unsigned char* ptr;

      unsigned char xx[54];
      int cuenta = 0;
      for(int i=0; i<54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
      }
      ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
      //alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
      alto = (abs)((long) (xx[25] << 24) + (long) (xx[24] << 16) + (long) (xx[23] << 8) + (long) (xx[22] << 0));
      //printf("largo img %li\n",alto);
      //printf("ancho img %li\n",ancho);

      ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

      #pragma omp parallel
      {
        while(!feof(image)){
          b = fgetc(image);
          g = fgetc(image);
          r = fgetc(image);


          unsigned char pixel = 0.21*r+0.72*g+0.07*b;

          ptr[cuenta] = pixel; //b
          //ptr[cuenta+1] = pixel; //g
          //ptr[cuenta+2] = pixel; //r


          cuenta++;

        }
      }

      //Matriz de 7x7
      int linea=0;
      int cuenta2=0;
      int inicioLin=0;
      int finLin=ancho-1;
      int valor=0;
      int a=1;
      const double startTime = omp_get_wtime();
      #pragma omp parallel
      {
        #pragma omp for schedule(dynamic)
          for (int i = 0; i < alto*ancho; i++) {

            if(linea<=3 || ( i==(inicioLin) && i<=(inicioLin+3) ) || ( i>=(finLin-3) && i==(finLin) ) || linea>=(alto-4) ){
              fputc(ptr[i], outputImage);
              fputc(ptr[i], outputImage);
              fputc(ptr[i], outputImage);
            }else{

              for(int j = -4; j <= 4 ; j++){
                a = i+(ancho*j); 
                valor = valor + ((ptr[a]+ptr[a-1]+ptr[a-2]+ptr[a-3]+ptr[a-4]+ptr[a+1]+ptr[a+2]+ptr[a+3]+ptr[a+4])/81);
              }   

              fputc(valor, outputImage);
              fputc(valor, outputImage);
              fputc(valor, outputImage);
              valor=0;

            }
            cuenta2++;
            if(cuenta2==ancho){ //fin de linea
              cuenta2=0;
              linea++;
              inicioLin=linea*ancho;
              finLin=inicioLin+(ancho)-1;
              //printf("linea: %i\n", linea);
            }
          }
      }

      /*for (int i = 0; i < alto*ancho*3; ++i) {
        fputc(ptr[i], outputImage);
        fputc(ptr[i+1], outputImage);
        fputc(ptr[i+2], outputImage);
      }*/

      const double endTime = omp_get_wtime();
      //printf("Tiempo de ejecucion: %lf\n", (endTime - startTime));
      printf("%i\t", (NUM_THREADS));
      printf("%lf\n", (endTime - startTime));

      free(ptr);
      fclose(image);
      fclose(outputImage);

    return 0;
}