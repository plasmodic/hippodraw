/** @file

hippodraw::TextPlotter class implementation

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextPlotter.cxx,v 1.71 2007/07/20 19:29:56 pfkeb Exp $

*/

// for truncation warning in debug mode
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "TextPlotter.h"

#include "datareps/TextDataRep.h"

#include <cassert>

using std::list;
using std::swap;
using std::string;

using namespace hippodraw;

TextPlotter::TextPlotter ( )
  : PlotterBase ( "TextPlotter" )
{
  m_datarep = new TextDataRep ( 0 );
  m_title = "none";
}

TextPlotter::TextPlotter ( const TextPlotter & plotter )
  : PlotterBase ( plotter )
{
}

TextPlotter::~TextPlotter ()
{
  if ( m_datarep != 0 ) delete m_datarep;
}

PlotterBase * TextPlotter::clone ()
{
  return new TextPlotter ( *this );
}

/* virtual */
void TextPlotter::addDataRep ( DataRep * rep )
{
  TextDataRep * textrep = dynamic_cast < TextDataRep * > ( rep );
  assert ( textrep != 0 );

  m_datarep = textrep;
}

int TextPlotter::getNumDataReps () const
{
  return 1;
}

DataRep * TextPlotter::getDataRep ( int ) const
{
  return m_datarep;
}

void TextPlotter::drawIn ( ViewBase * view )
{
  m_datarep->drawProjectedValues ( 0, view );
}

bool TextPlotter::hasNTupleBindings () const
{
  return false;
}

void TextPlotter::setRepresentation ( RepBase * rep )
{
  m_datarep->setPointRep ( rep );
}

RepBase * TextPlotter::representation () const
{
  return m_datarep->getRepresentation ();
}
 
/** @todo Implement TextRep that can display errors. */
void
TextPlotter::
setErrorDisplay ( hippodraw::Axes::Type, bool )
{
  assert ( false );
}

/** @todo Implement TextRep that can display errors. */
bool
TextPlotter::
errorDisplay ( hippodraw::Axes::Type ) const
{
  return false;
}

void TextPlotter::setAxisModel ( AxisModelBase *, hippodraw::Axes::Type )
{
}

void TextPlotter::setRepColor ( const Color & color )
{
  m_datarep->setRepColor ( color );
  notifyObservers ();
}

const Color & TextPlotter::repColor () const
{
  return m_datarep->getRepColor();
}

float TextPlotter::userToMarginX ( double x ) const
{
  return x;
}

float TextPlotter::userToMarginY ( double y ) const
{
  return y;
}

float TextPlotter::userToInvertedMarginY ( double y ) const
{
  return y;
}

void
TextPlotter::
toUserXY ( double mx, double my, bool, 
	   double & ux, double & uy ) const
{
  ux = mx;
  uy = my;
}

const DataRep * TextPlotter:: getParentDataRep () const
{
  return m_datarep -> getParentDataRep ();
}

void TextPlotter:: setParentDataRep ( DataRep * rep )
{
  m_datarep -> setParentDataRep ( rep );
  rep -> addObserver ( this );
}

void TextPlotter::willDelete ( const Observable * observable )
{
   const DataRep * rep = dynamic_cast < const DataRep * > ( observable );
   if ( getParentDataRep() == rep ){
     m_datarep->setParentDataRep ( 0 );
   }
}

TransformBase * TextPlotter::getTransform ()
{
  return 0;
}

/** @todo Returns something, if we can think what. */
NTuple *
TextPlotter::
createNTuple () const
{
  return 0;
}

void
TextPlotter::
update ()
{
  m_datarep -> update ();
}

bool
TextPlotter::
isTextPlotter () const
{
  return true;
}
