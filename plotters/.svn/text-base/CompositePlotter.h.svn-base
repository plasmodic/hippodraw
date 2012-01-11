/* -*- mode: c++ -*- */

/** @file

hippodraw::CompositePlotter class interface

Copyright (C) 2000-2007   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CompositePlotter.h,v 1.179 2007/09/06 19:45:41 pfkeb Exp $

*/

#ifndef _CompositePlotter_H_
#define _CompositePlotter_H_

#include "axes/AxesType.h"

#include <vector>

namespace hippodraw {

class AxisRepBase;
class AxisModelBase;
class AxisTick;
class BinToColor;
class DataRep;
class Color;
class NTuple;
class ProjectorBase;
class Range;
class Rect;
class RepBase;
class TransformBase;
class TupleCut;
class ViewBase;

/** A class for containing one or more DataRep objects in a single
    composite plot.  For example, an XyPlotter can have one DataRep
    for the data points and another for the function overlaid on
    them. One of the DataRep objects is considered @em active, as it is the one
    that will receive commands that make a change to its
    characteristics. 

    @request When 2 and only 2 DataRep objects are stacked, use right
    Y axis to show scale.

    @request Allow DataRep to be hidden.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API CompositePlotter
{

protected:

  /** The copy constructor. The copy constructor is not used.  Copying
      is done via XML serialization.
  */
  CompositePlotter( const CompositePlotter & plotter );

  /** The x dimension axis model.  In a composite display, a number of
      plotters can share the same axis model. The owner of the object,
      i.e. the one responsible for deleting it, is the plotter that
      created it. 
      
      @todo Generalize the containment of axis model to n-dimension.
  
  */
  AxisModelBase * m_x_axis;
  
  /** The y dimension axis model. In a composite display, a number of
      plotters can share the same axis model. The owner of the object,
      i.e. the one responsible for deleting it, is the plotter that
      created it.*/
  AxisModelBase * m_y_axis;
  
  /** The z dimension axis model. In a composite display, a number of
      plotters can share the same axis model. The owner of the object,
      i.e. the one responsible for deleting it, is the plotter that
      created it.*/
  AxisModelBase * m_z_axis;
  
  /** The label of the x axis is associated with the X axis model
      object. 
      
      @todo The axis labels should be a member of the axis model.
  */
  std::string m_x_label;
  
  /** The label of the y axis is associated with the Y axis model
      object. */
  std::string m_y_label;
  
  /** The label of the z axis is associated with the Z axis model
      object. */
  std::string m_z_label;
  
  /** The transform object.  It is responsible for transforming the
      original data coordinate system to another. */
  TransformBase * m_transform;

  /** The fits transform object.  It is useful when a fits image has
      periodical transform. */
  TransformBase * m_fits_transform;
  
  /** The index of the active DataRep or a negative value if more than
      one is active. 
  */
  int m_datarep_index;

  /** The type of container used to store the DataRep objects.
   */
  typedef std::vector < hippodraw::DataRep * > DataRepList_t;

  /** The list of the DataRep objects in the composite. */
  DataRepList_t m_datareps;

  /** A flag that when @c true indicates the plotter has already
      auto-scaled the axes.
  */
  bool m_has_autoscaled;

  /** A flag when set @c true indicates the plotter shows grid.
   */
  bool m_show_grid;

  /** A flag when set @c true indicates the box edge is show for color boxes.
   */
  bool m_box_edge;
  
  /** A flag which when set @c true indicates the plotter maintains a
      Z axis.
  */
  bool m_has_z;

  /** A flag which when set @c ture indicates the x axis is reversed.
   */
  bool m_reverse;

  /** Return @c true if any of the contained DataRep objects
      returns @c true from its isDirty() function. */
  bool isDirty () const;

  /** Get the z value at the specified point (x,y).  Returns the z
      value of the point at @a x and @a y.  If @a scaled is @c true,
      returns the z value multiplied by the scale factor, otherwise
      returns the true value.
   */
  virtual double getZValue ( double x, double y, bool scaled = true ) const;

  /** Adds the DataRep object to the composite if it is not already there. */
  void push_back ( hippodraw::DataRep * );

  virtual void setRangePrivate ( hippodraw::Axes::Type axis, 
				 const Range & range,
				 bool scaled = false, 
				 bool adjust_width = true );

public:

  /** The default constructor.
   */
  CompositePlotter ( );

  /** The destructor.
   */
  virtual ~CompositePlotter ();

  /** Makes a copy of the receiving object.
   */
  CompositePlotter * clone ();

  /** Sets the axis models for all of the components of the contained
      DataRep objects. This protected method should be called in the
      implementation of the copy constructor after the axis models
      have been created.
  */
  virtual void setAllAxisModels ();

  /** Checks if the plotter needs to be auto-scaled and do so if
      needed.   Returns @c true if it needed it.
  */
  bool  checkAutoScale ();

  /** Sets the Range on each axis that has the auto range flag set to
      include all the data.  This function scrolls through all the
      points plotters, asking for their preferred ranges.  It then
      sets the range to be the union of all of those.
  */
  virtual void autoScale ( );

  /** Sets the Range on the specified @a axis to include all the data.
   */
  void autoScale ( hippodraw::Axes::Type axis );

  /** Sets the range on the Z axis.
      @todo Determine why is Z axis so different from X and Y?
  */
  void autoScaleZ ();

  virtual void setRange ( hippodraw::Axes::Type, const Range &, 
			  bool scaled, bool adjust_width );

  virtual void setNumberOfBins ( hippodraw::Axes::Type axis, 
				 unsigned int number );

  /** Sets the representation. The representation of the active
      plotter is changed. */
  virtual void setRepresentation ( RepBase * pointrep );

  /** Returns the representation.  The data representation of the
      active plotter is returned. */
  virtual RepBase * representation ( ) const;

  virtual const BinToColor * getValueRep () const;

  /** Sets the object that will do the value to color transform, if
      any.
   */
  virtual void setValueRep ( BinToColor * rep );

  /** Returns the active projector. */
  virtual ProjectorBase * activeProjector () const;

  /** Returns the projector at index @a i. 
   */
  virtual ProjectorBase * getProjector (int i) const;

  /** Returns the number of DataRep objects in this plot. */
  virtual int getNumDataReps () const;

  /** Gets the parent DataRep object associated with the DataRep whose
      index number is index */
  virtual hippodraw::DataRep * getParentDataRep ( int index ) const;
  
  /** Gets the parent DataRep associated with the current/ active DataRep */
  virtual hippodraw::DataRep * getParentDataRep () const;
  
  /** Sets the parent DataRep associated with the DataRep whose
      index number is index */
  virtual void setParentDataRep ( int index, hippodraw::DataRep * parent );

  /** Sets the parent DataRep associated with the current/ active DataRep */
  virtual void setParentDataRep ( hippodraw::DataRep * parent );
  
  virtual hippodraw::DataRep * getDataRep ( int index ) const;

  /** Returns the selected DataRep if one is selected, otherwise
      returns the first one.
   */
  virtual hippodraw::DataRep * selectedDataRep () const;

  /** Returns the data rep under the mouse.
   */
  virtual hippodraw::DataRep * mouseSelectedDataRep () const;

  /** Adds the DataRep object to the composite.  Sends the
      DataRep::addingTo message to the @a rep.  Becomes an Observer
      of the DataRep object. */
  virtual void addDataRep ( hippodraw::DataRep * rep );

  /** Returns @c true if the selected DataRep object has DataSource
      bindings, otherwise returns @c false.
  */
  virtual bool hasNTupleBindings () const;

  /** Returns @c true if the plotter has zoom feature on the 
      Y Axis otherwise it returns false.
  */
  virtual bool hasZoomY () const;


  /** Sets the active DataRep object.  Sets the active DataRep object
      indexed by @a index.  Redraws if @a redraw is @c true.  If @a
      index is @c -1, then all the contained DataRep objects are set
      active.  If @a index is @c -2, then all contained DataRep
      objects are set inactive.
  */
  virtual int setActivePlot( int index, bool redraw );

  /** Returns the index of the active DataRep object. Returns the
      index of the active plotter. If returned value is -1, then all
      DataRep objects are active.  If the returned value is -2, then
      none are active.
  */
  virtual int activePlotIndex () const;

  /** Removes the DataRep from the composite.  If @a rep is contained
      by the plotter, then remove it, otherwise does nothing.*/
  virtual void removeDataRep ( hippodraw::DataRep * rep );

  /** Sets the axis model on the specified axis. Also, sets the same
      axis model to all of the contained plotters. */
  virtual void setAxisModel ( AxisModelBase * , hippodraw::Axes::Type );

  /** Sets the auto-ranging status  for all axes to @a flag.
   */
  virtual void setAutoRanging ( bool flag );

  /** Sets the show-grid status to @a flag.
   */
  virtual void setShowGrid ( bool flag );

  /** Gets the show-grid status to update the inspector.
   */
  virtual bool getShowGrid ();

  /** Sets the box edge status to @a flag.
   */
  virtual void setBoxEdge( bool flag );

  /** Gets the box edge status to update the inspector.
   */
  virtual bool getBoxEdge();

  /** Sets the display of the error bars on or off for the active
      plotter.
  */
  virtual void setErrorDisplay ( hippodraw::Axes::Type axis, bool );

  /** Returns the state of error bar display on specified @a axis for
      the active plotter.
  */
  virtual bool errorDisplay ( hippodraw::Axes::Type axis ) const;

  virtual void matrixTranspose ( bool yes );

  /** Sets the representation color of the active plotter.
   */
  virtual void setRepColor ( const Color & );

  /** Returns the color used for the data points from the active
      plotter.
  */
  virtual const Color & repColor () const;

  /** Returns the bin width on designated axis..  Returns the actual
      bin width if axis is binned and only one DataRep is in the
      display.  If multiple DataRep objects are in the display, then
      returns the bin width if all have the same width, or if only one
      is selected, otherwise returns -1.  If axis is not binned,
      returns 0.  This base class implementation returns 0.0 and
      should be overridden by derived classes. */
  virtual double getBinWidth (hippodraw::Axes::Type axis) const;

  virtual void setOffset ( hippodraw::Axes::Type axis, double offset );

  /** Sets the bin width.  Sets the bin width of the contained
      ProjectorBase objects to @a width along axis @a axis.
  */
  virtual void setBinWidth ( hippodraw::Axes::Type axis, double width );

  /** Resets bins */
  virtual void reset ();

  virtual double getOffset (hippodraw::Axes::Type axis) const;

  /** Returns the number of entries in the contained DataRep.  If
      multiple DataRep objects are active, returns 0, otherwise
      returns the number of entries of the elected DataRep.
  */
  virtual int getNumberOfEntries () const;

  /** Returns the smallest positive number from the data.   */
  virtual double getPosRange ( hippodraw::Axes::Type axis ) const;

  /** Sets the transform object.  A copy of the transform object is
      made by calling TransformBase#clone(), and the previous
      transform object is deleted. By doing it this way, a controller
      in an application does not need to keep track of how many
      references to a particular transform object are active.  Derived
      classes may override this function in order to check if the
      concrete type of transform object is one appropriate for them.
      @sa #m_transform.
  */
  virtual void setTransform ( TransformBase * );

  /** Set the fits transform object.*/
  virtual void setFitsTransform ( const std::string & );

  virtual void toUserXY ( double mx, double my, bool scaled, 
				double & ux, double & uy ) const;

  /** Creates a new NTuple from the selected DataRep object.  Creates and
      return a NTuple object from the selected DataRep if one is
      selected or only one exists.  Otherwise returns a null pointer.
   */
  virtual NTuple * createNTuple () const;

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
  virtual void fillPickedPoint ( double mx, double my, 
				 std::vector < double > & picked ) const;

  /** Process the return value for margin-to-user functions. */
  double processReturnValue ( double retval, 
			      hippodraw::Axes::Type axis, 
			      bool scaled ) const;

  /** Adds the values to the plot.  This method is provided as a convenience
      for the situation that only one DataRep exists and it responds
      to DataRep::addValues.  It calls this function on the first DataRep
      object.
  */
  virtual void addValues ( const std::vector < double > &  v );

  virtual void update ();

  /** Sets automatic generation of ticks on specified axis.
   */
  virtual void setAutoTicks ( hippodraw::Axes::Type axis, bool yes );

  virtual void setTicks ( hippodraw::Axes::Type axis, 
			  const std::vector < AxisTick > & ticks );

  virtual bool isTargetable () const;
  virtual hippodraw::DataRep * getTarget () const;
  virtual int indexOf ( const hippodraw::DataRep * rep ) const;
  virtual bool hasAxis ( hippodraw::Axes::Type axis ) const;

  /** Returns @c true if specified axis is scaled.
   */
  virtual bool isAxisScaled ( hippodraw::Axes::Type axis ) const;

  virtual AxisModelBase * getAxisModel ( hippodraw::Axes::Type axis ) const;

  /** Sets the auto-ranging.  Sets auto-ranging status for the @a axis
      to @a flag and auto-ranges all axis which has that status
      set.
  */
  virtual void setAutoRanging ( hippodraw::Axes::Type axis, bool flag );

  virtual bool isAutoRanging ( hippodraw::Axes::Type axis ) const;

  /** Sets the reverse status for the X axis to @a flag.
   */
  virtual void setReverse ( bool flag );

  virtual bool isReverse ( ) const;

  /** Sets the low end of the Range of data displayed.
   */
  virtual void setLowRange ( hippodraw::Axes::Type axis, int parm, 
			     bool dragging );

  /** Sets the high end of the Range of data displayed.
   */
  virtual void setHighRange ( hippodraw::Axes::Type axis, int parm, 
			      bool dragging );

  virtual const Range & getRange ( hippodraw::Axes::Type axis, 
				   bool scaled ) const;
  virtual void setScaleFactor ( hippodraw::Axes::Type  axis, double factor );

  /** Sets the scaling attribute of the AxisModelBase object.
   */
  virtual void setScaling ( hippodraw::Axes::Type  axis, bool on = true );

  /** Sets the title.  Sets the title of the active DataRep object.
      If multiple %DataRep objects are active, sets the title of the
      first one.
  */
  void setTitle ( const std::string & title );

  /** Returns the title.  Returns the title of the active DataRep
      object.  If multiple %DataRep objects are active, return the
      title of the first one.
  */
  const std::string & getTitle () const;

  virtual void setLabel ( hippodraw::Axes::Type, const std::string & value );
  virtual const std::string & getLabel ( hippodraw::Axes::Type ) const;
  virtual const std::string &
  getInternalLabel ( hippodraw::Axes::Type axis ) const;

  virtual TransformBase * getTransform () const;

  virtual TransformBase * getFitsTransform () const;

  /** Returns the aspect ratio.  The aspect ratio is the ratio of the
      width to the height that should be used when drawing the
      plotter.  If the aspect ratio has been set to a non-zero value,
      the return that ratio.  Otherwise, return the aspect ratio
      preferred by the contained TransformBase object.
  */
  virtual double getAspectRatio () const;

  /** Returns true if the axes have already been auto-scaled.
   */
  bool hasAutoScaled () const;

  /** Prepares the axes for drawing.
   */
  virtual void prepareToDraw ();

  /** Calculates the user rectangle given the current axis ranges.
   */
  Rect calcUserRectangle () const;

  /** Calculates the raw rectangle before transform.
   */
  Rect calcRawRectangle() const;

  /** Draws the projected values.  This implementation invokes @c
      drawProjValues on each of the DataRep objects in the composite.
      If multiple DataRep objects exist in the composite and one is
      selected (active), it is the one that is drawn last.
  */
  virtual void drawProjValues( ViewBase * view );

  /** Draws the data related to the axis representation.  Draw the
      data related to the axis representation.  If @a do_y is @c false
      do not do the Y axis ticks or labels.  If @a do_z is @c false,
      do not do the Z axis.
   */
  void drawAxisRep ( AxisRepBase * rep, ViewBase * view,
		     bool do_y, bool do_z );

  /** Check if Axis needs to be scaled or not. 
   */
  virtual void checkAxisScaling ();

  /** Enables the Z axis.
   */
  void setEnableZ ( bool yes );

  /** Sets the auto scaled flag.
   */
  void setAutoScaled ( bool flag = true );

  /** Auto scales the selected @a axis. */
  void autoScale ( AxisModelBase * model, hippodraw::Axes::Type axis );

  /** Fills the @a cuts vector with the TupleCuts contained by the
      DataRep objects.
  */
  void fillCutList ( std::vector < const TupleCut * > & cuts ) const;

  /** Sets the TupleCut range.  For the selected DataRep object, set
      its TupleCut object indexed by @a index Range to @a range.
  */
  virtual void setCutRangeAt ( const Range & range, unsigned int index );

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );


  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ();

  /** Returns @c true if plotter contents is convertable to image.
      Returns @c true if the plotter contains only one DataRep and
      that DataRep is convertable to an image.
  */
  bool isImageConvertable () const;

  /** Returns the scale factor used on specified axis.
   */
  double getScaleFactor ( Axes::Type axis ) const;


private:

  /** Set box edge status according to m_box_edge flag for the datarep.
      Works on when the rep of the data is ColorBoxPointRep class.
  */
  void toggleBoxEdge(DataRep* datarep);


};

} // namespace hippodraw

#endif // _CompositePlotter_H_
