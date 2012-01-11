#ifndef Hippo_OpenGLApp_h
#define Hippo_OpenGLApp_h

#include <X11/Xlib.h>
#include <GL/glx.h>
#include <vector>

/** The application class using OpenGL.  @attention This
    class was contributed by its author as an experiment to evaluate
    the core library classes with OpenGL.  This class and others in
    this directory require CMT for its build.

    @author Guy Barrand <barrand@lal.in2p3.fr>
*/

class OpenGLWindow;

class OpenGLApp {
public:
  OpenGLApp();
  ~OpenGLApp();
  bool initialize( int argc = 0,  char ** argv = 0);
  int exec();
  Display* getXDisplay();
  Colormap getXColormap();
  XVisualInfo* getXVisualInfo();
  GLXContext getGLXContext();
public:
  std::vector<OpenGLWindow*> fViews;
private:
  OpenGLWindow* findView(Window) const;
private:
  Display* m_display;
  Colormap m_colormap;
  XVisualInfo* m_vinfo;
  GLXContext m_ctx;
  bool m_privateColormap;
};

#endif // OpenGLApp_H
