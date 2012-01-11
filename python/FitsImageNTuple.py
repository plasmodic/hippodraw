"""
Create an XYZ HippoDraw NTuple from a FITS image file.

"""
#
# $Id: FitsImageNTuple.py,v 1.3 2006/09/24 20:21:32 pfkeb Exp $
#
# Author: J. Chiang <jchiang@slac.stanford.edu>
#

import sys
import hippo, pyfits
from FitsAxis import *

def FitsImageNTuple( file, resample=2 ):
    #
    # open the image file
    #
    image = pyfits.open(file)
    #
    # The primary header contains the coordinate data
    #
    prihdr = image[0].header
    #
    # build the axis objects and arrays
    #
    xaxis = FitsAxis( "x-axis",
                      keywordVal(prihdr, 'crval1', 0),
                      keywordVal(prihdr, 'cdelt1'),
                      keywordVal(prihdr, 'naxis1'),
                      ctype = keywordVal(prihdr, 'ctype1', 'x'),
                      crpix = keywordVal(prihdr, 'crpix1') )
    yaxis = FitsAxis( "y-axis",
                      keywordVal(prihdr, 'crval2', 0),
                      keywordVal(prihdr, 'cdelt2'),
                      keywordVal(prihdr, 'naxis2'),
                      ctype = keywordVal(prihdr, 'ctype2', 'y'),
                      crpix = keywordVal(prihdr, 'crpix2') )
    xx = xaxis.array()
    yy = yaxis.array()
    #
    # create the image NTuple and register it with HippoDraw
    #
    nt = hippo.NTuple()
    ntc = hippo.NTupleController.instance()
    ntc.registerNTuple(nt)
    #
    x = []
    y = []
    z = []
    for i in range (0, xaxis.naxis, resample):
        for j in range (0, yaxis.naxis, resample):
            x.append(xx[i])
            y.append(yy[j])
            z.append(image[0].data[j][i])
    nt.addColumn(xaxis.ctype, x)
    nt.addColumn(yaxis.ctype, y)
    #
    # use a place-holder name for now
    #
    nt.addColumn("Z values", z)  
    return nt

