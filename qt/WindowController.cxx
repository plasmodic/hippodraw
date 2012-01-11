/** @file

hippodraw::WindowController class implementation.

Copyright (C) 2002-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: WindowController.cxx,v 1.74 2007/07/02 18:17:16 pfkeb Exp $

*/

// for defects
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#include "Windows.h"
#endif

// for version
#ifdef HAVE_CONFIG_H // for VERSION
#include "config.h"
#endif

#include "WindowController.h"

#include "CanvasView.h"
#include "CanvasWindow.h"
#include "Inspector.h"

#include <qapplication.h>
#include <QtGui/QDesktopWidget>

#include <qdir.h>

#include <algorithm>

using std::find;
using std::list;
using std::mem_fun;
using std::for_each;

using namespace hippodraw;

WindowController * WindowController::s_instance = 0;
std::string WindowController::s_version ( VERSION );

const std::string & WindowController::version ()
{
  return s_version;
}

WindowController::WindowController ( )
  : m_active_window ( 0 ),
    m_inspector ( 0 ),
    m_assistant ( 0 ),
    m_quit ( true )
{
  s_instance = this;
}

WindowController::~WindowController ()
{
  s_instance = 0;
}

WindowController * WindowController::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new WindowController ();
  }

  return s_instance;
}

void WindowController::createInspector ()
{
  if ( m_inspector == 0 ) {
    m_inspector = new Inspector ();
  }
  positionInspector ();
}

void WindowController::newWindow ( CanvasWindow * window )
{
  unsigned int size = m_window_list.size ();
  if ( size == 0  &&
       m_quit == true ) {
    window->inhibitClose ();
  }
  else if ( size == 1 ) {
    CanvasWindow * w = m_window_list.front ();
    w->inhibitClose ( false );
  }

  m_window_list.push_back ( window );
  positionCanvas ( window );
  if ( m_inspector != 0 ) resizeCanvas ( window );
  setCurrentCanvas ( window ); 

  CanvasView * view = window->getCanvasView ();
  if ( m_inspector != 0 ) {
    view->setInspector ( m_inspector );
    moveInspector ( window );
  }
 
}

void
WindowController::
moveInspector ( CanvasWindow * window )
{
  if ( m_inspector != 0 ) {
    QPoint w_pos = window -> pos ();
    QSize w_size = window -> size ();
    int pos = w_pos.x() + static_cast < int >  ( 1.05 * w_size.width () );
    w_pos.setX ( pos );
    m_inspector -> move ( w_pos );
  }
}

void
WindowController::
quitOnLastWindowClose ( bool yes )
{
  m_quit = yes;
}

/** @bug Mac only.  Should not quit application on last window close.
    That is not the Mac OS X standard.
 */
void
WindowController::
aboutToClose ( CanvasWindow * window )
{
  m_window_list.remove ( window );
  // window will delete itself.

  if ( m_window_list.size() == 1 &&
       m_quit == true ) {
    CanvasWindow * w = m_window_list.front();
    w->inhibitClose ();
  }
  if ( m_window_list.empty() ) {
    if ( m_inspector != 0 ) {
      m_inspector -> hide ();
      if ( m_quit == true ) {
	m_inspector -> close ();
	m_inspector = 0;
      }
      window->autosaveSettings();
      m_window_list.clear ();
      qApp->quit ();
    }
  }
}

void WindowController::hasBeenHidden ()
{
  bool allhidden = true;
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < CanvasWindow * > :: iterator first = m_window_list.begin ();
  while ( first != m_window_list.end() ) {
    CanvasWindow * w = *first++;
    allhidden = w->isMinimized ();
    if ( allhidden == false ) break;
  }
  if ( allhidden == true ) {
    if ( m_inspector != 0 ) m_inspector->hide();
  }
}

void WindowController::unHide ( CanvasWindow * )
{
  if ( m_inspector != 0 )  m_inspector->show();
}

bool WindowController::okToQuit ()
{
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < CanvasWindow * > :: iterator first = m_window_list.begin ();
  bool ok = m_quit;

  if ( m_quit == true ) {
    while ( first != m_window_list.end() ) {
      CanvasWindow * window = *first++;
      ok = window->allowClose ();
      if ( ! ok ) break;
    }
  }

  return ok;
}

void
WindowController::
updateActions ()
{
  if ( m_window_list.empty() == false ) {
    for_each ( m_window_list.begin(), m_window_list.end(),
	       mem_fun ( &CanvasWindow::updateActions ) );
  }
}

void WindowController::closeAllWindows ( bool alsoDelete )
{
#ifdef ITERATOR_MEMBER_DEFECT
  std::
#endif
  list < CanvasWindow * > :: iterator first = m_window_list.begin ();
  while ( first != m_window_list.end() ) {
    CanvasWindow * window = *first++;
    window -> setAllowClose ( true );
    window->close ();
    delete window;
  }

  if ( m_inspector != NULL ) {
    m_inspector -> close();
    delete m_inspector;
  }
}

QRect WindowController::getScreenRect ()
{
  QDesktopWidget * dt = QApplication::desktop ();
  int screen = dt->primaryScreen ();

  return dt->screenGeometry ( screen );
}

/** @request @@ Should use QDesktop to get usuable screen size on the Mac
 */
void WindowController::positionCanvas ( CanvasWindow * window )
{
  const QRect & rect = getScreenRect ();
  QPoint p = rect.topLeft ();
  if ( m_active_window != 0 ) {
    p = m_active_window->pos();
    QPoint offset ( 10, 10 );
    p += offset;
  }

#ifdef __APPLE__
  p.setY ( p.y() + 23 );  //To be under Apple menubar.
#endif

  window->move ( p );
}
void
WindowController::
resizeCanvas ( CanvasWindow * window )
{
  if ( m_inspector == 0 ) return; // can't do it without inspector
 
  QPoint insp_pos = m_inspector -> pos ();
  QSize w_size = window -> size ();

  const QRect & rect = getScreenRect ();
  QSize s_size = rect.size ();
  s_size *= 0.95;
  QSize size = window -> sizeHint ();
  size.setHeight ( s_size.height () );
  
  if ( w_size.width() > insp_pos.x () ) {
    double x = insp_pos.x ();
    double w = w_size.width ();
    double scale = 0.90 * ( x / w );
    CanvasView * cv = window -> getCanvasView ();
    cv -> scaleBy ( scale );
    
    QSize size = window -> sizeHint ();
    int width = static_cast < int > ( 0.95 * insp_pos.x () );
    size.setWidth ( width );
    window -> resize ( size );
    }
}

void WindowController::positionInspector ( )
{
  const QRect & rect = getScreenRect ();
  QPoint pos = m_inspector->pos();
  QSize size = m_inspector->size ();
  int new_x = rect.width() - size.width();
  pos.setX ( new_x );
#ifdef __APPLE__
  pos.setY ( 23 );  //To be under Apple menubar.
#else
  pos.setY ( 0 );
#endif

  m_inspector->move ( pos );
}

void WindowController::setCurrentCanvas ( CanvasWindow * canvas )
{
  m_active_window = canvas;
}

CanvasWindow * WindowController::currentCanvas ()
{
  return m_active_window;
}

Inspector * WindowController::getInspector ()
{
  return m_inspector;
}

void
WindowController::
setFirstWindow ()
{
  createInspector ();
  quitOnLastWindowClose ();
  CanvasWindow * window = new CanvasWindow ();
  window->setCaption ();
 
  window->show();
}

void
WindowController::
openAssistant ()
{
#ifdef HAVE_HELP
#ifdef _MSC_VER
	QString exe_path;
#endif
  if ( m_assistant == 0 ) {
#ifdef _MSC_VER
	  // The following taken
//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dllproc/base/getmodulefilename.asp
      LPCTSTR module = "qthippo.dll";
	  HMODULE handle = GetModuleHandle ( module );
	  int size = MAX_PATH;
	  TCHAR path[MAX_PATH];
	  DWORD count = GetModuleFileName ( handle, path, size );
	  exe_path = QDir ( path ).absPath();

     int pos = exe_path.findRev ( QString ( module ) );
      exe_path.remove ( module ); // not trailing `/'
	  unsigned int len = exe_path.length();
	  exe_path.truncate( len -1 );
      CanvasWindow * current = currentCanvas();
	  m_assistant = new QAssistantClient( exe_path , current );
#else
    CanvasWindow * current = currentCanvas();
	// QTDIR defined in Makefile
    m_assistant = new QAssistantClient( QString ( QTDIR ), current );
#endif
  }

#ifdef _MSC_VER
  QString docs_path = exe_path + "/doc";
#else
  // `DOCS' defined in Makefile
  QString docs_path = QDir ( DOCS ).absPath();
#endif

  if ( m_assistant -> isOpen() == false ) {
	QString adp = docs_path + QString ( "/hippodraw.adp" );
    QStringList args;
    args << "-profile"
	 << adp;
    m_assistant -> setArguments ( args );
  }
  QString page ( "%1/assist_root.html" );
  page = page.arg ( docs_path );
  m_assistant -> 
    showPage ( page );
#endif // HAVE_HELP
}
