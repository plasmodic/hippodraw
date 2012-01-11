/* -*- mode: c++ -*- */
/** @file 

DisplayController class interface declaration.

Copyright (C) 2001-2005, 2007  The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: DisplayController.h,v 1.245 2007/07/20 19:29:56 pfkeb Exp $

*/

#ifndef _DisplayController_H_
#define _DisplayController_H_

#include "axes/AxesType.h"

#ifdef _MSC_VER
#include <msdevstudio/MSconfig.h>
#endif

#include <list>
#include <vector>

namespace hippodraw {

class AxisModelBase;
class DataRep;
class DataSource;
class LineDataRep;
class NTuple;
class NTupleProjector;
class PlotterBase;
class ProjectorBase;
class Range;
class XYTransform;
class ViewBase;
class ViewFactory;

using std::vector;

/** A singleton class that is the interface between GUI and the
    displays.  It frequently implements the application logic which
    should neither be in the GUI nor in the display objects.

    @todo DisplayController shouldn't need to know about the
    DataRepFactory.  Should probably move methods that use it to
    DataRepController.
    
    @todo Shouldn't const_cast<BinToColor *> the return value from 
    getValueRep() of the plotter object in the setValueBrkPt member 
    function. A member function returning a non const pointer to a BinToColor 
    object should be implemented in the PlotterBase class and that should
    be used in the setValueBrkPt member function.
    
  

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@slac.stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/
class MDL_HIPPOPLOT_API DisplayController
{

private:

  /** The pointer to the singleton object. */
  static DisplayController * s_instance;

  /** A null string that is returned when reference to a valid string
      can not be found. 
  */
  std::string m_null_string;

  /** A empty vector of strings.  This vector is returned by member
      functions that can not obtain a reference from a display. */
  std::vector< std::string > m_null_vector;

  /** Returns a newly created PlotterBase object. The DataRep
      prototype, @a rep, is used to selected the kind of plotter
      required by the @a rep. */
  PlotterBase * createPlotter ( const hippodraw::DataRep * rep ) const;

  /** Fixes the @a rep so it displays well on @a plotter.  If the
      Plotter has an axis on the log scale, and if the corresponding
      axis of the DataRep is binned, then fix the DataRep so that
      binning is on a log scale.
   */
  void fixLogIfBinned ( const PlotterBase *, hippodraw::DataRep * rep ) const;

  /** Returns projector that supports binding, if display has one,
      otherwise returns null pointer. */
  NTupleProjector * getBindingProjector ( const PlotterBase * ) const;

  /** Returns the unique DataRep.  Returns a DataRep if only one of
      contained by the @a plotter has NTuple bindings or if only one
      is active, otherwise returns a null pointer.
   */
  hippodraw::DataRep * 
  getUniqueWithNTupleBindings ( const PlotterBase * plotter );

  /** Returns the currently selected projector in the plot. */
  ProjectorBase * getProjector ( const PlotterBase * display ) const;

  /** Returns @c true if adding a DataRep of type @a type to the
      PlotterBase @ plotter is compatible, otherwise returns false. */
  bool isCompatible ( const PlotterBase * plotter,
		      const std::string & type ) const;

  /** Sets the binner on the DataRep.  Sets the binner on the DataRep
      @a rep to agree with the axis scaling on @a plotter with respect to
      linear or logarithmic scale.  If the @a rep is not using a
      binning projector, or if the @a rep is not binned on @a axis,
      then this member function does nothing.
   */
  void setBinner ( const PlotterBase * plotter, 
		   DataRep * rep,
		   Axes::Type axis ) const;

  /** Sets the binner of of any Binning projectors to agree with AxisModel. */
  void setBinner ( PlotterBase * plotter,
		   Axes::Type axis );

  /** Sets the axis model to @a type.  The string @a type should be
      "Linear" or "Log", otherwise an assertion is raised.  If @a
      plotter doesn't have a axis model for @a axis, nothing is done.
   */
  void setAxisModel ( PlotterBase * plotter,
		      Axes::Type axis,
		      const std::string & type );

  /** set the axis to log or linear, without reseting the bin width.  */
  void setAxisModelWithoutSetBin ( PlotterBase * plotter,
		      Axes::Type axis,
		      const std::string & type );
  /** Sets the bin width parameter.  If a single DataRep is on the
      display, or if a single DataRep on a multiple DataRep display is
      active, the bin width for that DataRep is set.  If multiple
      DataRep objects are active, then each one has its bin width set.
      In the case of multiple DataRep objects width different bin
      widths, the default Y or Z axis label is set to "density",
      otherwise set to "entries / bin". 

  */
  void setBinWidth ( PlotterBase * plotter,
		     Axes::Type axis,
		     double width );

  /** A private copy constructor in order to avoid copying.  */
  DisplayController ( const DisplayController & );

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  DisplayController();

public:

  /** Returns the pointer to the singleton instance. */
  static DisplayController * instance ();

  /** The destructor. */
  ~DisplayController();

  /** Returns the name of the DataSource for the DataRep contained in
      the @a plotter at index @a index. 
  */
  const std::string & getDataSourceName ( const PlotterBase * plotter,
					  int index );

  /** Returns the types of displays available. */
  const std::vector < std::string > & getDisplayTypes () const;

  /** Sets the value to color transform model.
   */
  void setValueTransform ( PlotterBase * plotter, const std::string & name );

  /** Sets a new value transform for the plotter.
   */
  void setValueTransform ( PlotterBase * plotter, int index );
   
  /** Creates and adds to the factory a new value transform.
   */
  void addValueTransform ( const std::string & name,
			   const std::vector < double > & ctrl_points );
  
  /** Removes the value to color transform model.  If the named value
      to color transform model is of typed user defined, then remove
      it from the corresponding factory.  Returns @ true if it was
      removed, otherwise returns false.
  */
  bool removeValueTransform ( const std::string & name );
  
  /** Save the updated control points of a BinToColor transform. 
      In this method, we remove and then add the updated transform. 
      This will make the changes effective immediatly.  
  */
  void saveValueCtrlPts( PlotterBase *plotter,
			 const std::vector <double> & sv);

 
  /** Returns the control points of the value to color transform
      model.
   */
  const std::vector <double> &  getValueCtrlPts ( const PlotterBase * plotter);
 
   /** Sets the control points for the value to color transform model.
   */
  void setValueCtrlPts ( PlotterBase * plotter,
			 const std::vector < double > &  sv);
 
 
  /** Returns the index to list of value to color transform names of
      the plotter.
   */
  int getValueTransformIndex ( PlotterBase * plotter );

  /** Returns the types of value to color transforms available.
   */
  const std::vector < std::string > & getValueTransformTypes () const;

  /** Returns @c true if value to color transform has control points.
      If PointRepBase object is one that transforms value to color and
      if the value to color transform can be varied, then returns @c
      true, otherwise returns @c false.
  */
  bool hasControlPoints ( const PlotterBase * plotter ) const;

  /** Returns @c true if value to color transform is user defined.
   */
  bool isUserDefinedValueTransform ( const PlotterBase * plotter ) const;

  /** Returns the types of text displays. */
  const std::vector < std::string > & getTextTypes () const;

  /** Creates a display object.  A Display of class @a name is
      created.  It should be a class that is not bound to an
      NTuple. Returns a derived class of PlotterBase appropriate for
      the newly created data rep.  If the class @a name could not be
      found, an exception is thrown.
  */
  PlotterBase * 
  createDisplay ( const std::string & name );

  /** Creates a display object.  A Display of class @a rep  is
      used.  It should be a class that is not bound to an
      NTuple. Returns a derived class of PlotterBase appropriate for
      the newly created data rep.  If the class @a name could not be
      found, an exception is thrown.
  */
  PlotterBase * createDisplay ( hippodraw::DataRep * rep );

  /** Creates a display object.  A DataRep of class @a name is
      created.  The data rep is bound to NTuple @a tuple and the
      binding set by @a bindings.  Returns a derived class of
      PlotterBase appropriate for the newly created data rep.  If the
      class @a name could not be found, an exception is thrown.
  */
  PlotterBase * 
  createDisplay ( const std::string & name,
		  const DataSource & tuple,
		  const std::vector< std::string > & bindings ) const;

  NTuple *
  createNTupleDiff ( PlotterBase * plotter );

  /** Returns difference display.  Creates and returns a PlotterBase
      object displaying the difference of two histogram.
  */
  PlotterBase * createDifferenceDisplay ( PlotterBase * plotter ) ;

  /** Returns the type of DataRep contained in the @a plotter at
      index @a index */
  const std::string & getType ( const PlotterBase * plotter,
				int index ) const;

  /** Returns the number of entries.  Returns the number of entries of
      the Data Rep indexed by @a index contain in the @a plotter. If
      the @a index is out of range, an assertion is raised.
  */
  int getNumberOfEntries ( const PlotterBase * plotter, int index = 0 ) const;

  /** Returns the number of underflow. If the @a index is out of range, 
      an assertion is raised.
   */
  int getUnderflow ( const PlotterBase * plotter, int index = 0 ) const;

  /** Returns the number of overflow. If the @a index is out of range, 
      an assertion is raised.
   */
  int getOverflow ( const PlotterBase * plotter, int index = 0 ) const;
  
  /** Returns the average value.  Returns the average value for the
      DataRep indexed by @a index contained in the @a plotter.  The
      average is taken along the axis @a axis.  If @a index is out of
      range, raises an assertion.
  */
  double getAverage ( const PlotterBase * plotter, 
		      const std::string & axis, int index = 0 ) const;

  /** Returns the average value.  Returns the average value for the
      DataRep indexed by @a index contained in the @a plotter.  The
      average is taken along the axis @a axis.  If @a index is out of
      range, raises an assertion.
  */
  double getAverage ( const PlotterBase * plotter, 
		      Axes::Type axis, int index = 0 ) const;

  /** Returns the root mean square along the specified @a axis 
      for DataRep indexed by @a index. 
      @bug Only works for the X axis. */
  double getRMS ( const PlotterBase * plotter, 
		  const std::string & axis, int index = 0 ) const;

  /** Returns the root mean square along the specified @a axis
      for DataRep indexed by @a index.
      @bug Only works for the X axis. */
  double getRMS ( const PlotterBase * plotter, 
		  Axes::Type axis, int index = 0 ) const;

  /** Sets the type of RepBase object to be used by the selected
      DataRep. Sets the type of RepBase object to be used by the
      selected DataRep.  The type is specified by the string @a
      point_rep which should be one of the types available from the
      PointRepFactory.  If @a point_rep is not found, the factory will
      throw an FactoryException.  Preserves the Color of the previous
      point representation on the new one.
  */
  void setPointRep ( PlotterBase * plotter,
		     const std::string & point_rep );

  /** Returns the style of the point representation.
   */
  unsigned int getRepStyle ( const PlotterBase * plotter ) const;

  /** Adds the DataRep to the PlotterBase object by including it.
   */
  void addDataRep ( PlotterBase *, hippodraw::DataRep * ) const;

  /** Adds the DataRep to the PlotterBase object by stacking it.
   */
  void stackDataRep ( PlotterBase *, hippodraw::DataRep * ) const;

  /** Creates a new DataRep object with class name @a name and adds it
      to the existing @a plotter. Connects the newly created DataRep
      to @a source and sets the bindings to @a bindings.  Returns
      DataRep if successful, otherwise returns a null pointer if the
      class @a name could not be found.  @note This method is used by
      the Inspector.
  */
  hippodraw::DataRep *
  addDataRep ( PlotterBase * plotter,
	       const std::string & name,
	       const DataSource * source,
	       const std::vector< std::string > & bindings ) const;

  /** Creates a new DataRep object with class name @a name and adds it
      to the existing @a plotter by stacking it. Connects the newly
      created DataRep to @a source and sets the bindings to @a
      bindings.  Returns DataRep if successful, otherwise returns a
      null pointer if the class @a name could not be found.  @note
      This method is used by the Inspector.
  */
  hippodraw::DataRep * 
  addDataRepStacked ( PlotterBase * plotter,
		      const std::string & name,
		      const DataSource * source,
		      const std::vector < std::string > & bindings ) const;

  /** Adds a LineDataRep to the Plotter object.
   */
  LineDataRep * addLineRep ( PlotterBase *, 
			     const std::string & axis, 
			     double value );

  /** Adds a LineRep to the Plotter object.
   */
  LineDataRep * addLineRep ( PlotterBase *, 
			     Axes::Type, 
			     double value );

  /** Set the error display.  For the active DataRep contained by @a
      plotter, sets its error display on the axis @a axis to on, if @a
      state is @c true, otherwise turns it off.
  */
  void setErrorDisplayed ( const PlotterBase *, 
			   Axes::Type axis, bool state ) const;

  /** Adds views from @a source to @a destination if they contain
      TextRep whose target is contained in @a destination.
  */
  void addTextViewToList ( std::vector < const ViewBase * > & destination,
			   const std::vector < const ViewBase * > & source );

  /** Removes TextRep Observer objects, if any, from its target. */
  void removeTextObservers ( const std::vector < const ViewBase * > & views );

  /** Creates an object of a class derived from DataRep which is
      textual data representation of type @a type. */
  hippodraw::DataRep *
  createTextDataRep ( const std::string & type,
		      const std::string & text = std::string() );
  
  /** Creates an object which is a derived class of ViewBase. The
      ViewFactory @a factory is used for its creation.  A TextRepBase
      object corresponding to @a name is created for use by the view.
      The target of the TextRepBase object is @a rep. The @a text is
      displayed if not null. */
  ViewBase * createTextView ( const ViewFactory * factory,
			      hippodraw::DataRep * rep,
			      const std::string & name,
			      const std::string & text = std::string () );

  /** Creates an object which is a derived class of ViewBase. The
      ViewFactory @a factory is used for its creation.  A TextRepBase
      object corresponding to @a name is created for use by the view.
      The view is made an observer of @a plotter. The @a text is
      displayed if not null.
  */
  ViewBase * createTextView ( const ViewFactory * factory,
			      PlotterBase * plotter,
			      const std::string & name,
			      const std::string & text = 0);

  /** Creates a plotter object for the @a DataRep with text
      representation of type @a name.  Uses the optional @a text if
      needed by the text representation. */
  PlotterBase * createTextPlotter ( hippodraw::DataRep * datarep,
				    const std::string & name,
				    const std::string & text = std::string() );
  
  /** Returns the active DataRep index for the DataRep objects
      contained in the @a plotter.  If multiple DataRep are active,
      then returns the index of non FunctionRep if there is only one,
      otherwise returns -1. */
  int activeDataRepIndex ( const PlotterBase * plotter ) const;

  /** Returns the active DataRep object for the DataRep objects
      contained in the @a plotter.  If multiple DataRep objects are
      active, then returns an object that is not a FunctionRep if
      there is only one, otherwise returns null pointer.
  */
  hippodraw::DataRep *  activeDataRep ( const PlotterBase * plotter ) const;

  /** Sets the active DataRep index.  Sets the active DataRep index
      and redraws the plot.  If @a index is negative, sets all the
      contained DataRep objects to be active.
  */
  void setActiveDataRepIndex ( PlotterBase * plotter,
			       int index );

  /** Returns @c true if the DataRep contained by the @a plotter at index
      @a index has NTuple bindings. */
  bool hasNTupleBindings ( const PlotterBase * plotter, int index );

  /** Returns @c true if all the NTuple Objects used by the @a plotter
      are save to or read from a file.*/
  bool areDataSourcesSaved ( const PlotterBase * );

  /** Returns the Datasource for the @a plotter's selected DataRep or
      the DataSource used by all the DataRep objects contrained by the
      @a plotter.
   */
  DataSource * getDataSource ( const PlotterBase * plotter );

  /** Returns the DataSource for the DataRep indexed by @a
      index. Returns null pointer if DataRep does not have
      DataSource bindings.  
  */
  DataSource * getDataSource ( const PlotterBase *, int index );

  /** Returns the labels of the DataSource columns for the DataRep
      contained in the @a plotter at index @a index. */
  const std::vector < std::string > & 
  getDataSourceLabels ( const PlotterBase * plotter, int index );

  /** Sets the ntuple on the selected DataRep, if it has ntuple
      bindings, otherwise does nothing. */
  void setNTuple ( PlotterBase * , const NTuple * ) const;

  /** Returns the axis binding options for a DataRep of type @a type.
   */
  const std::vector < std::string > &
  bindingOptions ( const std::string & type );

  /** Returns the axis bindings options for the DataRep contained by
      the @a plotter at index @a index. */
  const std::vector < std::string > & 
  bindingOptions ( const PlotterBase * plottter, int index );

  /** Returns the axis bindings, if any, otherwise returns an empty
      vector.
  */
  const std::vector< std::string > & 
  axisBindings ( const PlotterBase * plotter ) const;

  /** Returns the axis bindings for the indexed projector, if any,
      otherwise returns an empty vector.
  */
  const std::vector< std::string > & 
  axisBindings ( const PlotterBase * plotter, int index ) const;

  /** Sets the axis binding, if any, otherwise does nothing.  After
      setting the axis binding, set the plotter to auto-ranging and
      auto-ranges the data representations. */
  void setAxisBinding ( PlotterBase *, 
			const std::string & axis,
			const std::string & label );

  /** Sets all the axis bindings, if any, otherwise does nothing. */
  void setAxisBindings ( PlotterBase * plotter,
			 const std::vector< std::string > & labels ) const;

  /** Returns @c true only if all the data in a bound DataSource are
      valid.
   */
  bool isDataValid ( const PlotterBase * ) const;

  /** Returns @c true only if all the data in a bound DataSource are
      valid.
   */
  bool isDataValid ( const hippodraw::DataRep * ) const;

   /** Returns @c true if the designated axis is on a logarithmic scale. 
    */
  bool getLog ( const PlotterBase * plotter, const std::string & axis ) const;

  /** Returns @c true if the designated axis is on a logarithm scale. 
   */
  bool getLog ( const PlotterBase * plotter, 
		Axes::Type axis ) const;

  /** Sets a log scale on or off for the specified axis for the
      plotter.  If @a flag is @c true, then the log scale is set on,
      otherwise a linear scale is set on.  An XYTransform object is
      created and sent to the display. */
  void setLog ( PlotterBase * plotter,
		const std::string & axis, 
		bool flag );

  /** Sets the log scale on or off for the specified axis for the
      plotter.  If @a flag is @c true, then the log scale is set on,
      otherwise a linear scale is set on.  An XYTransform object is
      created and sent to the display.

      @todo This function should go away in favor of one taking string
      for axis argument.
  */
  void setLog ( PlotterBase * plotter,
		Axes::Type axis, 
		bool flag );

  /** Sets a log scale on or off for the specified axis in special
      case that the transform object is a XYTransform.    

      @sa setLog(PlotterBase *, hippodraw::Axes::Type, bool )
  */
  void setLog ( PlotterBase * plotter,
		Axes::Type axis,
		bool flag,
		XYTransform * transform );

  /** Set both X and Y axis to log or linear, without resetting 
      the bin width. 
   */
  void setTransformAxis ( PlotterBase * plotter,
                          const std::string & x,
		          const std::string & y);

  /** Creates and sets a new transform for the display. */
  void setTransform ( PlotterBase * plotter,
		      const std::string & name );

  /** Creates and sets the new XY transform for the display. */
  void setTransform ( PlotterBase * plotter,
		      const std::string & x,
		      const std::string & y );

  /** Creates and sets the new XYZ transform for the display. */
  void setTransform ( PlotterBase * plotter,
		      const std::string & x,
		      const std::string & y,
		      const std::string & z );

  /** Deletes a PlotterBase object. */
  void remove ( PlotterBase * );

  /** Returns the size of the representation. */
  float pointSize ( const PlotterBase * ) const;

  /** Returns @c true if active DataRep uses SymbolPointRep, otherwise
      returns @c false.
   */
  bool hasSymbolRep ( const PlotterBase * plotter ) const;

  /** Returns @c true if active DataRep uses LinePointRep, otherwise
      returns @c false.
   */
  bool hasLineRep ( const PlotterBase * plotter ) const;


  /** Sets the size of the representation. */
  void setRepSize ( PlotterBase * plotter, float size );

  /** Sets the interval count on the the NTuple of the active DataRep
      of the @a plotter. */
  void setIntervalCount ( const PlotterBase * plotter, 
			  unsigned int count );

  /** Sets the interval counting to @a yes on the NTuple of the active
      DataRep of the @a plotter.  */
  void setIntervalEnabled ( const PlotterBase * plotter, bool yes );

  /** Sets the interval counting to @a yes of all the NTuple used by
      the @a plotter. */
  void setAllIntervalEnabled ( const PlotterBase * plotter, bool yes );

  /** Sets the Range on specified axis.   Also sets auto Ranging to off. */
  void setRange ( PlotterBase * plotter,
		  Axes::Type axis, 
		  const Range & range );

  /** Sets the bin width parameter.  If a single DataRep is on the
      display, or if a single DataRep on a multiple DataRep display is
      active, the bin width for that DataRep is set.  If multiple
      DataRep objects are active, then each one has its bin width set.
      In the case of multiple DataRep objects width different bin
      widths, the default Y or Z axis label is set to "density",
      otherwise set to "entries / bin". */
  void setBinWidth ( PlotterBase * plotter,
		     const std::string & axis,
		     double width );

  /** Sets the bin width of the DataRep object.  Sets the bin width of
      the DataRep object and informs its plotter. */
  void setBinWidth ( DataRep * datarep,
		     Axes::Type axis,
		     double width );

  /** Sets the offset from dragging slider. */
  void setOffset ( PlotterBase * plotter,
		   const std::string & axis,
		   int parm,
		   bool dragging );

  /** Sets the offset parameter. */
  void setOffset ( PlotterBase * plotter,
		   Axes::Type axis,
		   double offset );

  /** Tests if specified axis is binned.  Returns @c true if only one
      DataRep is selected and it is binned on specified axis.
  */
  bool isAxisBinned ( PlotterBase * display, const std::string & axis );

  /** Creates a NTuple from the @a plotter.  Creates a NTuple from the
      contents of DataRep in the plotter.  The plotter must have a
      single active %DataRep, otherwise nothing is created.  The
      created %NTuple is registered with the NTupleController.
  */
  void createNTuple ( const PlotterBase * plotter );

};

} // namespace hippodraw

#endif // _DisplayController_H_
