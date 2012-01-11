/*  -*- mode: c++ -*-
 *
 * HippoPlot TransformFactory class interface
 *
 * Copyright (C) 2000, 2001, 2003   The Board of Trustees of The Leland
 * Stanford Junior University.  All Rights Reserved.
 *
 * $Id: TransformFactory.h,v 1.16 2005/10/30 00:10:18 pfkeb Exp $
 *
 */

#ifndef _TransformFactory_H_
#define _TransformFactory_H_

#include "pattern/Factory.h"

namespace hippodraw {

class TransformBase;

/** The singleton Factory class for creating objects whose class is
    derived from TransformBase.   

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
    @author Oded Wurman <owurman@stanford.edu>
 */

class MDL_HIPPOPLOT_API TransformFactory : public Factory < TransformBase >
{

 private:

  /** A pointer to the singleton object. */
  static TransformFactory * s_instance;

  /** A private copy constructor to prevent copying. */
  TransformFactory( const TransformFactory & );

  /** A default constructor for prevent creation except by itself. */
  TransformFactory();

  /** Initialize the factory with a set of default Transforms. */
  void initialize ();

  /** Creates an XYTransform object from independent X, Y and Z
      transforms designated by the names. */
  TransformBase * createXY ( const std::string & x_trans, 
			     const std::string & y_trans,
			     const std::string & z_trans );

 public:

  /** Returns a pointer to the singleton instance. */
  static TransformFactory * instance ();

  /** Creates a returns a new TransformBase object with given name.
      If the name contains a non-alphanumeric character, then an
      XYTransform is constructed by parsing the two alphanumeric parts
      of the name as names of UnaryTransform names. 

      Note that this functions name differs from the other Factory
      function nams becasue I (pfkeb) couldn't figure out how to
      override the function and yet be able to call the base class
      function. 

      @attention Do not try to use the given name first and if it
      fails catch the exception and parse the name for space.  Doing
      so will break the Java version based on jdk-1.3.1 because of
      apparent incompatiblities in exception handling between the
      version of gcc used to compile the Java VM and gcc 2.95.3.
  */
  TransformBase * createTransform ( const std::string & name );

  /** Creates an XYZTransform object from independent X, Y and Z
      transforms designated by the name. */
  TransformBase * createXYZ ( const std::string & x_trans, 
			      const std::string & y_trans,
			      const std::string & z_trans );

};

} // namespace hippodraw

#endif // _TransformFactory_H_
