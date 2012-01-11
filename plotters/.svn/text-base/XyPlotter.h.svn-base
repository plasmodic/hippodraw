/* -*- mode: c++; -*- */
 
/** @file

hippodraw::XyPlotter class interface

Copyright (C) 2004, 2006-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: XyPlotter.h,v 1.105 2007/09/06 19:45:42 pfkeb Exp $

*/

#ifndef _XyPlotter_H_
#define _XyPlotter_H_

#include "PlotterBase.h"

namespace hippodraw {

class AxisRepBase;
class CompositePlotter;

/** A @c Plotter class that plots points in 2 dimensions and option a
    third dimension in color.  It draws the X and Y axis.  It is a
    CompositePlotter; therefore, it can plot multiple sets of data
    points in the same space.  It can also plot multiple functions and
    other kinds of representations.

    @bug @@@@ Removing stacked DataRep causes crash.

    @bug Need a way to show invalid range for static histograms.

    @todo When multiple DataRep objects are contained by the plotter,
    one could set the title from one of them.

    @author Kaustuv <kaustuv@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Joy Rajiv <jrajiv@stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
*/
class MDL_HIPPOPLOT_API XyPlotter : public PlotterBase
{
private:

  /** Additional margin set by user in the plotter */
  double m_margin_top;
  double m_margin_bottom;
  double m_margin_left;
  double m_margin_z;


  /** The type of standard C++ container to hold CompositePlotter
      objects.
  */
  typedef std::vector < CompositePlotter * > PlotterList_t;

  /** The list of CompositePlotter objects contained by this
      plotter.
  */
  PlotterList_t m_plotters;

  /** Index to the active plotter.
   */
  int m_active_index;

  /** Flag to indicate if the view size of the plot can be changed
   */
  bool view_change_allowed;

  /** Default shape for XyPlotter is 360*180 or 256*256. 
      Used for binary transforms to output a FITS file.
   */
  std::vector < unsigned int > m_periodic_shape;

  /** Checks if auto scaling is required and does an auto scale on
      contained plotters as needed.
  */
  void checkAutoScale ();

  /** Prepares for drawing.  Prepare for drawing by setting the axis
      ranges if necessary.
  */
  void prepareToDraw ();

  /** Flag to indicat that the drawrect and marginrect need to be updated.
      For now it's used only to response to font size changing. 
  */
  bool m_need_update;


protected:

  /** The currently active plotter that draws the data points.
   */
  CompositePlotter * m_plotter;

  /** The axes model graphical representation. If another plotter in a 
      composite is controlling the axis, then the pointer may be
      null.*/
  AxisRepBase * m_axis_rep;

  /** The conversion ratio from the user coordinate system to the view's
      coordinate system along the color axis. */
  double m_z_ratio;

  /** Returns the TupleCut object at position @a i.
   */
  TupleCut * getCutAt ( unsigned int i );

public:

  /** The default constructor. */
  XyPlotter (  const std::string & name  =  "XyPlotter" );

  /** The copy constructor. */
  XyPlotter ( const XyPlotter & plotter );

  /** The destructor. */
  virtual ~XyPlotter ();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  PlotterBase * clone();


  /** Sets the display of the Z axis on.
   */
  virtual void setEnableZ ( bool yes = true );

  virtual void drawIn ( ViewBase * view );

  /** Draws cross-hairs.  Draws cross-hairs at the mouse hot spot.  The
      drawing is done at the X and Y position set by setCrossX and
      setCrossY.
   */
  virtual void drawCrossHairs ( ViewBase * view );

  virtual NTuple * createNTuple () const;

  /** Returns @c true.  Since the data area will be filled, an off
      screen pixmap would be more efficient. */
  virtual bool wantsPixmap ( ) const;
  
  /** Set the font to be used to override the default while drawing
      axis label.
  */
  void setLabelFont( FontBase* font, Axes::Type axes );
  
  /** What font is being used to override the default while drawing
      axis label. If return value is null pointer the default is being
      used.
  */
  virtual FontBase* labelFont ( Axes::Type axes ) const;
  
  /** Set the font to be used to override the default while drawing title.*/
  void setTitleFont( FontBase* font );
  
  /** What font is being used to override the default while drawing title
      of plot. If return value is null pointer the default is being used.
  */
  virtual const FontBase * titleFont ( ) const;

  /** Sets the active plot.  If @a index is non-negative, sets the
      active plot, to the one indexed by @a index, while setting the
      others, if any, to inactive state.  If @a index is negative, set
      all plots to be active.  If @a redraw is @c true, then redraws
      all plots.
  */
  virtual void setActivePlot ( int index, bool redraw );

  /** Returns the index of the active DataRep object.
   */
  virtual int activePlotIndex () const;

  /** Adds the DataRep object to the plotter.  Adds the DataRep object
      to the plotter by sharing the current X and Y axes.
  */
  virtual void addDataRep ( DataRep * rep );

  /** Adds the DataRep object to the plotter.  Adds the DataRep object
      to the plotter by sharing the current X axis, but with its own Y
      axis.
  */
  virtual void addDataRepStacked ( DataRep * rep );

  /** Removes the DataRep object from the plotter.
   */
  virtual void removeDataRep ( DataRep * rep );

  /** Sets the Range on each axis that has the auto range flag set to
      include all the data.  This function scrolls through all the
      points plotters, asking for their preferred ranges.  It then
      sets the range to be the union of all of those.
  */
  virtual void autoScale ( );

  /** Sets the object that will do the value to color transform, if
      any.  Sets the object that will be used to do the value to color
      transform, if any, and redraws.
   */
  virtual void setValueRep ( BinToColor * rep );

  /** Sets the auto-ranging status for all axes to @a flag.  Sets the
      auto-ranging status for all axes to @a flag and re-draws.
   */
  virtual void setAutoRanging ( bool flag );

  /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
      to @a flag and auto-ranges all axis which has that status
      set.  Redraws the plot.
  */
  virtual void setAutoRanging ( Axes::Type axis, bool flag );

  /** Sets the reverse status for the X axis to @a flag. Redraws the plot.
   */
  virtual void setReverse ( bool flag );

  /** Sets the show-grid flag to @a flag.
   */
  virtual void setShowGrid ( bool flag );

  /** Gets the show-grid flag to update Inspector.
   */
  virtual bool getShowGrid ();

  /** Sets the box-edge flag to @a flag.
   */
  virtual void setBoxEdge(bool flag);

  /** Gets the box edge flag to update Inspector.
   */
  virtual bool getBoxEdge();
  
  /** Sets the transform object and redraws.
   */
  virtual void setTransform ( TransformBase * );

  /** Sets the fits transform object.
   */
  virtual void setFitsTransform ( const std::string & );

  /** Creates a NTuple for a PickTable.  Creates an NTuple appropriate
      for a %PickTable for this plotter.  For plotters derived from
      this class the first three column labels and subsequent labels
      are as follows

      @li "Item number" @li The X axis label.  @li The Y axis label.
      @li For 2D plots with Y axis scaling, the 4th label is "Density"
      @li For 3D plots the 4th label is the Z axis label.  
      @li For 3D plots with Z axis scaling, the 5th label is "Density"
      
      @sa fillPickTuple.
  */
  virtual NTuple * createPickTuple ();

  /** Fills the picked data point.  Fills the vector @a picked with
      the picked data set.  The vector will be cleared before
      insertion of coordinates.  The resulting size of the vector
      depends on the type of DataRep in use and the contents is as
      follows.

      @li 2D plots without Y axis scaling: 0., X, Y
      @li 2D plots with Y axis scaling: 1., X, Y-scaled, Y
      @li 3D plots without Z axis scaling: 2., X, Y, Z
      @li 3D plots with Z axis scaling: 3., X, Y, Z-scaled, Z

      The first item is used to identify the type of picked point.
      @sa createPickNTuple.
  */
  virtual void fillPickedPointFrom ( double mx, double my, 
				     std::vector < double > & picked ) const;

  /** Sets the low end of the Range of data displayed.
   */
  virtual void setLowRange ( Axes::Type axis, int parm, 
			     bool dragging );

  /** Sets the high end of the Range of data displayed.
   */
  virtual void setHighRange ( Axes::Type axis, int parm, 
			      bool dragging );

  /** Sets the scaling attribute of the AxisModelBase object.
   */
  virtual void setScaling ( Axes::Type  axis, bool on = true );

  /** Returns the aspect ratio.  The aspect ratio is the ratio of the
      width to the height that should be used when drawing the
      plotter.  If the aspect ratio has been set to a non-zero value,
      the return that ratio.  Otherwise, return the aspect ratio
      preferred by the contained TransformBase object.
  */
  virtual double getAspectRatio () const;

  virtual void update ();

  /** Sets the representation. The representation of the active
      plotter is changed.
  */
  virtual void setRepresentation ( RepBase * pointrep );

  /** Returns the representation used by the plotter.
   */
  virtual RepBase * representation ( ) const;

  /** Returns the AxisModelBase derived class for the specified @a
      axis.
  */
  virtual AxisModelBase * getAxisModel ( Axes::Type axis ) const;

  /** Sets the AxisModel on the specified axis. Also, sets the same
      AxisModel to all of the contained plotters.
  */
  virtual void setAxisModel ( AxisModelBase * , Axes::Type );

  /** Sets the display of the error bars on or off. 
  */
  virtual void setErrorDisplay ( Axes::Type axis, bool );

  /** Returns the state of error bar display on specified @a axis.
   */
  virtual bool errorDisplay ( Axes::Type axis ) const;

  /** Sets the representation's color.
   */
  virtual void setRepColor ( const Color & );

  /** Returns the color used for the representation.
   */
  virtual const Color & repColor () const;

  /** Draws the projected values.  This implementation invokes @c
      drawProjValues on each of the DataRep objects in the composite.
      If multiple DataRep objects exist in the composite and one is
      selected (active), it is the one that is drawn last.
  */
  virtual void drawProjValues ( ViewBase * view );

  /** Returns true if the axes have already been auto-scaled.
   */
  bool hasAutoScaled () const;

  /** Returns @c true if the selected DataRep object has
      bindings to a DataSource, otherwise returns @c false.
  */
  virtual bool hasNTupleBindings () const;

  /** Returns @c true if the plotter has zoom feature on the Y axis
      otherwise it returns false
  */
  virtual bool hasZoomY () const;

  /** Check if Axis needs to be scaled or not. 
   */
  virtual void checkAxisScaling ();

  /** Returns the i-th projector.
   */
  virtual ProjectorBase * getProjector ( int i ) const;

  /** Returns the active projector.
   */
  virtual ProjectorBase * activeProjector () const;

  /** Returns the DataRep object at @a index.
   */
  virtual DataRep * getDataRep ( int index ) const;

  /** Returns the selected DataRep.
   */
  virtual DataRep * getTarget ( ) const;

  /** Returns @c true if receiving object is target-able.
   */
  bool isTargetable () const;

  /** Returns the number of DataRep objects contained by the
      plotter.
  */
  virtual int getNumDataReps () const;

  /** Returns the index of the selected DataRep object.
   */
  virtual int indexOf ( const DataRep * ) const;

  /** Returns the bins with of the target DataRep object.
   */
  virtual double getBinWidth ( Axes::Type ) const;

  /** Sets the bin width.  Sets the bin width of the contained DataRep
      objects to @a width along axis @a axis.  If only one is active,
      then set the bin width only to that one.
  */
  virtual void setBinWidth ( Axes::Type axis, double width );

 /** Resets the bins   */
  virtual void reset ( );

  /** Returns the range on the specified axis.  Returns the range of
      the data being plotted on the axis @a axis. If @a scaled is @c
      true, uses the scaled range.  @sa AxisModelBase::getRange.
  */
  virtual const Range & getRange ( Axes::Type axis, 
				   bool scaled ) const;

  /** Returns the smallest positive number from the data.
   */
  virtual double getPosRange ( Axes::Type axis ) const;

  /** Returns the offset on designated axis, similarly to
      getBinWidth.
  */
  virtual double getOffset (Axes::Type axis) const;

  /** Returns the transform object used by the plotter.
   */
  virtual TransformBase * getTransform () const;

  /** Returns the fits transform object.
   */
  virtual TransformBase * getFitsTransform () const;

  /** Adds data values to the plot.  This method is provided as a
      convenience when the plotter contains a single DataRep that
      responds to addValues.  For example a static histogram would
      respond to it.
  */
  virtual void addValues ( const std::vector < double > & v );

  virtual const std::string &
  getInternalLabel ( Axes::Type axis ) const;

  /** Returns the label for the specified axis.
   */
  virtual const std::string & getLabel ( Axes::Type ) const;

  /** Sets the label of the specified @a axis to @a value.
   */
  virtual void setLabel ( Axes::Type, const std::string & value );

  /** Returns the number of entries in the contained DataRep.
   */
  virtual int getNumberOfEntries () const;

  /** Returns @c true if the plotter has an axis of specified type.
   */
  virtual bool hasAxis ( Axes::Type axis ) const;

  /** Returns the value to color representation.  If the Z value is
      represented graphically, then return the object that controls
      the binary value to graphic attribute.
  */
  virtual const BinToColor * getValueRep () const;

  /** Returns @c true if auto-ranging is enabled, otherwise returns @c
      false.
  */
  virtual bool isAutoRanging ( Axes::Type axis ) const;

  /** Returns @c true if reverse is enabled, otherwise returns @c false.
   */
  virtual bool isReverse ( ) const;

  /** Transpose the X and Y axis of matrix representation.  If X-Y
      representation of is matrix represented as a vector with shape,
      then transpose the X and Y axis.  Otherwise, do nothing.
  */
  virtual void matrixTranspose ( bool yes );

  /** Returns the selected DataRep.  That is, the one that should
      receive commands to it. If multiple DataRep objects are
      selected, return a null pointer. */
  virtual DataRep * selectedDataRep () const;

  /** Sets automatic generation of axis ticks.
   */
  virtual void setAutoTicks ( Axes::Type axis, bool yes );

  /** Sets the ticks for the specified axis.
   */
  virtual void setTicks ( Axes::Type axis, 
			  const std::vector < AxisTick > & ticks );

  /** Sets the number of bins.  Sets the number of bins of the
      contained ProjectorBase objects to @a number along the axis @a
      axis.
   */
  virtual void setNumberOfBins ( Axes::Type axis, 
				 unsigned int number );

  /** Sets the offset.  Sets the edges of the bins to be offset from
      its current value by the amount @a offset along the axis @a
      axis.
  */
  virtual void setOffset ( Axes::Type axis, double offset );

  /** @brief Sets the range on the specified axis.  

      Sets the Range on the axis @a axis to @a range.  The
      TransformBase object contained by this object may adjust the
      range to fall within its limits.  If @a scaled is @c true, then
      the Range object's value is a scaled range, otherwise it is the
      true range.  if @a adjust_width is @c true, the bin width, if
      applicable, is adjusts so one has the same number of bins after
      the range is set, otherwise, the bin width is not changed.
  */
  virtual void setRange ( Axes::Type axis, const Range & range,
			  bool scaled = false, bool adjust_width = true );

  /** Fills the @a cuts vector with the TupleCuts contained by the
      DataRep objects.
  */
  void fillCutList ( std::vector < const TupleCut * > & cuts ) const;

  /** Sets the Range of the TupleCut object indexed by @a i.
   */
  virtual void setCutRangeAt ( const Range & range, unsigned int i );
  
  /** Gets the object of current XyPlotter is a TupleCut object, get
      the range of it.
  */
  virtual Range getCutRange ();

  /** Sets the TupleCut object indexed by @a i to be inverted.
   */
  virtual void setCutInverted ( unsigned int i, bool yes = true );

  /** Sets the TupleCut object index by @a i to be enabled.
   */
  virtual void setCutEnabled ( unsigned int i, bool yes = true );


  const std::vector <double> & getZValues();
  const std::vector <unsigned int> & getShape();

  /** Get the inversion state of the cut. */
  virtual bool getCutInversion ();

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ();

  /** Set the m_need_update flag, called by inspector.
   */
  virtual void setNeedUpdate(bool isChanged);

  /** Returns @c true if contents of the plotter is convertable to an
      image.  Returns @c true if the plotter contains only one
      CompositePlotter and it is contents are convertable to an image.
  */
  virtual bool isImageConvertable () const;

  /** Set and get additional margin on top,bottom,left of the plotter */
  virtual void setTopMargin( double top );
  virtual void setBottomMargin( double bottom );
  virtual void setLeftMargin( double left );
  virtual void setZMargin( double z );

  virtual double getTopMargin();
  virtual double getBottomMargin();
  virtual double getLeftMargin();
  virtual double getZMargin();

  /** Sets a scale factor on specified axis.
   */
  virtual void setScaleFactor ( Axes::Type axis, double factor );  

  /** Returns the scale factor on the specified axis.
   */
  virtual double getScaleFactor ( Axes::Type axis ) const;

  /** Returns @c true if specified axis is being scaled.
   */
  virtual bool isAxisScaled ( Axes::Type axis ) const;

};

} // namespace hippodraw

#endif // _XyPlotter_H_
