#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#if defined(_WIN32)
    #include <windows.h> /* must precede GL/gl.h on Windows */
#endif
#if defined(__APPLE__)
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <cmath>
#include <cstdlib>
#include <optional>

static int slices = 16;
static int stacks = 16;

constexpr double kPi = 3.14159265358979323846;

/* Geometry helpers (replacement for GLUT's built-in shapes) */

static void setNormal(double x, double y, double z)
{
    const double len = sqrt(x * x + y * y + z * z);
    glNormal3d(x / len, y / len, z / len);
}

static void sphereVertex(double theta, double phi)
{
    /* theta: polar angle in [0, PI], phi: azimuth in [0, 2 PI) */
    const double st = sin(theta);
    const double x = st * cos(phi);
    const double y = st * sin(phi);
    const double z = cos(theta);

    setNormal(x, y, z); /* on a unit sphere the normal is the position itself */
    glVertex3d(x, y, z);
}

static void torusVertex(double theta, double phi)
{
    /* lies in the x-y plane, hole along the z axis, like GLUT's torus */
    const double ring = 0.8, tube = 0.2;
    const double cp = cos(phi);

    setNormal(cp * cos(theta), cp * sin(theta), sin(phi));
    glVertex3d((ring + tube * cp) * cos(theta),
               (ring + tube * cp) * sin(theta),
               tube * sin(phi));
}

static void drawSphere(bool solid)
{
    if (solid)
    {
        for (int i = 0; i < stacks; ++i)
        {
            const double t0 = kPi * i / stacks;
            const double t1 = kPi * (i + 1) / stacks;

            glBegin(GL_QUADS);
            for (int j = 0; j < slices; ++j)
            {
                const double p0 = 2 * kPi * j / slices;
                const double p1 = 2 * kPi * (j + 1) / slices;

                sphereVertex(t0, p0);
                sphereVertex(t1, p0);
                sphereVertex(t1, p1);
                sphereVertex(t0, p1);
            }
            glEnd();
        }
    }
    else
    {
        /* latitude rings (the pole rings degenerate to points, skip them) */
        for (int i = 1; i < stacks; ++i)
        {
            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < slices; ++j)
                sphereVertex(kPi * i / stacks, 2 * kPi * j / slices);
            glEnd();
        }

        /* meridians */
        for (int j = 0; j < slices; ++j)
        {
            glBegin(GL_LINE_STRIP);
            for (int i = 0; i <= stacks; ++i)
                sphereVertex(kPi * i / stacks, 2 * kPi * j / slices);
            glEnd();
        }
    }
}

/* cone along the z axis, base circle at z = -h/2, apex at z = +h/2 */
static void drawCone(bool solid)
{
    const double r = 1.0, h = 1.0;

    if (solid)
    {
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < slices; ++j)
        {
            const double p0 = 2 * kPi * j / slices;
            const double p1 = 2 * kPi * (j + 1) / slices;

            /* lateral surface, flat shaded */
            {
                const double pm = 0.5 * (p0 + p1);
                setNormal(h * cos(pm), h * sin(pm), r);
                glVertex3d(0, 0, h / 2);
                glVertex3d(r * cos(p0), r * sin(p0), -h / 2);
                glVertex3d(r * cos(p1), r * sin(p1), -h / 2);
            }

            /* base cap */
            setNormal(0, 0, -1);
            glVertex3d(0, 0, -h / 2);
            glVertex3d(r * cos(p1), r * sin(p1), -h / 2);
            glVertex3d(r * cos(p0), r * sin(p0), -h / 2);
        }
        glEnd();
    }
    else
    {
        /* base circle */
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < slices; ++j)
        {
            const double p = 2 * kPi * j / slices;
            setNormal(0, 0, -1);
            glVertex3d(r * cos(p), r * sin(p), -h / 2);
        }
        glEnd();

        /* lateral rings at each stack level, radius shrinks towards the apex */
        for (int i = 1; i < stacks; ++i)
        {
            const double f = 1.0 - double(i) / stacks;

            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < slices; ++j)
            {
                const double p = 2 * kPi * j / slices;
                setNormal(h * cos(p), h * sin(p), r);
                glVertex3d(r * f * cos(p), r * f * sin(p), -h / 2 + h * i / stacks);
            }
            glEnd();
        }

        /* spokes from the apex to the base circle */
        glBegin(GL_LINES);
        for (int j = 0; j < slices; ++j)
        {
            const double p = 2 * kPi * j / slices;
            setNormal(h * cos(p), h * sin(p), r);
            glVertex3d(0, 0, h / 2);
            glVertex3d(r * cos(p), r * sin(p), -h / 2);
        }
        glEnd();
    }
}

static void drawTorus(bool solid)
{
    if (solid)
    {
        for (int i = 0; i < slices; ++i)
        {
            const double t0 = 2 * kPi * i / slices;
            const double t1 = 2 * kPi * (i + 1) / slices;

            glBegin(GL_QUADS);
            for (int j = 0; j < stacks; ++j)
            {
                const double p0 = 2 * kPi * j / stacks;
                const double p1 = 2 * kPi * (j + 1) / stacks;

                torusVertex(t0, p0);
                torusVertex(t1, p0);
                torusVertex(t1, p1);
                torusVertex(t0, p1);
            }
            glEnd();
        }
    }
    else
    {
        /* circles around the tube */
        for (int i = 0; i < slices; ++i)
        {
            const double t = 2 * kPi * i / slices;

            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < stacks; ++j)
                torusVertex(t, 2 * kPi * j / stacks);
            glEnd();
        }

        /* rings around the hole */
        for (int j = 0; j < stacks; ++j)
        {
            const double p = 2 * kPi * j / stacks;

            glBegin(GL_LINE_LOOP);
            for (int i = 0; i < slices; ++i)
                torusVertex(2 * kPi * i / slices, p);
            glEnd();
        }
    }
}

/* Rendering / input handlers */

static sf::Clock animClock;

static void display()
{
    const double a = animClock.getElapsedTime().asSeconds() * 90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1, 0, 0);

    glPushMatrix();
        glTranslated(-2.4, 1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawSphere(true);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, 1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawCone(true);
    glPopMatrix();

    glPushMatrix();
        glTranslated(2.4, 1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawTorus(true);
    glPopMatrix();

    glPushMatrix();
        glTranslated(-2.4, -1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawSphere(false);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0, -1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawCone(false);
    glPopMatrix();

    glPushMatrix();
        glTranslated(2.4, -1.2, -6);
        glRotated(60, 1, 0, 0);
        glRotated(a, 0, 0, 1);
        drawTorus(false);
    glPopMatrix();
}

static void resize(unsigned int width, unsigned int height)
{
    const float ar = height > 0 ? float(width) / float(height) : 1.0f;

    glViewport(0, 0, GLsizei(width), GLsizei(height));
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;

    sf::Window window(sf::VideoMode({640, 480}), "GL Shapes", sf::State::Windowed, settings);
    window.setPosition(sf::Vector2i(10, 10));
    window.setVerticalSyncEnabled(true);
    if (!window.setActive(true))
        return EXIT_FAILURE;

    resize(640, 480);

    glClearColor(1, 1, 1, 1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                resize(resized->size.x, resized->size.y);
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                    case sf::Keyboard::Scan::Escape:
                    case sf::Keyboard::Scan::Q:
                        window.close();
                        break;

                    case sf::Keyboard::Scan::Equal:      /* = and + */
                    case sf::Keyboard::Scan::NumpadPlus:
                        slices++;
                        stacks++;
                        break;

                    case sf::Keyboard::Scan::Hyphen:     /* - and _ */
                    case sf::Keyboard::Scan::NumpadMinus:
                        if (slices > 3 && stacks > 3)
                        {
                            slices--;
                            stacks--;
                        }
                        break;

                    default:
                        break;
                }
            }
        }

        display();
        window.display();
    }

    return EXIT_SUCCESS;
}