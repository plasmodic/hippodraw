/* -*- mode: c++ -*-
 *
 * HippoPlot DataRepFactory class interface
 *
 * Copyright (C) 2001-2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: DataRepFactory.h,v 1.18 2005/10/30 00:10:10 pfkeb Exp $
 *
 */

#ifndef _DataRepFactory_H_
#define _DataRepFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {
  class DataRep;

/** This Singleton class maintains a list of DataRep objects.  They can be
    looked up by their advertised name.  New objects are created by
    cloning existing ones. 
*/

class MDL_HIPPOPLOT_API DataRepFactory
  : public Factory < hippodraw::DataRep >
{

 private:

  /** The pointer to the singleton object. */
  static DataRepFactory * s_instance;

  /** A private copy constructor to avoid copying. */
  DataRepFactory ( const DataRepFactory & );

  /** Initializes the factory with a set of default DataReps. */
  void initialize ();

protected:

  /** The default constructor for avoiding creation except by itself or
      with derived classes. */
  DataRepFactory ( );

 public:

  /** The destructor.
   */
  ~DataRepFactory ();

  /** Returns the pointer to the singleton instance. */
  static DataRepFactory * instance ();

};

} // namespace hippodraw

#endif // _DataRepFactory_H_
