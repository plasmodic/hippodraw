/** @file

ContourFunctionRep class implementation

Copyright (C) 2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ContourFunctionRep.cxx,v 1.8 2007/01/21 21:10:55 pfkeb Exp $
 
*/

#include "ContourFunctionRep.h"

#include "colorreps/BinToBlack.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/NTuple.h"
#include "functions/FunctionBase.h"

using std::string;
using std::vector;

namespace dp = hippodraw::DataPoint3DTuple;

namespace hippodraw {

ContourFunctionRep::
ContourFunctionRep ( )
  : ContourPointRep ( ),
    m_steps ( 25 )
{
  BinToBlack * btb = new BinToBlack ( "black" );
  setValueTransform ( btb );
  
  unsigned int cols = dp::SIZE;
  m_ntuple = new NTuple ( cols );
  m_ntuple -> reserve ( m_steps * m_steps );
  
  vector < unsigned int > shape ( 3 );
  shape[0] = m_steps;
  shape[1] = m_steps;
  shape[2] = 3;
  m_ntuple -> setShape ( shape );
}

ContourFunctionRep::
~ContourFunctionRep()
{
}


void
ContourFunctionRep::
drawProjectedValues ( const Range & x_range, const Range & y_range,
		      const FunctionBase * function, 
		      TransformBase * transform,
		      ViewBase * view )
{
  m_ntuple-> clear ();
  
  double dx = x_range.length() / m_steps;
  double x  = x_range.low () + 0.5 * dx;

  double dy = y_range.length() / m_steps;
  double y_low = y_range.low () + 0.5 * dy;

  vector < double > coord ( 2 );
  vector < double > row ( dp::SIZE );
  row [dp::XERR] = 0.5 * dx;
  row [dp::YERR] = 0.5 * dy;
  row [dp::ZERR] = 0.;
  
  for ( unsigned int i = 0; i < m_steps; i++, x += dx ) {
    coord [0] = x;
    double y  = y_low;
    for ( unsigned int j = 0; j < m_steps; j++, y += dy ) {
      coord [1] = y;
      double value = function -> operator() ( coord );

      row[dp::X] = x;
      row[dp::Y] = y;
      row[dp::Z] = value;
      
      m_ntuple -> addRow ( row );
    } 
  }

  ContourPointRep::drawProjectedValues ( m_ntuple, transform, view );
}

} // namespace hippodraw

