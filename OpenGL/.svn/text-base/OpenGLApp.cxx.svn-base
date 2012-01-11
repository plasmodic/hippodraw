/* 
 *
 * $Id: OpenGLApp.cxx,v 1.5 2004/11/21 09:42:24 barrand Exp $
 *
 */

// this :
#include "OpenGLApp.h"

#include "OpenGLWindow.h"
#include "OpenGL.h"

#include <iostream>

OpenGLApp::OpenGLApp()
:m_display(0)
,m_colormap(0)
,m_vinfo(0)
,m_ctx(0)
,m_privateColormap(true)
{
}

OpenGLApp::~OpenGLApp ()
{
}

bool OpenGLApp::initialize ( int /*argc*/, char** /*argv*/)
{
  static int attributeList[] = { GLX_RGBA,
				 GLX_RED_SIZE, 2,
				 GLX_GREEN_SIZE, 2,
				 GLX_BLUE_SIZE, 2,
				 GLX_DOUBLEBUFFER,
				 GLX_DEPTH_SIZE, 1,
				 None };
  m_display  = XOpenDisplay(NULL);                                                         
  if(!m_display) {
    std::cout << "Can't open display." << std::endl;
    return false;
  }

  m_vinfo = glXChooseVisual(m_display,XDefaultScreen(m_display),attributeList);
  if(!m_vinfo) {
    std::cout << "Can't choose a visual." << std::endl;
    return false;
  }

  m_ctx = glXCreateContext(m_display,m_vinfo,NULL,GL_FALSE);
  if(!m_ctx) {
    std::cout << "Can't create a GLX context." << std::endl;
    return false;
  }

  if(m_privateColormap) {
    /* It is better to create a colormap adapted to the visual.*/
    m_colormap = XCreateColormap(m_display,XDefaultRootWindow(m_display),
				 m_vinfo->visual, AllocNone); 
  } else {
    /* Default colormap does not work on an SGI with SGI libGL.*/
    m_colormap = XDefaultColormap(m_display,XDefaultScreen(m_display));
  }
  if(!m_colormap) {
    std::cout << "Can't create X colormap." << std::endl;
    return false;
  }
  //std::cout << "X, OpenGL initialized." << std::endl;
  return true;
}

int OpenGLApp::exec()
{
  if(!m_display || !m_ctx) return 0;
  glXWaitX ();
  while(1) { 
    XEvent xevent;
    if(XPending(m_display)) {
      XNextEvent (m_display,&xevent);
      if(xevent.type==ConfigureNotify) {
        OpenGLWindow* view = findView(xevent.xconfigure.window);
        view->resize((int)xevent.xconfigure.width,
                     (int)xevent.xconfigure.height);
      }
    }
  }
  return 0;
}

Display* OpenGLApp::getXDisplay() { return m_display; }
Colormap OpenGLApp::getXColormap() { return m_colormap; }
XVisualInfo* OpenGLApp::getXVisualInfo() { return m_vinfo; }
GLXContext OpenGLApp::getGLXContext() { return m_ctx; }

OpenGLWindow* OpenGLApp::findView(Window aWindow) const {
  for(unsigned int index=0;index<fViews.size();index++){
    OpenGLWindow* view = fViews[index];
    if(aWindow==view->window()) return view;
  }
  return 0;
}
