// -*- mode:c++ -*- 

/** @file 

This is the SIP interface definition for PlotterBase

@note The commented out declarations require exposure of more classes
to SIP or are not needed by users.

Copyright (C)  2002-2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: PlotterBase.sip,v 1.43 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

namespace hippodraw {
//// class AxisModelBase;
class RepBase;
class ViewBase;
};

class PlotterBase : Observer, Observable
{
%TypeHeaderCode
#include "plotters/PlotterBase.h"
using hippodraw::ViewBase;
%End


public:
 /** The virtual destructor.  It is responsible for deleting the axis
      models. */
  virtual ~PlotterBase();

 /** Returns the name of the plotter.
  */
  const std::string & name () const;

  PlotterBase ( const std::string & );
  virtual PlotterBase * clone () = 0;

 /** Updates the plotter.  Updates the plotter by informing contained
      DataRep that an update is needed.  Does not notify its Oberver
      objects.
  */
  virtual void update () = 0;

  /** Sends notification to it's Observer
      ViewBase object that redrawing is needed.
  */
  virtual void update ( const Observable * );

 /** Sets the title to be displayed. The implementation is to pass
      the @a title parameter to the axes representation. */
  virtual void setTitle ( const std::string & );
 
 /** Gets the title to be displayed. */
  const std::string & getTitle() const;

  /** Sets the label of specified @a axis to @a label.  The default is
      to take the label from bound NTuple column.  Invoking this
      member function overrides that.  To return to taking the label
      from the NTuple column, @a label should be set to "%l". 

      @note This method is only used with the SIP based Python
      extension module.
  */
  void setLabel ( const std::string &, const std::string & );

// /** Sets the label of the specified @a axis to @a value.  The
//       implementation in this base class does nothing.
//    */
//   virtual void setLabel ( hippodraw::Axes::Type, const std::string & );

//   /** Returns the label for the specified axis.  The implementation in
//       this base class returns an empty string.
//   */
//   virtual const std::string & getLabel ( hippodraw::Axes::Type ) const;

//   /** Returns the internal label.  The internal label is either the
//       string set y a client, or one of @em %x, @em %y, or @em %z for
//       the corresponding axis.  The implementation in this base class
//       returns an empty string.
//   */
//  virtual const std::string &
//   getInternalLabel ( hippodraw::Axes::Type  ) const;

//   /** Sets the display of the error bars on or off. 
//       @todo Remove this function when no longer in use.
//   */
//   virtual void setErrorDisplay ( hippodraw::Axes::Type , bool ) = 0;

//   /** Returns the state of error bar display on specified @a axis. */
//   virtual bool errorDisplay ( hippodraw::Axes::Type  ) const = 0;

// /** @brief Sets the range on the specified axis.  

//       Sets the Range on the axis @a axis to @a range.  The
//       TransformBase object contained by this object may ajust the
//       range to fall within its limits.  If @a scaled is @c true, then
//       the Range object's value is a scaled range, otherwise it is the
//       true range.  if @a adjust_width is @c true, the bin width, if
//       applicable, is adjusts so one has the same number of bins after
//       the range is set, otherwise, the bin width is not changed.  The
//       implementation in this base class does nothing.

//       @bug Setting an range to 0, 100M on plot with empty NTuple causes
//       unknown exception.   Probably calculating too many bins.
  
//   */
//   virtual void setRange ( hippodraw::Axes::Type , const Range & ,
// 			  bool = false, bool = true );

  /** Sets the range on the specified axis.  Creates a Range object
      from the @a low and @a high arguments.  The Range is the scaled
      range.  It calls setRange ( hippodraw::Axes::Type, Range, bool )
      with third argument @c true. */
  void setRange ( const std::string &, double, double );

//  /** Returns the range on the specified axis.  Returns the range of
//       the data being plotted on the axis @a axis. If @a scaled is @c
//       true, uses the scaled range.  @sa AxisModelBase::getRange.  The
//       implemenation in this base class returns an empty Range.
//   */
//   virtual const Range & getRange ( hippodraw::Axes::Type , bool ) const;

//   /** Returns the smallest positive number from the data.   */
//   virtual double getPosRange ( hippodraw::Axes::Type ) const;

//   /** Sets the low end of the Range of data displayed.  The
//       implementation in this base class does nothing.
//    */
//   virtual void setLowRange ( hippodraw::Axes::Type , int , bool );

//   /** Sets the high end of the Range of data displayed.  The
//       implementation is this base class does nothing.
//    */
//   virtual void setHighRange ( hippodraw::Axes::Type , int , bool );


 /** Sets the number of bins.  Sets the number of bins of the plot.
      Not all plots have a bin attribute.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.
  */
  virtual void setNumberOfBins ( const std::string &, unsigned int );

// /** Sets the number of bins.  Sets the number of bins of the
//       contained ProjectorBase objects to @a number along the axis @a
//       axis.  The implementation in this class does nothing. @sa
//       ProbjectorBase::setNumberOfBins.
//    */
//   virtual void setNumberOfBins ( hippodraw::Axes::Type axis, 
// 				 unsigned int  );


//  /** Sets the offset.  Sets the edges of the bins to be offset from
//       its current value by the amount @a offset along the axis @a
//       axis. The implementation in this class does nothing. 
//   */
//   virtual void setOffset ( hippodraw::Axes::Type , double );

  /** Sets the bin offset.  Sets the bin offset parameter of the plot.
      Not all plots have a bin parameter.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.  The implementation is this base
      class does nothing.
  */
  virtual void setOffset ( const std::string & , double );

  virtual void setOffset ( const std::string, double );

  virtual void setOffsets ( double, double );

  /** Informs caller if the plotter prefers to use a pixmap or not.
      Returns @c true if the plotter might make good use of an off
      screen pix map, otherwise returns false.  If a plot contains
      thousands of points, then selection of the plot, or moving the
      plot on a canvas could be slow unless an off screen pixmap is
      used to flush previous drawing the the screen.  The default
      implementation in the base class is to return false.
  */
  virtual bool wantsPixmap ( ) const;


  /** Returns the low end of the X axis range.  
   */
  virtual double getLowRangeOnX ( );
  /** Returns the high end of the X axis range. 
      @deprecated This method is only used by the SIP built Python
      extension module and iskept only for running experiments.
   */
  virtual double getHighRangeOnX ( );

   /** Transpose the X and Y axis of matrix represention.  If X-Y
      represetnation of is matrix represented as a vector with shape,
      then transpose the X and Y axis.  Otherwise, do nothing.
      @note The implementation in this base class does nothing.
  */
  virtual void matrixTranspose ( bool );


//  /** Sets the bin width.  Sets the bin width of the contained
//       ProjectorBase objects to @a width along axis @a axis. The
//       implementation in this class does nothing.  The @sa
//       PrjectorBase::setBinWidth.
//   */
//   virtual void setBinWidth ( hippodraw::Axes::Type , double );

  /** Sets the bin width.  Sets the bin width parameter of the plot.
      Not all plots have a bin width parameter.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.  The implementation is this base
      class does nothing. 
  */
  virtual void setBinWidth ( const std::string &, double );

 /** Returns the bin width.  The implementation in the base class
      converts the string to an hippodraw::Axes::Type and calls the virtual
      function getBinWidth ( hippodraw::Axes::Type );.  It is declared as a
      convenience to the developer since bin width is a common
      attribute. */
  virtual double getBinWidth ( const std::string & );

//  /** Returns the bin width.  The implementation in the base class
//       does nothing.  It is declared as a convenience to the developer
//       since bin width is a common attribute. */
//   virtual double getBinWidth (hippodraw::Axes::Type ) const;

//  /** Returns the offset on designated axis, similarly to
//       getBinWidth. */
//   virtual double getOffset (hippodraw::Axes::Type ) const;

  /** Returns the offset on designated axis, similarly to
      getBinWidth. */
  double getOffset ( const std::string & ) const;

//   /** Returns the number of entries in the contained DataRep.
//   */
//   virtual int getNumOfEntries () const;

  /** Returns the number of entries in the contained DataRep.  

      @note This method is to workaround of SIP handling of virtual
      functions.  It calls the virtual function getNumberOfEntries().
    */
  int getNumOfEntries () const;


 /** Check if Axis needs to be scaled or not. 
   */
  virtual void checkAxisScaling ();

  /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
      to @a flag and auto-ranges all axis which has that status
      set. */
  virtual void setAutoRanging ( const std::string &, bool );

//   /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
//       to @a flag and auto-ranges all axis which has that status
//       set. The implementation in this base class does nothing.
//   */
//   virtual void setAutoRanging ( hippodraw::Axes::Type , bool );

  /** Sets the auto-ranging status  for all axes to @a flag.
   */
  virtual void setAutoRanging ( bool );


  /** This method declared pure virtual even thou it is not in C++ in
      order to trick SIP into calling the method via the virtual
      function table instead of calling the base class
      implementation. */
  virtual void addValues ( const std::vector < double > & ) = 0;

 /** Returns the number of DataRep objects contained in the plot.
      Implementation in this base return @c 0. */
  virtual int getNumDataReps () const = 0;
  /** Returns the specified DataRep or null pointer if it doesn't exits. */
  virtual DataRep * getDataRep ( int ) = 0;

  /** Removes the DataRep from the plot.  The base class
       implementation does nothing.

      @todo Remove this and the addDataRep
      member functions from this class. */
  virtual void removeDataRep ( DataRep * );

  /** Starts the drawing in view @c view.  This is the initial method
      that is called. */
  virtual void drawIn ( ViewBase *  ) = 0;

  /** Draws cross-hairs.  Draws cross-hairs at the mouse hot spot, if
      applicable.  The implementation in this base class does
      nothing.  Derived classes may override this function.
   */
  virtual void drawCrossHairs ( ViewBase * );

  /** Creates an NTuple representation of the plotted data.  Derived
      classes should create a representation appropriate for their
      plotting.  The %NTuple should contain the coordinates, the error
      on the coordinates, the values and the error on the values.

   */
  virtual NTuple * createNTuple () const = 0 /Factory/;
 
  /** Enables the Z axis, if plotter has one.  Implementation here
      does nothing.
  */
  virtual void setEnableZ ( bool = true );

//   /** Sets automatic generation of axis ticks.  This base class does
//       nothing.  Derived classes may interpret this function.
//    */
//   virtual void setAutoTicks ( hippodraw::Axes::Type , bool );
  
//   /** Sets the ticks for the specified axis.
//    */
//   virtual void setTicks ( hippodraw::Axes::Type , 
// 			  const std::vector < AxisTick > &  );

  /** Sets automatic ticks on or off.
   */
  virtual void setAutoTicks ( const std::string &, bool );

    /** Sets automatic generation of axis ticks on X axis.  This base
      class does nothing.  Derived classes may interpret this
      function.  

      @note This member function is temporary fix to memory
      leak problem in passing string with SIP.
   */
  virtual void setAutoTicksOnX ( bool );

    /** Sets the ticks for the X axis.  
      
      @note This method is provided to avoid interfacing the other one
      with the same name to Python.

      @note This method is temporary fix to memory leak problem with SIP.
   */
  virtual void setTicksOnX ( const std::vector < double > &,
			     const std::vector < std::string > & );

  /** Sets the ticks on specified axis.
   */
  virtual void setTicks ( const std::string & ,
			  const std::vector < double > & ,
			  const std::vector < std::string > &  );

  virtual void setNumberOfBins ( const std::string, unsigned int );
  

 /** Returns the aspect ratio.  The aspect ratio is the ratio of the
      width to the height that should be used when drawing the
      plotter.  If the aspect ratio has been set to a non-zero value,
      the return that ratio.  Otherwise, return the aspect ratio
      preferred by the contained TransformBase object.  The
      implementation in this base class always returns @c 0.0.
  */
  virtual double getAspectRatio () const;

  /** Sets the aspect ratio.  Set the ratio of the width to the height
      that should be used when drawing the plotter.  A value of 0.0
      indicates that any ratio is acceptable. 
  */
  void setAspectRatio ( double );

//  /** Returns the AxisModelBase derived class for the specified @a
//       axis. The implementation in this base class returns a null
//       pointer.
//   */
//   virtual AxisModelBase * getAxisModel ( hippodraw::Axes::Type  ) const;

 /** Adds a DataRep object to the plotter.  The implementation in
      this base class does nothing.  This function will be overridden
      by plotters that implement having DataRep objects. */
  virtual void addDataRep ( DataRep *  );


  /** Sets the active plot.  Redraws the plot if @a redraw is @c true. */
  virtual void setActivePlot ( int , bool );

  /** Returns the index of the active plotter.  Returns 0 if only one
      plot is in the display, or only one plot is supported. */
  virtual int activePlotIndex ( ) const;
  
  /** Returns the index of the parent datarep.  Returns -1 if there
      exist none */
  virtual int getParentDataRepIndex ( ) const;
  
  /** Sets the index of the parent datarep.*/
  virtual void setParentDataRepIndex ( int );

  /** Returns the parent plotter. Returns 0 if there exist none */
  virtual PlotterBase* getParentPlotter ( ) const;
  
  /** Sets the parent plotter.*/
  virtual void setParentPlotter( PlotterBase* );
  
  /** Resets the bins */
  virtual void reset ( );

//   /** Returns the active projector.  That is, the one that should
//       receive commands to it. */
//   virtual ProjectorBase * activeProjector () const;
  
  /** Returns the selected DataRep.  That is, the one that should
      receive commands to it. If multiple DataRep objects are
      selected, return a null pointer. */
  virtual DataRep * selectedDataRep () const;

//   /** Returns the projector at index @a i. 

//       @todo Remove this method when no longer used.
//    */
//   virtual ProjectorBase * getProjector (int ) const;

//  /** Returns the value to color represenatation.  If the Z value is
//       represented graphically, then return the object that controls
//       the binary value to graphic attribute.  This base class
//       implementation returns a null pointer.
//   */
//   virtual const BinToColor * getValueRep () const;

//   /** Sets the object that will do the value to color transform, if
//       any.  The implemenation is this base class does nothing.
//    */
//   virtual void setValueRep ( BinToColor *  );

//   /** Sets the transform object.  A copy of the transform object is
//       made by calling TransformBase#clone(), and the previous
//       transform object is deleted. By doing it this way, a controller
//       in an application does not need to keep track of how many
//       references to a particular transform object are active.  Derived
//       classes may override this function in order to check if the
//       concrete type of transform object is one appropriate for them.
//       @sa #m_transform.  The implementation in this base class does
//       nothing.
//   */
//   virtual void setTransform ( TransformBase * );

//   /** Returns the transform object used by the plotter. @sa
//       #m_transform.  The implementation in this base class returns a
//       null pointer.
//   */
//   virtual TransformBase * getTransform () const;

//   /** Sets the AxisModel on the specified axis. */
//   virtual void setAxisModel ( AxisModelBase * , hippodraw::Axes::Type ) = 0;

//   /** Returns @c true if auto-ranging is enabled, otherwise returns @c
//       false. The implemenation in this base class always returns
//       false.
//   */
//   virtual bool isAutoRanging ( hippodraw::Axes::Type  ) const;

  /** Auto scales the Range of the axes using preferred Range from the
      points plotters.  Sets the auto scaled flag to true.  The
      implementation in the base class does nothing.
  */
  virtual void autoScale();

//   /** Returns the data space rectangle. */
//   const HippoRectangle & getUserRect () const;

//   /** Returns the margin space where drawing takes place. */
//   HippoRectangle getMarginRect () const;

//  /** Converts from the drawing space to the data space. Converts the
//       drawing space coordinates @a mx and @a my, to the data space.
//       The results are return via @a ux and @a uy.  Applies the scale
//       factor of an axis if @a scaled is true.
//    */
//   virtual void marginToUserXY ( double , double , bool , 
// 				double & , double &  ) const = 0;

  /** Creates a NTuple for a PickTable.  The implemenation in this
      base class returns a null pointer.  Derived classes may override
      this member function to create a %PickTable appropriate for
      their class. 
  */
  virtual NTuple * createPickTuple () /Factory/;

//  /** Fills the picked data point.  Fills the vector @a picked with
//       the picked data set.  This method should be overriden by derived
//       classes that support picked data.  This base class
//       implementation does nothing.
//   */
//   virtual void fillPickedPoint ( double , double , 
// 				 std::vector < double > & ) const;

//   /** Converts the user Z coordinate into the margin color (X) coordinate. */
//   virtual float userToMarginColor ( double  ) const;

  /** Get the z value at the specified point (x,y).  Returns @c 0.0.
      Derived classes that support Z coordinates should override this
      method and eturns the z value of the point at @a x and @a y.  If
      @a scaled is @c true, returns the z value multiplied by the
      scale factor, otherwise returns the true value.
   */
  virtual double getZValue ( double , double , bool = true ) const;

//   /** Returns @c true if the plotter has an axis of specified type.
//       The implementation in this base class always returns #c false.
//   */
//   virtual bool hasAxis ( hippodraw::Axes::Type  ) const;

  /** Set the crossX value. */
  void setCrossX ( double  );

  /** Set the crossY value. */
  void setCrossY ( double  );

  /** Returns @c true if receiving object is targetable.  The plotter
      is targetable if one and only one targetable DataRep contained
      by the plotter is in selected state.  The implementation in this
      base class returns false because it never contains a targetable
      DataRep.
      @sa DataRep::isTargetable () const
  */
  virtual bool isTargetable () const;

  /** Returns the target DataRep contained by the plotter.  The
      implementation in the base class returns a null pointer because
      it never contains a targetable DataRep.
  */
  virtual DataRep * getTarget () const;

  /** Returns the index of the DataRep.  Returns the index of the
      DataRep @a rep if it is contained by the receiving object,
      otherwise returns -1.  This base class implementation always
      returns -1. .
  */
  virtual int indexOf ( const DataRep * ) const;

  /** Toggles the active DataRep, if 2 or more are contained by the
      plotter. Toggles the active plot.  With each call, the current
      active plot is deselected and the next one becomes the active
      plot.  When the end of the list of plots is reached, all plots
      become active.  If all plots are active then only the first
      becomes active.
  */
  void toggleActivePlot ();

  /** The unique ID number of this plotter. */
   virtual int plotterId () const;



// private to hide from Doxygen
private:
// Declare the pure virtual functions to keep SIP happy

//  /** Returns @c true if the plotter has NTuple binding, otherwise
//       returns false.
//   */
//   virtual bool hasNTupleBindings () const = 0;
  
  /** Sets the representation.  Derived classes must implement this
      member function.  Their interpretation of a representation
      varies.  For example, the representation could be a plotting
      symbol or a line.*/
  virtual void setRepresentation ( RepBase * ) = 0;

  /** Returns the representation used by the plotter. */
  virtual RepBase * representation ( ) const = 0;

  /** Sets the representation's color. */
  virtual void setRepColor ( const Color & ) = 0;

  /** Returns the color used for the representation. */
  virtual const Color & repColor () const = 0;

//   /** Converts the user X coordinate into the margin X coordinate. */
//   virtual float userToMarginX ( double  ) const = 0;

//   /** Converts the user Y coordinate into the margin Y coordinate. */
//   virtual float userToMarginY ( double  ) const = 0;

//   /** Converts the user Y coordinate into the top-left based margin Y
//       coordinate. */
//   virtual float userToInvertedMarginY ( double  ) const = 0;

//   virtual bool hasZAxis() const = 0;

};
