/* -*- c++ -*- */

/** @file

hippodraw::RepBase class interface

Copyright (C) 2000-2003, 2005, 2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: RepBase.h,v 1.52 2007/01/24 21:41:49 xiefang Exp $

*/

#ifndef _RepBase_H_
#define _RepBase_H_

#include "axes/AxesType.h"
#include "graphics/Color.h"
#include "graphics/LineStyle.h"
#include "graphics/SymbolType.h"

namespace hippodraw {

class BinToColor;
class DataSource;
class ProjectorBase;
class Range;
class TransformBase;
class ViewBase;

/** The base class for the point representation hierarchy.  A point
    representation draws something in a view object whose class is
    derived from ViewBase.  Examples are classes derived from
    PointRepBase and TextRepBase.
    
    This base class implements the behavior of having a name so that
    it can be found by name in a Factory.  It also implements the
    behavior of having a color and a size.

    The compiler generated copy constructor and assignment operator
    are valid for this class and thus not explicitly implemented.

    @author Paul_Kunz@slac.stanford.edu */

class MDL_HIPPOPLOT_API RepBase
{

protected:

  /** The name of the representation. */
  std::string m_name;

  /** The text to be displayed before the statistics.*/
  std::string m_text;

  /** The color of the representation. */
  Color m_color;

  /** The color of the representation when it is de-selected. */
  static const Color s_desel_color;

  /** The size of the representation. */
  float m_size;

  /** A flag to indicate the representation is de-selected. */
  bool m_desel;

  /** Sets the state of highlighting.
   */
  bool m_highlite;

  /** A constructor that sets the name and size of the
      representation. */
  RepBase ( const char *, float size );

  /** The copy constructor. */
  RepBase ( const RepBase & );

public:

  /** The virtual destructor */
  virtual ~RepBase();

  /** Returns a copy of the object.  Derived classes should implement
      this function by calling their copy constructor. */
  virtual RepBase * clone() = 0;

  /** Returns the name of the representation.  This name might be used
      by a Factory. */
  const std:: string & name () const;

  /** Sets the color of the representation */
  virtual void setColor ( const Color & );

  /** Sets the symbol of the representation.  Derived classes may use
      this method to set the style of the representation, if they have
      various styles.   This base class implementation does nothing.
   */
  virtual void setStyle ( unsigned int style );

  /** Returns the style of the point representation.  Derived class
      should override this member function if they support styles.
      This base class implementation returns 0.
   */
  virtual unsigned int getStyle ( ) const;

  /** Sets the text.*/
  void setText ( const std::string & text );

  /** Gets the text. */
  const std::string & getText() const;

  /** Get the color of the representation. */
  virtual const Color & getColor () const;

  /** Returns the representation's color. 

      @todo Remove this member in favor of getColor().
  */
  virtual const Color & color () const;

  /** Sets the representation to be selected if @a yes is @c true,
      otherwise set it to be deselected.
  */
  virtual void setSelected ( bool yes = true );

  /** Returns @c true if representation is in selected state, otherwise
      returns @c false.
  */
  virtual bool isSelected () const;

  /** Sets the highlighted state of the point representation.  If @a
      yes is true, then the point representation should show the non
      highlighted regions in some fashion, typically using light gray
      color instead of its normal color.  @sa setSelected (bool) @sa
      m_ranges.
  */
  virtual void setHighLighted ( bool yes );

  /** Returns the size of the representation. */
  float size () const;

  /** Sets the size of the representation.  Derived classes may use
      this method to set the size of the point representation and any
      of its components.
   */
  virtual void setSize ( float value );

  /** Returns true if the point representation displays error on the X
      axis.  The default implementation returns @c false.  May be
      overridden by derived classes. */
  virtual bool xError () const;

  /** Returns true if the point representation displays error on the Y
      axis.  The default implementation returns @c false.  May be
      overridden by derived classes. */
  virtual bool yError () const;

  /** Sets the error representation on specified @a axis, if any, to
      @a yes.
  */
  virtual void setErrorOn ( hippodraw::Axes::Type axis, bool yes = true );

  /** Returns the value transform.  A value transform changes a binary
      value to some graphical representations such as color. The
      implementation in this base class returns a null pointer.
      Derived classes that implement value transform should return a
      pointer to the object it uses.
  */
  virtual  const BinToColor * getValueTransform ( ) const;

  /** Sets the value transform.  The implementation in this base class
      does nothing.  Derived classes that implement a value transform
      should override this method and take possession of the received
      object.

      @todo Should have base class that doesn't know about color and
      BinToColor class should derive from it.
  */
  virtual void setValueTransform ( BinToColor * );

  /** Draws the projected values.  Draws the projected values
      contained in the DataSource @a ntuple.  Each row of the @a
      ntuple is data point tuple of some dimension.  Derived classes
      expect the DataSoruce to be of the correct dimension.  The drawn
      points undergo a transform represented by @a transform and are
      drawn to the view @a view.  If highlighting is on, uses the @a
      ranges to control highlighting.
  */
  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view ) = 0;

  /** Display an error message.  Display an error message instead of
      drawing a point.  This base class implementation displays a
      message with the name of the RepBase object.
  */
  virtual void displayError ( ViewBase & view );

  /** Returns @c true if receiving object uses a Color::Value as a
      property, otherwise returns @c false.  This base class
      implementation returns @c true as most derived classes use a
      Color::Value as a property.  Derived classes that do not, such
      as those that represent a value in color, should override this
      member function and return @c false.
  */
  virtual bool uses ( Color::Value ) const;

  /** Returns @c true if receiving object uses a
      hippodraw::Line::Style as a property, otherwise returns @c
      false.  This base class implementation returns false.  Derived
      classes that use hippodraw::Line::Style should override this
      member function and return @c true.
  */
  virtual bool uses ( hippodraw::Line::Style ) const;

  /** Returns @c true if receiving object uses a
      hippodraw::Symbol::Type as a property, otherwise returns @c
      false.  This base class implementation returns false.  Derived
      classes that use hippodraw::Symbol::Type should override this
      member function and return @c true.
  */
  virtual bool uses ( hippodraw::Symbol::Type ) const;

};

} // namespace hippodraw

#endif // _RepBase_H_
