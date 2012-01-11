/* -*- mode: c++ -*- */

/** @file

LogParabola  class interface

Copyright (C) 2004   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.

$Id: LogParabola.h,v 1.2 2006/05/10 03:05:05 cohen Exp $

*/

#ifndef _LogParabola_H_
#define _LogParabola_H_

#include "FunctionBase.h"

namespace hippodraw {

/** A function that can be used with a fitter. 
    The function represents the Log Parabola distribution:
 * \f[
 * \log{y} = log{N}-\alpha(\log{r})-\beta(\log{r})^2
 * \f]
    @author Johann Cohen-Tanugi <cohen@slac.stanford.edu>
    @author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
*/
class MDL_HIPPOPLOT_API LogParabola : public FunctionBase
{
private:
  enum { NORM, ALPHA, BETA};

protected:

  /** Initializes the function and parameter names. */
  virtual void initialize ();

  virtual double derivByParm ( int i, double x ) const;

public:

  /** The default constructor. */
  LogParabola ();

  /** This constructor takes the values of the parameters as an
      argument.
  */
  LogParabola ( double norm, double alpha, double beta);

  virtual FunctionBase * clone () const;

  /** The function call operator.  */
  virtual double operator () ( double x ) const;

  virtual void initialParameters ( const FunctionHelper * helper );

  virtual bool hasDerivatives () const;

};

} // namespace hippodraw

#endif // _LogParabola_H_
