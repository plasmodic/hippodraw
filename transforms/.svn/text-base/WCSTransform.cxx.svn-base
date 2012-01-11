/** @file

hippodraw::WCSTransform class implementation

Copyright (C) 2003-2006   The Board of Trustees of The Leland
Stanford Junior University.  All Rights Reserved.


*/

// for wcslib
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "transforms/WCSTransform.h"

#include "axes/AxisModelBase.h"
#include "graphics/Rectangle.h"
#include "UnaryTransform.h"

#include "wcslib/wcs.h"

#include <cmath>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace hippodraw;

using std::max;
using std::abs;
using std::vector;

/** Should never be called.. Sets the function name to "nil".
    Should be overriden by derived class.  If function name
    is "nil", then that indicated error condition that default
    constructor was called directly. 
*/
WCSTransform::WCSTransform() :
  PeriodicBinaryTransform()
{
}

WCSTransform::WCSTransform ( UnaryTransform * z,
						   bool is_periodic,
						   bool needs_grid,
						   bool needs_x_ticks,
						   bool needs_y_ticks,
						   double xlo, double xhi,
						   double xunits,
						   double ylo, double yhi,
						   double yunits) :
  PeriodicBinaryTransform ( z, is_periodic, needs_grid, needs_x_ticks,
    needs_y_ticks, xlo, xhi, xunits, ylo, yhi, yunits )
{
}

WCSTransform::
WCSTransform ( const WCSTransform & t ) :
  PeriodicBinaryTransform ( t )
{
}

WCSTransform::~WCSTransform ()
{
  if (m_wcs)
	wcsfree(m_wcs);
}

/* Use WCSLIB to do transform. */

void 
WCSTransform::
initwcs(const std::string &transformName, double* crpix, 
        double* crval, double* cdelt, double crota2, bool galactic)
{
    // Assert the sizeof wcsprm struct is smaller than the size allocated. 
    assert(sizeof(wcsprm)<=2000);
    m_wcs = reinterpret_cast<wcsprm*>(m_wcs_struct);
    m_wcs->flag = -1;

    // Call wcsini() in WCSLIB.
    int naxis = 2;
    wcsini(1, naxis, m_wcs);

    // Transfer parameters from c++ class to c stuct.
    std::string 
        lon_type = (galactic? "GLON-" : "RA---") + transformName,
        lat_type =  (galactic? "GLAT-" : "DEC--") + transformName;
    strcpy(m_wcs->ctype[0], lon_type.c_str() );
    strcpy(m_wcs->ctype[1], lat_type.c_str() );

    for( int i=0; i<naxis; ++i){
        m_wcs->crval[i] = crval[i]; // reference value
        m_wcs->crpix[i] = crpix[i]; // pixel coordinate
        m_wcs->cdelt[i] = cdelt[i]; // scale factor
    }
}

void 
WCSTransform::setLimitX(const Range &range)
{
        m_wcs->crpix[0] = (range.high() + range.low())/2.0
			  *UNITS_DEGREES/m_x_units;
        m_wcs->crval[0] = (range.high() + range.low())/2.0
			  *UNITS_DEGREES/m_x_units;
	m_wcs->flag = 0;

	PeriodicBinaryTransform::setLimitX(range);
}

void 
WCSTransform::setLimitY(const Range &range)
{
#if 0
        m_wcs->crpix[1] = m_y_units*(range.high() + range.low())/2.0
			  *UNITS_DEGREES/m_y_units;
        m_wcs->crval[1] = m_y_units*(range.high() + range.low())/2.0
			  *UNITS_DEGREES/m_y_units;
	m_wcs->flag = 0;
#endif

	PeriodicBinaryTransform::setLimitY(range);

}

/* virtual */
void
WCSTransform::
transform ( double & lon, double & lat ) const
{
    int ncoords = 1;
    int nelem = 2;
    double  imgcrd[2], pixcrd[2];
    double phi[1], theta[1];
    int stat[1];

    double worldcrd[] ={
	lon*UNITS_DEGREES/m_x_units,lat*UNITS_DEGREES/m_y_units
    };

    //    int returncode = 
    wcss2p(m_wcs, ncoords, nelem, worldcrd, phi, theta, imgcrd, pixcrd, stat);
    
    lon = pixcrd[0]/UNITS_DEGREES*m_x_units;
    lat = pixcrd[1]/UNITS_DEGREES*m_y_units; 
}


bool
WCSTransform::
inverseTransform ( double & lon, double & lat ) const
{
    int ncoords = 1;
    int nelem = 2;
    double worldcrd[2], imgcrd[2];
    double phi[1], theta[1];
    int stat[1];

    double pixcrd[] = {
	lon*UNITS_DEGREES/m_x_units,lat*UNITS_DEGREES/m_y_units
    };

    int returncode = 
    wcsp2s(m_wcs, ncoords, nelem, pixcrd, imgcrd, phi, theta, worldcrd, stat);

    // If there is invalid coordinate, return false.
    if ( returncode == 8 ) return false;

    // Make sure the inverse transform result in (-180,180) or (0,360)
    //if (worldcrd[0]<0) worldcrd[0]=worldcrd[0]+360;
    lon = worldcrd [0]/UNITS_DEGREES*m_x_units;
    lat = worldcrd [1]/UNITS_DEGREES*m_y_units;
	
    return true;
}

void
WCSTransform::
transform ( std::vector< double > & lon,
	    std::vector< double > & lat ) const
{     
  assert ( lat.size() == lon.size() );
  int size = lat.size();

  // Form vector to array.
  vector < double >  worldcrd ( 2*size );
  for ( int i = 0; i < size; i++ ) {
    worldcrd[2*i]= lon[i]*UNITS_DEGREES/m_x_units;
    worldcrd[2*i+1]= lat[i]*UNITS_DEGREES/m_y_units;
  }
  
  int ncoords = size;
  int nelem = 2;

  vector < double > pixcrd( 2*size );
  vector < double > imgcrd( 2*size );
  vector < double > phi( size );
  vector < double > theta( size );
  vector < int > stat ( size );

  //int returncode =
  wcss2p ( m_wcs, ncoords, nelem, &worldcrd[0], &phi[0], &theta[0],
	   &imgcrd[0], &pixcrd[0], &stat[0] );
  
  // From array to vector;  
  for ( int i = 0; i < size; i++ ) {
    lon[i]=pixcrd[2*i]/UNITS_DEGREES*m_x_units;
    lat[i]=pixcrd[2*i+1]/UNITS_DEGREES*m_y_units;
  }
}

