#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include "glad/glad.h"
#include <SFML/Graphics.hpp>

#include "imgui.h"
#include "imgui-SFML.h"


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
        [](int ic, int jc) {
            // Scale the coordinates to a small range.
            double x = ic / 500.0, y = jc / 500.0;
            double cx = x, cy = y;
            int iter = 0, max_iter = 30;
            double zx = x, zy = y;

            while (zx*zx + zy*zy < 4.0 && iter < max_iter) {
                double xt = zx*zx - zy*zy + cx;
                zy = 2.0*zx*zy + cy;
                zx = xt;
                iter++;
            }

            if (iter == max_iter) {
                return 1.0f;
            }
            double mag = std::sqrt(zx*zx + zy*zy);
            if (mag <= 0.0) mag = 1e-12;
            double mu = iter + 1.0 - std::log(std::log(mag)) / std::log(2.0);
            double smooth = mu / static_cast<double>(max_iter);
            return static_cast<float>(smooth);
            },
        };
constexpr size_t ops_count = std::size(ops);
int cur_op=0;
float speed[3]={0.007,0.013,0.011};

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
        default:                   return "An unknown error has occurred.";
    }
}


float tr=0.0f;
float tg=0.0f;
float tb=0.0f;

void init(float *arr, unsigned int DimX,unsigned int DimY)
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

void display(float* cdata, float* arr, unsigned int DimX,unsigned int DimY)
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
    glGetShaderiv(fs,GL_COMPILE_STATUS,&ok);
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
GLenum setupRenderEnvironment(unsigned int DimX,unsigned int DimY)
{
    // Use GLSL version 330 core, matching the context we requested
    const char* vertexShaderSource =
        "#version 330 core\n"
        "in vec2 vertex_pos;"  // 'in' instead of 'attribute'
        "in vec2 texel_pos;"   // 'in' instead of 'attribute'
        "out vec2 texcoord;"
        "void main()"
        "{"
        "  gl_Position = vec4(vertex_pos, 0.0, 1.0);"
        "  texcoord = texel_pos;"
        "}";

    const char* fragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;" // Use 'out vec4' for the final color
        "in vec2 texcoord;"
        "uniform sampler2D canvas_tex;"
        "void main()"
        "{"
        // Use texture() function, which is correct for modern GLSL
        "  FragColor = texture(canvas_tex, texcoord);"
        "}";
    canvas_pgm = loadShaderProgram(vertexShaderSource, fragmentShaderSource);
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
int main()
{
    // 1. Request a specific OpenGL version (this part is the same)
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    const auto& modes=sf::VideoMode::getFullscreenModes();
    auto selected_mode=modes.front();
    unsigned int DimX=selected_mode.size.x;
    unsigned int DimY=selected_mode.size.y;
    sf::RenderWindow window(selected_mode, "Screener",sf::State::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(sf::Context::getFunction)))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    GLenum res = setupRenderEnvironment(DimX,DimY);
    if (res != GL_NO_ERROR)
    {
        std::cout << "Error in setupRenderEnvironment: " << gleGetErrorString(res);
        return EXIT_FAILURE;
    }
    // Now init ImGui-SFML (after GL loader)
    bool imgui_init_res = ImGui::SFML::Init(window);
    if (!imgui_init_res)
    {
        std::cerr << "ImGui-SFML init failed\n";
        return EXIT_FAILURE;
    }
    auto *arr=new float[DimY*DimX];
    auto *cdata=new float[DimY*DimX*3];
    init(arr,DimX,DimY);
    sf::Clock deltaClock;
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                if (keyPressed->scancode==sf::Keyboard::Scan::Escape) window.close();
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        ImGui::SFML::Update(window, deltaClock.restart());
        // IMGUI: Create the UI window and sliders
        ImGui::Begin("Controls"); // Create a window

        ImGui::SliderFloat("Red Speed",   &speed[0], -0.02f, 0.02f);
        ImGui::SliderFloat("Green Speed", &speed[1], -0.02f, 0.02f);
        ImGui::SliderFloat("Blue Speed",  &speed[2], -0.02f, 0.02f);
        if (ImGui::SliderInt("Function", &cur_op, 0, ops_count - 1))
        {
            // If the slider was moved, re-initialize the background array.
            init(arr, DimX, DimY);
        }
        ImGui::End(); // End the window
        window.clear();
        display(cdata,arr,DimX,DimY);
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    delete[] arr;
    delete[] cdata;
    return EXIT_SUCCESS;
}
