/** @file

hippodraw::ListDialog class implementation

Copyright (C) 2004-2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: ListDialog.cxx,v 1.9 2006/07/30 15:53:27 pfkeb Exp $

*/

#include "ListDialog.h"

#include <cassert>

using std::string;

using namespace hippodraw;

ListDialog::
ListDialog ( QWidget * parent )
   : QDialog ( parent )
{
  setupUi ( this );
}

void
ListDialog::
setNames ( const std::vector < std::string > & names )
{
  for ( unsigned int i = 0; i < names.size (); i++ ) {
    const string & n = names[i];
    QString name ( n.c_str() );
    
    m_tuple_list -> setItem ( i, 0, new QTableWidgetItem ( QString::number (i+1) ) );
    m_tuple_list -> setItem ( i, 1, new QTableWidgetItem ( name ) );

    m_item_list.push_back ( std::make_pair ( i+1, n ) );
  }
  m_tuple_list -> setSelectionMode ( QAbstractItemView::SingleSelection );
  m_tuple_list -> setRangeSelected ( QTableWidgetSelectionRange(0, 0, 0, 1), true );
}

unsigned int
ListDialog::
selectedItem () const
{
  assert ( m_tuple_list -> selectedRanges() . size() == 1 );
  const QTableWidgetSelectionRange &selection = m_tuple_list -> selectedRanges() . at(0);
  assert ( selection.topRow() == selection.bottomRow() );
  return selection.topRow();
}

void
ListDialog::
doubleClicked ( )
{
  accept ();
}
