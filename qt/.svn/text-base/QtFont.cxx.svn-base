/** @file

QtFont class implementation. This is a an implementation of the
FontBase class which essentially makes it a wrapper around the QFont
class.

Copyright (C) 2004, 2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtFont.cxx,v 1.12 2007/06/28 20:03:02 pfkeb Exp $

*/

#include "QtFont.h"

#include <string>

using std::string;

namespace hippodraw {

QtFont::
QtFont()
  : m_font( QFont ("times") ),
    m_flag( false )
{
}
  
QtFont::
QtFont ( const std::string & family, int pointsize, int weight, bool italic )
  :m_font( family.c_str(), pointsize, weight, italic ),
   m_flag( false )
{
}

QtFont::
QtFont( const QFont& qfont )
{
  m_font = qfont;
  m_flag = true;
}

QtFont::
~QtFont()
{
}

string
QtFont::
family() const
{
  QString qstr = m_font.family();
  
  return qstr.toLatin1().data();
}

void
QtFont::
setFamily ( const std::string & family )
{
  m_font.setFamily( family.c_str() );
}

int
QtFont::
pointSize() const
{
  return m_font.pointSize();
}

void
QtFont::
setPointSize( int pointsize )
{
  m_font.setPointSize( pointsize );
}

int
QtFont::
weight () const
{
  return m_font.weight();
}

void
QtFont::
setWeight( int weight )
{
  m_font.setWeight( weight );
}

bool
QtFont::
italic () const
{
  return m_font.italic();
}

void
QtFont::
setItalic( bool enable )
{
  m_font.setItalic( enable );
}

const QFont &
QtFont::
font() const
{
  return m_font;
}

void
QtFont::
setFont( const QFont & qfont )
{
  m_font = qfont;
  m_flag = true;
}

void
QtFont::
unsetFont()
{
  m_flag = false;
}

bool
QtFont::
isSet()
{
  return m_flag;
}

} // namespace hippodraw



