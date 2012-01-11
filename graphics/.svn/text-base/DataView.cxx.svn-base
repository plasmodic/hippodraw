/** @file

hippodraw::DataView class implementation

Copyright (C) 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DataView.cxx,v 1.17 2007/03/05 22:49:48 xiefang Exp $

*/

// inconsistent dll linkage
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DataView.h"
#include "graphics/FontBase.h"

#include "axes/Range.h"
#include "plotters/PlotterBase.h"

using namespace hippodraw;

DataView::DataView( )
  : ViewBase ()
{
}

DataView::
DataView ( PlotterBase * plotter )
  // : ViewBase ( plotter )
{
  // Initilize list does not work with virtual class?
  m_plotter = plotter;
}

DataView::
~DataView()
{
}

const Rect &
DataView::
getMarginRect () const
{
  return m_margin_rect;
}

void
DataView::
setMarginRect ( const Rect & rect )
{
  m_margin_rect = rect;
}

/** @attention In the implementation, make sure the left margin stops
    growing at same time maximum Y tick labels stop growing.
 */
void
DataView::
prepareMarginRect ( )
{
  const Rect draw = getDrawRect();
  double width = draw.getWidth();
  double height = draw.getHeight();

  double marginXLeft = draw.getHeight () * 0.20;
  marginXLeft = std::min ( marginXLeft, 55. );
  double marginXRight = 20.0 ;

  // Get a pointer to the plotter.
  PlotterBase* plotter = getPlotter();

  // Set and adjust top margin
  double marginYTop = 30.0;
  if ( m_plotter -> hasAxis ( Axes::Z) )
    {
      marginYTop = 70.0;
    }
  const FontBase* titlefont = plotter->titleFont();
  if (titlefont) {
    marginYTop = marginYTop+titlefont->pointSize()-9.0;
  }
  const FontBase* zfont = plotter->labelFont(Axes::Z);
  if (zfont) {
    marginYTop = marginYTop+zfont->pointSize()-7.0;
  }
   
  // Set and adjust bottom margin
  double marginYBottom = 34.0 ;
  const FontBase* labelfont = plotter->labelFont(Axes::X);
  if (labelfont) {
    marginYBottom = marginYBottom+labelfont->pointSize()-11.0;
  }

  // Add additional margins. Now it can be added by PNG title, labels.
  marginYTop+=plotter->getTopMargin()+plotter->getZMargin();
  marginYBottom+=plotter->getBottomMargin();
  marginXLeft+=plotter->getLeftMargin();

  double aspect_ratio = m_plotter -> getAspectRatio ();

  double marginWidth = width - marginXLeft - marginXRight;
  double marginHeight =height - marginYTop - marginYBottom; 

  if ( aspect_ratio >  0.0 ) { 
    if ( marginWidth /aspect_ratio > marginHeight ){
      marginWidth = aspect_ratio*marginHeight;
    } 
    else {
      marginHeight = marginWidth/aspect_ratio;
    }
  }
  
  m_margin_rect.setRect ( marginXLeft, marginYTop,
			  marginWidth, marginHeight );
}

double
DataView::
userToMarginX ( double x ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();
  double diff = x - user_rect.getX ();
  double scale = m_margin_rect.getWidth() / user_rect.getWidth ();
  double margin_x = m_margin_rect.getX () + diff * scale;
  return margin_x;
}

double
DataView::
userToInvertedMarginX ( double x ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();
  double diff = x - user_rect.getX ();
  double scale = m_margin_rect.getWidth() / user_rect.getWidth ();
  double margin_ix = m_margin_rect.getX() + m_margin_rect.getWidth() - diff*scale;
  return margin_ix;
}

double
DataView::
userToMarginY ( double y ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();
  return m_margin_rect.getY () 
    + ( y - user_rect.getY () )
    * m_margin_rect.getHeight () / user_rect.getHeight ();
}

double
DataView::
userToInvertedMarginY ( double y ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();
  return m_margin_rect.getY () 
    + m_margin_rect.getHeight ()
    - ( y - user_rect.getY () )
    * m_margin_rect.getHeight () / user_rect.getHeight ();
}

double
DataView::
userToMarginColor ( double c ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();

  return m_margin_rect.getX () 
    + ( c - user_rect.getZ () )
    * m_margin_rect.getWidth () / user_rect.getDepth ();
}

double 
DataView::
marginToUserX ( double x ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();

  return user_rect.getX () 
    + ( x - m_margin_rect.getX() ) 
    / ( m_margin_rect.getWidth () / user_rect.getWidth() ); 
}

double 
DataView::
marginToInvertedUserX ( double x ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();

  return user_rect.getX () 
    + ( m_margin_rect.getX() + m_margin_rect.getWidth() - x ) 
    / ( m_margin_rect.getWidth () / user_rect.getWidth() ); 
}

double
DataView::
marginToUserY ( double y ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();

  return user_rect.getY () 
    + ( y - m_margin_rect.getY() ) 
    /  ( m_margin_rect.getHeight () * user_rect.getHeight () );
}

double
DataView::
marginToInvertedUserY ( double y ) const
{
  const Rect & user_rect = m_plotter -> getUserRect ();

  return user_rect.getY () + 
    ( m_margin_rect.getY() 
      + m_margin_rect.getHeight() - y ) 
    / ( m_margin_rect.getHeight () / user_rect.getHeight () ) ;
}
