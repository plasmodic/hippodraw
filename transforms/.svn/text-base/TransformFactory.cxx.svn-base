/**@ file

TransformFactory class implementation

Copyright (C) 2001-2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: TransformFactory.cxx,v 1.43 2007/07/02 18:17:16 pfkeb Exp $

*/

// for wcslib
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef _MSC_VER
// for truncation warning
// for CLONE_DEFECT
#include "msdevstudio/MSconfig.h"
#endif //_MSC_VER

#include "transforms/TransformFactory.h"

// List of default Transforms
#include "HammerAito.h"
#include "Lambert.h"
#include "Mercator.h"
#include "Cartesian.h"
#include "GlobalSinusoidal.h"
#include "ARC.h"
#include "TAN.h"
#include "SIN.h"
#include "STG.h"
#include "AIR.h"
#include "LinearTransform.h"
#include "LogTransform.h"
#include "XYTransform.h"
#include "XYZTransform.h"

#include <iostream>

#include <cassert>

using std::cout;
using std::endl;
using std::string;

namespace hippodraw {

TransformFactory * TransformFactory::s_instance = 0;

TransformFactory::TransformFactory ()
{
}

TransformFactory * TransformFactory::instance ()
{
  if ( s_instance == 0 ) {
    s_instance = new TransformFactory ();
    s_instance->initialize ();
  }
  return s_instance;
}

void TransformFactory::initialize ()
{
  LinearTransform * lt = new LinearTransform ();
  add ( lt  );
  add ( new LogTransform () );
  add ( new XYTransform ( lt, lt, lt ) );
#ifdef HAVE_WCSLIB
  add ( new HammerAito ( lt ) );
  add ( new Lambert ( lt ) );
  add ( new Mercator ( lt ) );
  add ( new Cartesian ( lt ) );
  add ( new GlobalSinusoidal ( lt ) );
  add ( new ARC ( lt ) );
  add ( new TAN ( lt ) );
  add ( new SIN ( lt ) );
  add ( new STG ( lt ) );
  add ( new AIR ( lt ) );
#endif
}

/** @todo Fix it so that more than one white space could be in string. 
    @todo Check of DISTANCE_DEFECT workaround works or find better way.
*/
TransformBase *
TransformFactory::
createTransform ( const std::string & name )
{

  // size_type could be 32 or 64 bit unsigned int
  string::size_type pos = name.find_first_of ( " -" );

  if ( pos == string::npos ) { // no space, so regular name
    TransformBase * transform = prototype ( name );
    assert ( transform != 0 );
    return transform->clone ();
  }

  // else construct XYTransform
  string name1 = name.substr ( 0, pos );
  string name2 = name.substr ( pos+1 );

  string::size_type pos2 = name2.find_first_of ( " -" );

  if ( pos2 == string::npos ) { 
    // no space, so 2D transform. Z is linear
    // by default
    return createXY ( name1, name2, "Linear" );
  }

  string name21 = name2.substr ( 0, pos2 );
  string name22 = name2.substr ( pos2+1 );

  return createXY ( name1, name21, name22 );
}

TransformBase * TransformFactory::createXY ( const std::string & x,
					     const std::string & y,
					     const std::string & z )
{
  TransformBase * x_obj = prototype ( x );  
  TransformBase * y_obj = prototype ( y );
  TransformBase * z_obj = prototype ( z );

  UnaryTransform * x_unary = dynamic_cast < UnaryTransform * > ( x_obj );
  UnaryTransform * y_unary = dynamic_cast < UnaryTransform * > ( y_obj );
  UnaryTransform * z_unary = dynamic_cast < UnaryTransform * > ( z_obj );

  if ( x_unary == 0 || y_unary == 0 || z_unary == 0 ) {
    cout << "Could not create XYTransform" << x << " " 
	 << y << " " << z << endl;
    return 0;
  }

#ifdef CLONE_DEFECT
  UnaryTransform * xt 
	  = dynamic_cast < UnaryTransform * > ( x_unary->clone () );
  UnaryTransform * yt 
	  = dynamic_cast < UnaryTransform * > ( y_unary->clone () );
  UnaryTransform * zt 
	  = dynamic_cast < UnaryTransform * > ( z_unary->clone () );
#else
  UnaryTransform * xt = x_unary->clone ();
  UnaryTransform * yt = y_unary->clone ();
  UnaryTransform * zt = z_unary->clone ();
#endif

  return new XYTransform ( xt, yt, zt );
}

TransformBase * TransformFactory::createXYZ ( const std::string & x,
					      const std::string & y,
					      const std::string & z )
{
  TransformBase * x_obj = prototype ( x );  
  TransformBase * y_obj = prototype ( y );
  TransformBase * z_obj = prototype ( z );

  UnaryTransform * xut = dynamic_cast < UnaryTransform * > ( x_obj );
  UnaryTransform * yut = dynamic_cast < UnaryTransform * > ( y_obj );
  UnaryTransform * zut = dynamic_cast < UnaryTransform * > ( z_obj );

  if ( xut == 0 || yut == 0 || zut == 0 ) {
    cout << "Could not create XYZTransform"
	 << x << " " << y << " " << z << endl;
    return 0;
  }

#ifdef CLONE_DEFECT
  UnaryTransform * xt 
	  = dynamic_cast < UnaryTransform * > ( xut->clone () );
  UnaryTransform * yt 
	  = dynamic_cast < UnaryTransform * > ( yut->clone () );
  UnaryTransform * zt 
	  = dynamic_cast < UnaryTransform * > ( zut->clone () );
#else
  UnaryTransform * xt = xut->clone ();
  UnaryTransform * yt = yut->clone ();
  UnaryTransform * zt = zut->clone ();
#endif

  return new XYZTransform ( xt, yt, zt );
}

} // namespace hippodraw
