#
# $Id: FitsAxis.py,v 1.5 2006/09/24 20:21:32 pfkeb Exp $
#
# Author: J. Chiang <jchiang@slac.stanford.edu>
#
import sys, os
import numarray, pyfits

class FitsAxis:
    """
    Encapsulate the most basic keyword data required to define a FITS
    image axis:
       CRVALn The coordinate value at the reference pixel.
       CDELTn The pixel "size" at the reference pixel.
       NAXISn The number of pixels spanning the n-th dimension.
       CTYPEn The type of the coordinate, e.g., "RA---TAN",
              "GLON-CAR". Note that this keyword also often contains the
              projection info.
       CRPIXn The reference pixel number.  This is a float and can have
              non-whole number value.
    """
    def __init__(self, name, naxis=1, crval=0, cdelt=1,
                 ctype = "none", crpix = 1):
        if ( type(name) == type(" ") ):
            #
            # These values are being set by hand, 
            #
            self.name = name
            self.crval = crval
            self.cdelt = cdelt
            self.naxis = naxis
            self.ctype = '%s' % ctype
            self.crpix = crpix
        else:
            #
            # Assume this is a primary header and grab the keyword data,
            # but check anyways
            #
            if (name.__doc__ != "FITS header class."):
                print "This is not a FITS header."
                return
            axis = "%s" % naxis
            self.name = keywordVal(name, "CTYPE"+axis, "axis %s" % naxis)
            self.crval = keywordVal(name, "CRVAL"+axis, 0)
            self.cdelt = keywordVal(name, "CDELT"+axis, 1)
            self.naxis = keywordVal(name, "NAXIS" + axis)
            self.ctype = self.name
            self.crpix = keywordVal(name, "CRPIX"+axis, 1)
        self.array = ((numarray.arange(self.naxis) + 1 - self.crpix)*self.cdelt
                      + self.crval)
    def updateFitsHeader(self, header, axis):
        axis = '%s' % axis
        header.update('CRVAL' + axis, self.crval,
                      'reference value for ' + self.name + ' coordinate')
        header.update('CDELT' + axis, self.cdelt,
                      'step in ' + self.name + ' coordinate')
        header.update('CRPIX' + axis, self.crpix,
                      'reference pixel for ' + self.name + ' coordinate')
        header.update('CTYPE' + axis, self.ctype, 'units for ' + self.name)

def keywordVal(header, keyword, default=1):
    """
    A 'safe' way of reading the keyword values from a pyfits header
    object
    """
    try:
        value = header[keyword]
    except KeyError:
        value = default
    return value

if __name__ == "__main__":
    fitsobj = pyfits.HDUList()
    hdu = pyfits.PrimaryHDU()
    x = FitsAxis("x-axis", 10, 0., 0.2)
    y = FitsAxis("y-axis", 10, 0., 0.1)
    x.updateFitsHeader(hdu.header, 1)
    y.updateFitsHeader(hdu.header, 2)
    hdu.data = numarray.zeros((y.naxis, x.naxis))    # note the ordering
    fitsobj.append(hdu)
    os.system("rm -f test.fits")
    fitsobj.writeto('test.fits')
