/** @file

hippodraw::SymbolPointRep class implementation

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: SymbolPointRep.cxx,v 1.133 2007/03/14 23:28:28 xiefang Exp $

*/

#ifdef _MSC_VER
// include max() and min() missing from Microsoft Visual C++
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "SymbolPointRep.h"

#include "ErrorBarRep.h"

#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"

#include "projectors/NTupleProjector.h"
#include "transforms/PeriodicBinaryTransform.h"

#include <cmath>

#include <cassert>

using std::string;
using std::vector;

using namespace hippodraw;

SymbolPointRep::
SymbolPointRep ( hippodraw::Symbol::Type symbol, float size )
  : PointRepBase ( "Symbol", size ),
    m_plot_symbol( symbol )
{
  m_error_rep = new ErrorBarRep();
}

SymbolPointRep::SymbolPointRep ( )
  : PointRepBase ( "Symbol", 2.0 ),
    m_plot_symbol( Symbol::SOLIDSQUARE )
{
  m_error_rep = new ErrorBarRep();
}

SymbolPointRep::SymbolPointRep ( const SymbolPointRep & point_rep )
  : PointRepBase( point_rep ), 
  m_x( point_rep.m_x ),
  m_y( point_rep.m_y ),
  m_x_flag( point_rep.m_x_flag ),
  m_y_flag( point_rep.m_y_flag ),
  m_plot_symbol( point_rep.m_plot_symbol )
{
  RepBase * clone = point_rep.m_error_rep->clone();
  m_error_rep = dynamic_cast< ErrorBarRep * > ( clone );
}

SymbolPointRep::~SymbolPointRep()
{
  delete m_error_rep;
}

RepBase * SymbolPointRep::clone()
{
  return new SymbolPointRep( *this );
}

void SymbolPointRep::setColor ( const Color & color )
{
  RepBase::setColor ( color );

  if ( m_error_rep ) {
    m_error_rep->setColor ( color );
  }
}

void SymbolPointRep::setStyle ( unsigned int symbol )
{
  m_plot_symbol = Symbol::convert ( symbol );
}

unsigned int
SymbolPointRep::
getStyle ( ) const
{
  return m_plot_symbol;
}

void
SymbolPointRep::
setErrorOn ( Axes::Type axis, bool flag )
{
  if ( axis == Axes::X ) {
    m_error_rep -> setXError ( flag );
    m_x_flag = flag;
  }
  else if ( axis == Axes::Y ) {
    m_error_rep -> setYError ( flag );
    m_y_flag = flag;
  }
}

bool SymbolPointRep::xError () const
{
  return m_x_flag;
}

bool SymbolPointRep::yError () const
{
  return m_y_flag;
}

/**@todo The ViewBase object is not used, so why is it in interface.
 */
void
SymbolPointRep::
transformPoints ( const TransformBase & tb,
		  ViewBase & )
{
  try {
    const BinaryTransform & tf 
      = dynamic_cast < const BinaryTransform & > ( tb );

    tf.transform ( m_x, m_y );
  }
  catch ( ... ) {
    assert ( false );
  }
}

namespace dp = hippodraw::DataPoint2DTuple;

/** @todo Should @a transform be const in this method. 
 */
void
SymbolPointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  unsigned int size = ntuple -> rows ();
  if ( size == 0 ) return;

  const BinaryTransform * tf 
    = dynamic_cast < BinaryTransform * > ( transform );
  assert ( tf != 0 );
  beginPlot ( size, tf, view );
 
  for ( unsigned int i = 0; i < size; i++ ) {
    drawProjectedValue ( i, ntuple, tf, view );
  }  

  const Color & cur_color = color ();

  // Transform vector
  tf -> transform (m_x, m_y);
  view -> drawPoints ( m_x, m_y, m_plot_symbol, m_size, cur_color );

  if ( m_x_flag || m_y_flag ) {
    m_error_rep -> drawProjectedValues ( ntuple, transform, view );
  }
}

void
SymbolPointRep::
beginPlot ( unsigned int size,
	    const BinaryTransform * tf, 
	    const ViewBase * view )
{
  m_x.clear ();
  m_y.clear ();

  m_x.reserve ( size );
  m_y.reserve ( size );

  c_x_range = view -> getRange ( Axes::X );
  c_y_range = view -> getRange ( Axes::Y );

  if ( tf -> isPeriodic () ) {
    c_periodic_tf = dynamic_cast < const PeriodicBinaryTransform * > ( tf );
  }
}

bool
SymbolPointRep::
drawProjectedValue ( unsigned int i, 
		     const DataSource * ntuple, 
		     const BinaryTransform * tf, 
		     ViewBase * view )
{
  bool retval = false;

  const vector < double > & row = ntuple -> getRow ( i );
  double x = row [ dp::X ];
  double y = row [ dp::Y ];

  double xtemp = x;
  double ytemp = y;

  if ( tf -> isPeriodic() )
    {
      xtemp = c_periodic_tf  -> moduloSubX ( x, 
	c_periodic_tf->rotation(Axes::X)/UNITS_RADIANS
	*c_periodic_tf->units(Axes::X));
      ytemp = c_periodic_tf  -> moduloSubY ( y, 
	c_periodic_tf->rotation(Axes::Y)/UNITS_RADIANS
	*c_periodic_tf->units(Axes::Y));
      
      c_periodic_tf->rotateByOffsets( y, x );
    }

  const Range & xrange = view -> getRange ( Axes::X );
  const Range & yrange = view -> getRange ( Axes::Y );

  if ( xrange.includes ( xtemp ) && 
       yrange.includes ( ytemp ) ) 
    {
      //tf -> transform( x, y );
      m_x.push_back ( x );
      m_y.push_back ( y );
      retval = true;
    }

  return retval;
}

bool
SymbolPointRep::
uses ( Symbol::Type ) const
{
  return true;
}

void
SymbolPointRep::
setSize ( float value )
{
  RepBase::setSize ( value );

  m_error_rep -> setSize ( value / 4. );
}    
