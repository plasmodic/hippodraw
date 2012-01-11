/** @file 

hippodraw::FileOpenEvent implementation

Copyright (C) 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: FileOpenEvent.cxx,v 1.2 2006/06/30 23:02:49 pfkeb Exp $

*/

#include "FileOpenEvent.h"

#include <QtCore/QEvent>
#include <QtCore/QEvent>

using namespace hippodraw;

FileOpenEvent::
FileOpenEvent ( const QString & name )
  : QEvent ( OpenEventType ),
    m_file_name ( name )
{
}

FileOpenEvent::~FileOpenEvent ()
{
}

QString
FileOpenEvent::
file () const
{
  return m_file_name;
}
