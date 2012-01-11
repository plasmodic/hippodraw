/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for DataSource

Copyright (C)  2002, 2004, 2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: DataSource.sip,v 1.7 2007/02/24 22:24:38 pfkeb Exp $

@author <Paul_Kunz@slac.stanford.edu>

*/

class DataSource
{
%TypeHeaderCode
#include "datasrcs/DataSource.h"
#include <string>
using hippodraw::DataSource;
%End

protected:

  DataSource ();

public:

  virtual ~DataSource ();

  void setName ( const std::string & );
  const std::string & getName() const;

  bool setLabelAt ( const std::string &, unsigned );
  virtual void setLabels ( const std::vector<std::string> & );

  virtual const std::string & getLabelAt ( unsigned );

  void setTitle ( const std::string & );
  const std::string & title () const;

  virtual unsigned columns () const;
  virtual unsigned rows () const = 0;
  virtual bool empty () const = 0;

private :
  virtual void valueAt ( unsigned int, unsigned int ) const = 0;
};
