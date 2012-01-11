// -*- mode:c++ -*-

/** @file 

The SIP interface definition for SaveAsImageEvent

Copyright (C)  2004  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: SaveAsImageEvent.sip,v 1.3 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu
*/

class SaveAsImageEvent : QCustomEvent
{
%TypeHeaderCode
#include "qt/SaveAsImageEvent.h"
using hippodraw::SaveAsImageEvent;
using hippodraw::PlotterBase;
%End

public:


  /** The constructor taking the @a plotter object and image @a
      filename as arguments.
  */
  SaveAsImageEvent ( const PlotterBase * , const std::string &  );

  /** The copy constructor. */
  SaveAsImageEvent ( const SaveAsImageEvent & );

  /** The required virtual destructor. */
  virtual ~SaveAsImageEvent ();


  /** Returns the PlotterBase object that created the update event.
   */
  PlotterBase * plotter () const;

  /** Returns the filename for the image.
   */
  const std::string & filename () const;


};
