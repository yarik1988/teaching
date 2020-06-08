//считает площадь треугольника + рисует окружность по алгоритму Брезенхема
#include <stdio.h>
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
float tx[3];
float ty[3];
int i=0;

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta);//calculate the tangential factor

	float radial_factor = cosf(theta);//calculate the radial factor

	float x = r;//we start at angle = 0

	float y = 0;

	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + cx, y + cy);//output vertex

		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them

		float tx = -y;
		float ty = x;

		//add the tangential vector

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		//correct using the radial factor

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0.5, 0.3, 1);
glLineWidth(2.0);
glPointSize(5.0);

glBegin(GL_POINTS);
for (int j=0;j<i;j++) glVertex2f(tx[j],ty[j]);
glEnd();

if (i==3)
{
glBegin(GL_LINE_LOOP);
glVertex2f(tx[0],ty[0]);
glVertex2f(tx[1],ty[1]);
glVertex2f(tx[2],ty[2]);
glEnd();
}
DrawCircle(0,0,50,30);
glColor3f(1, 0, 0);
glutSwapBuffers();
}

float GetSquare()
{

float n1=sqrt(pow(tx[0]-tx[1],2)+pow(ty[0]-ty[1],2));
float n2=sqrt(pow(tx[1]-tx[2],2)+pow(ty[1]-ty[2],2));
float n3=sqrt(pow(tx[2]-tx[0],2)+pow(ty[2]-ty[0],2));
float p=(n1+n2+n3)/2;
float result=sqrt(p*(p-n1)*(p-n2)*(p-n3));
return result;
}


void MouseClick(int button, int state, int x, int y)
{
if (state==GLUT_DOWN&&i<3)
	{
	tx[i]=x-300;
	ty[i]=300-y;
	i++;
	if (i==3) std::cout<<"Triangle square="<<GetSquare();
	glutPostRedisplay();
	}

}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(600,600);
  glutInitWindowPosition(700, 100);
  glutCreateWindow("Simple GLUT test");
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glEnable(GL_POINT_SMOOTH);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(-300,300,-300,300);
  glutDisplayFunc(display);
  glutMouseFunc(MouseClick);
  glutMainLoop();
  return EXIT_SUCCESS;
}
