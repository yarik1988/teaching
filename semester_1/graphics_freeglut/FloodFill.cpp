#include <cstdio>
#include <cstdlib>
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <ctime>
#define DimX 50
#define DimY 50
int Width=600;
int Height=600;
struct rgb_color
            {
            GLfloat r,g,b;
            rgb_color() {this->r=this->g=this->b=0.0;};
            rgb_color (float r,float g,float b) {this->r=r;this->g=g;this->b=b;};
            bool operator==(rgb_color& comp) {return (this->r==comp.r&&this->g==comp.g&&this->b==comp.b);};
            bool operator!=(rgb_color& comp) {return (this->r!=comp.r||this->g!=comp.g||this->b!=comp.b);};
            };
rgb_color arr[DimX][DimY];
rgb_color old_color;
rgb_color flood_color(1,0,0);

void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0, 0, 0);
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++)
{
glColor3f(arr[i][j].r,arr[i][j].g,arr[i][j].b);
glRecti(i,j,i+1,j+1);
}
glutSwapBuffers();
}

void init()
{
srand (time(NULL));
for (int i=0;i<DimX;i++)
for (int j=0;j<DimY;j++)
    if (rand()%2==0) arr[i][j].r=arr[i][j].g=arr[i][j].b=1.0;
else arr[i][j].r=arr[i][j].g=arr[i][j].b=0.0;
}

void fill_rec(int x,int y)
{
arr[x][y]=flood_color;
if (x+1<DimX&&arr[x+1][y]==old_color) fill_rec(x+1,y);
if (x>0&&arr[x-1][y]==old_color) fill_rec(x-1,y);
if (y+1<DimY&&arr[x][y+1]==old_color) fill_rec(x,y+1);
if (y>0&&arr[x][y-1]==old_color) fill_rec(x,y-1);
}


void Flood(int button,int state,int x,int y)
{
if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
    {
    int mx=x*DimX/(float)Width;
    int my=(Height-y)*DimY/(float)Height;
    old_color=arr[mx][my];
    if (old_color!=flood_color) fill_rec(mx,my);
    }
glutPostRedisplay();
}
void reshape(int w,int h)
{
Width=w;
Height=h;
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluOrtho2D(0,DimX,0,DimY);
glViewport(0,0,w,h);
}
int main(int argc, char** argv)
{
  init();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(Width,Height);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Simple GLUT test");
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0,DimX,0,DimY);
  glutDisplayFunc(display);
  glutMouseFunc(Flood);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return EXIT_SUCCESS;
}
