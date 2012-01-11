/* -*- mode: c++ -*- */

/** @file

hippodraw::ProjectorBase class interface.

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: ProjectorBase.h,v 1.271 2006/10/22 00:20:27 pfkeb Exp $

*/

#ifndef _ProjectorBase_H_
#define _ProjectorBase_H_

#include "axes/AxesType.h"
#include "pattern/Observable.h"
#include "pattern/Observer.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <vector>

namespace hippodraw {

class AxisModelBase;
class BinsBase;
class DataSource;
class NTuple;
class Range;
class TupleCut;
class TransformBase;

/** The base class for the Projector hierarchy.  

    Derived classes create DataSource of projected values.  The format of
    the NTuple is either DataPoint2DTuple or DataPoint3DTuple.  There
    are multiple clients of NTuple including a derived class RepBase,
    the the function fitters.

    This base class also declares the virtual interface to inquiring
    about the range of the projected values which must be implemented
    in the derived classes.  The Projectors can find the range much more
    efficiently than clients using iterators to projected values.

    @author Stephane Bonneaud <gandalf@slac.stanford.edu>
    @author Xie Fang <xiefang@stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API ProjectorBase
  : public Observable, public Observer
{

private: 

  /** Marks the object as dirty.  A projection is marked dirty if some
      change was made that requires re-calculating the projection.
  */
  bool m_isDirty;

protected:

  /** The copy constructor. */
  ProjectorBase ( const ProjectorBase & projector );

  /** The NTuple representing the result of the projection.  
      
      @note An NTuple object is probably too heavy for non-binning
      projectors.  In the future, we'll create a class which behaves
      like one, but maps the data from the data source NTuple to
      required getRow().

   */
  DataSource * m_proj_values;

  /** The @c AxisModel along the X axis.  All @c Projectors have at
      least an X axis and maybe additional ones. 

  @todo Should be generalized to n-dimensions by having having a
  vector of AxisModelBase objects.
  */
  AxisModelBase * m_x_axis;

  /** The @c AxisModel along the Y axis.  All @c Projectors have at
      least a Y axis and maybe additional ones. */
  AxisModelBase * m_y_axis;

  /** The @c AxisModel along the Z axis.  May not be used by all
      Projectors. */
  AxisModelBase * m_z_axis;

  /** Dummy member so that getZLabel can return a reference.
      @todo Remove when getLabel takes axis argument
  */
  std::string m_z_label;

  /** Vector of acceptable PointReps. */
  std::vector < std::string > m_pointreps;

  /** The target projector.  Use of the target depends on derived
      classes.  Target projector are observed, thus some of the
      implement is define in this base class.
   */
  const ProjectorBase * m_target;

  /** Function to add the acceptable point reps. 
      @todo Why is this in the ProjectorBase and not DataRep ?
   */
  virtual void addPointReps () = 0;

  /** Sorted Z values in this projector. Used to export FITS file.
   */
  mutable std::vector < double > m_zval;


public:
  
  /** The default constructor. */
  ProjectorBase();

  /** The virtual destructor. */
  virtual ~ProjectorBase();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual ProjectorBase * clone() = 0;

  /** Sets the axis binding.  Sets binding of the axis with name @a
      axis to entity with name @a label.  It is expected that this
      member function will only be called on a derived class which can
      process it in a meaningful way. Thus if this method is called on
      this base class, an assertion is thrown.
   */
  virtual void setAxisBinding ( const std::string & axis,
				const std::string & label );

  /** Transposes the X and Y axis.  The implementation in this base
      class does nothing.
  */
  virtual void matrixTranspose ( bool yes );

  /** Sets the axis binding.  Sets binding of all axis entities with
      name in the vector.  This member function should only be called with
      a derived class which can process it in a meaningful way. Thus
      if this method is called on this base class, an assertion is
      thrown.
   */
  virtual 
  void setAxisBindings ( const std::vector < std::string > & bindings );

  /** Returns the axis bindings.  Returns the name associated with
      each axis. This member function should only be called with a
      derived class which can process it in a meaningful way.  thus if
      this method is called on this base class, an assertion is
      raised.
  */
  virtual const std::vector < std::string > & getAxisBindings () const;

  /** Returns @c true if the data source used by this projector is
      empty.  Returns @c true if the data source used by this
      projector is empty, otherwise returns @c true.  The data source
      is empty, i.e. has no data, then many member functions of this
      class can not respond with meaning full results.
  */
  virtual bool isEmpty () const = 0;

  /** Informs the projector to prepare its projected values for
      plotting.  This base class implementation sets the dirty flag to
      @c false indicating that the values are already prepared.
      Projectors that need to calculate the projected values should
      override this method and also set the dirty flag to @c
      false. For example, a projector might accumulate data into
      bins, and the position of the bin and its number of entries are
      used for the projected values.  */
  virtual void prepareValues ();

  /** Returns the range of the raw data.  */
  virtual Range dataRangeOn ( Axes::Type ) const = 0;

  /** Returns the preferred Range.  Returns the Range preferred by the
      projector. The default implementation in this base class is to
      return full range of the data on the axis.  Derived classes, may
      need to override this implementation. */
  virtual Range preferredRange ( Axes::Type ) const;

  /** Returns @c true if the projector has been marked dirty.  A dirty
      projector is one whose projected values are invalid because of
      some change (e.g., an axis range has changed.) */
  bool isDirty () const;

  /** Sets the dirty flag to @a value. */
  virtual void setDirty ( bool value = true );

  /** Returns the number of bins. Returns the number of bins on the
      specified axis.  The result is 0 if the axis is not binned. 
  */
  virtual int getNumberOfBins ( Axes::Type ) const;

  /** Sets the AxisModel for axis @a axis. */
  virtual void setAxisModel ( Axes::Type axis, AxisModelBase * );

  /** Get the AxisModel for axis @a axis. */
  virtual AxisModelBase * getAxisModel ( Axes::Type axis )const;

  /** Returns @c true if specified axis is binned.  An axis that is
      binned responds to the width and offset controls.  This base class
      implementation returns false. */
  virtual bool isAxisBinned ( const std::string & axis ) const;

  /** Returns @c true if the projected values are the result of
      binning.  The implementation in this base class returns @c
      false.  It maybe overridden in derived classes that do
      binning. */
  virtual bool isValueBinned () const;

  /** Finds the title of the plot. */
  virtual const std::string & getTitle() const = 0;

  /** Finds the range of the projected values. It is up to derived
      classes to determine what this means. 

      @todo Resolve if this method should be here.

  */
  virtual Range valueRange () const = 0;

  /** Returns the Range along the specified axis.  The behavior
      implemented here is to return the range of the AxisModelBase
      owned by the PlotterBase. However, derived classes may override
      this function, such as projectors that have binners, if this
      behavior is incorrect.  */
  virtual const Range & getRange ( Axes::Type ) const;

  /** Sets the range of the selected axis.  The base class
      implementation only sets the dirty flag.  Derived classes may
      need to do more.  The option @c bool parameter if true indicates
      a binning projector should hold the bin width parameter
      constant. */
  virtual void setRange ( Axes::Type axis, 
			  bool const_width = true );

  /** Returns the minimum positive value. 
  @todo Resolve if this method should be here. 
  */
  virtual double getPosOn ( Axes::Type ) const = 0;

  /** Sets the number of bins. Sets the number of bins along the axis
      @a axis to @a number.

      Not all projectors has a notation of a bin width.  This
      interface is declared for those that do.  The implementation in
      this base class is to does nothing.
  */
  virtual void setNumberOfBins ( Axes::Type axis, 
				 unsigned int number );

  /** Sets the bin width. Sets the bin width to @a width along the
      axis @a axis. 

      Not all projectors has a notation of a bin width.  This
      interface is declared for those that do.  The implementation in
      this base class is to does nothing.
  */
  virtual const Range &  setBinWidth ( Axes::Type axis,
				       double width );

  /** Sets the bin width.  This method is intended to be used in
      conjunction with a slider widget in a graphical user interface.
      The slider range is assumed to be 0 to 100 with the current
      value at @a parm.  If the slider is still being dragged, then @a
      dragging is @c true, otherwise it is false.  A derived class
      determines withs own bin width based on the width it had when
      slider dragging first started.  and the current position of the
      slider.   The implementation in this base class does nothing.
   */
  virtual const Range & setBinWidth ( Axes::Type axis, 
				      int parm, 
				      bool dragging );

  /** Sets the bin offset.  
      @sa setBinWidth ( const std::string &, int, bool ) 
  */
  virtual void setOffset ( const std::string & axis, 
			   int parm, 
			   bool dragging );

  /** Sets the bin offset.  Sets the offset of bins by fraction @a
      offset of the bin width. 

      Not all projectors has a notation of a bin width.  This
      interface is declared for those that do.  The implementation in
      this base class is to does nothing.
  */
  virtual void setOffset ( Axes::Type axis, double offset );


  /** Resets the bins. This interface is for the StHist1DProjector
   */

  virtual void reset ();


  /** Returns the offset of the axis. 

      Not all projectors has a notation of a bin width.  This
      interface is declared for those that do.  The implementation in
      this base class is to does nothing.
  */

  virtual double getOffset ( Axes::Type axis ) const;

  /** Returns the bin width of the axis. 

      Not all projectors has a notation of a bin width.  This
      interface is declared for those that do.  The implementation in
      this base class is to does nothing.
  */
  virtual double getBinWidth ( Axes::Type axis ) const;

  /** Finds the X axis label of the plot.  A plotter may ask the
      projector for the default label. */
  virtual const std::string & getXLabel() const = 0;

  /** Finds the Y axis label of the plot.  Derived classes should
      return a label for the Y axis that is meaningful for the kind of
      projection.  The argument @a flag can be used to distinguish
      between two possible labels, depending on some state of the
      projector.
   */
  virtual const std::string & getYLabel ( bool flag = false ) const = 0;

  /** Finds the Z axis label of the plot.  A plotter may ask the
      projector for the default label. */
  virtual const std::string & getZLabel() const;

  /** Returns the index of the @a label.  For example, the column
      index. */
  virtual int indexOf ( const std::string & label ) const;


  /** Returns the total number of entries that went into creating the
      projected values. It is up to derived classes to interpret what
      this means.  Some may find it meaningless and raise an exception
      and/or assertion.   */
  virtual int getNumberOfEntries () const = 0;

  /** Returns the number of underflow. */
  virtual int getUnderflow () const = 0;

  /** Returns the number of overflow. */
  virtual int getOverflow () const = 0;


  /** Get the Average of all projected values on the specified axis.
      @todo this should be computed externally with the iterators
   */
  virtual double getAverage(Axes::Type axis) const;

  /** Returns the RMS.  Returns the root mean square of the data
      points along the axis @a axis. 
  */
  virtual double getRMS ( Axes::Type axis );

  /** Get the z value at the specified point (x,y). */
  virtual double getZValue ( double x, double y ) const;

  /** Add values to the projection.  This method is provided as
      convenience for derived classes that respond to it.  The
      implementation in this class does nothing.
  */
  virtual void addValues ( const std::vector < double > & v );

  /** Return the vector of Point Reps. */
  const std::vector < std::string > & getPointReps() const;

  /** Creates an NTuple representation of the projected values.  Derived
      classes should create a representation appropriate for their
      projection.  They should at least include the coordinates, the
      width of the coordinates, the value and the error on the value.
   */
  virtual DataSource * createNTuple () const = 0;

  /** Returns DataSource representation of projected values. 
   */
  const DataSource * getProjectedValues () const;

  virtual const DataSource * createOldStyleNTuple () const;

   /** Template for projectors that can return subsets of the NTuple
       used by the DataRep. 
   */
   virtual NTuple * getNTupleAfterCuts () const;

   /** Fill the vector with values from named column with the present
       set of cuts applied.  This default implementation does nothing.
   */
   virtual void fillColumnAfterCuts(const std::string & column,
                                    std::vector<double> & columnData) const;

  /** Creates a NTuple from the projector's DataSource object using @a
      cut_list for not including rows that do not pass the cuts.
      @note This base class implementation returns a null pointer.
  */
  virtual 
  NTuple * createNTupleWith ( const std::vector< TupleCut > & cut_list ) const;

  /** Returns whether the projector wants scale factor.  Returns @c
      true if the projector wants to scale the @a axis, otherwise
      returns false.  A projector supporting a histogram, for example,
      would want to scale the Y axis by the bin width in order to show
      entries per bin instead of density.  This base class
      implementation always returns @c false.
   */
  virtual bool wantsScaleFactor ( const std::string & axis ) const;

  /** Sets the target of normalization.  The meaning of normalizing
      the projected values of one projector to another one depends on
      the derived class.  This implementation in this base does
      nothing.
   */
  virtual void normalizeTo ( const ProjectorBase * target );

  /** Sets a scale factor on the output so that the number of entries
      appears to be @a number.  This feature might be used, for
      example, to compare two histograms that have different number of
      entries by normalizing one to the other.
  */
  virtual void normalizeTo ( double norm );

  /** Sets the scaling number of entries on if @a on is @c true,
      otherwise turns if off.
  */
  virtual void setNormalizing ( bool on );

  /** Implements the Observer pattern.  The implementation in this
      abstract base class does nothing.  Derived class may override
      this method if they are observers.
   */
  virtual void update ( const Observable * );

  /** Checks for scaling the display of the axis range.  This base
      class implementation does nothing.  Derived classes should
      override this method if scaling of the axis is meaningful to
      them.
  */
  virtual void checkScaling ();


  /** Return all Z values.
   */
  const std::vector <double> & getZValues();

  /** Get the shape of the projected values. Used to export FITS file.
   */
  const std::vector <unsigned int> & getShape();

  /** Return Z values after binary transform.
   */
  const std::vector <double> & getZAfterTransform(TransformBase * transform);

  /** Returns @c true if projector has binding to a DataSource.  This
      base class implementation returns @c false.  Derived classes
      that have DataSource binding should override this member
      function and return @c true.
  */
  virtual bool hasDataSourceBindings () const;

  /** Set the minimum entries/bin.
   */
  virtual void setMinEntries( int entries );

  /** Get the minimum entries/bin.
   */
  virtual int getMinEntries ( );

  /** Returns @c true if the projected values can converted to an
      image.  Returns @c true if this projected values can be
      converted to an image.  An image is defined as a regular grid of
      values. As a convenience to implementers of derived classes,
      this base class method returns @a false.
  */
  virtual bool isImageConvertable () const;

};

} // namespace hippodraw

#endif // _ProjectorBase_H_
