/** @file

hippodraw::ColumnPointRep class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ColumnPointRep.cxx,v 1.113 2006/09/25 22:45:25 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ColumnPointRep.h"
#include "ErrorBarRep.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

#include <cmath>

#include <cassert>

using std::abs;
using std::string;
using std::vector;

using namespace hippodraw;

/** @todo Could postpone creation of the ErrorBarRep until flag is set
    to require one.  However, if postponed, got to remember to set the
    color. 
*/
ColumnPointRep::ColumnPointRep ()
  : PointRepBase ( "Column", 1.0 ),
    m_line_style( Line::Solid ),
    m_y_flag ( false )
{
  m_error_rep = new ErrorBarRep();
}

ColumnPointRep::ColumnPointRep ( const ColumnPointRep & point_rep )
  : PointRepBase ( point_rep ), 
    m_line_style ( point_rep.m_line_style ),
    m_y_flag ( point_rep.m_y_flag )
{
  RepBase * clone = point_rep.m_error_rep->clone ();
  m_error_rep = dynamic_cast< ErrorBarRep  *> ( clone );
}

ColumnPointRep::~ColumnPointRep ()
{
  delete m_error_rep;
}

RepBase * ColumnPointRep::clone ()
{
  return new ColumnPointRep ( *this );
}

void ColumnPointRep::setColor ( const Color & color )
{
  RepBase::setColor ( color );

  if ( m_error_rep ) {
    m_error_rep->setColor ( color );
  }
}

void
ColumnPointRep::
setStyle ( unsigned int style )
{
  m_line_style = Line::convert ( style );
}

unsigned int
ColumnPointRep::
getStyle ( ) const
{
  return m_line_style;
}

void
ColumnPointRep::
setErrorOn ( Axes::Type axis, bool flag )
{
  if ( axis == Axes::Y ) {
    m_error_rep->setYError ( flag );
    m_y_flag = flag;
  }
}

bool ColumnPointRep::yError () const
{
  return m_y_flag;
}

void
ColumnPointRep::
drawValues ( ViewBase * view ) const
{
  const Color & cur_color = color();
  view -> drawPolyLine ( m_x, m_y, m_line_style, cur_color, m_size );
}

namespace dp = hippodraw::DataPoint2DTuple;

/** @todo Should the TransformBase * be const?
 */
void
ColumnPointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  m_x.clear();
  m_y.clear();

  unsigned int size = ntuple -> rows ();
  unsigned int reserve = 2 * ( size + 1 );

  m_x.reserve ( reserve );
  m_y.reserve ( reserve );

  double last_x = 0.0;

  const Rect & user_rect = view -> getUserRect ();

  for ( unsigned int i = 0; i < size; i ++ ) {
    const vector < double > & row = ntuple -> getRow ( i );
    double x = row [ dp::X ];
    double hw = row [ dp::XERR ];
    if ( i == 0 ) {
      m_x.push_back ( x - hw );
      m_y.push_back ( 0.0 ); // The first y is always zero.

      m_x.push_back ( x -hw );
    }
    else {
      m_x.push_back ( last_x );
    }
    double y = row [ dp::Y ];
    m_y.push_back ( y ); // X was already set.
    x += hw;
    m_x.push_back ( x );
    m_y.push_back ( y );

    last_x = x;
  }
  m_x.push_back ( last_x );

  const BinaryTransform * t 
    = dynamic_cast < const BinaryTransform * > ( transform );
  assert ( t != 0 );

  m_y.push_back ( 0.0 ); // The last y is always zero.
  assert ( m_x.size() == m_y.size() );

  t -> transform ( m_x, m_y );

  user_rect.makeInBounds ( m_x, m_y );

  drawValues ( view );

  if ( m_y_flag ) {
    m_error_rep -> drawProjectedValues ( ntuple, transform, view );
  }
}

bool
ColumnPointRep::
uses ( Line::Style ) const
{
  return true;
}

void
ColumnPointRep::
setSize ( float value )
{
  RepBase::setSize ( value );

  m_error_rep -> setSize ( value );
}    
