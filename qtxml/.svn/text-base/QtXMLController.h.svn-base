/* -*- mode: c++ -*-
 * 
 * HippoPlot QtXMLController class interface
 *
 * Copyright (C)  2002  The Board of Trustees of The Leland Stanford
 * Junior University.  All Rights Reserved.
 *
 * $Id: QtXMLController.h,v 1.54 2005/10/30 00:10:16 pfkeb Exp $
 *
 *
 */

#ifndef _QtXMLController_H_
#define _QtXMLController_H_

#include "qt/libqthippo.h"


#ifdef _MSC_VER
#include "msdevstudio/MSconfig.h"
#include "datasrcs/NTuple.h"
#include <qdom.h>
#endif

#include "xml/XmlController.h"

class QDomDocument;
class QDomElement;

namespace hippodraw {

/** A singleton class that is does XML serialization and
    deserialization with Qt XML module.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/

class MDL_QTHIPPOPLOT_API QtXMLController : public XmlController
{

private:

  /** The pointer to the singleton object. */
  static QtXMLController * s_instance;

  /** A private copy constructor in order to avoid copying. */
  QtXMLController ( const QtXMLController & );

protected:

  /** A default constructor for avoiding creation except by itself or
      with derived classes. */
  QtXMLController();

  virtual bool fileExists ( const std::string & filename ) const;
  virtual void newDocument ( const std::string & name );

public:

  /** Returns the pointer to the singleton instance. */
  static QtXMLController * instance ();

};

} // namespace hippodraw

#endif // _QtXMLController_H_
