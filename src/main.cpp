/* A simple program to show how to set up an X window for OpenGL rendering.
 g++ -I ../../../libs/libpng/ -I ../../../libs/zlib -L ../../../libs/libpng/ -L ../../../libs/zlib/ -pthread -Wall -g main_png.cpp -lpthread -lGL -lGLU -lX11 -lz ../../../libs/libpng/libpng16.a -o 1
 */
 
 //TRY XCheckWindowEvent
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include <GL/glx.h>    /* this includes the necessary X headers */
#include <GL/gl.h>
#include <GL/glu.h>

#include <X11/X.h>    /* X11 constant (e.g. TrueColor) */
#include <X11/keysym.h>

#include "mWidget.h"

using namespace std;
int frames = 0;
double all_time = 0;
chrono::duration<double> delta_time;
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};

Display   *dpy;
Window     win;
GLfloat    xAngle = 42.0, yAngle = 82.0, zAngle = 112.0;

mWidget *mainWindow = nullptr;

void fatalError(const string &message)
{
  cout << "main: " << message << endl;
}

void redraw(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0, (GLdouble)1024, 0, (GLdouble)768);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 mainWindow->Draw();
 glXSwapBuffers(dpy, win);/* buffer swap does implicit glFlush */
}

int main(int argc, char **argv)
{
  XVisualInfo         *vi;
  Colormap             cmap;
  XSetWindowAttributes swa;
  GLXContext           glxc;
  XEvent               event;
  int                  dummy;

  /*** (1) open a connection to the X server ***/

  dpy = XOpenDisplay(NULL);
  if (dpy == NULL)
    {fatalError("could not open display");return 0;}
  /*** (2) make sure OpenGL's GLX extension supported ***/
  if(!glXQueryExtension(dpy, &dummy, &dummy))
    {fatalError("X server has no OpenGL GLX extension");return 0;}
  /*** (3) find an appropriate visual ***/
  /* find an OpenGL-capable RGB visual with depth buffer */
  vi = glXChooseVisual(dpy, DefaultScreen(dpy), dblBuf);
  if (vi == NULL){
    fatalError("no RGB visual with depth buffer");return 0;
  }
  /*** (4) create an OpenGL rendering context  ***/
  /* create an OpenGL rendering context */
  glxc = glXCreateContext(dpy, vi, /* no shared dlists */ None,
                        /* direct rendering if possible */ GL_TRUE);
  if (glxc == NULL)
    {fatalError("could not create rendering context");return 0;}
  /*** (5) create an X window with the selected visual ***/
  /* create an X colormap since probably not using default visual */
  cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen), vi->visual, AllocNone);
  swa.colormap = cmap;
  swa.border_pixel = 0;
  swa.event_mask = KeyPressMask |ExposureMask| ButtonPressMask | StructureNotifyMask | PointerMotionMask;
  win = XCreateWindow(dpy, RootWindow(dpy, vi->screen), 0, 0,
                      1024, 768, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &swa);
  XSetStandardProperties(dpy, win, "main", "main", None,
                         argv, argc, NULL);
  /*** (6) bind the rendering context to the window ***/
  glXMakeCurrent(dpy, win, glxc);
  /*** (7) request the X window to be displayed on the screen ***/
  XMapWindow(dpy, win);
  /*** (8) configure the OpenGL context for rendering ***/
  glDisable(GL_DEPTH_TEST); /* enable depth buffering */
  glDepthFunc(GL_LESS);    /* pedantic, GL_LESS is the default */
  glClearDepth(1.0);       /* pedantic, 1.0 is the default */
  /* frame buffer clears should be to black */
  glClearColor(0.2, 0.2, 0.3, 1.0);
  /* establish initial viewport */
  /* pedantic, full window size is default viewport */
  glViewport(0, 0, 1024, 768);
  
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  
  //create widget
  mainWindow = new mWidget("firework");
  mainWindow->width = 1024;
  mainWindow->height = 768;
  /*** (9) dispatch X events ***/
  bool running = true;
  auto start_time = chrono::system_clock::now();
  auto end_time = chrono::system_clock::now();
  while (running)
  {
    end_time = chrono::system_clock::now();
    while(XPending(dpy)) /* loop to compress events */
    {
      XNextEvent(dpy, &event);
      switch (event.type)
      {
        case KeyPress:
        {
          KeySym     keysym;
          XKeyEvent *kevent;
          char       buffer[1];
          /* It is necessary to convert the keycode to a
           * keysym before checking if it is an escape */
          kevent = (XKeyEvent *) &event;
          if (   (XLookupString(kevent,buffer,1,&keysym,NULL) == 1)
              && (keysym == (KeySym)XK_Escape) )
            running = false;
          break;
        }
        case ButtonPress:
          switch (event.xbutton.button)
          {
            case 1: mainWindow->MouseDown(event.xbutton.x, event.xbutton.y);
              break;
            case 2: //right button
              break;
            case 3: //middle button
              break;
          }
          break;
        case MotionNotify:
          mainWindow->MouseMove(event.xmotion.x, event.xmotion.y);
          break;
        case ConfigureNotify:
          glViewport(0, 0, event.xconfigure.width,
                     event.xconfigure.height);
          /* fall through... */
          case Expose:
          break;
        case DestroyNotify:
          cout << "DestroyNotify!" << endl;
          running = false;
          break;
      }
      XFlush(dpy);
    } 
    
    ++frames;
    delta_time = end_time-start_time;
    double dt = delta_time.count();
    all_time += dt;
    mainWindow->Update(dt);
    redraw();
    start_time = end_time;
  }
  cout << "FPS: " << frames/all_time << endl;
  cout << "time: " << all_time << " sec"<< endl;
  delete mainWindow;
  glXMakeCurrent(dpy, win, 0);
  glXDestroyContext(dpy, glxc);
  XDestroyWindow(dpy, win);
  cout << "GOOD!" << endl;
  return 0;
}
