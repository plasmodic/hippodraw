/* -*- mode: c++ -*- */

/** @file

FitterFactory class interface

Copyright (C) 2003   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: FitterFactory.h,v 1.4 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _FitterFactory_H_
#define _FitterFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class Fitter;

/** A Factory singleton class for creating objects whose class derives
    from Fitter.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
 */
class MDL_HIPPOPLOT_API FitterFactory : public Factory <Fitter>
{

 private:

  /** The application's default fitter, if non empty.
   */
  std::string m_default;

  /** A pointer to the singleton object. */
  static FitterFactory * s_instance;

  /** A private copy constructor to prevent copying. */
  FitterFactory ( const FitterFactory & );

  /** Initializes the factory with a set of default functions. */
  void initialize ();

protected:

  /** A default constructor for prevent creation except by the
      itself or the derived classes. */
  FitterFactory();

 public:

  /** Returns a pointer to the singleton instance. */
  static FitterFactory * instance ();

  /** Sets the application's default Fitter.  Sets the application's
      default fitter, if the @a name exists, otherwise throws a
      std::runtime_error exception.
  */
  void setDefault ( const std::string & name );

  /** Returns the default Fitter.
   */
  const std::string & getDefault () const;

};

} // namespace hippodraw

#endif // _FitterFactory_H_
