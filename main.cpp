#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

int a,b,c,d,e,f,g,h,tx,ty;
float angulo;

void init(void)
{
 glClearColor(0.0,0.0,0.0,1.0);
 glMatrixMode(GL_PROJECTION);
 gluOrtho2D(0.0,200.0,0.0,200.0);
}

void setpoint(GLint x,GLint y)
{
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}

void transform(int x,int y,int tx,int ty)
{
    glColor3f (0.0, 1.0, 0.0);
    setpoint(x+tx,y+ty);
    glColor3f (255.0, 255.0, 255.0);
}

void rotate(int x,int y,float angulo){
    glColor3f (0.0, 1.0, 0.0);
    glRotatef(angulo,0.0,0.0,1.0);
    setpoint(x,y);
    glColor3f (255.0, 255.0, 255.0);
}

int round1(double number)
{
 return (number>=0) ? (int)(number+0.5):(int)(number-0.5);
}
void LineWithDDA(int x0,int y0,int x1,int y1)
{
 int dy=y1-y0;
 int dx=x1-x0;
 int steps,i;
 float xinc,yinc,x=x0,y=y0;
 if(abs(dx)>abs(dy))
 {
  steps=abs(dx);
 }
 else
 {
  steps=abs(dy);
 }
 xinc=(float)dx/(float)steps;
 yinc=(float)dy/(float)steps;
 setpoint(round1(x),round1(y));
 transform(round1(x),round1(y),tx,ty);
 //rotate(round1(x),round1(y),angulo);
 for(i=0;i<steps;i++)
 {
  x+=xinc;
  y+=yinc;
  setpoint(round1(x),round1(y));
  transform(round1(x),round1(y),tx,ty);
  //rotate(round1(x),round1(y),angulo);
 }
 glutSwapBuffers();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    LineWithDDA(a,b,c,d);
    LineWithDDA(c,d,e,f);
    LineWithDDA(e,f,g,h);
    LineWithDDA(g,h,a,b);
    glFlush();
}

void validaOpcao(int opcao){
    system("cls");
    switch(opcao){
        case 1:
            printf("\nTranslacao em x: ");
            scanf("%d",&tx);
            printf("\nTranslacao em y: ");
            scanf("%d",&ty);
            glutCreateWindow("Translacao Poligono");
            init();
            glutDisplayFunc(draw);
            glutMainLoop();
            break;
        case 2:
            printf("\nValor do angulo: ");
            scanf("%f",&angulo);
            glutCreateWindow("Rotacao Poligono");
            init();
            glutDisplayFunc(draw);
            glutMainLoop();
            break;
        case 0:
            break;
    }
}

void showMenu(){

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

int main(int argc,char **argv)
{

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(500,500);

     printf("Vértices do Poligono\n\n");
     printf("x1 : ");
     scanf("%d",&a);
     printf("y1 : ");
     scanf("%d",&b);
     printf("x2 : ");
     scanf("%d",&c);
     printf("y2 : ");
     scanf("%d",&d);
     printf("x3 : ");
     scanf("%d",&e);
     printf("y3 : ");
     scanf("%d",&f);
     printf("x4 : ");
     scanf("%d",&g);
     printf("y4 : ");
     scanf("%d",&h);
     showMenu();

     return 0;
}
