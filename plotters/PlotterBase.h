/* -*- mode: c++ -*- */

/** @file
 
hippodraw::PlotterBase class interface.

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: PlotterBase.h,v 1.303 2007/09/06 19:45:42 pfkeb Exp $

*/

#ifndef _PlotterBase_H_
#define _PlotterBase_H_

#include "axes/AxesType.h"
#include "graphics/Rectangle.h"

#include "pattern/Observable.h"
#include "pattern/Observer.h"

namespace hippodraw {

class AxisModelBase;
class AxisTick;
class BinToColor;
class Color;
class DataRep;
class NTuple;
class ProjectorBase;
class Range;
class RepBase;
class TupleCut;
class TransformBase;
class ViewBase;
class FontBase;

/** The base class for the PlotterBase hierarchy.  Objects of this
    class are responsible for drawing the axis, titles, labels, etc.,
    but not the data points.  The class creates and maintains models
    for its axis which are of class AxisModelBase. 

    @request Make title of plot optional and not the default.  Use the
    space for plotting.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Jim Chiang < jchiang@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Joy Rajiv <jrajiv@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>

*/
class MDL_HIPPOPLOT_API PlotterBase :
  public Observer, public Observable
{

private:

  /** Sets the scaling attribute of the AxisModelBase object.  The
      implementation in this base class does nothing.
  */
  virtual void setScaling ( Axes::Type  axis, bool on = true );

   void setPlotterId();

protected:
  
  /** The aspect ratio.  The ratio of the width to the height that the
      should be used when drawing the plotter.  A ratio of zero, the
      default, indicates that any ratio is acceptable.
  */
  double m_aspect_ratio;
  
  /** The name of the type of plotter.  Used to lookup the plotter in
      a factory. */
  std::string m_name;
  
  /* The three following fields (m_title, m_x_label, m_y_label) have
      default values. The PlotterBase object gets them from the
      ProjectorBase object, and the ProjectorBase object gets them
      from the DataSource object. */
  
  /** The main label; the title of the plot. */
  std::string m_title;
  
  /** The x position of the cross hair. */
  double m_crossX;
  
  /** The y position of the cross hair. */
  double m_crossY;
  
  /** The abstract data space. In the user space X increases to the
      right and Y increases upward.  The coordinate system is that of
      the data points being plotting
  */
  Rect m_user_rect;

  /** The raw rectangle before transform.
   */
  Rect m_raw_rect;

  /** Parent plotter */
  PlotterBase* m_parent_plotter;
  
  /** Parent DataRep index */
  int m_parent_datarep_index;

   /** Default constructor needed to ensure m_plotterId is set. */
   PlotterBase ();

  /** A constructor taking name of type of plotter as argument. */
  PlotterBase ( const std::string & name );

  /** The copy constructor. */
  PlotterBase ( const PlotterBase & plotter );

  /** Returns true if the axes have already been auto-scaled. */
  bool hasAutoScaled () const;


  /** Returns true if the current range on the axes have been saved */
  bool current_range_saved;

   /** A unique plotter id.
    */
   int m_plotterId;

   /** A vector of x and y ranges to be used as a circular buffer allowing
       one to cycle through the saved plot "views". 
   */
   std::vector< std::vector<double> > m_views;

   /** Index of the current view */
   int m_currentView;

   /** Reference count for creating plotter ids */
   static int s_plotters;

public:

  /** The virtual destructor.  It is responsible for deleting the axis
      models. */
  virtual ~PlotterBase();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual PlotterBase * clone() = 0;

  /** Returns the name of the plotter.  @sa #m_name. */
  const std::string & name () const;

  /** Sets the scale factor on the specified axis.  The implementation
      in this base class does nothing.
   */
  virtual void setScaleFactor ( Axes::Type  axis, double factor );

  /** Returns @c false.   Should be overrided by derived class.
   */
  virtual bool isAxisScaled ( Axes::Type axis ) const;

  /** Sends notification to it's Observer
      ViewBase object that redrawing is needed.
  */
  virtual void  update ( const Observable * );

  /** Updates the plotter.  Updates the plotter by informing contained
      DataRep that an update is needed.  Does not notify its Observer
      objects.
  */
  virtual void update () = 0;

  /** Returns @c true if the plotter has DataSource binding, otherwise
      returns false.
  */
  virtual bool hasNTupleBindings () const = 0;


 /** Returns @c true if the plotter has zoom feature on the Y axis,
     otherwise returns false.
  */
  virtual bool hasZoomY () const ;

  /** Sets the bin width.  Sets the bin width of the contained
      ProjectorBase objects to @a width along axis @a axis. The
      implementation in this class does nothing.  The @sa
      PrjectorBase::setBinWidth.
  */
  virtual void setBinWidth ( Axes::Type axis, double width );

  /** Sets the bin width.  Sets the bin width parameter of the plot.
      Not all plots have a bin width parameter.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.  The implementation is this base
      class does nothing. 
  */
  virtual void setBinWidth ( const std::string & axis, double width );

  /** Sets the bin width according to parameter of dragging slider.
      Does nothing if more than one DataRep is active.
   */
  virtual void setBinWidth ( Axes::Type axis,
			     int parameter, bool dragging );

  /** Sets the offset.  Sets the edges of the bins to be offset from
      its current value by the amount @a offset along the axis @a
      axis. The implementation in this class does nothing. 
  */
  virtual void setOffset ( Axes::Type axis, double offset );

  /** Sets the bin offset.  Sets the bin offset parameter of the plot.
      Not all plots have a bin parameter.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.  The implementation is this base
      class does nothing.
  */
  virtual void setOffset ( const std::string & axis, double offset );

  /** Sets the number of bins.  Sets the number of bins of the plot.
      Not all plots have a bin attribute.  Since histogram-ming
      plots are commonly used, this method has been declared as a
      convenience to script writers.
  */
  virtual void setNumberOfBins ( const std::string & axis, 
				 unsigned int number );

  /** Sets the number of bins.  Sets the number of bins of the
      contained ProjectorBase objects to @a number along the axis @a
      axis.  The implementation in this class does nothing. @sa
      ProbjectorBase::setNumberOfBins.
   */
  virtual void setNumberOfBins ( Axes::Type axis, 
				 unsigned int number );

  /** Returns the AxisModelBase derived class for the specified @a
      axis. The implementation in this base class returns a null
      pointer.
  */
  virtual AxisModelBase * getAxisModel ( Axes::Type axis ) const;

  /** Adds a DataRep object to the plotter.  The implementation in
      this base class does nothing.  This function will be overridden
      by plotters that implement having DataRep objects. */
  virtual void addDataRep ( DataRep * rep );

  /** Sets the active plot.  Redraws the plot if @a redraw is @c true. */
  virtual void setActivePlot ( int index, bool redraw );

  /** Returns the index of the active plotter.  Returns 0 if only one
      plot is in the display, or only one plot is supported. */
  virtual int activePlotIndex ( ) const;
  
  /** Returns the index of the parent DataRep.  Returns -1 if there
      exist none */
  virtual int getParentDataRepIndex ( ) const;
  
  /** Sets the index of the parent DataRep.*/
  virtual void setParentDataRepIndex ( int index );

  /** Returns the parent plotter. Returns 0 if there exist none */
  virtual PlotterBase* getParentPlotter ( ) const;
  
  /** Sets the parent plotter.*/
  virtual void setParentPlotter( PlotterBase* plotter );
  
  /** Resets the bins */
  virtual void reset ( );

  /** Returns the active projector.  That is, the one that should
      receive commands to it. */
  virtual ProjectorBase * activeProjector () const;
  
  /** Returns the selected DataRep.  That is, the one that should
      receive commands to it. If multiple DataRep objects are
      selected, return a null pointer. */
  virtual DataRep * selectedDataRep () const;

  /** Returns the projector at index @a i. 

      @todo Remove this method when no longer used.
   */
  virtual ProjectorBase * getProjector (int i) const;

  /** Returns the number of DataRep objects contained in the plot.
      Implementation in this base return @c 0. */
  virtual int getNumDataReps() const;

  /** Returns the specified DataRep or null pointer if it doesn't exits. */
  virtual DataRep * getDataRep ( int index ) const;

  /** Removes the DataRep from the plot.  The base class
       implementation does nothing.

      @todo Remove this and the addDataRep
      member functions from this class. */
  virtual void removeDataRep ( DataRep * );

  /** Prepare the plotter for drawing.  The base class implementation
      does nothing.  Derived classes should do what is needed before
      drawing such as auto scaling. The ViewBase class may depend on
      the axis ranges being set.
   */
  virtual void prepareToDraw ();

  /** Starts the drawing in view @c view.  This is the initial method
      that is called. */
  virtual void drawIn ( ViewBase * view ) = 0;

  /** Draws cross-hairs.  Draws cross-hairs at the mouse hot spot, if
      applicable.  The implementation in this base class does
      nothing.  Derived classes may override this function.
   */
  virtual void drawCrossHairs ( ViewBase * view );

  /** Sets the representation.  Derived classes must implement this
      member function.  Their interpretation of a representation
      varies.  For example, the representation could be a plotting
      symbol or a line.
      
      @bug This method is confusing because programmer may not know if
      it means PointRep or DataRep.
  */
  virtual void setRepresentation ( RepBase * pointrep ) = 0;

  /** Returns the representation used by the plotter. */
  virtual RepBase * representation ( ) const = 0;

  /** Returns the value to color representation.  If the Z value is
      represented graphically, then return the object that controls
      the binary value to graphic attribute.  This base class
      implementation returns a null pointer.
  */
  virtual const BinToColor * getValueRep () const;

  /** Sets the object that will do the value to color transform, if
      any.  The implementation is this base class does nothing.
   */
  virtual void setValueRep ( BinToColor * rep );

  /** Sets the transform object.  A copy of the transform object is
      made by calling TransformBase#clone(), and the previous
      transform object is deleted. By doing it this way, a controller
      in an application does not need to keep track of how many
      references to a particular transform object are active.  Derived
      classes may override this function in order to check if the
      concrete type of transform object is one appropriate for them.
      The implementation in this base class does nothing.
  */
  virtual void setTransform ( TransformBase * );

  /** Set the fits transform object. It is created in the CompositePlotter
      class. Useful when a fits image has transform.
  */
  virtual void setFitsTransform ( const std::string & );

  /** Returns the transform object used by the plotter.  The
      implementation in this base class returns a null pointer.
  */
  virtual TransformBase * getTransform () const;

  /** Returns the fits transform object.
   */
  virtual TransformBase * getFitsTransform () const;

  /** Sets the AxisModel on the specified axis. */
  virtual void setAxisModel ( AxisModelBase * , Axes::Type ) = 0;

  /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
      to @a flag and auto-ranges all axis which has that status
      set. */
  virtual void setAutoRanging ( const std::string &  axis, bool flag );

  /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
      to @a flag and auto-ranges all axis which has that status
      set. The implementation in this base class does nothing.
  */
  virtual void setAutoRanging ( Axes::Type axis, bool flag );

  /** Sets the auto-ranging status  for all axes to @a flag.
   */
  virtual void setAutoRanging ( bool flag );

  /** Set the reverse status to @a flag.
   */
  virtual void setReverse ( bool flag );

  /** Sets the show-grid status to @a flag.
   */
  virtual void setShowGrid ( bool flag );

  /** Set the box-edge status to @a flag.
   */
  virtual void setBoxEdge (bool flag );

  /** Get the box-edge status to update the inspector.
   */
  virtual bool getBoxEdge ();

  /** Gets the show-grid status to update the inspector.
   */
  virtual bool getShowGrid ();

  /** Returns @c true if auto-ranging is enabled, otherwise returns @c
      false. The implementation in this base class always returns
      false.
  */
  virtual bool isAutoRanging ( Axes::Type axis ) const;

  /** Return @c true if reverse is enabled, otherwise returns @c false.
   */
  virtual bool isReverse () const;


  /** Auto scales the Range of the axes using preferred Range from the
      points plotters.  Sets the auto scaled flag to true.  The
      implementation in the base class does nothing.
  */
  virtual void autoScale();

  /** Sets the title to be displayed. The implementation is to pass
      the @a title parameter to the axes representation. */
  void setTitle( const std::string & title );

  /** Gets the title to be displayed. */
  const std::string & getTitle() const;

  /** Sets the label of specified @a axis to @a label.  The default is
      to take the label from bound DataSource column.  Invoking this
      member function overrides that.  To return to taking the label
      from the DataSource column, @a label should be set to "%l". 

      @note This method is only used with the SIP based Python
      extension module.
  */
  virtual void setLabel ( const std::string & axis, 
			  const std::string & label );

  /** Sets the label of the specified @a axis to @a value.  The
      implementation in this base class does nothing.
   */
  virtual void setLabel ( Axes::Type, const std::string & value );

  /** Returns the label for the specified axis.  The implementation in
      this base class returns an empty string.
  */
  virtual const std::string & getLabel ( Axes::Type ) const;


  /** Returns the internal title.  The internal title is either the
      title set by a client, or @em @%t.  The later ones mean that the
      object should return the corresponding title of the
      DataSource, if bound to one.
  */
  virtual const std::string &
  getInternalTitle ( ) const;

  /** Returns the internal label.  The internal label is either the
      label set by a client, or one of @em @%x, @em @%y, or @em @%z
      for the corresponding axis.  The later ones mean that the object
      should return the corresponding label of the DataSource. The
      implementation in this base class returns an empty string.
  */
  virtual const std::string &
  getInternalLabel ( Axes::Type axis ) const;

  /** Check if Axis needs to be scaled or not. 
   */
  virtual void checkAxisScaling ();

  /** Returns the bin width.  The implementation in the base class
      does nothing.  It is declared as a convenience to the developer
      since bin width is a common attribute. */
  virtual double getBinWidth (Axes::Type axis) const;

  /** Returns the bin width.  The implementation in the base class
      converts the string to an Axes::Type and calls the virtual
      function getBinWidth ( Axes::Type );.  It is declared as a
      convenience to the developer since bin width is a common
      attribute. */
  double getBinWidth ( const std::string & axis ) const;

  /** Returns the offset on designated axis, similarly to
      getBinWidth. */
  virtual double getOffset (Axes::Type axis) const;

  /** Returns the offset on designated axis, similarly to
      getBinWidth. */
  double getOffset ( const std::string & axis ) const;

  /** Returns the number of entries in the contained DataRep.
  */
  virtual int getNumberOfEntries () const;

  /** Returns the number of entries in the contained DataRep.  

      @note This method is to workaround of SIP handling of virtual
      functions.  It calls the virtual function getNumberOfEntries().
    */
  int getNumOfEntries () const;

  /** Sets the display of the error bars on or off. 
      @todo Remove this function when no longer in use.
  */
  virtual void setErrorDisplay ( Axes::Type axis, bool ) = 0;

  /** Returns the state of error bar display on specified @a axis. */
  virtual bool errorDisplay ( Axes::Type axis ) const = 0;

  /** @brief Sets the range on the specified axis.  

      Sets the Range on the axis @a axis to @a range.  The
      TransformBase object contained by this object may adjust the
      range to fall within its limits.  If @a scaled is @c true, then
      the Range object's value is a scaled range, otherwise it is the
      true range.  if @a adjust_width is @c true, the bin width, if
      applicable, is adjusts so one has the same number of bins after
      the range is set, otherwise, the bin width is not changed.  The
      implementation in this base class does nothing.

  */
  virtual void setRange ( Axes::Type axis, const Range & range,
			  bool scaled = false, bool adjust_width = true );

  /** Sets the range on the specified axis.  Creates a Range object
      from the @a low and @a high arguments.  The Range is the scaled
      range.  It calls setRange ( hippodraw::Axes::Type, Range, bool )
      with third argument @c true. */
  void setRange ( const std::string & axis, double low, double high );

  /** Returns the range on the specified axis.  Returns the range of
      the data being plotted on the axis @a axis. If @a scaled is @c
      true, uses the scaled range.  @sa AxisModelBase::getRange.  The
      implementation in this base class returns an empty Range.
  */
  virtual const Range & getRange ( Axes::Type axis, 
				   bool scaled ) const;

  /** Return the range of the raw data on the specified axis. */
  virtual Range getDataRange ( Axes::Type axis );

  /** Returns the smallest positive number from the data.   */
  virtual double getPosRange ( Axes::Type axis ) const;

  /** Sets the low end of the Range of data displayed.  The
      implementation in this base class does nothing.
   */
  virtual void setLowRange ( Axes::Type axis, int parm, 
			     bool dragging );

  /** Sets the high end of the Range of data displayed.  The
      implementation is this base class does nothing.
   */
  virtual void setHighRange ( Axes::Type axis, int parm, 
			      bool dragging );

  /** Sets the representation's color. */
  virtual void setRepColor ( const Color & ) = 0;

  /** Returns the color used for the representation. */
  virtual const Color & repColor () const = 0;

  /** Returns the data space rectangle. */
  const Rect & getUserRect () const;

  /** Returns the raw rectangle before transform. */
  const Rect & getRawRect () const;

  /** Creates a DataSource for a PickTable.  The implementation in this
      base class returns a null pointer.  Derived classes may override
      this member function to create a %PickTable appropriate for
      their class. 
  */
  virtual NTuple * createPickTuple ();

  /** Fills the picked data point.  Fills the vector @a picked with
      the picked data set.  This method should be overridden by derived
      classes that support picked data.  This base class
      implementation does nothing.
  */
  virtual void fillPickedPointFrom ( double mx, double my, 
				     std::vector < double > & picked ) const;

  /** Get the z value at the specified point (x,y).  Returns @c 0.0.
      Derived classes that support Z coordinates should override this
      method and returns the z value of the point at @a x and @a y.  If
      @a scaled is @c true, returns the z value multiplied by the
      scale factor, otherwise returns the true value.
   */
  virtual double getZValue ( double x, double y, bool scaled = true ) const;

  /** Returns @c true if the plotter has an axis of specified type.
      The implementation in this base class always returns @c false.
  */
  virtual bool hasAxis ( Axes::Type axis ) const;

  /** Set the crossX value. */
  void setCrossX ( double val );

  /** Set the crossY value. */
  void setCrossY ( double val );

  /** Adds data values to the plot.  This method is provided as a
      convenience when the plotter contains a single DataRep that
      responds to addValues.  For example a static histogram would
      respond to it.  The implementation here does nothing, so needs
      to be overridden by a derived class.
  */
  virtual void addValues ( const std::vector < double > & v );

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
  void setAspectRatio ( double ratio );

  /** Creates an DataSource representation of the plotted data.  Derived
      classes should create a representation appropriate for their
      plotting.  The %DataSource should contain the coordinates, the error
      on the coordinates, the values and the error on the values.

   */
  virtual NTuple * createNTuple () const = 0;

  /** Informs caller if the plotter prefers to use a pixmap or not.
      Returns @c true if the plotter might make good use of an off
      screen pix map, otherwise returns false.  If a plot contains
      thousands of points, then selection of the plot, or moving the
      plot on a canvas could be slow unless an off screen pixmap is
      used to flush previous drawing the the screen.  The default
      implementation in the base class is to return false.
  */
  virtual bool wantsPixmap ( ) const;

  /** Sets the offsets on the x and y axis */
  virtual void setOffsets( double xoffset, double yoffset );

  /** Enables the Z axis, if plotter has one.  Implementation here
      does nothing.
  */
  virtual void setEnableZ ( bool yes = true );

  /** Sets automatic generation of axis ticks.  This base class does
      nothing.  Derived classes may interpret this function.
   */
  virtual void setAutoTicks ( Axes::Type axis, bool yes );

  /** Sets the ticks for the specified axis.
   */
  virtual void setTicks ( Axes::Type axis, 
			  const std::vector < AxisTick > & ticks );

  /** Sets automatic ticks on or off.
   */
  virtual void setAutoTicks ( const std::string &, bool on );

  /** Sets automatic generation of axis ticks on X axis.  This base
      class does nothing.  Derived classes may interpret this
      function.  

      @note This member function is temporary fix to memory
      leak problem in passing string with SIP.
   */
  virtual void setAutoTicksOnX ( bool yes );

  /** Sets the ticks for the X axis.  
      
      @note This method is provided to avoid interfacing the other one
      with the same name to Python.

      @note This method is temporary fix to memory leak problem with SIP.
   */
  virtual void setTicksOnX ( const std::vector < double > & values,
			     const std::vector < std::string > & labels );

  /** Sets the ticks on specified axis.
   */
  virtual void setTicks ( const std::string & axis,
			  const std::vector < double > & values,
			  const std::vector < std::string > & labels );

  /** Returns the low end of the X axis range.  

   */
  virtual double getLowRangeOnX ( ) const;

  /** Returns the high end of the X axis range. 
      @deprecated This method is only used by the SIP built Python
      extension module and is kept only for running experiments.
   */
  virtual double getHighRangeOnX () const;

  /** Transpose the X and Y axis of matrix representation.  If X-Y
      representation of is matrix represented as a vector with shape,
      then transpose the X and Y axis.  Otherwise, do nothing.
      @note The implementation in this base class does nothing.
  */
  virtual void matrixTranspose ( bool yes );

  /** Returns @c true if receiving object is target-able.  The plotter
      is target-able if one and only one target-able DataRep contained
      by the plotter is in selected state.  The implementation in this
      base class returns false because it never contains a target-able
      DataRep.
      @sa DataRep::isTargetable () const
  */
  virtual bool isTargetable () const;

  /** Returns the target DataRep contained by the plotter.  The
      implementation in the base class returns a null pointer because
      it never contains a target-able DataRep.
  */
  virtual DataRep * getTarget () const;

  /** Returns the index of the DataRep.  Returns the index of the
      DataRep @a rep if it is contained by the receiving object,
      otherwise returns -1.  This base class implementation always
      returns -1. .
  */
  virtual int indexOf ( const DataRep * rep ) const;

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

   /** Save the current set of plot ranges which define a "view"
       for this plot.
       @return index of the saved view.
       @param range_values A vector of 4 elements containing the 
              x and y ranges.
    */
   int saveView ( const std::vector<double> & range_values );

  
   /** Save the current set of plot ranges which define a "view"
       for this plot.at the position i. The views from position
       i onward are deleted
       @return index of the saved view.
       @param range_values A vector of 4 elements containing the 
              x and y ranges.
       @param i the position at which range_values should be inserted at. 
    */
 int saveViewAtIndex(const std::vector<double>& range_values,unsigned int i ); 

   /** Set the view by @a index. */
   void setView ( int index );

   /** Cycle through each set of plot ranges, thereby changing
       the view with each call.
       @return The index of the current view.
       @param stepForward Direction to step through the views.
   */
   int nextView (bool stepForward=true);


   /** Return the number of stored views. */
   int numViews () const;

   /** Delete a view by @a index */
   void deleteView ( int index );

   /** Return the current view index. */
   int currentView () const;

  /** Fills the @a cuts vector with the TupleCut objects contained by
      the DataRep objects, if any.  This base class implementation
      does nothing.
  */
  virtual void fillCutList ( std::vector < const TupleCut * > & cuts ) const;

  /** Sets the Range of a TupleCut object.  Sets the Range of a
      TupleCut object, if any.  This base class implementation does
      nothing.
   */
  virtual void setCutRangeAt ( const Range & range, unsigned int i );

  /** Sets current_range_saved member to flag
   */
  virtual void setCurrentRangeSaved( bool flag );

   /** Returns current_range_saved member t
   */
  virtual bool getCurrentRangeSaved( );

  /** Returns all Z values. 
   */
  virtual const std::vector <double> & getZValues();
  virtual const std::vector <unsigned int> & getShape();

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  
  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ();

  /** What font is being used to override the default while drawing
      axis label. If return value is null pointer the default is being
      used. Base class returns NULL.
  */
  virtual FontBase* labelFont ( Axes::Type axes ) const;

  /** What font is being used to override the default while drawing title
      of plot. If return value is null pointer the default is being used.
      Base class returns NULL.
  */
  virtual const FontBase * titleFont ( ) const;

  /** Returns @c true if contents of the plotter is convertable to an
      image.  Returns @c true if the contents of the plotter can be
      taken as a regular grid of values, <em>i.e.</em> an image. As a
      convienence to implementers of derived classes, this base class
      method returns @c false.
  */
  virtual bool isImageConvertable () const;

  /** Return @c true if it's a text plotter. */
  virtual bool isTextPlotter() const;


  /** Set and get additional margin on top,bottom,left of the plotter */
  virtual void setTopMargin( double top );
  virtual void setBottomMargin( double bottom );
  virtual void setLeftMargin( double left );
  virtual void setZMargin( double z );
  virtual double getTopMargin();
  virtual double getBottomMargin();
  virtual double getLeftMargin();
  virtual double getZMargin();

  /** Set the m_need_update flag, called by inspector.
   */
  virtual void setNeedUpdate(bool isChanged);

  /** Returns 1.0.   This method should be overriden by a derived class.
   */
  virtual double getScaleFactor ( Axes::Type axis ) const;

};

} // namespace hippodraw

#endif // _PlotterBase_H_
