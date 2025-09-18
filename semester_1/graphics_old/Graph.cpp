// Graph.cpp : Defines the entry point for the console application.
//

#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <stack>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
void mouse_action(int button, int state, int x, int y);
void mouse_move(int x,int y);
void graph_create(int ori);
void wide_search(int);
void vertex_delete(int);
const int gl_width=100;
const int gl_height=100;
int window_height=800;
int window_width=800;
bool is_oriented;
int vertice_number=10;
int link_prob=50;
/********************/
struct Node
{
float x,y;
double dist;
Node* precessor;
std::vector<Node*> pts;
Node():precessor(NULL){dist=-1;}
};
Node* move_node=NULL;
std::vector<Node*> graph;
std::stack<Node*> mystack;
std::set<Node*> pointed_vertices;
//clear path data
void path_clear()
{
for (int i=0;i<graph.size();i++)
    {
    graph[i]->precessor=NULL;
    graph[i]->dist=-1;
    }
}
//arrow vertices
const GLfloat vertices[] = {
                            -1.5f,  -7.5f,
                            0.0f,    -5.0f,
                            0.0f,    0.0f,
                            1.5f, -7.5f,
};

float norm(float x,float y)
{
return sqrtf(x*x+y*y);
}

void drawArrowHead(float x,float y,float ang,float scale=1)
{
float vert[8];

for (int i=0;i<8;i+=2)
{
vert[i]=vertices[i]*cosf(ang)+vertices[i+1]*sinf(ang);
vert[i+1]=-vertices[i]*sinf(ang)+vertices[i+1]*cosf(ang);
}

for (int i=0;i<8;i+=2)
{
vert[i]=vert[i]*scale+x;
vert[i+1]=vert[i+1]*scale+y;
}
glVertexPointer(2, GL_FLOAT, 0, vert);
glEnableClientState(GL_VERTEX_ARRAY);
glColor3f(0, 0.5f, 0);
glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void display()
{
glClear(GL_COLOR_BUFFER_BIT);
glColor3f(0.1, 0.1, 0.1);
glPointSize(5.0);
glBegin(GL_POINTS);
for (int i=0;i<graph.size();i++)
glVertex2d(graph[i]->x,graph[i]->y);
glEnd();

for (int i=0;i<graph.size();i++)
for (int j=0;j<graph[i]->pts.size();j++)
{
float begx=graph[i]->x;
float begy=graph[i]->y;
float endx=graph[i]->pts[j]->x;
float endy=graph[i]->pts[j]->y;
glColor3f(0, 0, 1);
glBegin(GL_LINES);
glVertex2d(begx,begy);
glVertex2d(endx,endy);
glEnd();
float ang=atan2(endx-begx,endy-begy);
if (is_oriented) drawArrowHead(endx,endy,ang,0.5);
}
glPointSize(10.0);
for (std::set<Node*>::iterator it=pointed_vertices.begin();it!=pointed_vertices.end();it++)
        {
        glColor3f(1, 0, 0);
        glBegin(GL_POINTS);
        glVertex2d((*it)->x,(*it)->y);
        glEnd();
        }
/*****************************/
if (pointed_vertices.size()==2)
{
Node* curr=*(++pointed_vertices.begin());
glBegin(GL_LINES);
glColor3f(1, 0, 0);
while (curr->precessor!=NULL)
    {
    glVertex2d(curr->x,curr->y);
    glVertex2d(curr->precessor->x,curr->precessor->y);
    curr=curr->precessor;
    }
glEnd();
}
// ImGui stuff
ImGui_ImplOpenGL2_NewFrame();
ImGui_ImplGLUT_NewFrame();
ImGui::NewFrame();
ImGuiIO& io = ImGui::GetIO();
if (io.KeysDown[ImGuiKey_Escape]) glutLeaveMainLoop();
ImGui::Begin("MAIN TOOLBAR",nullptr,ImGuiWindowFlags_AlwaysAutoResize);
ImGui::SliderInt("Number of vertices",&vertice_number,3,30);
ImGui::SliderInt("Connection probability",&link_prob,0,100);
if (ImGui::Button("Create ordinary graph")) graph_create(0);
if (ImGui::Button("Create oriented graph")) graph_create(1);
if (ImGui::Button("Find shortest path")) wide_search(0);
if (ImGui::Button("Delete selected vertices")) vertex_delete(0);
ImGui::End();
ImGui::Render();
ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
glutSwapBuffers();
if (io.MouseReleased[0]) mouse_action(GLUT_LEFT_BUTTON,GLUT_UP,io.MousePos.x,io.MousePos.y);
if (io.MouseReleased[1]) mouse_action(GLUT_RIGHT_BUTTON,GLUT_UP,io.MousePos.x,io.MousePos.y);
if (io.MouseClicked[0]) mouse_action(GLUT_LEFT_BUTTON,GLUT_DOWN,io.MousePos.x,io.MousePos.y);
if (io.MouseClicked[1]) mouse_action(GLUT_RIGHT_BUTTON,GLUT_DOWN,io.MousePos.x,io.MousePos.y);
if (io.MouseDown[0]) mouse_move(io.MousePos.x,io.MousePos.y);
}

Node* find_node(int x,int y)
{
for (int i=0;i<graph.size();i++)
         {
         float tx=x*gl_width/(float)window_width;
         float ty=y*gl_height/(float)window_height;
         if (norm(graph[i]->x-tx,graph[i]->y-ty)<2) return graph[i];
         }
return NULL;
}

void wide_search(int)
{
if (pointed_vertices.size()!=2) return;
path_clear();
std::set<Node*> cur_nodes;

cur_nodes.insert(*pointed_vertices.begin());
(*pointed_vertices.begin())->dist=0;

while (cur_nodes.size()>0)
    {
    Node* curr=*cur_nodes.begin();
    cur_nodes.erase(cur_nodes.begin());
    for (int i=0;i<curr->pts.size();i++)
        {
        float node_len=norm(curr->x-curr->pts[i]->x,curr->y-curr->pts[i]->y);
        if (curr->pts[i]->dist==-1||curr->pts[i]->dist>curr->dist+node_len)
            {
            curr->pts[i]->precessor=curr;
            curr->pts[i]->dist=curr->dist+node_len;
            cur_nodes.insert(curr->pts[i]);
            }
        }
    }
glutPostRedisplay();
}

/***********************************************************/
void mouse_action(int button, int state, int x, int y)
{
if (button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN)
        {
        move_node=find_node(x,y);
        if (move_node) {pointed_vertices.insert(move_node);path_clear();}
        }
if (button==GLUT_LEFT_BUTTON&&state==GLUT_UP) move_node=NULL;
if (button==GLUT_RIGHT_BUTTON&&state==GLUT_DOWN)
{
std::set<Node*>::iterator it=pointed_vertices.find(find_node(x,y));
if (it!=pointed_vertices.end()) {pointed_vertices.erase(it);path_clear();}
}
}
/********************************************/
void mouse_move(int x,int y)
{
if (move_node!=NULL)
        {
        move_node->x=x*gl_width/(float)window_width;
        move_node->y=y*gl_height/(float)window_height;
        }
glutPostRedisplay();
}
/******************************************/
void graph_create(int ori)
{
for (int i=0;i<graph.size();i++) delete graph[i];
graph.clear();
pointed_vertices.clear();
srand(time(NULL));
  for (int i=0;i<vertice_number;i++)
      {
      Node *curr=new Node;
      curr->x=rand()%gl_width;
      curr->y=rand()%gl_height;
      graph.push_back(curr);
      }
if (ori)
    for (int i=0;i<graph.size();i++)
        {
        for (int j=0;j<graph.size();j++)
            {
            is_oriented=true;
            if (rand()%100<link_prob&&j!=i)
            graph[i]->pts.push_back(graph[j]);
            }
        }
else
for (int i=0;i<graph.size();i++)
        {
        for (int j=i+1;j<graph.size();j++)
            {
            if (rand()%100<link_prob)
                    {
                    is_oriented=false;
                    graph[i]->pts.push_back(graph[j]);
                    graph[j]->pts.push_back(graph[i]);
                    }
            }
        }
glutPostRedisplay();
}
/******************************************/
void reshape(int w,int h)
{
window_width=w;
window_height=h;
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluOrtho2D(0,gl_width,gl_height,0);
glViewport(0,0,w,h);
}
//*****************************************/
void close()
{
for (int i=0;i<graph.size();i++) delete graph[i];
graph.clear();
}
/******************************************/
void vertex_delete(int x)
{
path_clear();
for (auto it=pointed_vertices.begin();it!=pointed_vertices.end();it++)
    {
        for (auto i=graph.begin();i<graph.end();i++)
        {
         auto itr=std::find((*i)->pts.begin(), (*i)->pts.end(), *it);
         if (itr!=(*i)->pts.end())
             (*i)->pts.erase(itr);
        }
    }
std::vector<Node*> new_graph;
for (auto i=graph.begin();i<graph.end();i++)
if (pointed_vertices.find(*i)==pointed_vertices.end())
    new_graph.push_back(*i);
graph=new_graph;
pointed_vertices.clear();
glutPostRedisplay();



}
/********************************/
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(window_width,window_height);
    glutInitWindowPosition(700, 100);
    glutCreateWindow("Graph application");
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutMouseFunc(mouse_action);
    glutMotionFunc(mouse_move);
    glutDisplayFunc(display);
    glutIdleFunc(glutPostRedisplay);
    glutReshapeFunc(reshape);
    glutCloseFunc(close);
    /****************************************/
    /****************************************/
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();
    ImGui_ImplGLUT_InstallFuncs();
    gluOrtho2D(0,gl_width,gl_height,0);
    glutMainLoop();
    return EXIT_SUCCESS;
}


