#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

#define PI 3.14159265
#define NUM_PONTOS 4

int pontos[3][NUM_PONTOS];
int matrizN[3][4];

void inicializaOpenGL()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //Define o modo inicial de apresentação
    glutInitWindowPosition(50,50); // Define a posição inicial da tela a ser apresentada
    glutInitWindowSize(800, 600); // Define o tamanho da tela a ser apresentada
}

void inicializaCores()
{
    glClearColor(1.0,1.0,1.0,0);// define a cor de fundo da janela (no caso, branco)
    glColor3f(1.0,0.0,0.0);// define cor do traço (no caso, vermelho)
    glPointSize(4.0);// define espessura do traço
    gluOrtho2D(0 , 500 , 0 , 500);
}

int round1(double number)
{
    return (number>=0) ? (int)(number+0.5):(int)(number-0.5);
}

void DDA(int x0,int y0,int x1,int y1)
{
    x0 += 200;
    y0 += 150;
    x1 += 200;
    y1 += 150;
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
        glColor3f(0.0,0.0,0.0);
        for(int i = 0; i < NUM_PONTOS; i++)
        {
            if((i + 1) == NUM_PONTOS)
                DDA(pontos[0][i], pontos[1][i],
                        pontos[0][0], pontos[1][0]);
            else
                DDA(pontos[0][i], pontos[1][i],
                        pontos[0][i + 1], pontos[1][i + 1]);
        }
    glEnd();

    glBegin(GL_POINTS);
        glColor3f(1.0,0.0,0.0);
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

float** multiplicaMatriz(float **m1, float **m2, int linhas, int colunas) {
    float **matriz = (float**) malloc (sizeof(float*) * linhas);
    for(int m = 0; m < linhas; m++) {
        matriz[m] = (float*) malloc (sizeof(float) * colunas);
    }

    float soma = 0.0;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
                soma += m1[i][k] * m2[k][j];
            matriz[i][j] = soma;
            soma = 0.0;
        }
    }

    return matriz;
}

int** getMatrizTransalacao(int tx, int ty) {
    int **matrizT = (int**) malloc (sizeof(int*) * 3);
    for(int m = 0; m < 3; m++) {
        matrizT[m] = (int*) malloc (sizeof(int) * 3);
    }

    int i, j;

    //monta matriz de translação
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(i == j)
                matrizT[i][j] = 1;
            if (i > j)
                matrizT[i][j] = 0;
        }
    }
    matrizT[0][2] = tx;
    matrizT[1][2] = ty;
    matrizT[0][1] = 0;

    return matrizT;
}

void matrizTranslacao(int tx, int ty)
{
    int **matrizT = getMatrizTransalacao(tx, ty);

    int i, j, k, soma = 0;

    //calcula matriz resultante
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 4; j++)
        {
            for(k = 0; k < 3; k++)
                soma += matrizT[i][k] * pontos[k][j];
            matrizN[i][j] = soma;
            soma = 0;
        }
    }
}

float** getMatrizRot(int angulo) {
    float **matrizR = (float**) malloc (sizeof(float*) * 3);
    for(int m = 0; m < 3; m++) {
        matrizR[m] = (float*) malloc (sizeof(float) * 3);
    }

    float radianos;

    radianos = (angulo * PI) / 180;

    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if ((i == j) && (i != 2))
                matrizR[i][j] = cos(radianos);
            else
                matrizR[i][j] = 0;
        }
    }
    matrizR[0][1] = -sin(radianos);
    matrizR[1][0] = sin(radianos);
    matrizR[2][2] = 1;

    return matrizR;
}

void matrizRot(int angulo)
{
    float **matrizR = getMatrizRot(angulo);
    float soma = 0.0;

    //calcula matriz resultante
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            for(int k = 0; k < 3; k++)
                soma += matrizR[i][k] * pontos[k][j];
            matrizN[i][j] = soma;
            soma = 0.0;
        }
    }
}

void matrizEscala(int sx, int sy)
{
    int i, j, k, matrizE[3][3], soma = 0;

    //monta matriz de escala
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(i == j)
                matrizE[i][j] = 1;
            else
                matrizE[i][j] = 0;
        }
    }
    matrizE[0][0] = sx;
    matrizE[1][1] = sy;

    //calcula matriz resultante
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 4; j++)
        {
            for(k = 0; k < 3; k++)
                soma += matrizE[i][k] * pontos[k][j];
            matrizN[i][j] = soma;
            soma = 0;
        }
    }
}

void matrizEspelhamento(int opcao)
{
    int i, j, k, matrizE[3][3], soma = 0;

    //monta matriz de escala
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(i == j)
                matrizE[i][j] = 1;
            else
                matrizE[i][j] = 0;
        }
    }
    if (opcao == 0)
        matrizE[1][1] = -1;
    else if (opcao == 1)
        matrizE[0][0] = -1;
    else if (opcao == 2)
    {
        matrizE[0][0] = -1;
        matrizE[1][1] = -1;
    } else {
        int x0, x1, y0, y1;
        scanf("%d%d%d%d", &x0, &y0, &x1, &y1);

        double m = (double)(y1 - y0) / (double)(x1 -x0);
        double teta = atan(m);

        int **m1 = getMatrizTransalacao(x0, y0);
        float **m2 = getMatrizRot(teta);
        int **m3 = getMatrizTransalacao(-x0, -y0);

        float **mResultante = multiplicaMatriz((float**)m1, m2, 3, 3);
        mResultante = multiplicaMatriz(mResultante, (float**)m3, 3, 3);

        float s = 0.0;

        for(i = 0; i < 3; i++)
        {
            for(j = 0; j < 4; j++)
            {
                for(k = 0; k < 3; k++)
                    s += mResultante[i][k] * pontos[k][j];
                matrizN[i][j] = s;
                s = 0;
            }
        }

        return;
    }

    //calcula matriz resultante
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 4; j++)
        {
            for(k = 0; k < 3; k++)
                soma += matrizE[i][k] * pontos[k][j];
            matrizN[i][j] = soma;
            soma = 0;
        }
    }
}

void matrizCisalhamento(int opcao, int sh)
{
    int i, j, k, matrizC[3][3], soma = 0;

    //monta matriz de escala
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(i == j)
                matrizC[i][j] = 1;
            else
                matrizC[i][j] = 0;
        }
    }
    if (opcao == 0)
        matrizC[0][1] = sh;
    else if (opcao == 1)
        matrizC[1][0] = sh;

    //calcula matriz resultante
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 4; j++)
        {
            for(k = 0; k < 3; k++)
                soma += matrizC[i][k] * pontos[k][j];
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

void rotacao()
{
    int angulo;

    printf("\nValor do angulo de rotacao(em graus): ");
    scanf("%d",&angulo);

    matrizRot(angulo);
    inicializaOpenGL();
    glutCreateWindow("Rotacao Poligono");
    inicializaCores();
    glutDisplayFunc(draw);
    glutMainLoop();
}

void escala()
{
    int sx, sy;

    printf("\nEscala em X: ");
    scanf("%d",&sx);
    printf("\nEscala em Y: ");
    scanf("%d",&sy);

    matrizEscala(sx, sy);
    inicializaOpenGL();
    glutCreateWindow("Escala Poligono");
    inicializaCores();
    glutDisplayFunc(draw);
    glutMainLoop();
}

void espelha()
{
    int opcao;
    printf("0 - Em relacao a Ox\n");
    printf("1 - Em relacao a Oy\n");
    printf("2 - Em relacao a Ox e Oy\n");
    printf("3 - Em relação a uma reta\n");
    scanf("%d", &opcao);
    matrizEspelhamento(opcao);
    inicializaOpenGL();
    glutCreateWindow("Espelhamento Poligono");
    inicializaCores();
    glutDisplayFunc(draw);
    glutMainLoop();
}

void cisalha()
{
    int opcao, sh;
    printf("0 - Em relacao a Ox\n");
    printf("1 - Em relacao a Oy\n");
    scanf("%d", &opcao);
    printf("\nFator: ");
    scanf("%d", &sh);

    matrizCisalhamento(opcao, sh);
    inicializaOpenGL();
    glutCreateWindow("Cisalhamento Poligono");
    inicializaCores();
    glutDisplayFunc(draw);
    glutMainLoop();
}

void validaOpcao(int opcao)
{
    system("clear");
    switch(opcao)
    {
    case 1:
        translacao();
        break;
    case 2:
        rotacao();
        break;
    case 3:
        escala();
        break;
    case 4:
        espelha();
        break;
    case 5:
        cisalha();
        break;
    case 0:
        break;
    }
}

void showMenu()
{
    int op;

    system("clear");
    printf("--------------MENU--------------\n");
    printf("1 - Translacao\n");
    printf("2 - Rotacao\n");
    printf("3 - Escala\n");
    printf("4 - Espelhamento\n");
    printf("5 - Cisalhamento\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
    scanf("%d",&op);
    validaOpcao(op);
}

void definePontos()
{
    pontos[0][0] = 0;  pontos[0][1] = 50;  pontos[0][2] = 150; pontos[0][3] = 0;
    pontos[1][0] = 50; pontos[1][1] = 50;  pontos[1][2] = 100; pontos[1][3] = 100;
    pontos[2][0] = 1;  pontos[2][1] = 1;   pontos[2][2] = 1;   pontos[2][3] = 1;
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    definePontos();
    showMenu();
    return 0;
}

