/* -*- mode: c++ -*- */

/** @file

hippodraw::CutPlotter class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: CutPlotter.h,v 1.97 2007/05/29 19:32:22 pfkeb Exp $

*/

#ifndef _CutPlotter_H_
#define _CutPlotter_H_

#include "XyPlotter.h"

namespace hippodraw {

class Color;
class DataSource;
class TupleCut;
class PointRepBase;

/** A PlotterBase derived class that serves a base class for cut plotters.   

    This class is an Observer of its target plots.

    @bug If one removes a cut by cutting it from the canvas, the cut
    does not get removed from its target.

    @bug If cut range is set from python outside the display range,
    the cut range color is displayed outside the axis representation.
    Try function_empty.py.

    @bug Fitting a function seems to fit only the data under the cut.

    @request Be able to display number of points passing cut.

    @author Paul_Kunz@slac.stanford.edu 
*/
class MDL_HIPPOPLOT_API CutPlotter: public XyPlotter
{

protected:

  /** The constructor.
   */
  CutPlotter ( const std::string & name );

  /** The STL container type used for targets of the cut.
   */
  typedef std::list < hippodraw::DataRep * > TargetList_t;

  /** A list of DataRep objects which have this cut applied.
   */
  TargetList_t m_targets;

  /** A pointer to the data representation that holds the cuts.
   */
  hippodraw::DataRep * m_datarep;

  /** Update the target DataRep when a change has been made in the
      TupleCut. */
  void updateTargets ();

  /** Removes this cut from all of its targets. */
  void removeFromTargets ( );

public:

  /** This default constructor. */
  CutPlotter ( );

  /** The copy constructor. */
  CutPlotter ( const CutPlotter & plotter );

  /** The destructor. */
  virtual ~CutPlotter ();

  /** Adds a DataRep to the list of targets. If @a rep is not the
      DataRep for this plotter, then add it as target of this cut and
      to @a rep's list of observers otherwise do nothing. 
      
      @note The CutPlotter needs to be in list of observers so it will
      be notified when @a rep is deleted.
  */
  void addCutTarget ( DataRep * rep );

  /** Returns @c true if specified DataRep is a target of this cut. */
  bool isTarget ( DataRep * ) const;

  /** Get the list of targets. */
  const std::list < DataRep * > & getCutTargets () const;

  /** Remove this cut from one of its target @a rep. */
  void removeFromTarget ( DataRep * rep );

  /** Responds to willDelete message emitted from one of the
      Observers.  Responds to message emitted from one of @c this
      object's Observable objects when it is about to be deleted.
      Does nothing unless @a observable is a target of the cut.  In
      that case, removes the target. */
  virtual void willDelete ( const Observable * observable );

  /** Returns the list of TupleCut objects used by this plotter.
   */
  const std::vector < TupleCut > & getCuts () const;

  /** Sets the TupleCut objects to be used by this plotter.
   */
  void setCuts ( const std::vector < const TupleCut * > & cuts );

  /** Returns @c true as a CutPlotter always has DataSource bindings. */
  virtual bool hasNTupleBindings () const;

  /** Removes all cut targets. */
  void clearCutTargets ();

  /** Sets the axis binding of both the Cut and its DataRep.
   */
  virtual void setAxisBinding ( const std::vector < std::string > & bindings );

  /** Sets the color of the cut acceptance region. */
  void setCutColor ( const Color & );

  /** Get the color of the accepted region. */
  const Color & getCutColor () const;

  /** Sets the Range of the cut. */
  virtual void setCutRangeAt ( const Range & range, unsigned int index );

  /** Sets the Range on the X axis.  

      @note This function is here so that the SIP Python extension
      module doesn't need to know about Axes::Type.
   */
  virtual void setCutRange ( double low, double high );

  /** Sets the cut Range to include the full range of data.

      @todo The shading showing the cut range should go to the end of
      the last bin and not just to the end of the last data.
  */
  virtual void setCutRangeFull () = 0;

  /** Sets the cut as active.  Sets the cut as active or not depending
      on value of @a yes.  An active cut displays its cut range in
      normal color, while an inactive displays it in light gray.
  */
  void setActive ( bool yes );

  /** Sets the DataSource for the Cut and its DataRep object. */
  void setNTuple ( const DataSource * ntuple );

  /** Reverse the meaning of the cut Range on the give axis. */
  virtual void toggleInverted (  ) = 0;

  /** Sets the state of the cut's enabled status.
   */
  void setEnabled ( bool yes = true );

  /** Responds to the update message from one of its Observable
      objects.  If the update comes from one of its m_target objects
      does nothing, otherwise notifies its Observer objects.
  */
  virtual void update ( const Observable * object );

  /** Add tuple cut to the plotter. */
  virtual void addTupleCut ( DataRep * rep );

  /** Returns @c false.  Returns @c false since a CutPlotter can never
      be used to generate an image.
 */
  virtual bool isImageConvertable () const;

};

} // namespace hippodraw

#endif // _CutPlotter_H_
