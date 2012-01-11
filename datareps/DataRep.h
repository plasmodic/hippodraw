/* -*- mode: c++ -*- */

/** @file

hippodraw::DataRep class interface.

Copyright (C) 2001-2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: DataRep.h,v 1.229 2007/07/20 19:29:56 pfkeb Exp $

*/

#ifndef _DataRep_H_
#define _DataRep_H_

#include "pattern/Observable.h"
#include "pattern/Observer.h"

#include "axes/AxesType.h"
#include "axes/Range.h"

#ifdef _MSC_VER
#include "datasrcs/TupleCut.h"
#endif

#include "graphics/Color.h"

namespace hippodraw {

  class AxisModelBase;
  class Color;
  class CutRangeRep;
  class DataSource;
  class NTuple;
  class ProjectorBase;
  class RepBase;
  class TransformBase;
  class TupleCut;
  class ViewBase;

/** The base class for data representations.  Derived classes of
    DataRep are responsible for constructing a projector (derived from
    ProjectorBase) and a point representation (derived from
    PointRepBase) appropriate for their representations.  See 
    <a href="../../user/html/datareps_root.html">this page</a>
    for examples of concrete DataRep classes.

    Most of the implementation of classes derived from this base class
    is in the base class.  Derived classes consist mostly of a
    constructor and support for the point representation.

    If the DataRep object is one that is bound to a DataSource, then
    it becomes an Observer of its DataSource object.

    @bug @@@@@@ Function cut does not get restored from document.

    @request Be able to take difference between two data reps and plot
    it.

    @request @@@@@@ Be able to do Lego plots.

    @author Paul_Kunz@slac.stanford.edu 
    @author Sanket B. Malde <sanket@stanford.edu>
*/


class MDL_HIPPOPLOT_API DataRep
  : public Observable, public Observer
{

protected:

  /** The name of the display as it might be displayed in a GUI.  For
      example, the name might be "Histogram" or "ScatterPlot". */
  std::string m_name;

  /** The title of the object.  The title of the object that might be
      used to annotate the display of the object.  For example, if
      bound to DataSource, then the title of the DataSource object.
  */
  std::string m_title;

  /** A projector used to transform data to projected values that will
      be plotted. A display may have multiple projectors that make up
      its composite display.  This data member points to the one that
      should receive messages from the display.  Projectors are owned
      by the plotters that use them, and the projectors are responsible
      for deleting the plotters.  */
  ProjectorBase * m_projector;

  /** The representation used for drawing each data point in the view.
   */
  RepBase * m_rep;

  /** There are times when a given DataRep results from some operations
      performed on some other DataRep. The following variable contains pointer
      to the original data rep*/
  DataRep * m_parent_datarep;

  /** The representation of the cut or highlighted range.
   */
  CutRangeRep * m_cut_rep;

  /** The default constructor. */
  DataRep ( );

  /** The copy constructor. */
  DataRep ( const DataRep & );

  /** Draws the cuts.   If there are cuts, draws them.
   */
  void drawCuts ( TransformBase * transform, ViewBase * view );

  /** Throws exception with message saying argument is out of range.
   */
  void throwRangeException ();

 public:

  /** The destructor. */
  virtual ~DataRep();

  /** The clone function returns an object of its own kind which is a
      copy of this object. */
  virtual DataRep * clone () = 0;

  /** Returns the name of the display.   See m_name. */
  const std::string & name () const;

  /** Returns @c true if the data representation has specified axis
      type @a axis 

      @note This member function pure virtual to avoid
      problems with the SIP based Python extension module.  It throws
      an assertion if called.
   */
  virtual bool hasAxis ( Axes::Type axis ) const;

  /** Sets the title.  Sets the title of the object.  A title might be
      used to annotate the display of the object.  Setting the title
      to the string "%t" has the meaning of setting the object's
      default title.  The default title is the one returned by
      ProjectorBase::getTitle().
  */
  void setTitle ( const std::string & title );

  /** Returns the the current title of the object. 
   */
  const std::string & getTitle () const;

  /** Responds to update message from the data source. */
  virtual void update ( const Observable * );

  /** Returns @c true if the data representation is capable of error
      display on the data values.  The implementation in this base
      class returns @c false.  This member function should be
      overridden by derived classes capable of displaying errors on the
      data points.
  */
  virtual bool hasErrorDisplay ( ) const;

  /** Returns @c true if the %DataRep's projector can bind to DataSource.  The
      implementation in this base class is to return @c true. */
  virtual bool hasNTupleBindings () const;

  /** Sets the axis binding.  Sets binding of the axis with name @a
      axis to the DataSource column with label @a label.  If the derived
      class does not have %DataSource bindings, then does nothing.
      Derived classes may also do further processing upon reception
      of this method.
   */
  virtual void setAxisBinding ( const std::string & axis, 
				const std::string & label );

  /** Sets the axis bindings.  Sets the binding of all axis to the
      DataSource column with the labels in the vector.  If the derived
      class does not have %DataSource bindings, then does nothing.
      Derived classes may also do further processing upon reception
      of this method.
   */
  virtual
  void setAxisBindings ( const std::vector < std::string > & bindings );

  /** Transposes the X and Y axis.  If internal representation of the
      X-Y matrix is represented by a vector with shape, then transpose
      the X and Y axis.  Otherwise do nothing.

      @note The implementation passes the request to the contained
      projector.
  */
  virtual void matrixTranspose ( bool yes );

  /** Draws the projected values..  Each of the @ref
      overview_projectedvalue objects will be drawn in @a view after a
      coordinate transform represented by @a transform using its m_rep
      object.  Uses its @ref overview_projector object to access the
      projected values stored in a DataSource.  Some DataRep object may be
      dependent on the projected value objects are returned in a
      certain order.
  */
  virtual void drawProjectedValues ( TransformBase * transform, 
				     ViewBase * view );

  /** Returns the smallest positive number from in the range of the
      data. */
  double getPosRange ( Axes::Type axis );

  /** Returns the representation used to draw the data points. */
  RepBase * getRepresentation () const;

  /** Returns the color used for the representation. */
  const Color & getRepColor () const;

  /** Return @c true if the data representation has changed is some
      way and it needs to be re-drawn. */
  bool isDirty () const;

  /** Returns true if the error bars are being displayed. */
  bool isErrorDisplayed ( Axes::Type ) const;

  /** Returns the preferred range to display the data. 

  @todo In asking for the preferred Range, should also tell binning
  projectors to reset the number of bins so that auto ranging will
  lead to same results as when plot was initially created.
  */
  Range preferredRange ( Axes::Type axis ) const;

  /** Sets the AxisModel for axis @a axis. */
  void setAxisModel ( Axes::Type axis, AxisModelBase * );

  /** Sets the display of error bar, if applicable, on the specified
      @a axis. */
  void setErrorDisplay ( Axes::Type axis, bool );

  /** Sets the representation of the data points.  The @a pointrep
      replaces the existing one, and the existing one is then
      destroyed.  Maybe be overridden in derive class to do additional
      processing.
  */
  virtual void setPointRep ( RepBase * pointrep );

  /** Sets the color of the data representation. */
  void setRepColor ( const Color & );

  /** Sets the size of the representation. */
  void setRepSize ( double size );

  /** Sets the style type of the point representation.
   */
  void setRepStyle ( int style );


  /** Sets the dirty flag.  When set, the display may need to
      reinitialize itself before the next drawing. */
  virtual void setDirty ( bool yes = true );

  /** Sets the range of the @a axis.  The optional @a const_width
      parameter if @c true, tells binning projectors to hold a
      constant bin width parameter. */
  void setRange ( Axes::Type axis, bool const_width = true );


  /** Returns the Projector object controlling the data. */
  virtual ProjectorBase * getProjector ( ) const;

  /** Sets the data representation as selected.  Selection means the
      representations is drawn in its normal color, If representation
      is not selected, then it draws in a lighter shade.
  */
  virtual void setSelected ( bool yes = true );

  /** Returns @c true if data representation is in selected state.
      @sa setSelected
  */
  virtual bool isSelected ( ) const;

  /** Returns @c true if the DataRep accepts functions on @a number
      variables. The default implementation returns @c false.  Derived
      class should re-implement this member function if they accept
      functions of specified number of variables. 
  */
  virtual bool acceptFunction ( int number );

  /** Returns true if this DataRep has zero rows in its DataSource. Returns
      false if the projector is not an NTupleProjector. */
  virtual bool hasZeroRows ( );

  /** Returns true if this DataRep has zoom feature on the Y axis */
  
  virtual bool hasZoomY () const;


  /** Add the data values to the DataRep.  This virtual method is
      provided as a convenience for those derived classes that respond
      to it.  Nothing is done if this object has DataSource bindings.
      Otherwise the message is forward to its ProjectorBase object.
  */
  virtual void addValues ( const std::vector < double > & v );

  /** Returns an DataSource representation of the projected data.
   */
  virtual const DataSource * getProjectedValues () const;

   /** Returns the full NTuple but with the current set of cuts applied.
    */
   virtual NTuple * getNTupleAfterCuts () const;

   /** Fills a vector with named column data with the current set of cuts
       applied.
   */
   virtual void fillColumnAfterCuts(const std::string & column,
                                    std::vector<double> & columnData) const;

  /** Returns a NTuple that is a copy of the projector's DataSource
      with rows not passing region cuts removed.
   */
  NTuple * createNTupleUnderRegion () const;

  /** Returns the mean.  Returns the mean data points along the axis
      @a axis.
   */
  virtual double getMean ( const std::string & axis );

  /** Returns the RMS.  Returns the root mean square of the data
      points along the axis @a axis. 
  */
  virtual double getRMS ( const std::string & axis );

  /** Returns @c true if the specified axis is binned, otherwise returns
      @c false. 
  */
  bool isAxisBinned ( Axes::Type axis ) const;

  /** Gets the parent DataRep */
  virtual DataRep * getParentDataRep() const; 

  /** Sets the parent DataRep */
  virtual void setParentDataRep( DataRep * parent_datarep );

  /** Updates the data representation. Updates the data
      representation, however, derived classes are allowed to be lazy
      and not do the actual update until needed by
      drawProjectedValues.
  */
  virtual void update ();

  /** Normalizes the receiving object to the @a target one.
   */
  virtual void normalizeTo ( const DataRep * target );

  /** Sets the bin width, if binned, on specified @a axis to @a
      width.
  */
  const Range & setBinWidth ( Axes::Type axis, double width );

  /** Returns the number of entries used for the DataRep object.
   */
  int getNumberOfEntries () const;

  /** Returns @c true if the receiving object is target-able.  Most
      derived classes of DataRep are target-able and return @c true.
      That is, they can be targets of cuts, functions, etc.  If a
      derived class is not intended to be a target, it should return
      @a false.  The implementation in this base class returns @c
      true.
  */
  virtual bool isTargetable () const;

  /** Returns a new NTuple representation of the receiving object.
      The possession of the NTuple is taken by the calling object.
   */
  NTuple * createNTuple () const;

  /** Determines if extra space is needed for a color rep.
      @deprecated Use hasAxis() instead.
   */
  virtual bool needColorRep() const;

  /** Returns @c true if the receiving object has one or more regional
      cuts.
   */
  bool hasCut () const;

  /** Adds a cut to the data representation.
   */
  void addCut ( const TupleCut & cut );

  /** Adds a cut to the data representation.
   */
  void addCut ();

  /** Remove a region cut on the data representation.
   */
  void removeCut ();

  /** Returns a reference to the cut list.
   */
  const std::vector < TupleCut > & getCuts () const;

  /** Returns the point representation that draws the cuts.
   */
  RepBase * getCutRep ();

  /** Sets the Color of the PointRepBase object.
   */
  void set ( Color::Value value );

  /** Sets the Range of the TupleCut index by @a i.
   */
  void setCutRangeAt ( const Range & range, unsigned int i );

  /** Changes state of inversion flag to be the opposite of its
      current state.
  */
  void toggleInverted ( unsigned int i );

  /** Sets the TupleCut indexed by @a i to be enabled or not.
   */
  void setEnabled ( unsigned int i, bool yes = true );

  /** Returns @c true if this data representation needs an XY matrix
      parameters to be set in order to display itself.  An example
      would be the Image DataRep.  The implementation in this base
      class returns @c false.
   */
  virtual bool needsMatrixSet () const;

  /** Returns @c true if the receiving object can be converted to an
      image.  Returns @c true if this data representation can be
      converted to an image.  An image is defined as a regular grid of
      values.  
  */
  virtual bool isImageConvertable () const;

};

} // namespace hippodraw

#endif // _DataRep_H_
