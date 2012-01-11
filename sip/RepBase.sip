/* -*- mode:c++ -*- */

/** @file

SIP interface definition for RepBase

@note The commented out code requires exposure to more types to SIP.

Copyright (C)  2002, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

$Id: RepBase.sip,v 1.6 2007/02/24 22:24:38 pfkeb Exp $

@author <Paul_Kunz@slac.stanford.edu>

*/

class RepBase /Abstract/
{
%TypeHeaderCode
#include "reps/RepBase.h"
using hippodraw::RepBase;
using hippodraw::Color;
%End

public:

///  RepBase ();


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
  virtual void setStyle ( unsigned int  );

  /** Returns the style of the point representation.  Derived class
      should override this member function if they support styles.
      This base class implementation returns 0.
   */
  virtual unsigned int getStyle ( ) const;

  /** Sets the text.*/
  void setText ( const std::string &  );

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

  /** Returns the size of the representation. */
  float size () const;

  /** Sets the size of the representation. */
  void setSize ( float  );

  /** Returns true if the point representation displays error on the X
      axis.  The default implementation returns @c false.  May be
      overridden by derived classes. */
  virtual bool xError () const;

  /** Returns true if the point representation displays error on the Y
      axis.  The default implementation returns @c false.  May be
      overridden by derived classes. */
  virtual bool yError () const;

  /** Sets the error representation on specified @a axis, if any, to
      @a yes. */
//  virtual void setErrorOn ( hippodraw::Axes::Type axis, bool yes = true );

//   /** Returns the value transform.  A value transform changes a binary
//       value to some graphical representations such as color. The
//       implementation in this base class returns a null pointer.
//       Derived classes that implement value transform should return a
//       pointer to the object it uses.
//   */
//   virtual  const BinToColor * getValueTransform ( ) const;

//   /** Sets the value transform.  The implementation in this base class
//       does nothing.  Derived classes that implement a value transform
//       should override this method and take possession of the received
//       object.

//       @todo Should have base class that doesn't know about color and
//       BinToColor class should derive from it.
//   */
//   virtual void setValueTransform ( BinToColor * );

//   /** Draws the projected values.  Draws the projected values
//       contained in the DataSource @a ntuple.  Each row of the %NTuple is
//       data point tuple of some dimension.  Derived classes expect the
//       DataSoruce to be of the correct dimension.  The drawn points
//       undergo a transform represented by @a transform and are draw to
//       the view @a view.
//   */
//   virtual void drawProjectedValues ( const DataSource * ,
// 				     TransformBase * ,
// 				     ViewBase *  ) = 0;


};
