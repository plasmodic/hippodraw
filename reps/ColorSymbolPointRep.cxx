/** @file

hippodraw::ColorSymbolPointRep class implementation

Copyright (C) 2001-2005   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ColorSymbolPointRep.cxx,v 1.77 2007/01/21 21:10:55 pfkeb Exp $

*/

// For truncation warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "ColorSymbolPointRep.h"

#include "colorreps/BinToColor.h"
#include "colorreps/BinToColorFactory.h"
#include "datasrcs/DataPointTuple.h"
#include "datasrcs/DataSource.h"
#include "graphics/ViewBase.h"
#include "transforms/BinaryTransform.h"

#include <cassert>

using namespace hippodraw;

using std::vector;

ColorSymbolPointRep::
ColorSymbolPointRep ( hippodraw::Symbol::Type symbol, float size )
  : SymbolPointRep ( symbol, size )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  m_color_gen = factory -> create ( "Rainbow" );
  m_name = "ColorSymbol"; // need to override what base class did
}

ColorSymbolPointRep::ColorSymbolPointRep ( )
  : SymbolPointRep ( )
{
  BinToColorFactory * factory = BinToColorFactory::instance ();
  m_color_gen = factory -> create ( "Rainbow" );
  m_name = "ColorSymbol"; // need to override what base class did
}

ColorSymbolPointRep::ColorSymbolPointRep( const ColorSymbolPointRep & rep )
  : SymbolPointRep ( rep ),
    m_color_gen ( rep.m_color_gen->clone () )
{
}

ColorSymbolPointRep::~ColorSymbolPointRep()
{
}

RepBase * ColorSymbolPointRep::clone()
{
  return new ColorSymbolPointRep( *this );
}

const BinToColor *
ColorSymbolPointRep::
getValueTransform ( ) const
{
  return m_color_gen;
}

void
ColorSymbolPointRep::
setValueTransform ( BinToColor * btc )
{
  delete m_color_gen;
  m_color_gen = btc;
}

namespace dp = hippodraw::DataPoint3DTuple;

void
ColorSymbolPointRep::
drawProjectedValues ( const DataSource * ntuple,
		      TransformBase * transform,
		      ViewBase * view )
{
  m_x.clear ();
  m_y.clear ();
  m_colors.clear ();

  unsigned int size = ntuple -> rows ();
  const BinaryTransform * bt 
    = dynamic_cast < const BinaryTransform * > ( transform );

  SymbolPointRep::beginPlot ( size, bt, view );
  const Range & range = view -> getRange ( Axes::Z );
  double high = range.high();
  double low = range.low();

  bt -> transformZ ( high );
  bt -> transformZ ( low );

  Range newrange ( low, high, range.pos() );

  // This needs to be done before we quit.
  m_color_gen->setRange ( newrange );

  if ( size == 0 ) return;

  m_x.reserve ( size );  
  m_y.reserve ( size );
  m_colors.reserve ( size );

  for ( unsigned int i = 0; i < size; i++ ) {
    const vector < double > & row = ntuple -> getRow ( i );

    double value = row [ dp::Z ];

    if ( range.includes ( value ) == false ) continue;


    const Range & limit_z = bt -> limitZ ();
    bool retval =  limit_z.includes ( value );
    if ( retval == false ) continue;
    retval = drawProjectedValue ( i, ntuple, bt, view );
    if ( retval == false ) continue;

    bt -> transformZ ( value );
    Color color;
    m_color_gen -> doubleToColor ( value, color );
    m_colors.push_back ( color );
  }
  const BinaryTransform * bint 
    = dynamic_cast < const BinaryTransform * > ( transform );

  bint -> transform (m_x, m_y);
  view -> drawPoints ( m_x, m_y, m_colors, m_plot_symbol, m_size );

}

bool
ColorSymbolPointRep::
uses ( Color::Value ) const
{
  return false;
}
