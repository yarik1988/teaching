#include <iostream>
#include <math.h>
#include <GL\freeglut.h>
#include <vector>
#include <time.h>
#include <fstream>
#include <stdexcept>
#include "SOIL.h"
using namespace std;

int gl_width, gl_height;
int window_height=600;
int window_width=800;
double matrix[16];
double mx,my;

struct ball
{
float rad;
float x;
float y;
float a;
float vx;
float vy;
float va;
};
const float G=0.01;
const float Dempf=0.8;
float gravityX=0;
float gravityY=0;
std::vector<ball> bunch;
/******************************************/
struct img
{
unsigned char* data;
GLuint tex;
int w,h;
} scr,img;
/******************************************/
void init(int n)
{
bunch.clear();
ball temp;
srand(time(NULL));
  for (int i=0;i<n;i++)
  {
  temp.rad=rand()%5+2;
  temp.x=rand()%(gl_width-2*(int)temp.rad)+temp.rad;
  temp.y=rand()%(gl_height-2*(int)temp.rad)+temp.rad;
  temp.a=0;
  temp.vx=rand()%100/1000.0;
  temp.vy=rand()%100/1000.0;
  temp.va=(rand()%1000-500)/2000.0;
  bunch.push_back(temp);
 }
}
/******************************************/
void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(1,1,1);
float tw=window_width/(float)scr.w;
float th=window_height/(float)scr.h;
glBindTexture(GL_TEXTURE_2D, scr.tex);

glMatrixMode(GL_TEXTURE);
glLoadIdentity();
glTranslatef(-mx/300.0,my/300.0,0);
float val=pow(pow(mx-window_width/2,2)+pow(my-window_height/2,2),0.5);
glRotatef(val/100.0,0,0,1);
glMatrixMode(GL_MODELVIEW);

glBegin( GL_QUADS );
glTexCoord2d(0.0,0.0);
glVertex2d(0.0,0.0);
glTexCoord2d(tw,0.0);
glVertex2d(gl_width,0.0);
glTexCoord2d(tw,th);
glVertex2d(gl_width,gl_height);
glTexCoord2d(0.0,th);
glVertex2d(0.0,gl_height);
glEnd();
glBindTexture(GL_TEXTURE_2D,img.tex);

glMatrixMode(GL_TEXTURE);
glLoadIdentity();
glMatrixMode(GL_MODELVIEW);

for (int i=0;i<bunch.size();i++)
{
glTranslatef(bunch[i].x,bunch[i].y,0);
glRotatef(bunch[i].a,0,0,1);
glBegin(GL_QUADS);
glTexCoord2d(0.0,0.0);
glVertex2d(-bunch[i].rad,-bunch[i].rad);
glTexCoord2d(1.0,0.0);
glVertex2d(-bunch[i].rad,bunch[i].rad);
glTexCoord2d(1.0,1.0);
glVertex2d(bunch[i].rad,bunch[i].rad);
glTexCoord2d(0.0,1.0);
glVertex2d(bunch[i].rad,-bunch[i].rad);
glEnd();
glLoadMatrixd(matrix);
}
glutSwapBuffers();
}
/***********************************/
void reshape(int w,int h)
{
window_width=w;
window_height=h;
gl_height=100;
gl_width=gl_height*w/h;
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluOrtho2D(0,gl_width,0,gl_height);
glViewport(0,0,w,h);
glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
init(100);
}
/*****************************/
void run()
{

for (int i=0;i<bunch.size();i++)
{
bunch[i].x+=bunch[i].vx;
bunch[i].y+=bunch[i].vy;
bunch[i].vx+=gravityX;
bunch[i].vy+=gravityY;
bunch[i].a+=bunch[i].va;
if (bunch[i].x<bunch[i].rad) {bunch[i].vx*=-Dempf;bunch[i].x=bunch[i].rad;};
if (bunch[i].y<bunch[i].rad) {bunch[i].vy*=-Dempf;bunch[i].y=bunch[i].rad;};
if (bunch[i].x>gl_width-bunch[i].rad) {bunch[i].vx*=-Dempf;bunch[i].x=gl_width-bunch[i].rad;}
if (bunch[i].y>gl_height-bunch[i].rad) {bunch[i].vy*=-Dempf;bunch[i].y=gl_height-bunch[i].rad;}
}

glutPostRedisplay();
}

void keypress(unsigned char key,int x,int y)
{
switch (key)
    {
    case 27: glutLeaveMainLoop();break;
    case ' ': {gravityX=0;gravityY=0;}break;
    }
}
void s_key(int key, int x, int y)
{
switch (key)
{
case GLUT_KEY_LEFT: {gravityX=-G;gravityY=0;}break;
case GLUT_KEY_RIGHT: {gravityX=G;gravityY=0;}break;
case GLUT_KEY_UP: {gravityX=0;gravityY=G;}break;
case GLUT_KEY_DOWN: {gravityX=0;gravityY=-G;}break;
}
}
/*******************************/
void mouse_click(int button, int state, int x, int y)
{
if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
        {
            int gl_x=x*gl_width/window_width;
            int gl_y=(window_height-y)*gl_height/window_height;
            for (std::vector<ball>::iterator it=bunch.begin();it!=bunch.end();it++)
            if (pow(gl_x-it->x,2)+pow(gl_y-it->y,2)<=pow(it->rad,2)) {bunch.erase(it);it--;}
        }
}
/*******************************/
void mouse_move(int x,int y)
{mx=x;my=y;}
/*******************************/
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(window_width,window_height);
  glutInitWindowPosition(50,50);
  glutCreateWindow("Game application");
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(run);
  glutSpecialFunc(s_key);
  glutKeyboardFunc(keypress);
  glutMouseFunc(mouse_click);
  glutPassiveMotionFunc(mouse_move);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_ALPHA_TEST);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /********************************/
  glGenTextures(1, &img.tex);
  glBindTexture(GL_TEXTURE_2D, img.tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  img.data =   SOIL_load_image("../ball.png", &img.w, &img.h, 0, SOIL_LOAD_AUTO);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.w, img.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
  SOIL_free_image_data(img.data);
  /***********************************************************************/
  glGenTextures(1, &scr.tex);
  glBindTexture(GL_TEXTURE_2D, scr.tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  scr.data =   SOIL_load_image("../tiles.bmp", &scr.w, &scr.h, 0, SOIL_LOAD_AUTO);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scr.w, scr.h, 0, GL_RGB, GL_UNSIGNED_BYTE, scr.data);
  SOIL_free_image_data(scr.data);
  /******************************************************************/
  glutFullScreen();
  glutMainLoop();

return 0;
}
