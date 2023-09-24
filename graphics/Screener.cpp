#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <functional>

typedef std::function<float(int, int)> binOp;
static const binOp ops[]={
        [](int ic, int jc) {return 2*cos(ic/20.0)+2*cos(jc/20.0);},
        [](int ic, int jc) {return (ic*ic+jc*jc)/5000.0;},
        [](int ic, int jc) {return cos(ic/20.0)*cos(jc/20.0);},
        [](int ic, int jc) {return ((ic*ic/100)^(jc*jc/100))/1000.0;},
        [](int ic, int jc) {return ic*jc/2000.0;},

        [](int ic, int jc) {return sin((ic|jc-ic&jc)/100.0);},
        [](int ic, int jc) {return sin((ic^jc)/100.0);},
        [](int ic, int jc) {return 0.5*sin(jc/10.0)+ic/100.0+0.1*rand()/(float)RAND_MAX;},
        [](int ic, int jc) {return log(fabs(ic*ic/10.0+jc)+1);},
        [](int ic, int jc) {return ic/(10*sin(jc/10.0)+20.0);},


        [](int ic, int jc) {return atan(ic/100.0)+atan(jc/100.0);},
        [](int ic, int jc) {return atan(ic*ic/(jc*jc+100.0));},
        [](int ic, int jc) {return log(fabs(ic*ic/10.0+jc)+1);},
        [](int ic, int jc) {return ic/(10*sin(jc/10.0)+20.0);},
        [](int ic, int jc) {return exp(std::abs(ic+jc)/200.0)+0.5*sin(jc/100.0)+0.5*cos(ic/100.0);},
        };
const size_t num_ops = std::extent<decltype(ops)>::value;
int cur_op=0;
float speed[3]={0.007,0.013,0.011};
int DimX=1920;
int DimY=1080;

struct Vertex
{
    float position[2];
    float texcoord[2];
};

GLuint canvas_vao=0;
GLuint canvas_vbo=0;
GLuint canvas_tex=0;
GLuint canvas_pgm=0;

const Vertex qdata[6]=
{
    {{-1.0f,-1.0f},{0.0f, 0.0f}},
    {{ 1.0f,-1.0f},{1.0f, 0.0f}},
    {{-1.0f, 1.0f},{0.0f, 1.0f}},
    {{ 1.0f,-1.0f},{1.0f, 0.0f}},
    {{ 1.0f, 1.0f},{1.0f, 1.0f}},
    {{-1.0f, 1.0f},{0.0f, 1.0f}}
};

const char * gleGetErrorString(GLenum code)
{
    switch (code)
    {
        case GL_NO_ERROR:          return "No error has been recorded.";
        case GL_INVALID_ENUM:      return "An enumerated argument is assigned an invalid value.";
        case GL_INVALID_VALUE:     return "A numeric argument is out of range.";
        case GL_INVALID_OPERATION: return "The specified operation is not allowed in the current state.";
        case GL_STACK_OVERFLOW:    return "The specified operation would cause a stack overflow.";
        case GL_STACK_UNDERFLOW:   return "The specified operation would cause a stack underflow.";
        case GL_OUT_OF_MEMORY:     return "The specified operation is out of memory.";
        case GL_TABLE_TOO_LARGE:   return "The specified table exceeds the maximum supported table size.";
        default:                   return "An unknown error has occurred.";
    }
}


float tr=0.0f;
float tg=0.0f;
float tb=0.0f;

float *arr;
float *cdata;

void init()
{
    for (int i=0;i<DimX;i++)
        for (int j=0;j<DimY;j++)
            arr[j*DimX+i]=ops[cur_op](i-DimX/2,j-DimY/2);
}

inline float anima(float x)
{
float drob=x>0?x-(int)x:(int)x-x;
return 2*std::min(drob,1-drob);
}

void display()
{
tr+=speed[0];
tg+=speed[1];
tb+=speed[2];
for (int i=0;i<DimX;i++)
    for (int j=0;j<DimY;j++)
    {
        cdata[3*(j*DimX+i)]=anima(tr+arr[j*DimX+i]);
        cdata[3*(j*DimX+i)+1]=anima(tg+arr[j*DimX+i]);
        cdata[3*(j*DimX+i)+2]=anima(tb+arr[j*DimX+i]);
    }
ImGui_ImplOpenGL2_NewFrame();
ImGui_ImplGLUT_NewFrame();
ImGui::NewFrame();
ImGuiIO& io = ImGui::GetIO();
if (io.KeysDown[ImGuiKey_Escape]) glutLeaveMainLoop();
ImGui::Begin("RGB change speed",nullptr,ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove);
ImGui::SliderFloat3("RGB",speed,-0.02,0.02);
if (ImGui::Button("Next Image"))
{
    cur_op++;
    cur_op%=num_ops;
    init();
}
ImGui::End();
ImGui::Render();
glClear(GL_COLOR_BUFFER_BIT);
glUseProgram(canvas_pgm);
glActiveTexture(GL_TEXTURE0);
glUniform1i(glGetUniformLocation(canvas_pgm,"canvas_tex"),0);
glBindTexture(GL_TEXTURE_2D,canvas_tex);
glTexSubImage2D(GL_TEXTURE_2D,0,0,0,DimX,DimY,GL_RGB,GL_FLOAT,cdata);
glBindVertexArray(canvas_vao);
glDrawArrays(GL_TRIANGLES,0,6);
glBindTexture(GL_TEXTURE_2D,0);
glBindBuffer(GL_ARRAY_BUFFER,0);
glBindVertexArray(0);
glUseProgram(0);
ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
glutSwapBuffers();
}



void run()
{
glutPostRedisplay();
}
// load shader program function
GLuint loadShaderProgram(const char *vs_source, const char *fs_source)
{
    GLint ok=GL_FALSE;typedef unsigned char byte_t;
    GLuint vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vs_source,0);
    glCompileShader(vs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot compile vertex shader!\n");
        glDeleteShader(vs);
        return 0;
    }

    GLuint fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fs_source,0);
    glCompileShader(fs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot compile fragment shader!\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    GLuint pgm=glCreateProgram();
    glAttachShader(pgm,vs);
    glAttachShader(pgm,fs);
    glLinkProgram(pgm);
    glGetProgramiv(pgm,GL_LINK_STATUS,&ok);
    if (!ok)
    {
        fprintf(stderr,"Cannot link shader program!\n");
        glDeleteShader(vs);
        glDeleteShader(fs);
        glDeleteProgram(pgm);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return pgm;
}
/************************************************************/
GLenum setupRenderEnvironment()
{
    canvas_pgm=loadShaderProgram(
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
#else
    "#version 400\n"
#endif
    "attribute vec2 vertex_pos;"
    "attribute vec2 texel_pos;"
    "out vec2 texcoord;"
    "void main(void)"
    "{"
    "  gl_Position=vec4(vertex_pos,0.0,1.0);"
    "  texcoord=texel_pos;"
    "}",
#ifdef GL_ES_VERSION_2_0
    "#version 100\n"
#else
    "#version 400\n"
#endif
    "uniform sampler2D canvas_tex;"
    "in vec2 texcoord;"
    "void main(void)"
    "{"
    "  gl_FragColor=texture(canvas_tex,texcoord);"
    "}"
    );
    if (!canvas_pgm)
        return glGetError();

    glGenVertexArrays(1,&canvas_vao);
    glBindVertexArray(canvas_vao);

    glGenBuffers(1,&canvas_vbo);
    glBindBuffer(GL_ARRAY_BUFFER,canvas_vbo);

    glBufferData(GL_ARRAY_BUFFER,sizeof(qdata),qdata,GL_STATIC_DRAW);

    glVertexAttribPointer(glGetAttribLocation(canvas_pgm,"vertex_pos"),2,GL_FLOAT,GL_FALSE,
        sizeof(Vertex),(const GLvoid *)offsetof(Vertex,position));
    glEnableVertexAttribArray(glGetAttribLocation(canvas_pgm,"vertex_pos"));

    glVertexAttribPointer(glGetAttribLocation(canvas_pgm,"texel_pos"),2,GL_FLOAT,GL_FALSE,
        sizeof(Vertex),(const GLvoid *)offsetof(Vertex,texcoord));
    glEnableVertexAttribArray(glGetAttribLocation(canvas_pgm,"texel_pos"));

    glGenTextures(1,&canvas_tex);
    glBindTexture(GL_TEXTURE_2D,canvas_tex);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,DimX,DimY,0,
        GL_RGB,GL_FLOAT,0);

    glBindTexture(GL_TEXTURE_2D,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    return glGetError();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitContextVersion(4,0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE|GLUT_DEBUG);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutCreateWindow("ScreenSaver");
    glutFullScreen();
    DimX=glutGet(GLUT_SCREEN_WIDTH);
    DimY=glutGet(GLUT_SCREEN_HEIGHT);
    arr=new float[DimY*DimX];
    cdata=new float[DimY*DimX*3];
    for (GLenum res=glewInit(); res!=GLEW_OK; res=GLEW_OK)
    {
        fprintf(stderr,"Error: %s\n",glewGetErrorString(res));
        return EXIT_FAILURE;
    }
    (void)glGetError();
    for (GLenum res=setupRenderEnvironment(); res!=GL_NO_ERROR; res=GL_NO_ERROR)
    {
        fprintf(stderr,"Error: %s\n",gleGetErrorString(res));
        return EXIT_FAILURE;
    }
    glEnable(GL_POINT_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutDisplayFunc(display);
    glutIdleFunc(run);
    init();
    //imgui setup
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    ImGui::StyleColorsDark();
    ImGui_ImplGLUT_Init();
    ImGui_ImplOpenGL2_Init();
    ImGui_ImplGLUT_InstallFuncs();
    glViewport(0,0,DimX,DimY);


    glutMainLoop();
    delete[] arr;
    delete[] cdata;
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplGLUT_Shutdown();
    ImGui::DestroyContext();

    return EXIT_SUCCESS;
}
