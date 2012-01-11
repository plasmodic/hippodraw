/** @file

hippodraw::RepBase class implementation

Copyright (C) 2001, 2002, 2004, 2005 The Board of Trustees of The
Leland Stanford Junior University.  All Rights Reserved.

$Id: RepBase.cxx,v 1.34 2007/01/24 21:41:49 xiefang Exp $
*/

// for dll interface warning
#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#endif

#include "RepBase.h"

#include "graphics/ViewBase.h"

using std::string;

using namespace hippodraw;

const Color RepBase::s_desel_color ( 225, 225, 225 );

RepBase::RepBase ( const char * string, float size )
  : m_name ( string ), 
  m_size ( size ),
  m_desel ( false )
{
}

RepBase::RepBase ( const RepBase & rep )
  : m_name ( rep.m_name ),
    m_text ( rep.m_text ),
    m_color ( rep.m_color ),
    m_size ( rep.m_size ),
    m_desel ( rep.m_desel )
{
}

/* virtual */
RepBase::~RepBase ()
{
}

const string & RepBase::name () const
{
  return m_name;
}

void 
RepBase::setColor ( const Color & color )
{
  m_color = color;
}

const Color & RepBase::getColor () const
{
  return m_color;
}

const Color & RepBase::color () const
{
  if ( m_desel ) {
    return s_desel_color;
  }

  return m_color;
}

void RepBase::setSelected ( bool yes )
{
  m_desel = !yes;
}

bool
RepBase::
isSelected () const
{
  return m_desel == false;
}

void
RepBase::
setHighLighted ( bool yes )
{
  m_highlite = yes;
}

float RepBase::size () const
{
  return m_size;
}

void RepBase::setSize ( float size )
{
  m_size = size;
}

void
RepBase::
setStyle ( unsigned int )
{
  // do nothing
}

unsigned int
RepBase::
getStyle ( ) const
{
  return 0;
}

/* virtual */
bool RepBase::xError () const
{
  return false;
}

/* virtual */
bool RepBase::yError () const
{
  return false;
}

void
RepBase::
setErrorOn ( Axes::Type, bool )
{
}

void RepBase::setText ( const std::string & text )
{
  m_text = text;
}

const std::string & 
RepBase::getText() const
{
  return m_text;
}


const BinToColor *
RepBase::
getValueTransform () const
{
  return 0;
}

void
RepBase::
setValueTransform ( BinToColor * )
{
  // do nothing
}

void
RepBase::
displayError ( ViewBase & view )
{
  string error ( "Error: " );
  error += m_name;
  error += " can not be displayed.";
  view.drawText ( error, 5.0, 0.0, m_size, 0, 'l', 't', true );
}

bool
RepBase::
uses ( Color::Value ) const
{
  return true;
}

bool
RepBase::
uses ( Line::Style ) const
{
  return false;
}

bool
RepBase::
uses ( Symbol::Type ) const
{
  return false;
}
