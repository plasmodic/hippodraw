/* -*- mode: c++ -*- */

/** @file
 
hippodraw::CutController class interface

Copyright (C) 2001-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CutController.h,v 1.93 2007/06/07 18:16:05 pfkeb Exp $

*/

#ifndef _CutController_H_
#define _CutController_H_

#include "graphics/Color.h"
#include "axes/AxesType.h"

#include <list>

namespace hippodraw {
class DataRep;
class DataSource;
class PlotterBase;
class CutPlotter;
class Cut1DPlotter;
class Cut2DPlotter;
class NTupleProjector;
class Range;
class TupleCut;
class ViewBase;

/** A singleton class that handles the application logic for cuts.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API CutController
{

private:

  /** The type of standard C++ container used to hold PlotterBase
      objects.
  */
  typedef std::vector < PlotterBase * > PlotterList_t;

  /** The pointer to the singleton object. */
  static CutController * s_instance;

  /** A private copy constructor in order to avoid copying. */
  CutController ( const CutController & );

  /** A list of cutplotter-axis pair that have been placed in zoom/pan mode. */
  std:: list < std::pair< PlotterBase *, Axes::Type > > m_zoom_pan;
  
  /** A temporary list of selected (cut) PlotterBase objects. The selection
      is made by the application calling one of the #getCutList member
      functions.  A selected (Cut) PlotterBase object can be access by index
      into this list. */
  mutable std::vector < PlotterBase * > m_cut_list;

  /** A temporary list of string corresponding to cuts. */
  mutable std::vector < std::string > m_cut_strings;

  /** Connects the DataRep object to its appropriate Cut Plotter.  This
      method is intended to be used when creating new objects that are
      copies of old ones If the newly created object contains cuts,
      then it's corresponding cut Plotter needs to add it to its target
      list. In the list @a views, the appropriate Cut Plotter object
      should be found. */
  void connectDataRep ( DataRep * rep, 
			const std::vector < const ViewBase * > & views );

  /** Connects the DataRep object to appropriate Cut Plotter.  This
      method is intended to be used when creating new objects that are
      copies of old ones If the newly created object contains cuts,
      then it's corresponding cut Plotter needs to add it to its target
      list. In the list @a views, the appropriate cut Plotter object
      should be found. */
  void connectDataRep ( const TupleCut * cut,
			DataRep * rep,
			const std::vector < const ViewBase * > & views );

protected:

  /** A default constructor for avoiding creation except by itself or with
      derived classes. */
  CutController();

 public:

  /** Returns the pointer to the singleton instance. */
  static CutController  * instance ();

  /** Adds a one dimension cut to the @a plotter.  The cut uses the
      same DataSource as the @a plotter and is bound to the column
      with label @a label.

      @deprecated This function is only here for the SIP Python module
      so that SIP will wrap Cut1DPlotter instead of PlotterBase.
   */
  Cut1DPlotter * addCut ( PlotterBase * plotter, const std::string & label );

  /** Adds a cut to the @a plotter.  Adds a simple one/two dimensional
      cut to the @a plotter.  The cut uses the same DataSource as the
      @a plotter and is bound to the column with label @a
      binding. Returns the cut plotter object that was added.
  */
  PlotterBase * addCut ( PlotterBase * plotter,
			 std::vector< std::string > & bindings );
  
  /** Adds an existing cut to a @a plotter.  If only one contained
      DataRep object by the @a plotter is active, then add the cut to
      it, otherwise add the cut to each having DataSource bindings.
      If @a plotter is itself a cut, then add the cut to its targets
      recursively.  If @a cut_plotter is not a CutPlotter, then throw
      an exception.
   */
  void addCut ( PlotterBase * cut_plotter,
		PlotterBase * plotter );

  /** Adds each of the existing cuts in the sequence to the plotter.
   */
  void addCuts ( const std::vector < PlotterBase * > & cut_list,
		 PlotterBase * plotter );

  /** Adds cut (associated with the given cut plotter)to the projector
      of target rep and registers target as the target of the
      cut_plotter for cut plotter.
  */ 
  void linkCutAndRep ( CutPlotter * cut_plotter,
		       DataRep * target );

  
  /** Creates a new cut plotter.  The newly created cut plotter is bound
      to the @a source with @a bindings.  Optionally, the color of the
      shading showing the cut inclusive range can be changed by
      overriding the default file for @a color.  The @a name argument
      is ignored and may be used in the future to create different
      kinds of cuts. Returns the pointer to the cut plotter.
  */
  CutPlotter * 
  createCut ( const std::string & name, 
	      const DataSource * source,
	      const std::vector < std::string > & bindings,
	      const Color & color ) const;
  
  /** Removes the @a cut from the @a plotter. If the @a plotter does
      not have selected DataRep object, does nothing. */
  void removeCut ( PlotterBase * cut_plotter, PlotterBase * plotter );

  /** Clears and fills the @a cut_list with those PlotterBase objects
      in @a plotter_list that are CutPlotter objects.
  */
  void fillCutList ( const std::vector < PlotterBase * > & plotter_list,
		     std::vector < CutPlotter * > & cut_list );

 /** Clears and fills the @a cut_list with the CutPlotter objects
     that target any DataRep cbject contained by the @a plotter.
  */
  void fillCutList ( const PlotterBase * plotter,
		     std::vector < PlotterBase * > & cut_list );

 /** Clears and fills the @a cut_list with the CutPlotter objects
      that target the @a data_rep.
  */
  void fillCutList ( const DataRep * data_rep,
		     std::vector < PlotterBase * > & cut_list );

public:
  /** Returns a list of TupleCut objects that are used by the DataRep.
   */
  const std::vector < const TupleCut * > & 
  getCutList ( const DataRep * rep  ) const; 

  /** Returns a list of CutPlotter objects that are in PlotterBase
      list and which use the same DataSource as @a rep.
  */
  const std::vector < PlotterBase * > & 
  getCutList ( const std::vector < PlotterBase * > &,
	       const DataSource * rep  ) const; 

  /** Clears and fills the @a web with the PlotterBase objects in @a
      plotters, each of their CutPlotter objects that target them, and
      all of their targets.  If the web could not be created, clears
      the @a web.
  */
  void fillCutWeb ( const std::vector < PlotterBase * > & plotters,
		    std::vector < PlotterBase * > & web );

  /** Appends to the @a web the plotter and any cuts targeting the
      contained active data rep.
  */
  void appendToWeb ( PlotterBase * plotter,  PlotterList_t & web );

  /** Appends to the @a web the @a cutter if it is not already in it.*/
  void appendToWeb ( CutPlotter * cutter,
		     PlotterList_t & web );

  /** Appends to @a web all the targets of each @a cutters. */
  void appendToWeb ( const std::vector < PlotterBase * > & cutters,
		     PlotterList_t & web );

  /** Finds and returns the PlotterBase object containing the @a
      data_rep.
   */
  PlotterBase * findPlotter ( const DataRep * data_rep );

  /** Fills the @a cut_map with the TupleCut objects contained the
      list of views, if any.  The key of the map is the address of
      the TupleCut object represented as integer.
  */
  void fillTupleCutList ( const std::vector < const ViewBase * > & views,
			  std::vector < const TupleCut * > & cut_list );

  /** Connects the DataRep objects to its appropriate (cut) PlotterBase.  This
      method is intended to be used when creating new objects that are
      copies of old ones If the newly created object contains cuts,
      then it's corresponding (cut) PlotterBase needs to add it to its target
      list. In the list @a views, the appropriate (cut) PlotterBase object
      should be found. */
  void connectDataRep ( const std::list < ViewBase * > & targets,
			const std::vector < const ViewBase * > & views );

  /** Sets the zoom/pan status of plotter.  If @a yes is @c true, the
      @a plotter is put in list of plotters with the zoom pan mode set,
      otherwise it is removed from the list. 
  */
  void setZoomPan ( PlotterBase * plotter,
		    hippodraw::Axes::Type axis,  bool yes );

  /** Returns @c true if @a plotter is in zoom/pan list. 
   */
  bool isZoomPan ( const PlotterBase * cut_plotter,
		   hippodraw::Axes::Type axis ) const;

  static void fillAcceptedRows ( std::vector < bool > & array,
				 const DataSource * source,
				 const std::vector< const TupleCut * > & cuts);

  /** Creates a new column in @a destination DataSource with elements of value 1.0 if element of @a source is contained in correspoding column in @a destinations. */
  void createIdCut ( const DataSource * source, DataSource * destination );

};

} // namespace hippodraw

#endif // _CutController_H_
