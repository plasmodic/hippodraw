/* -*- mode: c++ -*- */

/** @file

FontBase class interface. This is a interface class. All the concrete
implementation lie in other directories.

Copyright (C) 2004  The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: FontBase.h,v 1.7 2005/10/30 00:10:12 pfkeb Exp $

*/

#ifndef _FontBase_H_
#define _FontBase_H_

#include <string>

namespace hippodraw
{
  /** A namespace for weight constants.*/
  namespace FontWeight
  {
    enum weight{ Light, Normal, DemiBold, Bold, Black, Invisible };
  }

/** A abstract base class for font handling.

    @author Kautuv <kaustuv@stanford.edu>
*/
class FontBase
{
  
protected:
  
  /** The default constructor. */
  FontBase() {};
  
  /** Constructor which is called to set the family */
  FontBase ( const std::string & family, 
	     int pointsize = 12,
	     int weight = hippodraw::FontWeight::Normal, 
	     bool italic = false );

public:
  
  /** The destructor. */
  virtual ~FontBase(){};
  
  /** Get the family associated with the font. */
  virtual std::string family() const = 0;
  
  /** Set the family associated with the font. */
  virtual void setFamily( const std::string & family ) = 0;
  
  /** Get the pointsize of the font. */
  virtual int pointSize() const = 0;
  
  /** Set the pointsize of the font. */
  virtual void setPointSize( int pointsize ) = 0;
  
  /** Get the weight of the font. */
  virtual int weight() const = 0;
  
  /** Set the weight of the font. */
  virtual void setWeight( int weight ) = 0;
  
  /** Returns @c true if the font is italic, othwise returns @c false.
   */
  virtual bool italic () const = 0;
  
  /** Set the font to be italic */
  virtual void setItalic( bool enable ) = 0;
  
};

} // namespace hippodraw

#endif  //_FontBase_H_
