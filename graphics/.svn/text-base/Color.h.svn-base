/* -*- mode:c++ -*- */

/** @file

Color class interface

Copyright (C) 2000, 2001, 2003, 2005   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: Color.h,v 1.31 2006/09/29 21:33:41 xiefang Exp $
*/

#ifndef _Color_H_
#define _Color_H_

#include "pattern/libhippo.h"

#include <map>
#include <string>
#include <vector>

namespace hippodraw {

/** A Color class for creating the color object following the standard
    RGB color space.  This class represents color in a way similar to
    color classes of GUI tool kits such as Java and Qt.  It should,
    thus, be straight-forward to convert from one to the other.

    The compiler-generated copy constructor, destructor, and
    assignment operator are all valid and, thus, not explicitly
    implemented.

    @sa http://www.w3.org/pub/WWW/Graphics/Color/sRGB.html .

    @author Stephane Bonneaud <gandalf@slac.stanford.edu> 
*/
class MDL_HIPPOPLOT_API Color
{

public:

  /** A few common colors.
      A few common colors for use by clients.

      @note A few common colors are provided as a convenience to the
      programmer. They are better to use than constructing a Color by
      name because they are typed checked and take up less space.  At
      one point, it took over 5K byte more space in the core library
      if colors were created by name.
  */
  enum Value { red, green, blue, yellow, orange, cyan, magenta,
	       black, darkgray, lightgray, white };

 private:

  /** The type of container mapping string to Color object
   */
  typedef std::map < std::string, Color > ColorMapByName_t;

  /** The color by name map.
   */
  static ColorMapByName_t s_color_by_name;

  /** The list of available color names.
   */
  static std::vector < std::string > s_color_names;

  /** The red component of the color. */
  int m_red;

  /** The green component of the color. */
  int m_green;

  /** The blue component of the color. */
  int m_blue;

  /** Initializes the mapping of a name to Color object.
   */
  static void initColorByNameMap (); 

public:

  /** The following constructor takes red, green, and blue components as
  arguments.  The range of the values is 0 to 255. If the default
  values are taken, then the color will be black.*/
  explicit Color ( int red =  0, int green = 0, int blue = 0 );

  /** A constructor using Color::Value to set the color.
   */
  explicit Color ( Color::Value value );

  /** A constructor using a @a name to set the color.  A constructor
      using a @a name to set the color.  The @a name must match one of
      the values of the Color::Value enumeration, otherwise the color
      is set to Color::black.
   */
  explicit Color ( const std::string & name );

  /** Returns @c true if @a name is a valid color.
   */
  static bool isValid ( const std::string & name );

  /** Adds a named color to the color by name map.
   */
  static void addColor ( const char * name, const Color & color );

  /** Returns the list of color names.
   */
  static const std::vector < std::string > & colorNames ();

  /** Sets the color.  Sets the color by one of the known color names.
      If the @a name is not found in the color map, then sets the
      color to black.
   */
  void setColor ( const std::string & );

  /** Sets the color. */
  void setColor ( int red =  0, int green = 0, int blue = 0 );

  /** Sets the color.
   */
  void setColor ( Color::Value value );

  int getRed () const;

  int getGreen () const;

  int getBlue () const;

  /** Instead of getting a random color, this static method loop over
   *  the 5 common colors: red, green, blue, magenta and orange.
   */
  static Color::Value getColor();

  /** Color index for composite plotter.
   */
  static int colorIndex;

  bool operator==(Color c);

};

} // namespace hippodraw

#endif // _Color_H_
