""" -*- python -*-

This example demonstrates applying a cut using numarray
Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: cuts_complex.py,v 1.2 2007/06/03 20:08:40 pfkeb Exp $

"""
from load_hippo import app, canvas

import hippo

num = None
try:
    import numpy as num
except :
    import numarray as num

#
# Gets the FITS controller which will do the reading of the file.
#
ntc = hippo.FitsController.instance()

#
# Get full path to FITS file in case this script is not run from this
# directory
#
import sys
full_path = sys.path[0] + '/' + 'SimEvents.fits'

#
# Find out what the names of the HDUs in the file.
#
hdus = ntc.getNTupleNames ( full_path )
print "Names of the Header/Data Units are..."
print hdus

events = ntc.createDataArray ( full_path, hdus[1] )


# Set matrix of ploats
canvas.setPlotMatrix ( 3, 3 )

from hippo import Display, Cut

#
# Create a 1D histogram
#
hist = Display ( "Histogram", events, ('GLON', ) )
canvas.addDisplay ( hist )

#
# First create cuts individually 
#
from hippo import Cut
cut1 = Cut ( events, ( 'zenith_angle', ) )
canvas.addDisplay ( cut1 )
cut1.setCutRange (  80., 100., 'x' )

#
# Create a another cut and set its range
#
cut2 = Cut ( events, ( 'energy', ) )
canvas.addDisplay ( cut2 )
cut2.setCutRange ( 1e3, 1e4, 'x' )
cut2.setLog ( 'x', True )
cut2.setLog ( 'y', True )

cuts = ( cut1, cut2 )
#
# Add the list of cuts to display
#
datarep = hist.getDataRep()
datarep.applyCuts ( cuts )
hist.applyCuts ( cuts )

#
# If applying only one cut one would do
# datarep.applyCut ( cut1 )
#
print "Added multiple cuts to one display"


#
# Now we use nummary's boolean arrays to create cut
#
hist2 = Display ( "Histogram", events, ('GLON', ) )
canvas.addDisplay ( hist2 )

# first cut
zenith = events['zenith_angle']
za = num.logical_and ( zenith > 80., zenith < 100. )

# second cut
energy = events['energy']
ec = num.logical_and ( energy > 1000., energy < 10000. )

# create cut and apply it
aand = num.logical_and (za, ec)
cutc = Cut ( events, 'complex cut', aand ,  hist2 )
canvas.addDisplay ( cutc )
cutc.setLog ( 'y', True )

print "Used Boolean numarrays to create cut"
