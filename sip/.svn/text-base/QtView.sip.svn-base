// -*- mode:c++ -*-

/** @file

SIP interface definition for QtView

@note The commented out code requires exposure to more types to SIP.

Copyright (C) 2003, 2004   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: QtView.sip,v 1.7 2007/02/24 22:24:38 pfkeb Exp $

 Author: Paul_Kunz@slac.stanford.edu

*/

class QtView : ViewBase, QCanvasRectangle
{
%TypeHeaderCode
#include "qt/QtView.h"
using hippodraw::QtView;
using hippodraw::PlotterBase;
using hippodraw::Observable;
using hippodraw::ViewBase;
%End

public:

  /** The constructor.  
   */
  QtView ( PlotterBase *  );


  /** The required virtual destructor. */
  virtual ~QtView ();

  /** Re-Implements the inherited function from QCanvasPoygonalItem.
      First calls base class to draw the contents.  If selected, then
      also draws the border and sends update message to m_inspector.
  */
  virtual void draw ( QPainter & );

  /** Overrides pure virtual function of QCanvasPolygonalItem. */
  virtual void drawShape ( QPainter & );

  /** Responds to Obvervable update message. If @a display is not
      null, responds to Obvervable update message by informing the
      QCanvas that its rectangluar has changed.
   */
  virtual void update ( const Observable * );

  /** Sets display status of cross-hairs.  Sets the display of
      cross-hairs at the previous defined position on if @a flag is @c
      true, otherwise does not display the cross-hairs.
      
      @sa setCrossX and setCrossY.
   */
  virtual void setShowCrossHairs ( bool );

//   /** Overrides virtual function of ViewBase. */
//   virtual HippoRectangle getDrawRect () const;

  /** Sets the drawing rectangle in the paint device space. */
  void setDrawRect ( const QRect &  );

  /** Sets the drawing rectangle in the paint device space. */
  virtual void setDrawRect ( float , float , float , float  );

//  virtual void initPlot ( double );
  
  /** Finalizes the plot view by unlocking the QApplication. */
//  virtual void endPlot ();

  /** Converts the data space coordinate X to the view space coordinate X. */
  int toViewX ( double  ) const;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  int toViewY ( double  ) const;

  virtual void fillPickedPoint ( double , double , 
				 std::vector < double > &  ) const;
  
  /** Set the page width. It is used for setting the dimensions of the
      draw rectangle */
  virtual void setPageWidth( int  );

private:

/** The copy constructor. */
  QtView ( const QtView & );
};
