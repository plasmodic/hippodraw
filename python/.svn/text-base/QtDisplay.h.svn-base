/* -*- mode: c++ -*- */

/** @file

hippodraw::QtDisplay class interface.

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtDisplay.h,v 1.104 2007/03/27 18:53:06 pfkeb Exp $

*/

#ifndef _QtDisplay_H_
#define _QtDisplay_H_

#include "axes/AxesType.h"

#include <vector>
#include <map>

namespace boost {
  namespace python {
    class list;
    class tuple;
  }
}

namespace hippodraw {

class DataRep;
class Observer;
class PyFunctionRep;
class PyDataRep;
class PyDataSource;
class PyFunctionRep;
class PlotterBase;
#ifndef BOOST_DEFECT
class FunctionBase;
#endif
class RepBase;
class DataSource;
class NTuple;
class PyNTuple;
class QtRootNTuple;

/** Qt Displays wraps a derived class of PlotterBase.  The purpose of
    this class is to provide an interface for Python running in a
    different thread from the application. It locks the QApplication
    object before forwarding messages to the contained PlotterBase
    object.  It unlocks the QApplication object upon return.

    @request @@@@@@ Need to add methods that PyDataRep implements so
    user doesn't need to know about DataRep objects.

    @request @@@@@@ Implement constructor that takes as first argument
    a DataSource/DataArray and if binding is one element, create
    Histogram, if two, create ColorPlot.

    @request Need to check each method that does a lock to see if an
    exception could be thrown.  If yes, and no try/catch block, then
    application will hang.

    @todo Too may try/catch blocks.  All exception inherit from
    std::exception, so why not put one static function to take care of
    the exceptions and qApp lock/unlock.

    @author James Chiang <jchiang@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class QtDisplay 
{

private:

   /** A vector to store the PyDataRep pointers to be returned by
       reference via the getDataReps() method */
   mutable std::vector< PyDataRep * > m_pyDataReps;

   /** The saved X and Y ranges to be used with the resize() method. */
   std::map<std::string, std::pair <double, double> > m_ranges;

   /** The saved X and Y bin widths to be used with the resize() method. */
   std::map<std::string, double > m_binWidths;

  /** Create a QtDisplay.  The @a type of QtDisplay will be looked up in the
      DataRepFactory.  It will be connected to the DataSource @a nt.  The
      bindings is represented by @a bindings.  This is the function that
      does the work and is called by the constructors. */
  void createDisplay ( const std::string & type, 
		       const DataSource & nt, 
		       const std::vector < std::string > & bindings );

protected:

  /** The contained PlotterBase object.   */
  PlotterBase * m_plotter;

  /** A default constructor for derived classes. */
  QtDisplay ();

public:

  /** Create a display that has no DataSource bindings.
   */
  QtDisplay ( const std::string & type );

  /** Creates a display for existing DataRep @a rep.
   */
  QtDisplay ( PyFunctionRep * rep );

   /** Create a display using a vector of axis bindings, transparently 
       wrapping the createDisplay private method. 
   */
   QtDisplay ( const std::string & type,
               const DataSource & nt,
               const std::vector< std::string > & bindings );

   /** Create a display bound to a PyDataSource object using a vector 
       of axis bindings, transparently wrapping the createDisplay 
       private method. 
   */
   QtDisplay ( const std::string & type,
               const PyDataSource & nt,
               const std::vector< std::string > & bindings );

   /** Create a display bound to RootNTuple using a vector of axis
       bindings, transparently wrapping the createDisplay private
       method.
   */
   QtDisplay ( const std::string & type,
               const QtRootNTuple & nt,
               const std::vector< std::string > & bindings );

   /** Create a display bound to RootNTuple using a vector of axis
       bindings and indexes to array of TLeaf.
   */
   QtDisplay ( const std::string & type,
               const QtRootNTuple & nt,
               const std::vector< std::string > & bindings,
	       boost::python::list indices );

  /** Create a display by creating a ListTuple with the list objects
      contained in the tuple.
   */
  QtDisplay ( const std::string & type,
	      boost::python::tuple,
	      const std::vector < std::string > & bindings );

  /** Create a display by creating a ListTuple with the list objects
      contained in the list.
   */
  QtDisplay ( const std::string & type,
	      boost::python::list,
	      const std::vector < std::string > & bindings );

   /** Create a display from a pointer to an existing PlotterBase object */
   QtDisplay (PlotterBase * plotter);

   ~QtDisplay();

  /** Returns the wrapped display object. */
  PlotterBase * display();

  /** Creates and adds a DataRep to the display.  
      
      @param type The type of DataRep.  Will be used to search for the
      type in the DataRepFactory.  If the type doesn't exist, nothing
      is created.  No checks are made that the new DataRep is
      consistent for viewing with existing ones.
      @param ntuple A pointer to the DataSource to be used.
      @param bindings The labels of the columns in the DataSource to be used.
  */
  void addDataRep ( const std::string & type, 
		    const DataSource * ntuple,
		    const std::vector < std::string > & bindings );

  /** Adds a DataRep to the display by stacking, i.e. sharing X axis
      but not the Y axis.
  */
  void addDataRepStacked ( const std::string & type, 
			   const DataSource * ntuple,
			   const std::vector < std::string > & bindings );

   /** Adds a DataRep created within Python */
   void addDataRep ( PyDataRep * pyRep );

   /** Adds a Function (as PyFunctionRep) */
   void addDataRep ( PyFunctionRep * pyFuncRep );

#ifndef BOOST_DEFECT
   /** Adds a Function (as a FunctionBase object) */
   void addFunction ( FunctionBase * function );
#endif

  /** Returns the selected DataRep in the plotter. */
  PyDataRep * getDataRep ();

   /** Returns a vector of all DataReps in the plotter. */
   const std::vector<PyDataRep *> &getDataReps () const;

  /** Sets the title of the display. 
   */
  void setTitle ( const std::string & title );

   /** Returns the title of the display. */
   const std::string &getTitle() const;

  /** Sets the label on the corresponding axis. 
   */
  void setLabel ( const std::string & axis, const std::string & label );

   /** Returns the label on the specified axis. */
   const std::string & getLabel( const std::string &axis) const;

  /** Sets the number of bins. */
  void setNumberOfBins ( const std::string & axis, unsigned int number );

  /** Returns the bin width on the specified axis. */
  double getBinWidth ( const std::string & axis ) const;

  /** Sets the bin width on the specified axis. */
  void setBinWidth ( const std::string & axis, double width, 
                     bool save=false );

  /** Sets the bin width on the specified axis. This version is needed 
      since Boost.Python cannot accommodate default arguments directly. */
  void setBinWidth ( const std::string & axis, double width );

  /** Sets the offset of the bins on the specified axis. */
  void setOffset ( const std::string & axis, double offset );

  /** Sets the Range on the specified axis. */
  void setRange ( const std::string & axis, double low, double high, 
                  bool save=false );

   /** Sets the Range on the specified axis. This version is needed 
       since Boost.Python cannot accommodate default arguments directly. */
   void setRange ( const std::string & axis, double low, double high );

   /** Get the Range of the specified axis. */
   std::vector<double> getRange ( const std::string & axis );

   /** Save the current set of plot ranges which define a "view"
       for this plot.
       @return The index of the saved view.
    */
   int saveView ();

   /** Set the view by @a index. */
   void setView ( int index );

   /** Cycle through each set of plot ranges, thereby changing
       the view with each call.
       @return The index of the current view.
       @param stepForward Direction to step through the views.
   */
   int nextView ( bool stepForward = true );

   /** Return the number of stored views. */
   int numViews ();

   /** Delete a view by @a index. */
   void deleteView( int index );

   /** Return the current view index. */
   int currentView ();

  /** Sets the auto ranging flag on specified axis. */
  void setAutoRanging ( const std::string & axis, bool flag );

  /** Sets the specified axis to Logarithmic binning (if applicable )
      and display. */
  void setLog ( const std::string & axis, int flag );

   /** Return 1 if the specified axis is logarithmically scaled. */
   int getLog ( const std::string & axis );

  /** Sets the Transform. */
  void setTransform ( const std::string & name );

  /** Sets the PointRep for the display. 
   */
  void setPointRep ( RepBase * rep );

   /** Set the contour levels if a ContourPointRep */
   void setContourLevels ( const std::vector<double> &levels );

  /** Sets the aspect ratio. 
   */
  void setAspectRatio ( double ratio );

  /** Resets all bins
   */
  void reset();

  /** Adds the values to the display. */
  void addValues ( const std::vector < double > & v );

  /** Creates and returns an PyNTuple representing the display.
   */
  PyNTuple * createNTuple () const;

   /** Return the number of entries in the display. */
   double numberOfEntries () const;

   /** Restore the X and Y dimensions of the display to their saved
       values */
   void resize();

   /** Return the Id to the plotter object.  

       @note If returning an int, then gcc 2.95.3 has internal
       compiler error coming from Boost.Python.
    */
   int plotterId() const;

   /** Set the color map by name. */
   void setColorMap ( const std::string & name );

  /** Updates the display.  

      @bug @@@@ For the script, autontuple.py, this method behaves
      strangely.
   */
  void update ();

  /** Adds an observer to PlotterBase object.
   */
  void addObserver ( hippodraw::Observer * observer );

  /** Sets the axis ticks to specified points and values.
   */
  void setTicks ( const std::string & axis,
		  const std::vector < double > & values,
		  const std::vector < std::string > & labels );

  /** Sets the ticks to be automatic or manual.
   */
  void setAutoTicks ( const std::string & axis, bool yes );

   /** Unlock the QApplication in case of exception. */
   void unlock();

/** Applies a cut to the wrapped PlotterBase.  If @a cut is a wrapper
    for a CutPlotter, then applies the cut to the wrapped PlotterBase
    object, otherwise an exception is thrown.
 */
  void applyCut ( QtDisplay * cut );

/** Applies a cuts to the wrapped PlotterBase.  If @a the cuts are
    rappers for a CutPlotter, then applies the cuts to the wrapped
    PlotterBase object.  If any of the cuts is not wrapper for a cut,
    an exception will be thrown.
 */
  void applyCuts ( const std::vector < QtDisplay * > &  cuts );


  /** Creates a data array wrapper for the contents of the display.
   */
   PyDataSource * createDataArray() const;

};

} // namespace hippodraw

#endif // _QtDisplay_H_
