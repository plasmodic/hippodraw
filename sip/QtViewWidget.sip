/* -*- mode:c++ -*- */

/** @file

This is the SIP interface definition for QtViewWidget

@note The commented out code requires exposure of more types to SIP.

Copyright (C)  2003, 2005  The Board of Trustees of
The Leland Stanford Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: QtViewWidget.sip,v 1.21 2007/02/24 22:24:38 pfkeb Exp $

@author Paul_Kunz@slac.stanford.edu

*/

class QtViewWidget : QWidget
{
%TypeHeaderCode
#include "qt/QtViewWidget.h"
using hippodraw::QtViewWidget;
using hippodraw::Observable;
using hippodraw::PlotterBase;
%End

public:

  /** The standard QWidget constructor.  See Qt documentation for
      details. */
  QtViewWidget ( QWidget * /TransferThis/ = 0,
	         const char * = 0, unsigned = 0 );


 /** The required virtual destructor. */
  virtual ~QtViewWidget ();

  /** Re-implements inherited member to test something for SIP.
      @attention Shouldn't really need to do this.  But if it isn't
      here, then we have to get SIP to accept the ViewBase class.  The
      problem with that is getting SIP to accept all the argument type
      of all the pure virtual functions.  Until that is done, this
      implementation gives a way out.*/
  void setPlotter ( PlotterBase * );


  virtual QSize sizeHint () const;

  /** Reimplements the inherited function.  First calls base class to
      draw the contents and then draws the border if selected. */
  virtual void draw ( QPainter & );

  /** Overrides virtual function of QWidget to do drawing. */
  virtual void paintEvent ( QPaintEvent * );

  virtual void update ( const Observable * );

//   /** Overrides virtual function of ViewBase. */
//   virtual HippoRectangle getDrawRect () const;

  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( const QRect &  );

  /** Sets the drawing rectangle in the paint device space. */
  virtual void setDrawRect ( float , float , float , float  );

  /** Initializes the plot view by locking the QApplication. */
//  virtual void initPlot ( double  );

  /** Finalizes the plot view by unlocking the QApplication. */
//  virtual void endPlot ();

  /** Converts the data space coordinate X to the view space coordinate X. */
  int toViewX ( double  ) const;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  int toViewY ( double  ) const;

  virtual void fillPickedPoint ( double , double , 
				 std::vector < double > &  ) const;

  virtual void setDoubleBuffering( unsigned );

  /** overrides the inherited virtual function.  If the event is a
      mouse event with control key down, then toggles the active
      DataRep.  Otherwise, passes passes the event to QObject.
  */
  virtual void mousePressEvent ( QMouseEvent * );



  void setInspector ( Inspector * );



private:

  QtViewWidget ( const QtViewWidget & );

};
