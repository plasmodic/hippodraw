// -*- mode:c++ -*-

/** @file

This is the SIP interface definition for DataRep

@note The commented out code requires exposure to more types to SIP.

Copyright (C)  2002, 2004, 2005  The Board of Trustees of  
The Leland Stanford Junior University.  All Rights Reserved.

This file is part of sihippo

$Id: DataRep.sip,v 1.11 2007/09/11 21:46:22 pfkeb Exp $

Author: Paul_Kunz@slac.stanford.edu

*/
/// class hippodraw::RepBase;

class DataRep
{
%TypeHeaderCode
#include "datareps/DataRep.h"
using hippodraw::DataRep;
using hippodraw::RepBase;
using hippodraw::NTuple;
using hippodraw::DataSource;
%End

protected:

//   /** A projector used to transform data to projected values that will
//       be plotted. A display may have multiple projectors that make up
//       its composite display.  This data member points to the one that
//       should receive messages from the display.  Projectors are owned
//       by the plotters that use them, and the projectors are responsible
//       for deleting the plotters.  */
//   ProjectorBase * m_projector;

//   /** There are times when a given DataRep results from some operations
//       performed on some other DataRep. The following variable contains pointer
//       to the original data rep*/
//   DataRep * m_parent_datarep;
  
  /** The default constructor. */
  DataRep ( );

  /** The copy constructor. */
  DataRep ( const DataRep & );
 

public:

 /** The destructor. */
  virtual ~DataRep();

  /** The clone function returns an object of its own kind which is a
      copy of this object. */
  virtual DataRep * clone () = 0;

  /** Returns the name of the display.   See m_name. */
  const std::string & name () const;

  /** Responds to update message from the data source. */
  virtual void update ( const Observable * );

  /** Returns @c true if the data representation is capable of error
      display on the data values.  The implementation in this base
      class returns @c false.  This member function should be
      overridden by derived classes capable of displaying errors on the
      data points.
  */
  virtual bool hasErrorDisplay ( ) const;

  /** Returns @c true if the %DataRep's projector can bind to ntuple.  The
      implementation in this base class is to return @c true. */
  virtual bool hasNTupleBindings () const;

  /** Sets the axis binding.  Sets binding of the axis with name @a
      axis to the NTuple column with label @a label.  If the derived
      class does not have %NTuple bindings, then does nothing.
      Derived classes may also do further processing upon reception
      of this method.
   */
  virtual void setAxisBinding ( const std::string & , 
				const std::string &  );

  /** Sets the axis bindings.  Sets the binding of all axis to the
      NTuple column with the labels in the vector.  If the derived
      class does not have %NTuple bindings, then does nothing.
      Derived classes may also do further processing upon reception
      of this method.
   */
  virtual
  void setAxisBindings ( const std::vector < std::string > &  );

  /** Transposes the X and Y axis.  If internal representation of the
      X-Y matrix is represented by a vector with shape, then transpose
      the X and Y axis.  Otherwise do nothing.

      @note The implementation passes the request to the contained
      projector.
  */
  virtual void matrixTranspose ( bool  );

//   /** Draws the projected values..  Each of the @ref
//       overview_projectedvalue objects will be drawn in @a view after a
//       coordinate transform represented by @a transform using its m_rep
//       object.  Uses its @ref overview_projector object to access the
//       projected values stored in a NTuple.  Some DataRep object may be
//       dependent on the projected value objects are returned in a
//       certain order.
//   */
//   virtual void drawProjectedValues ( TransformBase * , 
// 				     ViewBase *  );

//   /** Returns the smallest positive number from in the range of the
//       data. */
//   double getPosRange ( hippodraw::Axes::Type  );

  /** Returns the representation used to draw the data points. */
  RepBase * getRepresentation () const;

  /** Returns the color used for the representation. */
  const Color & getRepColor () const;

  /** Return @c true if the data representation has changed is some
      way and it needs to be re-drawn. */
  bool isDirty () const;

//   /** Returns true if the error bars are being displayed. */
//   bool isErrorDisplayed ( hippodraw::Axes::Type ) const;

  /** Returns the preferred range to display the data. 

//   @todo In asking for the preferred Range, should also tell binning
//   projectors to reset the number of bins so that auto ranging will
//   lead to same results as when plot was initially created.
//   */
//   Range preferredRange ( hippodraw::Axes::Type  );

//   /** Sets the AxisModel for axis @a axis. */
//   void setAxisModel ( hippodraw::Axes::Type , AxisModelBase * );

//   /** Sets the display of error bar, if applicable, on the specified
//       @a axis. */
//   void setErrorDisplay ( hippodraw::Axes::Type , bool );

  /** Sets the representation of the data points.  The @a pointrep
      replaces the existing one, and the existing one is then
      destroyed.  Maybe be overridden in derive class to do additional
      processing.
  */
  virtual void setPointRep ( RepBase *  );

  /** Sets the color of the data representation. */
  void setRepColor ( const Color & );

  /** Sets the size of the representation. */
  void setRepSize ( float  );

  /** Sets the style type of the point representation.
   */
  void setRepStyle ( int  );


  /** Sets the dirty flag.  When set, the display may need to
      reinitialize itself before the next drawing. */
  virtual void setDirty ( bool = true );

//   /** Sets the range of the @a axis.  The optional @a const_width
//       parameter if @c true, tells binning projectors to hold a
//       constant bin width parameter. */
//   void setRange ( hippodraw::Axes::Type, bool = true );


//   /** Returns the Projector object controlling the data. */
//   virtual ProjectorBase * getProjector ( ) const;

  /** Determines if extra space is needed for a color rep. */
  virtual bool needColorRep() const;

  /** Sets the data representation as selected.  Selection means the
      representations is drawn in its normal color, If representation
      is not selected, then it draws in a lighter shade.
  */
  virtual void setSelected ( bool = true );

  /** Returns @c true if data representation is in selected state.
      @sa setSelected
  */
  virtual bool isSelected ( ) const;

  /** Returns @c true if the DataRep accepts functions on @a number
      variables. The default implementation returns @c false.  Derived
      class should re-implement this member function if they accept
      functions of specified number of variables. 
  */
  virtual bool acceptFunction ( int  );

  /** Returns true if this DataRep has zero rows in its NTuple. Returns
      false if the projector is not an NTupleProjector. */
  virtual bool hasZeroRows ( );

  /** Add the data values to the DataRep.  This virtual method is
      provided as a convenience for those derived classes that respond
      to it.  Nothing is done if this object has NTuple bindings.
      Otherwise the message is forward to its ProjectorBase object.
  */
  virtual void addValues ( const std::vector < double > &  );

  /** Returns an ntuple representation of the projected data.
   */
  virtual const DataSource * getProjectedValues ();

   /** Returns the full NTuple but with the current set of cuts applied.
    */
   virtual NTuple * getNTupleAfterCuts () const;

  /** Returns the mean.  Returns the mean data points along the axis
      @a axis.
   */
  virtual double getMean ( const std::string &  );

  /** Returns the RMS.  Returns the root mean square of the data
      points along the axis @a axis. 
  */
  virtual double getRMS ( const std::string &  );

//   /** Returns @c true if the specified axis is binned, otherwise returns
//       @c false. 
//   */
//   bool isAxisBinned ( hippodraw::Axes::Type  ) const;

  /** Gets the parent DataRep */
  virtual DataRep * getParentDataRep(); 

  /** Sets the parent DataRep */
  virtual void setParentDataRep( DataRep * );

  /** Updates the data representation. Updates the data
      representation, however, derived classes are allowed to be lazy
      and not do the actual update until needed by
      drawProjectedValues.
  */
  virtual void update ();

  /** Normalizes the receiving object to the @a target one.
   */
  virtual void normalizeTo ( const DataRep *  );

//   /** Sets the bin width, if binned, on specified @a axis to @a
//       width.
//   */
//   void setBinWidth ( hippodraw::Axes::Type , double  );

  /** Returns the number of entries used for the DataRep object.
   */
  int getNumberOfEntries () const;

  /** Returns @c true if the receiving object is target-able.  Most
      derived classes of DataRep are target-able and return @c true.
      That is, they can be targets of cuts, functions, etc.  If a
      derived class is not intended to be a target, it should return
      @a false.  The implementation in this base class returns @c
      true.
  */
  virtual bool isTargetable () const;

//   /** Returns @c true if the data representation has specified axis
//       type @a axis
//    */
//   virtual bool hasAxis ( hippodraw::Axes::Type axis ) const = 0;



};
