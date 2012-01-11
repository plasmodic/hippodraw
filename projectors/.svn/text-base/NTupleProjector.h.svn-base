/* -*- mode: c++; -*-  */

/** @file

hippodraw::NTupleProjector class interface

Copyright (C) 2000-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: NTupleProjector.h,v 1.129 2006/04/12 20:45:53 xiefang Exp $

*/

#ifndef _NTupleProjector_H_
#define _NTupleProjector_H_

#include "ProjectorBase.h"

namespace hippodraw {

class DataSource;

/** An NTupleProjector is a projector that projects data from an
    DataSource object.  The projector connects to the data via a
    pointer to the @c DataSource and indexes to the columns that are
    used.  This method allows changing the projection to another
    DataSource while keeping the column indexes the same.

    This class supports the list of cuts and member functions for 
    testing if a row of the data table should be accepted based on
    those cuts. 

    The class implements the Observer and Observable pattern.  It is a
    Observer of the DataSource and responses to update messages from
    it by setting itself dirty and notifying its Observers.

    @author Jordi Burguet Castell <jordi.burguet-castell@cern.ch>
    @author Paul F. Kunz <paul_kunz@slac.stanford.edu>
    @author Sanket B. Malde <sanket@stanford.edu>
    @author Matan Shacham <matan@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
*/

class MDL_HIPPOPLOT_API NTupleProjector : virtual public ProjectorBase
{

  /** The type of container for list of cuts.
   */
  typedef std::vector < const TupleCut * > CutList_t;

  /** Set to @a false if any data is bound column is invalid.
   */
  mutable bool m_is_valid;

  /** Creates an empty NTuple with same number of columns and labels
      as the project's DataSource.
   */
  NTuple * createEmptyNTuple () const;

  /** Fills the DataSource source from the projectors DataSource
      object from rows that pass all the TupleCut objects in @a
      cut_list.
   */
  void fillNTuple ( NTuple * ntuple, const CutList_t & cut_list ) const;

protected:

  /** The list of binding options for the Projector.  The @em binding
      refers to a connection to the data.  For example, a 1D histogram
      binds to one axis and the option might be "X", while a scatter
      plot binds to two axes with the options "X" and "Y". The binding
      option is maintained as a @c string which can be interpreted by
      derived classes in various ways.  For example, a DataSource
      projector could take the string as a label for a column.
      Derived classes should initialize this data member in their
      constructor. */
  std::vector< std::string > m_binding_options;

  /** The list of current bindings. */
  mutable std::vector< std::string > m_bindings;

  /** A vector containing indexes to the columns of the
      DataSource.  The value UINT_MAX indicates that the column is not
      bound. Derived classes use these indexes in conjunction with
      m_binding_options. */
  std::vector< unsigned int > m_columns;

  /** A list of cuts that filter the projection. */
  CutList_t m_cut_list;

  /** The pointer to the data source  being projected.  It is a pointer
      because the same data can be shared by multiple projectors. */
  const DataSource * m_ntuple;

  /** The minimum number of columns that must be bound. The minimum
      number of columns that must be bound.  This member must be set
      by derived class according to its needs.  The default setting is
      zero.
   */
  unsigned int m_min_bindings;

  /** The following constructor takes the number of @a columns that
      the projector is capable of binding with.   */  
  NTupleProjector ( unsigned int columns );

  /** The copy constructor. */
  NTupleProjector ( const NTupleProjector & projector );

  /** Returns the range of data on the specified column.  The range
      may need to be calculated. */
  Range dataRange ( int column ) const;

  /** Returns a range considering both data and error. */
  Range dataRangeWithError ( int data, int error ) const;

  /** Returns the minimum positive value on the specified column. */
  double getPos ( int column ) const;

  /** Returns the minimum positive values considering both data and
      error. */
  double getPosWithError ( int data, int error ) const;

  /** Sets the axis binding for @a axis binding option to the column
      with label @a label.  If the @a label doesn't exist in the data
      source, then the axis becomes unbound.*/
  void setAxisBinding ( int axis, const std::string & label );

public:

  /** The destructor. */
  virtual ~NTupleProjector();

  /** Responds to update message from the data source.  Implementation
      of Observer pattern. */
  virtual void update ( const Observable * );

  /** Responds to willDelete message from the observed data source.
      Replaces the currently referenced data source with a null one.
      This is an implementation of the Observer pattern.
  */
  virtual void willDelete ( const Observable * );

  /** Returns a vector of strings describing the binding options.
   */
  virtual const std::vector< std::string > & bindingOptions () const;

  /** Returns the current axis bindings. Returns the label for each
      column to which the projector is bound.  If column is not bound,
      returns the string "nil" for that column.
  */
  virtual const std::vector < std::string > & getAxisBindings () const;


  /** Returns an index to the binding option.  The parameter @a axis
      should match one of the strings returned by binding Options().
      If not, an assertion is made. */
  unsigned int indexOfBindingOption ( const std::string & axis ) const;

  /** Returns the index of the @a label.  For example, the column
      index.
  */
  virtual int indexOf ( const std::string & label ) const;

  /** Sets whether the X error options are to be enabled or not. This
      base class implementation does nothing.*/
  virtual void setXErrorOption ( bool enable );

  /** Sets whether the Y error options are to be enabled or not. This
      base class implementation does nothing.*/
  virtual void setYErrorOption ( bool enable );

  /** Sets the axis binding.  The parameter @a axis should match one
      of the strings returned by bindingOptions(). The parameter @a
      label should match one of the column labels of the DataSource.  If
      not, the corresponding axis becomes unbound. */
  virtual void setAxisBinding ( const std::string & axis, 
				const std::string & label );

  /** Sets the axis bindings, Sets, in the order of the binding
      options, the binding to the DataSource by labels contained in @a
      labels. If @a labels does not contain sufficient number of
      entries, the remaining bindings are unset. 
  */
  virtual void setAxisBindings ( const std::vector < std::string > & labels );

  /** Changes the DataSource used for the projections to @a source.  The
      column indexes remain the same if the new %DataSource has at least as
      many columns as the old.  Any indexes exceeding the number of
      columns of the new DataSource are reset to 0. */
  virtual void setNTuple ( const DataSource * source );

  /** Returns the DataSource used by the projector. */
  virtual const DataSource * getNTuple () const;

  /** Returns the DataSource used by the projector. */
  virtual DataSource * getNTuple ();

  /** Returns the name of the DataSource used by this projector. */
  virtual const std::string & getNTupleName () const;

  /** This function is called when the DataSource has been changed to a
      new one. Derived classes must override this function and take
      appropriate action.  
  */
  virtual void changedNTuple() = 0;

  /** Returns the range of data on the specified axis.  The range may
      need to be calculated. */
  virtual Range dataRangeOn ( Axes::Type ) const = 0;

  /** Adds the cut to the list of cuts. */
  virtual void addCut ( const TupleCut * cut );

  /** Removes the cut from the cut list.
   */
  virtual void removeCut ( const TupleCut * cut );

  /** Returns the list of TupleCut objects used by this projector. */
  const std::vector < const TupleCut * > & getCutList () const;

  /** For row @a i of the column in the DataSource, returns @c true if all
      the cuts accept the row.
 */
  bool acceptRow ( unsigned int i, const CutList_t & cut_list ) const;

  /** Finds the title of the plot.  This implementation gets the title
      from the DataSource.
  */
  virtual const std::string & getTitle() const;

  virtual const std::string & getXLabel() const;
  virtual const std::string & getYLabel ( bool flag = false ) const;

  /** Returns the number of entries in to the projected values
      list. */
  int getNumberOfEntries () const;


  /** Returns the number of underflow. It is meaningless. */
  virtual int getUnderflow () const;

  /** Returns the number of overflow. It is meaningless. */
  virtual int getOverflow () const;
  
  /** Checks if the entries in a given row are within the range of the
      axis model. */
  virtual bool inRange ( int row ) const;

  /** Get the Average of all projected values on the specified axis. */
  virtual double getAverage ( Axes::Type axis ) const;

  /* Implements ProjectorBase. */
  virtual bool isEmpty () const;

  /* Create an DataSource given the present set of cuts.
   */
  virtual NTuple * getNTupleAfterCuts () const;

   /** Fill a vector with the data from the named column with the current
       set of cuts applied.
   */
   virtual void fillColumnAfterCuts(const std::string & column,
                                    std::vector<double> & columnData) const;

  virtual 
  NTuple * createNTupleWith ( const std::vector< TupleCut > & cut_list ) const;

  /** Returns @c true if all the data in the bound DataSource is valid.
   */
  bool isDataValid () const;

  /** Returns @c true.  Returns @c true as this projector or derived
      classes form it have DataSource binding.
  */
  virtual bool hasDataSourceBindings () const;

};

} // namespace hippodraw

#endif // _NTupleProjector_H_
