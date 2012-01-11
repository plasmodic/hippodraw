/* Hippo OpenGLView implementation
 *
 */

// this :
#include "OpenGLWindow.h"

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "plotters/CompositePlotter.h"

#include "OpenGL.h"

#include <iostream>

OpenGLWindow::OpenGLWindow ( 
 Display* aDisplay
,Colormap aColormap
,XVisualInfo* aVisualInfo
,GLXContext aGLXContext
)
:m_display( aDisplay )
,m_colormap(aColormap)
,m_vinfo(aVisualInfo)
,m_ctx(aGLXContext)
,m_window(0)
,m_width(0)
,m_height(0)
{
  if(!m_display) return;
  if(!m_vinfo) return;

  std::string title = "OpenGLWindow";

  int x = 0;
  int y = 0;
  unsigned int width = 600;
  unsigned int height = 600;

  XSetWindowAttributes swa;

  //std::cout << "depth " << m_vinfo->depth << std::endl;

  swa.colormap     = m_colormap;
  swa.border_pixel = 0L;
  swa.event_mask   = StructureNotifyMask | ExposureMask | ButtonPressMask;
  m_window = XCreateWindow (m_display, 
			    XDefaultRootWindow(m_display),
			    x,y,width,height,
			    0,
			    m_vinfo->depth,
			    InputOutput,
			    m_vinfo->visual,
			    CWBorderPixel|CWColormap|CWEventMask,
                            &swa);
  if(!m_window) {
    std::cout << "Can't create an X window." << std::endl;
    return;
  }
  //std::cout << "X window created." << std::endl;

  XTextProperty tp;
  char* sl = (char*)title.c_str();
  XStringListToTextProperty (&sl, 1, &tp);
  XSizeHints sh;
  sh.flags = USPosition | USSize;
  XSetWMProperties (m_display, m_window, &tp, &tp, 0, 0, &sh, 0, 0);
  XFree (tp.value);

  //std::cout << "map X window..." << std::endl;
  XMapWindow (m_display,m_window);
  XRaiseWindow (m_display,m_window);

  m_width = width;
  m_height = height;
}


OpenGLWindow::~OpenGLWindow()
{
}
void OpenGLWindow::flush()
{
  glFinish();
  glXSwapBuffers(m_display,m_window);
}

Window OpenGLWindow::window() const
{
  return m_window;
}

void OpenGLWindow::resize(int aWidth,int aHeight) {
  if(glXMakeCurrent(m_display,m_window,m_ctx)==False) {
    std::cout << "glXMakeCurrent failed." << std::endl;
  }
  glViewport     (0,0,aWidth,aHeight);
  glScissor      (0,0,aWidth,aHeight);
  m_width = aWidth;
  m_height = aHeight;
  paint();
  glXSwapBuffers(m_display,m_window);
}
void OpenGLWindow::paint() {
  // Pure OpenGL (no X11, no GLX).

  glEnable       (GL_LINE_STIPPLE);
  glEnable       (GL_DEPTH_TEST);
  glEnable       (GL_SCISSOR_TEST);
  glShadeModel   (GL_FLAT);

  //glClearColor   (0.5,0.5,0.5,0);
  glClearColor   (1,1,1,1);
  glClear        (GL_COLOR_BUFFER_BIT);
  glClear        (GL_DEPTH_BUFFER_BIT);

  glMatrixMode   (GL_PROJECTION); 
  glLoadIdentity ();
  glOrtho        (0,m_width,0,m_height,-1,1);
  glMatrixMode   (GL_MODELVIEW);

  setRect(0,0,m_width,m_height);

  glLoadIdentity();

  drawSelf();
  flush();
}
