#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

#define NUM_PONTOS 4

int pontos[3][NUM_PONTOS];
int matrizN[3][4];

void inicializaOpenGL()
{
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //Define o modo inicial de apresentação
  glutInitWindowPosition(50,50); // Define a posição inicial da tela a ser apresentada
  glutInitWindowSize(500, 500); // Define o tamanho da tela a ser apresentada
}

void inicializaCores()
{
  glClearColor(1.0,1.0,1.0,0);// define a cor de fundo da janela (no caso, branco)
  glColor3f(1.0,0.0,0.0);// define cor do traço (no caso, vermelho)
  glPointSize(1.0);// define espessura do traço
  gluOrtho2D(0 , 500 , 0 , 500);
}

int round1(double number)
{
  return (number>=0) ? (int)(number+0.5):(int)(number-0.5);
}

void DDA(int x0,int y0,int x1,int y1)
{
  int dy = y1 - y0;
  int dx = x1 - x0;
  int passos, i;
  float xinc, yinc, x = x0, y = y0;

  if(abs(dx) > abs(dy))
    passos = abs(dx);
  else
    passos = abs(dy);

  xinc = (float)dx / (float)passos;
  yinc = (float)dy / (float)passos;

  glVertex2i(round1(x), round1(y));
  for(i=0; i < passos; i++)
  {
    x += xinc;
    y += yinc;
    glVertex2i(round1(x), round1(y));
  }
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);

    for(int i = 0; i < NUM_PONTOS; i++)
    {
       if((i + 1) == NUM_PONTOS)
        DDA(matrizN[0][i], matrizN[1][i],
            matrizN[0][0], matrizN[1][0]);
      else
        DDA(matrizN[0][i], matrizN[1][i],
            matrizN[0][i + 1], matrizN[1][i + 1]);
    }
    glEnd();
    glFlush();
}

void matrizTranslacao(int tx, int ty)
{
  int matrizT[3][3], soma = 0,i,j,k;

  //monta matriz de translação
  for(i = 0; i < 3; i++){
    for(j = 0; j < 3; j++){
        if(i == j)
            matrizT[i][j] = 1;

        if (i > j)
            matrizT[i][j] = 0;
    }
  }
    matrizT[0][2] = tx;
    matrizT[1][2] = ty;
    matrizT[0][1] = 0;



  //calcula matriz resultante
  for(i = 0; i < 3; i++){
    for(j = 0; j < 4; j++){
        for(k = 0; k < 3; k++){
            soma += matrizT[i][k]*pontos[k][j];
        }
        matrizN[i][j] = soma;
        soma = 0;
    }
  }
}

void translacao()
{
  int tx, ty;

  printf("\nTranslacao em X: ");
  scanf("%d",&tx);
  printf("\nTranslacao em Y: ");
  scanf("%d",&ty);

  matrizTranslacao(tx, ty);
  inicializaOpenGL();
  glutCreateWindow("Translacao Poligono");
  inicializaCores();
  glutDisplayFunc(draw);
  glutMainLoop();
}

void validaOpcao(int opcao)
{
  system("cls");
  switch(opcao){
    case 1:
      translacao();
      break;
    case 2:
      break;
    case 0:
      break;
  }
}

void showMenu()
{
  int op;

  system("cls");
  printf("--------------MENU--------------\n");
  printf("1 - Translacao\n");
  printf("2 - Rotacao\n");
  printf("0 - Sair\n");
  printf("Opcao: ");
  scanf("%d",&op);
  validaOpcao(op);
}

void definePontos()
{
  pontos[0][0] = 50; pontos[0][1] = 100; pontos[0][2] = 100; pontos[0][3] = 50;
  pontos[1][0] = 50; pontos[1][1] = 50; pontos[1][2] = 100; pontos[1][3] = 100;
  pontos[2][0] = 1;   pontos[2][1] = 1;   pontos[2][2] = 1;   pontos[2][3] = 1;
}
int main(int argc,char **argv)
{
  glutInit(&argc,argv);
  definePontos();
  showMenu();
  return 0;
}
