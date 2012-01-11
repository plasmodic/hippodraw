/* -*- mode: c++ -*- */

/** @file

hippodraw::ContourPointRep class interface

Copyright (C) 2003-2006   The Board of Trustees of The Leland Stanford Junior
University.  All Rights Reserved.

$Id: ContourPointRep.h,v 1.36 2007/09/05 18:04:45 pfkeb Exp $

*/

#ifndef _ContourPointRep_H_
#define _ContourPointRep_H_

#include "LinePointRep.h"

namespace hippodraw {

class BinToColor;

/** A PointRep class that draws a contour lines.

    The ContourPointRep transforms a two dimension matrix of projected
    values into a set of colored contour levels.  The algorithm used
    is adapted from <a
    href="http://local.wasp.uwa.edu.au/~pbourke/papers/conrec/index.html">

    CONREC</a>,
    which is a contouring Fortran 77 subroutine written by <a
    href="http://astronomy.swin.edu.au/~pbourke/">Paul Bourke</a>, of
    the <a href="http://astronomy.swin.edu.au/">Center for
    Astrophysics and Supercomputing</a>, Swinburne University of
    Technology in Melbourne, Australia.

    @note This class inherits from LinePointRep to share handling of
    the lines at the edges of the drawing rectangle.

    @bug @@ Contours should be smoother.  Try function2d.py to see
    contours of double Gaussian.

    @author Xie Fang xiefang @@ stanford.edu
    @author Sanket B. Malde  sanket @@ stanford.edu
    @author Tao Xu taox @@ stanford.edu
*/
  class MDL_HIPPOPLOT_API ContourPointRep : public LinePointRep
{

private:
  
  /** The color scale generator. */
  BinToColor * m_bin_to_color;
  
  /** The vector of colors used for drawing the lines. */
  std::vector< Color > m_colorvec;

  /** The vector of line style used for drawing the lines. Only 
      used for the line style color map.
  */
  std::vector < Line::Style > m_stylevec;

  /** The number of contours to be drawn. */
  int m_numContours;

  /** The max Z value of the data. */
  double m_maxValue;
  
  /** The min Z value of the data. */
  double m_minValue;

  /** The min positive Z value of the data. For log scale.*/
  double m_minPos;

  /** The vector of contour values. */
  std::vector < double > m_values;

  /** A boolean which is true if we are using user input contour
      values. */
  bool m_usingUserValues;


public:

  /** The default constructor. */
  ContourPointRep();
  
  /** Constructor that takes size as argument. */
  ContourPointRep ( float size );
  
  /** The copy constructor. */
  ContourPointRep( const ContourPointRep & point_rep );

  /** The destructor. */
  ~ContourPointRep();

  /** The clone function returns an object of its own kind which
      is a copy of this object at this moment. */
  virtual RepBase * clone();

  virtual const BinToColor * getValueTransform ( ) const;
  virtual void setValueTransform ( BinToColor * );
  virtual void drawProjectedValues ( const DataSource * ntuple,
				     TransformBase * transform,
				     ViewBase * view );

  void drawContourTicks ( const TransformBase &, 
			  ViewBase &,
			  const std::vector < double > & );

  /** Get the m_usingUserValues boolean. */
  bool getUsingUserValues () const;

  /** Set the m_usingUserValues boolean */
  void setUsingUserValues ( bool flag );

  /** Set number of contours. */
  void setNumContours ( int i );

  /** Returns the number of contours. */
  int getNumContours () const;

  /** Set user defined contour values. */
  void setContourValues ( std::vector < double > & values,
			  ProjectorBase * proj );

private:

/*
Copyright (c) 1996-1997 Nicholas Yue

This software is copyrighted by Nicholas Yue. This code is base on the work of
Paul D. Bourke CONREC.F routine

The authors hereby grant permission to use, copy, and distribute this
software and its documentation for any purpose, provided that existing
copyright notices are retained in all copies and that this notice is included
verbatim in any distributions. Additionally, the authors grant permission to
modify this software and its documentation for any purpose, provided that
such modifications are not distributed without the explicit consent of the
authors and that existing copyright notices are retained in all copies. Some
of the algorithms implemented by this software are patented, observe all
applicable patent law.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/


  /** The method that actually calculates the contour lines. 

      The implementation is modified version of Nicholas Yue's C++
    adaption of CONREC, to use STL vector instead of a double **
    array.

    @author Nicholas Yue

  */
  void createContours (  const DataSource * ntuple,
			 const TransformBase * transform );
  
  /** Returns the z value where the @em i-th level contour is to be
      drawn. */
  double getContour ( int i, const TransformBase * transform );

  /** Returns the intersection of the segment given by h[p1] and h[p2]
      with the segment given by xh[p1] and xh[p2]. */
  double intersect ( int p1, int p2, double * h, double * xh );
  
  /** Set the min and max Z values of the data being plotted. 
   */
  void setMinMax ( const DataSource * );

  /** Prepare the vector of contour values. 
   */
  void setContourVector ( const TransformBase * transform );

  /** Initialization member function used by constructors. 
   */
  void init ();  
    


private:

  /** Does the actual drawing of the contour and the contour ticks
   */
  virtual void drawValues ( const TransformBase &, 
			    ViewBase & vb );

  /** Does the actual drawing using different line styles. Use the
      line style vector m_stylevec.
  */
  virtual void drawValuesWithStyle ( const TransformBase &,
				     ViewBase & vb );

};

} // namespace hippodraw

#endif // _ContourPointRep_H_
