/* -*- mode: c++ -*- */

/** @file

QtRootController class interface.

Copyright (C) 2004, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: QtRootController.h,v 1.11 2007/03/27 18:53:06 pfkeb Exp $

*/

#ifndef _QtRootController_H_
#define _QtRootController_H_

#include <string>
#include <vector>

namespace hippodraw {

class RootController;
class DataSource;
class PyDataSource;
/** A wrapper for RootController, so that when RootController is used
    in Qt based application, the application object can be locked and
    unlocked before calling any ROOT functions.  ROOT is not thread
    safe, so this is necessary to avoid the application thread and
    Python thread from clashing.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class QtRootController 
{

private:

  /** The singleton QtRootController instance. */
  static QtRootController * s_instance;

  /** The singleton RootController object. */
  static RootController * m_instance;

  /** The default constructor, protected from being used. */
  QtRootController ();

  /** The copy  constructor, protected from being used. */
  QtRootController ( const QtRootController &);

public:

  /** The singleton instance method.
   */
  static QtRootController * instance ();

  /** Creates a QtRootNTuple wrapper for RootNTuple object.
   */
  DataSource * createNTuple ( const std::string & filename, 
			      const std::string & treename );

  /** Creates a PyDataSource wrapper to the RootNTuple object.
   */
  PyDataSource * createDataArray ( const std::string & filename,
				   const std::string & treename );

  /** Returns the names of the top level tree in the Root file.
   */
  const std::vector < std::string > & 
    getNTupleNames ( const std::string & filename );

};

} // namespace hippodraw

#endif // _QtRootController_H_
