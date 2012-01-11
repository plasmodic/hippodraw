/* -*- mode:c++ -*- */

/** @file

SIP interface definition for QtViewImp

@note The commented out code requires exposure of more types to SIP.

Copyright (C)  2003, 2005  The Board of Trustees of
The Leland Stanford Junior University.  All Rights Reserved.

$Id: QtViewImp.sip,v 1.12 2007/02/24 22:24:38 pfkeb Exp $

@author <Paul_Kunz@slac.stanford.edu>

*/

class QtViewImp : ViewBase
{
%TypeHeaderCode
#include "qt/QtViewImp.h"
using hippodraw::ViewBase;
%End

private:

  /** Converts coordinate from view space to canvas space. */
  virtual int toCanvasX ( double ) const = 0; // need at least one

  /** Converts coordinate from view space to canvas space. */
  virtual int toCanvasY ( double ) const = 0; 

  /** Converts the data space coordinate X to the view space coordinate X. */
  virtual int toViewX ( double ) const = 0;

  /** Converts the data space coordinate Y to the view space coordinate Y. */
  virtual int toViewY ( double ) const = 0;

public:

  /** The constructor.  
      @todo Lots more methods could be protected.
  */
  QtViewImp ( PlotterBase * );

   /** The copy constructor. */
  QtViewImp ( const QtViewImp & );


  /** Sets the receiver of update messages.  When this object receives
      update() from one of it Observable objects, it emits a
      customEvent message to this object, if one exists. 
  */
  void setInspector ( QObject * );

//   /** Returns the Rectangle within which points representations are
//       draw. The coordinate is shifted by position of view on
//       canvas. */
//   virtual HippoRectangle getMarginRect() const;

  /** Finalizes the plot view by unlocking the QApplication. */
//  virtual void endPlot ();

   /** Fills the picked point vector. Converts the view coordinates @a
      x and @a y to data coordinates.  The results are put in the
      vector @a picked.
   */
  virtual void fillPickedPoint ( double , double , 
				 std::vector < double > & ) const = 0;

  /** Converts the view space coordinate @a x expressed as a @c double
      to one expressed as an @c int. */
//   inline int toView ( double  ) const;
  int toView ( double  ) const;

//   virtual void drawLines ( const std::vector< double > & ,
// 			   const std::vector< double > & ,
// 			   hippodraw::Line::Style ,
// 			   const Color & ,
// 			   float  );

//   virtual void drawColorLines ( const std::vector< double > & ,
// 				const std::vector< double > & ,
// 				hippodraw::Line::Style ,
// 				const std::vector < Color > & ,
// 				float  );

//   virtual void drawViewLines ( const std::vector< double > & ,
// 			       const std::vector< double > & ,
// 			       hippodraw::Line::Style ,
// 			       bool ,
// 			       float  );

//   virtual void drawViewLines ( const std::vector< double > & ,
// 			       const std::vector< double > & ,
// 			       hippodraw::Line::Style ,
// 			       const Color & ,
// 			       float  );

//   virtual void drawPolyLine ( const std::vector< double > & ,
// 			      const std::vector< double > & , 
// 			      hippodraw::Line::Style ,
// 			      const Color & ,
// 			      float  );
  /** Set the crossX value. */
  void setCrossX ( double  );

  /** Set the crossY value. */
  void setCrossY ( double  );

  
  virtual void drawText ( const std::string &, float , float ,
			  float , float , char , char ,
			  bool );

//   virtual void drawText ( const std::string &, float , float ,
// 			  float , float , char , char ,
// 			  bool , const FontBase *  );

//   virtual void drawUserText ( const std::string &, float , float ,
// 			      float , float , char , char );

//  virtual void drawMag ( float , float , int , float );
//  virtual void drawMag ( float , float , int , float , QFont & );

//   virtual void drawPoints ( const std::vector<double> & ,
// 			    const std::vector<double> & ,
// 			    hippodraw::Symbol::Type , 
// 			    float  );

//   virtual void drawPoints ( const std::vector< double > & ,
// 			    const std::vector< double > & , 
// 			    const std::vector< Color > & ,
// 			    hippodraw::Symbol::Type , 
// 			    float  );

  virtual void drawSquare ( double , double , double , double ,
			    int , int , int );

  virtual void drawViewSquare ( float , float , float , float ,
				int , int , int  );

  float userToDrawX ( double ) const;
  float userToDrawY ( double ) const;
  float userToDrawColor ( double ) const;
  
  virtual void setDefaultFont( const QFont&  );
  
  virtual const QFont & defaultFont();

};
