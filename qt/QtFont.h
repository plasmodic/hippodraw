/* -*- mode: c++ -*- */

/** @file

QtFont class interface. This is a an implementation of the FontBase
class which essentially makes it a wrapper around the QFont class.

Copyright (C) 2004, 2007   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

$Id: QtFont.h,v 1.10 2007/06/28 20:03:02 pfkeb Exp $

*/

#ifndef _QtFont_H_
#define _QtFont_H_

#include <qfont.h>

#include "graphics/FontBase.h"

namespace hippodraw {

/** An implementation of FontBase that uses a Qt QFont as implementation.

    @author Kaustuv <kaustuv@stanford.edu>
 */
class QtFont : public FontBase
{
private:

  /** The font object */
  QFont m_font;

  /** Keeps a track if the font is set of not. */
  bool m_flag;
  
public:

  /** The default constructor. */
  QtFont();
  
  /** Constructor which is called to set the family */
  QtFont( const std::string & family, int pointsize = 12,
	  int weight = QFont::Normal, bool italic = false );

  
  /** This constructor provides and way of creting QtFont from QFont */
  QtFont( const QFont& qfont );

  /** The destructor. */
  virtual ~QtFont();

  /** Get the family associated with the font. */
  virtual std::string family() const;
  
  /** Set the family associated with the font. */
  virtual void setFamily( const std::string & family );
  
  /** Get the pointsize of the font. */
  virtual int pointSize () const;
  
  /** Set the pointsize of the font. */
  virtual void setPointSize( int pointsize );
  
  /** Get the weight of the font. */
  virtual int weight () const;
  
  /** Set the weight of the font. */
  virtual void setWeight( int weight );
  
  virtual bool italic () const;
  
  /** Set the font to be italic */
  virtual void setItalic( bool enable );

  /** Get the font object held in here */
  virtual const QFont & font() const;

  /** Sets the font object of this class to be Null. i.e. programs using
      this class should have a logic built into it which now will start using
      default font for drawing. Sets the object m_flag as false. */
  void unsetFont();

  /** Set the font object of this class to be the one described by qfont */
  void setFont( const QFont & qfont );

  /** Is the font set ? */
  bool isSet();

};

} // namespace hippodraw

#endif //_QtFont_H_


 
