/** @file

hippodraw::ColorBoxPointRep class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ColorBoxPointRep.cxx,v 1.118 2007/06/06 22:23:39 pfkeb Exp $

*/

#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ColorBoxPointRep.h"

#include "colorreps/BinToColor.h"
#include "colorreps/BinToColorFactory.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"
#include "transforms/PeriodicBinaryTransform.h"

#include "plotters/PlotterBase.h"


#include <cassert>
#include <cmath>

using namespace hippodraw;

using std::vector;

ColorBoxPointRep::
ColorBoxPointRep ( )
  : PointRepBase ( "ColorBox", 0.0 )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  m_bin_to_color = factory -> create ( "Rainbow" );
  m_box_edge = false;
}

ColorBoxPointRep::ColorBoxPointRep ( const ColorBoxPointRep & point_rep )
  : PointRepBase ( point_rep )
{
  BinToColor * btc = point_rep.m_bin_to_color;

  m_bin_to_color = btc -> clone ();
  m_box_edge = point_rep.m_box_edge;
}

ColorBoxPointRep::~ColorBoxPointRep()
{
  delete m_bin_to_color;
}

RepBase * ColorBoxPointRep::clone()
{
  return new ColorBoxPointRep( *this );
}

const BinToColor *
ColorBoxPointRep::
getValueTransform ( ) const
{
  return m_bin_to_color;
}

void
ColorBoxPointRep::
setValueTransform ( BinToColor * btc )
{
  delete m_bin_to_color;
  m_bin_to_color = btc;
}

namespace dp = hippodraw::DataPoint3DTuple;

void
ColorBoxPointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  const Range & range = view -> getRange ( Axes::Z );
  PlotterBase * plotter = view -> getPlotter ();

  double high = range.high();
  double low = range.low();

  const BinaryTransform * bt
    = dynamic_cast < const BinaryTransform * > ( transform );
  assert ( bt != 0 );

  bt -> transformZ ( high );
  bt -> transformZ ( low );

  Range newrange ( low, high );
  m_bin_to_color->setRange ( newrange );
  const Rect & user_rect = view -> getUserRect ();
  // Use the rectangle before transform to define box border.
  const Rect & raw_rect = view -> getRawRect();

  bool isLinear = bt -> isLinearInXY ();
  bool surpress_zero = low == 0.0;

  unsigned int size = ntuple -> rows ();

  const Color & rep_color = plotter->repColor();
  
  // keep initialization out of loop.
  const PeriodicBinaryTransform * pbt
        = dynamic_cast < const PeriodicBinaryTransform * > ( bt );


  for ( unsigned int i = 0; i < size; i++ ) {
    const vector < double > & row = ntuple -> getRow ( i );

    double value = row [ dp::Z ];
    if ( surpress_zero && value == 0.0 ) continue;

    double half_xwidth = row [ dp::XERR ];
    double half_ywidth = row [ dp::YERR ];

    double x = row [ dp::X ];
    double y = row [ dp::Y ];
    // Do rotation before defining box border.    
    // Avoid wrong box size caused by rotation or transform.
 
    if ( pbt != NULL )
      pbt->rotateByOffsets ( y, x );

    // Define box border.
    double x1 = x - half_xwidth;
    double y1 = y - half_ywidth;
    double x2 = x + half_xwidth;
    double y2 = y + half_ywidth;

    double xtl = x1;
    double ytl = y1;
    double xbl = x1;
    double ybl = y2;
    double xtr = x2;
    double ytr = y1;
    double xbr = x2;
    double ybr = y2;

    raw_rect.makeInBounds ( xtl, ytl );
    raw_rect.makeInBounds ( xbl, ybl );
    raw_rect.makeInBounds ( xtr, ytr );
    raw_rect.makeInBounds ( xbr, ybr );
     
    // Do transform after defining box border.
    if ( isLinear == false ) {
      bt -> transform ( xtl, ytl );
      bt -> transform ( xbl, ybl );
      bt -> transform ( xtr, ytr );
      bt -> transform ( xbr, ybr );
    }


    if ( m_desel ) {
      if ( isLinear == false ) {
	std::vector <double> xx;
	xx.push_back(xtl);
	xx.push_back(xbl);
	xx.push_back(xbr);
	xx.push_back(xtr);
	std::vector <double> yy;
	yy.push_back(ytl);
	yy.push_back(ybl);
	yy.push_back(ybr);
	yy.push_back(ytr);
	Color black(Color::black);
	view -> drawPolygon ( xx, yy, black, black );
      } else {
	const int gray = 256;
	view -> drawSquare ( x1, y1, x2, y2, gray, gray, gray );
      }
      return;
    }
     

    bt -> transformZ ( value );

	Color color = rep_color; 

    if ( value > user_rect.getZ() ) {
       if ( !user_rect.isInDepth ( value ) ) {
          double x(0), y(0);
          user_rect.makeInBounds(x, y, value);
       }
       if ( m_desel ) { // deselected
	 color = s_desel_color;
       }
       else { 
	 m_bin_to_color -> doubleToColor ( value, color );
       }


       if ( isLinear == false ) {
	 /* Draw polygon to fix all periodic transforms */
	 std::vector <double> xx;
	 xx.push_back(xtl);
	 xx.push_back(xbl);
	 xx.push_back(xbr);
	 xx.push_back(xtr);
	 std::vector <double> yy;
	 yy.push_back(ytl);
	 yy.push_back(ybl);
	 yy.push_back(ybr);
	 yy.push_back(ytr);
	 Color black(Color::black);
	 view -> drawPolygon ( xx, yy, color, m_box_edge?black:color );

       } else {
	 /* Draw square to improve performance
	    Maybe a function that draws the square and the edges at the
	    same time will improve the performance more
	 */
	 view -> drawSquare ( xtl, ytl, xbr, ybr,
			      color.getRed (), 
			      color.getGreen(), 
			      color.getBlue() );
	 if (m_box_edge) {
	   std::vector<double> x;
	   x.push_back(x1);
	   x.push_back(x2);
	   x.push_back(x2);
	   x.push_back(x1);
	   x.push_back(x1);
	   
	   std::vector<double> y;
	   y.push_back(y1);
	   y.push_back(y1);
	   y.push_back(y2);
	   y.push_back(y2);
	   y.push_back(y1);
	   
	   Color color(Color::black);
	   view->drawPolyLine ( x, y, Line::Solid, color, 1.0 );
	 }
       }
    } 

  } 
}

bool
ColorBoxPointRep::
uses ( Color::Value ) const
{
  return false;
}


void 
ColorBoxPointRep::setBoxEdge( bool show )
{
  m_box_edge = show;
}
