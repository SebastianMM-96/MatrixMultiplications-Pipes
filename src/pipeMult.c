
/*
La ejecución de la multiplicación será mediante el comando:
./mult <N> donde N será el número de filas y columnas de cada matriz
*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        int N = atoi(argv[1]);
        int pid,fills,fil,col,idHijo,fi,multi;

        int *matA,*matB;
        int matResultado[N][N];	// = (int *) malloc((N*N) * sizeof(int));
        int i,j;

        int pHijos[N][2],pPadre[2],buffer[N*2];

        int final[1];
        final[0]=1234;

        //Comprobamos el argumento en trado
        if((N<0)||(N>20))
        {
                perror("Error n debe estar entre 0 i 20");
                exit(1);
                                /*
				El valor máximo que podrá tener N será 20. Como resultado de la ejecución deben
				mostrarse las tres matrices.
				*/
        }


        //Se crean las matrices en las posiciones de memoria
        matA = (int *) malloc((N*N) * sizeof(int));

        for(i=0; i<N*N; i++)
                matA[i]= rand()%9+1;

        matB = (int *) malloc((N*N) * sizeof(int));

        for(i=0; i<N*N; i++)
                matB[i]= rand()%9+1;



        //Se muestran las matrices por pantalla
        printf("\nMatriz A:\n---------------\n");

        for(i=0;i<N;i++)
        {
                for(j=0;j<N;j++)
                        printf("%d  ",matA[j+(N*i)]);
                printf("\n");
        }

        printf("\n\nMatriz B:\n---------------\n");

        for(i=0;i<N;i++)
        {
                for(j=0;j<N;j++)
                        printf("%d  ",matB[j+(N*i)]);
                printf("\n");
        }


        //Creamos N pipes para los hijos
        for(i=0;i<N;i++)
        {
                if(pipe(pHijos[i])<0)
                {
                        perror("Error creando pipe para el hijo");
                        exit(1);
                }
        }

        //Creamos un pipe de vuelta para el padre
        if(pipe(pPadre)<0)
        {
                perror("Error creando pipe para el padre");
		exit(1);
        }

        //Creamos N hijos
        for(i=0;i<N;i++)
        {
                idHijo=i;
                pid=fork();
                if(pid==0)
                        break;
        }

        switch(pid)
        {
                case -1:
                        perror("Error en fork");
                        exit(1);
                        break;

                case 0:         //HIJO
                        //Cerramos las entradas que no usamos
                        for(i=0;i<N;i++)
                        {
                                close(pHijos[i][1]);	
                                if(i!=idHijo)
                                {
                                        close(pHijos[i][0]);
                                }
                        }

                        close(pPadre[0]);

                        fi=0;

                        while(fi==0)
                        {
                                //Leemos del pipe y los ponemos en el bufferer
                                if(read(pHijos[idHijo][0],buffer,sizeof(buffer))<0)
                                {
                                        perror("Error en read del Hijo");

                                        exit(1);
                                }


                                if(buffer[0]==final[0])
                                {
                                        fi=1;
                                        break;
                                }
                                //Realizamos los calculos 
                                multi=0;
                                for(i=0;i<N;i++)
                                        multi = multi + buffer[i]*buffer[N+i];

                                //Llenamos el bufferer con el id proceso y el resultado de la multiplicacion
                                buffer[0]=idHijo;
                                buffer[1]=multi;
                                //Escribimos al pipe del padre el contenido del bufferer
                                if(write(pPadre[1],buffer,sizeof(buffer))<0)
                                {
                                        perror("Error en write del Hijo");
                                        exit(1);
                                }
                        }

                        break;
                                //END HIJO
                default:
                                //PADRE
                        for(fil=0;fil<N;fil++)
                        {
                                for(col=0;col<N;col++)
                                {
                                        //Ponemos el contenido del bufferer en 0
                                        for(i=0;i<N*2;i++)
                                                buffer[i]=0;

                                        for(i=0;i<N;i++)
                                        {
                                                //Ponemos en el bufferer el numero de fila de A y columna de B
                                                buffer[i]=matA[(N*fil)+i];
                                                buffer[N+i]=matB[(N*i)+col];
                                        }

                                        //Escribimos al pipe del hijo correspondiente el contenido del bufferer
                                        if(write(pHijos[col][1],buffer,sizeof(buffer))<0)
                                        {
                                                perror("Error en write del Padre");
                                                exit(1);
                                        }

					//Leemos el resultado del pipe del Hijo Correspondiente
                                        if(read(pPadre[0],buffer,sizeof(buffer))<0)
                                        {
                                                perror("Error en el read del Padre");
                                                exit(1);
                                        }
					matResultado[fil][col]=buffer[1];
                                }
                        }

                        //Indicamos que hijos han acabado

                        for(i=0; i<N; i++)
                                write(pHijos[i][1], final, sizeof(final));


                        printf("\n\nLa matriz resultante es:\n---------------\n");

                        //Mostramos por pantalla la solucion

                        for(i=0;i<N;i++)
                        {
                                for(j=0;j<N;j++)
                                        printf("%d  ",matResultado[i][j]);
                                printf("\n");
                        }

                        break;
        }

        free(matA);
        free(matB);


        return 0;
}
