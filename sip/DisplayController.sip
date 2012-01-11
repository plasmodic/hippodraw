// -*- mode:c++ -*-

/** @file

SIP interface definition for DisplayController

Copyright (C)  2002-2006  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: DisplayController.sip,v 1.38 2007/09/11 21:46:22 pfkeb Exp $

@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/

class DisplayController
{
%TypeHeaderCode
#include "controllers/DisplayController.h"
#include "datasrcs/NTuple.h"
#include "datareps/DataRepException.h"
#include "plotters/PlotterBase.h"
using hippodraw::DisplayController;
using hippodraw::DataSource;
using hippodraw::PlotterBase;
%End


private:

  DisplayController();

public:

  ~DisplayController ();
  static DisplayController * instance ();

  PlotterBase * createDisplay ( const std::string &, 
                                const DataSource &, 
                                const std::vector<std::string> & );

  PlotterBase * createDisplay ( const std::string & )
    throw ( DataRepException ) /Factory/;

  DataRep * addDataRep ( PlotterBase *, const std::string, 
			 const DataSource *, 
			 const std::vector < std::string > & );

  DataRep * addDataRepStacked ( PlotterBase *, const std::string, 
				const DataSource *, 
				const std::vector < std::string > & );

  int activeDataRepIndex ( const PlotterBase * ) const;
  void setActiveDataRepIndex ( PlotterBase *, int );

  void setPointRep ( PlotterBase *, const std::string );

  LineDataRep * addLineRep ( PlotterBase *, const std::string &, double );

  const std::vector < std::string > & getDisplayTypes () const;
  const std::vector < std::string > & getTextTypes () const;

  const std::vector < std::string > & getValueTransformTypes () const;
  void setValueTransform ( PlotterBase *, int );
  void setBinWidth ( PlotterBase *, const std::string &, double );
  void setLog ( PlotterBase *, const std::string &, bool );
  double getAverage ( const PlotterBase *, std::string &, int = 0 );
  double getRMS ( const PlotterBase *, std::string &, int = 0 );
  int getNumberOfEntries ( const PlotterBase *, int = 0 );

};
