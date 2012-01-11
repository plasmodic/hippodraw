// -*- mode:c++ -*-
/** @file

This is the SIP interface definition for CutController

Copyright (C) 2003, 2005  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

This file is part of sihippo

$Id: CutController.sip,v 1.10 2007/02/24 22:24:38 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/

class CutController
{
%TypeHeaderCode
#include "controllers/CutController.h"
using hippodraw::CutController;
using hippodraw::PlotterBase;
%End

private:

CutController ();
CutController ( const CutController & );

public:

  /** Returns the pointer to the singleton instance. */
  static CutController * instance ();
/** Adds a one dimension cut to the @a plotter.  The cut uses the
      same NTuple as the @a plotter and is bound to the column with
      label @a label.   

      @note This function is only here for the SIP Python module so
      that SIP will wrap Cut1DPlotter instead of PlotterBase.
   */
  Cut1DPlotter * addCut ( PlotterBase *, const std::string & );
/** Adds a cut to the @a plotter.  Adds a simple one/two dimensional cut
      to the @a plotter.  The cut uses the same NTuple as the @a plotter
      and is bound to the column with label @a binding. Returns the cut
      plotter object that was added. */
  PlotterBase * addCut ( PlotterBase *,
			 std::vector< std::string > &  );
  
  /** Adds a cut (associated with the given cut plotter) to a Plotter. */
  void addCut ( PlotterBase * ,PlotterBase *  );

  /** Adds cut (associated with the given cut plotter) to the projector
      of target rep and registers targetrep as the target of the
      cut_plotter for a 1D/2D cutplotter */ 
//  void linkCutAndRep ( PlotterBase * , DataRep *  );
  
//   /** Adds cut (associated with the given cut plotter)to the projector
//       of target rep and registers targetrep as the target of the
//       cut_plotter for cut plotter.
//   */ 
   void linkCutAndRep ( CutPlotter * , DataRep * );

  
  /** Creates a new cut plotter.  The newly created cut plotter is bound
      to the @a ntuple with @a bindings.  Optionally, the color of the
      shading showing the cut inclusive range can be changed by
      overriding the default file for @a color.  The @a name argument
      is ignored and may be used in the future to create different
      kinds of cuts. Returns the pointer to the cut plotter. */
  PlotterBase * 
  createCut ( const std::string & , 
	      const DataSource * ,
	      const std::vector < std::string > & ,
	      const Color &  ) const;
  
  /** Removes the @a cut from the @a plotter. If the @a plotter does
      not have selected DataRep object, does nothing. */
  void removeCut ( PlotterBase * , PlotterBase * );

//   /** Clears and fills the @a cut_list with those PlotterBase objects
//       in @a plotter_list that are CutPlotter objects.
//   */
//   void fillCutList ( const std::vector < PlotterBase * > & ,
// 		     std::vector < CutPlotter * > & );

//   /** Clears and fills the @a cut_list with the CutPlotter objects
//       that target the @a data_rep.
//   */
//   void fillCutList ( const DataRep * ,
// 		     std::vector < PlotterBase * > & );

//   /** Returns a list of CutPlotter objects that are in PlotterBase
//       list and which use the same DataSource as @a ntuple.
//   */
//   const std::vector < PlotterBase * > & 
//   getCutList ( const std::vector < PlotterBase * > &,
// 	       const DataSource *   ) const; 

//   /** Clears and fills the @a web with the PlotterBase objects in @a
//       plotters, each of their CutPlotter objects that target them, and
//       all of their targets.  If the web could not be created, clears
//       the @a web.
//   */
//   void fillCutWeb ( const std::vector < PlotterBase * > & ,
// 		    std::vector < PlotterBase * > &  );

//   /** Appends to the @a web the plotter and any cuts targeting the
//       contained active data rep.
//   */
//   void appendToWeb ( PlotterBase * ,  PlotterList_t &  );

//   /** Appends to the @a web the @a cutter if it is not already in it.*/
//   void appendToWeb ( CutPlotter * , PlotterList_t &  );

//   /** Appends to @a web all the targets of each @a cutters. */
//   void appendToWeb ( const std::vector < PlotterBase * > & ,
// 		     PlotterList_t &  );

  /** Finds and returns the PlotterBase object containing the @a
      data_rep.
   */
  PlotterBase * findPlotter ( const DataRep *  );

//   /** Fills the @a cut_map with the TupleCut objects contained the
//       list of views, if any.  The key of the map is the address of
//       the TupleCut object represented as integer. */
//   void fillTupleCutList ( const std::vector < const ViewBase * > & ,
// 			  std::vector < TupleCut * > &  );

//   /** Connects the DataRep objects to its appropriate (cut) PlotterBase.  This
//       method is intended to be used when creating new objects that are
//       copies of old ones If the newly created object contains cuts,
//       then it's corresponding (cut) PlotterBase needs to add it to its target
//       list. In the list @a views, the appropriate (cut) PlotterBase object
//       should be found. */
//   void connectDataRep ( const std::list < ViewBase * > & ,
// 			const std::vector < const ViewBase * > &  );

  /** Sets the axis binding of the display and cut. Raises an
      assertion if the display's plotter is not derived from
      (cut) PlotterBase. */
//  void setAxisBinding ( PlotterBase * ,
//			const std::string & ,
//			const std::string &  );

//   /** Sets the zoom/pan status of plotter.  If @a yes is @c true, the
//       @a plotter is put in list of plotters with the zoom pan mode set,
//       otherwise it is removed from the list. 
//   */
//   void setZoomPan ( PlotterBase * ,
// 		    hippodraw::Axes::Type ,
// 		    bool  );

//   /** Returns @c true if @a plotter is in zoom/pan list. 
//    */
//   bool isZoomPan ( const PlotterBase * ,
// 		   hippodraw::Axes::Type  ) const;
};
