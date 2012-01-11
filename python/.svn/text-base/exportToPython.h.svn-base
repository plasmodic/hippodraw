/* -*- mode: c++ -*- */

/** @file

Declaration of functions exporting HippoDraw classes to Python with
Boost.Python

Copyright (C) 2002-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: exportToPython.h,v 1.58 2007/06/07 18:16:05 pfkeb Exp $

*/

#ifndef _ExportToPython_H_
#define _ExportToPython_H_

namespace hippodraw {

/** A namespace encapsulating functions to interface HippoDraw classes
    to %Python using Boost.Python.  For the list of C++ member
    functions that are exported, see which functions are referenced by
    the functions declared in this name space.

    @request On Mac OS X, get lots of warning messages about use of
    long double, must have a gcc flag to fix that.

    @request Write m4 macro to allow user to fix path to Python
    includes and lib.

    @request Consider using ax_boost_python.m4 from autoconf macro archive.

    @request Consider other Python finding m4 macros from the autoconf macro archive.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matthew Langston <langston@slac.stanford.edu>
*/
namespace Python
{
  /** Exports @b HippoDraw C++ classes to Python.  Call this function
      from within a BOOST_PYTHON_MODULE definition to incorporate @b
      HippoDraw classes within that module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Matthew Langston <langston@slac.stanford.edu>
    @author Selim Tuvi <stuvi@slac.stanford.edu>
   */
  void exportToPython ();

  /** Exports the CanvasWindow class to Python.  Directly exports
      CanvasWindow to Python.  There is no locking and unlocking of
      QApplication singleton object, so this object must be
      instantiated from the same thread as the QApplication object.
  */
  void export_CanvasWindow();

  /** Exports the DataSource class to Python.  Directly exports the
      DataSource class to Python.  There is no locking and unlocking
      of the QApplication singleton object.  However, since invoking
      member functions of this class do not directly change the GUI,
      they may be invoked from any thread.

      @request Add getShape(int) function to allow user to get shape
      of a column.
  */
  void export_DataSource ();

  /** Exports the ListTuple class to Python.  Directly exports ListTuple
      to Python.  There is no locking and unlocking of the QApplication
      singleton object.  However, since invoking member functions of
      this class do not directly change the GUI, they may be invoked
      from any thread. 
  */
  void export_ListTuple();

  /** Exports the NTuple class to Python.  Directly exports NTuple
      to Python.  There is no locking and unlocking of the QApplication
      singleton object.  However, since invoking member functions of
      this class do not directly change the GUI, they may be invoked
      from any thread. 
  */
  void export_NTuple();

  /** Exports the PyNTuple class to Python.  Directly exports PyNTuple
      to Python.  There is locking and unlocking of the QApplication
      singleton object where needed.
  */
  void export_PyNTuple();

  /** Exports the CircularBuffer class to Python.  Directly exports
      CircularBuffer to Python.  There is no locking and unlocking of
      the QApplication singleton object.  However, since invoking
      member functions of this class do not directly change the GUI,
      they may be invoked from any thread.
      
      @bug @@@@ Probably needs wrapper for application lock
  */
  void export_CircularBuffer();

  /** Exports the NTupleController class to Python.
  
      @request Provide interface to DataSourceController.
  */
  void export_NTupleController ();

  /** Exports CutContoller class to Python. */
  void export_CutController();

  /** Exports the Observer class to Python.
   */
  void export_Observer ();

  /** Exports the QtDisplay class to Python. */
  void export_QtDisplay();

  /** Exports the QtCut class to Python.  */
  void export_QtCut();

  /** Exports the PyFunctionRep class to Python. */
  void export_Function();

  /** Exports the PyCanvas class to Python. */
  void export_Canvas();

  /** Exports the PyApp class to Python. */
  void export_HDApp();

  /** Exports the PyDataRep class to Python. */
  void export_DataRep();

  /** Exports the RepBase class to Python. */
  void export_RepBase ();

  /** Exports the template base class of PointRepFactory. 
   */
  void export_Factory_PointRep ();

  /** Exports the PointRepFactory class to Python. 
   */  
  void export_PointRepFactory ();

  /** Exports the NumArrayTuple class to Python. 
   */
  void export_NumArrayTuple ();

  /** Exports the RootController.
   */
  void export_RootController();

  /** Exports the RootNTuple.
   */
  void export_RootNTuple();

  /** Exports the FitsController.
   */
  void export_FitsController ();

  /** Exports the FitsNTuple.
   */
  void export_FitsNTuple ();

  /** Exports the FitterFactory.
   */
  void export_FitterFactory ();

  /** Exports the Fitter class to Python.
   */
  void export_Fitter ();

  /** Exports the FitterFactory.
   */
  void export_FunctionFactory ();

  /** Export the base class of objective functions.
   */
  void export_FCNBase ();

  /** Export the base class of stated objective functions.
   */
  void export_StatedFCN ();

  /** Exports the base class of data source objective functions.
   */
  void export_NTupleFCN ();

  /** Export the FunctionBase and its FunctionWrap.
   */
  void export_FunctionBase ();

   /** Export PyDataSourceWrapper.
    */
   void export_DataArray ();

  /** Export hippodraw::Symbo::lType enum.
   */
  void export_SymbolType ();

  /** Export hippodraw::LineStyle enumeration.
   */
  void export_LineStyle ();

  /** Exports to Python the Color class and its enumerations.
   */
  void export_Color ();

} // namespace Python
} // namespace hippodraw

#endif // _ExportToPython_H_
