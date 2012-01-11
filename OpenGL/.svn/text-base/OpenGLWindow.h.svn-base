/* -*- mode: c++ -*-
 *
 * Hippo OpenGLView class interface
 *
 */

#ifndef Hippo_OpenGLWindow_h
#define Hippo_OpenGLWindow_h

// Inheritance :
#include "OpenGL/OpenGLView.h"

#include <X11/Xlib.h>
#include <GL/glx.h>

/** The window  class using OpenGL.  @attention This
    class was contributed by its author as an experiment to evaluate
    the core library classes with OpenGL.  This class and others in
    this directory require CMT for its build.

    @author Guy Barrand <barrand@lal.in2p3.fr>
*/
class OpenGLWindow : public OpenGLView {
public:
  OpenGLWindow ( Display*,Colormap,XVisualInfo*,GLXContext );
  virtual ~OpenGLWindow ();
  void flush();
  Window window() const;
  void resize(int,int);
private:
  void paint();
private:
  Display* m_display;
  Colormap m_colormap;
  XVisualInfo* m_vinfo;
  GLXContext m_ctx;
  Window m_window;
  int m_width;
  int m_height;
};


#endif // Hippo_OpenGLWindow_h
