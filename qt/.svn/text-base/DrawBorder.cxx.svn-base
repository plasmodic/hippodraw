/** @file

hippodraw::DrawBorder class implementation

Copyright (C) 2002, 2003, 2005-2006  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DrawBorder.cxx,v 1.18 2006/01/08 21:52:28 pfkeb Exp $

*/

// for truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "DrawBorder.h"

#include "graphics/Color.h"
#include "graphics/ViewBase.h"

#include <vector>

using std::vector;

using namespace hippodraw;

DrawBorder * DrawBorder::s_instance = 0;

DrawBorder::DrawBorder ()
{
}

DrawBorder::DrawBorder ( const DrawBorder & )
{
  // Left empty to avoid copying.
}

DrawBorder * DrawBorder::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new DrawBorder ();
  }
  return s_instance;
}

void DrawBorder::setView (ViewBase * view)
{
  m_view = view;
}

ViewBase * DrawBorder::getView ()
{
  return m_view;
}

void DrawBorder::draw()
{

  Rect rect = m_view->getDrawRect();
  double width = rect.getWidth();
  double height = rect.getHeight();

  width = width - 2;
  height = height - 2;

  vector <double> vx (8);
  vector <double> vy (8);

  vx [0] = 2;
  vy [0] = 2;
  vx [1] = width;
  vy [1] = 2;

  vx [2] = width;
  vy [2] = 2;
  vx [3] = width;
  vy [3] = height;

  vx [4] = width;
  vy [4] = height;
  vx [5] = 2;
  vy [5] = height;

  vx [6] = 2;
  vy [6] = height;
  vx [7] = 2;
  vy [7] = 2;

  m_view->drawViewLines ( vx, vy, Line::Solid, Color(180, 180, 180), 0 );

  // Now draw the knobs.

  drawKnob ( 2, 2);                 //Upper Right.
  drawKnob ( width / 2, 2);         //Upper Middle.
  drawKnob ( width, 2);             //Upper Left.

  drawKnob ( 2, height / 2);        //Middle Left.
  drawKnob ( width, height / 2);    //Middle Right.

  drawKnob ( 2, height);            //Lower left.
  drawKnob ( width / 2, height);    //Lower middle.
  drawKnob ( width, height);        //Lower right.

}

void DrawBorder::drawKnob( double x, double y)
{

  int size = 2;

  vector <double> vx (8);
  vector <double> vy (8);

  vx [0] = x-size;
  vy [0] = y-size;
  vx [1] = x+size;
  vy [1] = y-size;

  vx [2] = x+size;
  vy [2] = y-size;
  vx [3] = x+size;
  vy [3] = y+size;

  vx [4] = x+size;
  vy [4] = y+size;
  vx [5] = x-size;
  vy [5] = y+size;

  vx [6] = x-size;
  vy [6] = y+size;
  vx [7] = x-size;
  vy [7] = y-size;

  m_view->drawViewLines ( vx, vy, Line::Solid, Color(180, 180, 180), 0 );
}
