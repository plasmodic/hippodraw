/* -*- mode: c++ -*- */

/** @file

hippodraw::PyDataRep class interface
 
Copyright (C)  2002-2006  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: PyDataRep.h,v 1.45 2006/04/15 17:26:20 pfkeb Exp $

*/

#ifndef PyDataRep_H
#define PyDataRep_H

#include "graphics/Color.h"
#include "graphics/LineStyle.h"
#include "graphics/SymbolType.h"

namespace hippodraw {

class DataRep;
class RepBase;
class DataSource;
class PyDataSource;
class NTuple;
class QtCut;

/** This class is the public interface the what the user sees as the
    @em DataRep object from Python.  

    @author J. Chiang <jchiang@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Sanket B Malde <sanket@slac.stanford.edu>
*/
class PyDataRep
{

private:

  /** The actual DataRep object. */
  hippodraw::DataRep * m_datarep;

   /** Vector of values returned by const reference for getColumnWithCuts
       method.
    */ 
   std::vector<double> m_columnData;

   /** A map of symbol types, keyed by name. */
   static std::map< std::string, hippodraw::Symbol::Type > s_symbols;

   /** A map of line styles, keyed by name. */
   static std::map< std::string, hippodraw::Line::Style > s_lineStyles;

   /** A flag to indicate that we have the static data members. */
   static bool s_have_static_members;

   /** Create the color map. */
   static void makeColorMap();

   /** Create the plotting symbols map. */
   static void makeSymbolMap();

   /** Create the line style map. */
   static void makeLineStyleMap();

   /** Used in the constructors.
    */
   void init();

public:

  /** A constructor. */
  PyDataRep ( hippodraw::DataRep * );

   /** Constructor to create data reps from python. */
   PyDataRep ( const std::string & type,
               const DataSource * ntuple,
               const std::vector< std::string > & bindings );

   /** Constructor to create data reps from python using a PyDataSource
       object.
   */
   PyDataRep ( const std::string & type,
               const PyDataSource * ntuple,
               const std::vector< std::string > & bindings );

  /** The default constructor. */
  PyDataRep ();

   /** Copy constructor. */
   PyDataRep ( PyDataRep * pyRep );

  /** Returns the names of the types of DataRep objects available.
   */
  static const std::vector < std::string > & names ();

  /** The get method. */
  hippodraw::DataRep * getDataRep();

  /** Sets the size of the point representation.
   */
  void setSize ( double value );

   /** Set the point representation. */
   void setPointRep ( RepBase * pointRep );

   /** Set the axis bindings for a single axis. */
   void setAxisBinding ( const std::string &axis, 
                         const std::string &label );

   /** Set the axis bindings for all axes at once. */
   void setAxisBindings ( const std::vector< std::string > & bindings );

   /** Set the weight for DyHistogram (or Color Plot or Contour plots). */
   void setWeight ( const std::string &label );

   /** Return the display name used to create this data rep. */
   const std::string & name () const;

   /** Get the bin width for the specified axis. */
   double getBinWidth ( const std::string &axis );

  /** Returns the mean value along axis @a axis. 
   */
  double getMean ( const std::string & axis );

  /** Returns the RMS of the mean value along axis @a axis. 
   */
  double getRMS ( const std::string & axis );

   /** Return the number of entries in the rep. */
   double numberOfEntries() const;

   /** Apply a cut. */
   void applyCut ( QtCut * cut );

  /** Apply a sequence of cuts to the DataRep.
   */
  void applyCuts ( const std::vector < QtCut * > & cuts );

  /** Returns the names of the available colors.
   */
  const std::vector < std::string > & colorNames ();

  /** Sets the Color of the point representation to one of Color::Value.
   */
  void set ( Color::Value value );

   /** Set the color used for plotting, selecting from a predefined
       palette of "black", "red", "green", "blue", "yellow", "cyan",
       "magenta".
   */
   void setColor ( const std::string & color );

   /** Turn on the error bars. */
   void setErrorDisplay( const std::string &axis, bool flag );

   /** Create an NTuple for the displayed data. */
   const NTuple * createNTuple () const;

   /** Return the current NTuple but with the present set of cuts
       applied.
   */
   NTuple * getNTupleWithCuts () const;

   /** Return the named column with the present set of cuts applied. */
   const std::vector<double> & 
   getColumnWithCuts(const std::string & column);

  /** Returns a new NTuple from the bound DataSource by selecting rows
      that satisfy the region cut.
   */
  NTuple * createNTupleUnderRegion () const;

  /** Adds a new region cut.
      @bug @@ Only works on X axis.
   */
  void addCut ( );

  /** Sets the range of the region cut.
   */
  void setCutRange ( double low, double high );

  /** sets the plotting symbol and size for a point.
   */
  void set ( hippodraw::Symbol::Type type );


   /** Set the plotting symbol and size.
       @deprecated Use set(hippodraw::Symbol::Type) instead.
    */
   void setSymbol( const std::string &symbolName, float size=2.);

  /** sets the plotting line style.
   */
  void set ( hippodraw::Line::Style style );

   /** Set the plotting line style.
       @deprecated Use set(hippodraw::Line::Style) instead.
    */
   void setLineStyle( const std::string &lineStyle );

  /** Normalizes the receiving object to the @a target one. */
  void normalizeTo ( const PyDataRep * target );

  /** Sets the bin width, if binned, on @a axis to @a width.
      @bug Doesn't always work if display hasn't drawn yet.
   */
  void setBinWidth ( const std::string & axis, double width );


};

} // namespace hippodraw

#endif // PyDataRep_H
