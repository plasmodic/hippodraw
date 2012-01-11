/* -*- mode: c++ -*- */

/** @file

TextPlotter class interface

Copyright (C) 2001-20054   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TextPlotter.h,v 1.61 2006/12/05 00:01:34 xiefang Exp $

*/

#ifndef _TextPlotter_H_
#define _TextPlotter_H_

#include "PlotterBase.h"

namespace hippodraw {

class TextDataRep;
class TextRepBase;

/** A @c Plotter class that draws text. It is the glue between a
    derived class of ProjectorBase and a derived class of TextRep.
    When it is time to plot, this class calls the TextRep with a
    reference to the projector and a reference to the View as
    arguments. The derived class from TextRep asks for explicit values
    from the projector.

    @todo The amount of stuff this class inherits and doesn't use is
    another sign that the PlotterBase is too heavy.  For example,
    everything dealing with the axes are only needed by the XYPlotter.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

*/
class MDL_HIPPOPLOT_API TextPlotter : public PlotterBase
{

private:

  /** The DataRep object being used. */
  TextDataRep * m_datarep;

public:

  /** The default constructor. */
  TextPlotter ();

  /** The copy constructor. */
  TextPlotter( const TextPlotter & plotter );

  /** The destructor. */
  ~TextPlotter();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual PlotterBase * clone();

  /** Returns @c false since TextPlotter does not have ntuple bindings. */
  bool hasNTupleBindings () const;

  /** Sets the @c PointRep.  The PointRep @a pointrep replaces the
      existing one (which is destroyed). */
  virtual void setRepresentation ( RepBase * );

  /** Returns the representation used by the plotter. */
  virtual RepBase * representation () const;


  virtual void addDataRep ( hippodraw::DataRep * rep );
  virtual int getNumDataReps () const;
  virtual hippodraw::DataRep * getDataRep ( int ) const;
  virtual TransformBase * getTransform ();

  virtual void setErrorDisplay ( hippodraw::Axes::Type axis, bool );
  virtual bool errorDisplay ( hippodraw::Axes::Type axis ) const;

  virtual void drawIn( ViewBase * view );
  void setAxisModel ( AxisModelBase *, hippodraw::Axes::Type );
  void setRepColor ( const Color & );
  const Color & repColor () const;

  /** Does nothing */
  virtual float userToMarginX ( double x ) const;

  /** Does nothing */
  virtual float userToMarginY ( double y ) const;

  /** Does nothing */
  virtual float userToInvertedMarginY ( double y ) const;

  virtual void toUserXY ( double mx, double my, bool scaled, 
			  double & ux, double & uy ) const;

  /** Returns the target DataRep of its own TextDataRep object. The
      target DataRep is the one used for the source of the textual
      data. */
  const hippodraw::DataRep * getParentDataRep () const;

  /** Sets the target DataRep of its own TextDataRep object. @sa
      getTarget. */
  void setParentDataRep ( hippodraw::DataRep * );
  
  virtual void willDelete ( const Observable * observable );

  virtual NTuple * createNTuple () const;

  virtual void update ();

  /* Return @c true if it's a text plotter. */
  virtual bool isTextPlotter() const;

};

} // namespace hippodraw

#endif // _TextPlotter_H_
