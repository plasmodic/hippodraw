/* -*- mode: c++ -*- */

/** @file

HiNTupleXML class interface

Copyright (C)  2002, 2004  The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: HiNTupleXML.h,v 1.20 2006/02/16 18:31:06 xiefang Exp $


*/

#ifndef _HiNTupleXML_H_
#define _HiNTupleXML_H_

#include "BaseXML.h"

namespace hippodraw {

class DataSource;

/** A singleton class that is does XML serialization and
    deserialization of derived classes of HiNTuple.

    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API HiNTupleXML : virtual public BaseXML
{

private:

  /** A private copy constructor in order to avoid copying. */
  HiNTupleXML ( const HiNTupleXML & );

  /** The tag name for the name attribute. */
  std::string m_name;

public:

  /** The only public constructor. */
  HiNTupleXML ( XmlController * );

  /** Returns a newly created XmlElement for @a DataSource. 
   */
  XmlElement * createElement ( const DataSource & ntuple );

  /** Returns the DataSource  corresponding to XmlElement.
   */
  const DataSource * getObject ( const XmlElement & tag ) const;

  /** Returns the DataSource corresponding to the filename.
   */
  const DataSource * getObject ( const std::string ) const;

  /** Returns the name of the HiNTuple object.
   */
  std::string getName ( const XmlElement & tag ) const;

};

} // namespace hippodraw

#endif // _HiNTupleXML_H_
