#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <GLFW/glfw3.h>

#include "mWidget.h"

using namespace std;
int frames = 0;
double all_time = 0;
chrono::duration<double> delta_time;

mWidget *mainWindow = nullptr;

void fatalError(const string &message)
{
    cout << "main: " << message << endl;
}
/* program & OpenGL initialization */
static void init(void)
{
    glDisable(GL_DEPTH_TEST); /* enable depth buffering */
    glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
    glClearDepth(1.0);       /* pedantic, 1.0 is the default */
    /* frame buffer clears should be to black */
    glClearColor(0.2, 0.2, 0.3, 1.0);
    /* establish initial viewport */
    /* pedantic, full window size is default viewport */

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

void redraw(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mainWindow->Draw();
}

void reshape(GLFWwindow* w, int width, int height)
{
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluOrtho2D(0, (GLdouble)width, 0, (GLdouble)height);
    glOrtho(0., (GLdouble)width, 0., (GLdouble)height, -1., 1.);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//========================================================================
// Callback function for mouse button events
//========================================================================

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;

    if (action == GLFW_PRESS)
    {
	double cursorX=0;
	double cursorY=0;
        glfwGetCursorPos(window, &cursorX, &cursorY);
        mainWindow->MouseDown(cursorX, cursorY);
    }
}


//========================================================================
// Callback function for cursor motion events
//========================================================================

void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    mainWindow->MouseMove(x, y);
    /*if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
    {
        alpha += (GLfloat) (x - cursorX) / 10.f;
        beta += (GLfloat) (y - cursorY) / 10.f;

        cursorX = x;
        cursorY = y;
    }*/
}

//========================================================================
// Handle key strokes
//========================================================================

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action != GLFW_PRESS)
        return;

    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        /*case GLFW_KEY_SPACE:
            init_grid();
            break;
        case GLFW_KEY_LEFT:
            alpha += 5;
            break;
        case GLFW_KEY_RIGHT:
            alpha -= 5;
            break;
        case GLFW_KEY_UP:
            beta -= 5;
            break;
        case GLFW_KEY_DOWN:
            beta += 5;
            break;
        case GLFW_KEY_PAGE_UP:
            zoom -= 0.25f;
            if (zoom < 0.f)
                zoom = 0.f;
            break;
        case GLFW_KEY_PAGE_DOWN:
            zoom += 0.25f;
            break;*/
        default:
            break;
    }
}

int main(int argc, char **argv)
{
    GLFWwindow* window=nullptr;
    int width=1024;
    int height=768;
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        exit( EXIT_FAILURE );
    }

    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

    window = glfwCreateWindow( width, height, "firework", NULL, NULL );
    if (!window)
    {
        fprintf( stderr, "Failed to open GLFW window\n" );
        glfwTerminate();
        exit( EXIT_FAILURE );
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Set callback functions
    glfwSetFramebufferSizeCallback(window, reshape);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval( 1 );

    glfwGetFramebufferSize(window, &width, &height);
    reshape(window, width, height);

    // Parse command-line options
    init();

    //create widget
    string prefix="";
#ifdef FILES_PREFIX
    prefix = FILES_PREFIX;
#endif
    mainWindow = new mWidget("firework", prefix + "/");
    mainWindow->width = 1024;
    mainWindow->height = 768;
    auto start_time = chrono::system_clock::now();
    auto end_time = chrono::system_clock::now();
    // Main loop
    while( !glfwWindowShouldClose(window) )
    {
        end_time = chrono::system_clock::now();
        delta_time = end_time-start_time;
        double dt = delta_time.count();
        mainWindow->Update(dt);
        redraw();
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        ++frames;
        all_time += dt;
        start_time = end_time;
    }

    delete mainWindow;
    // Terminate GLFW
    glfwTerminate();

    cout << "FPS: " << frames/all_time << endl;
    cout << "time: " << all_time << " sec"<< endl;
    // Exit program
    exit( EXIT_SUCCESS );
}

