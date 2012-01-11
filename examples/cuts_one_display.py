""" -*- python -*-

This example demonstrates applying mulitiple cuts to one display

Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: cuts_one_display.py,v 1.2 2006/02/22 20:07:00 pfkeb Exp $

"""
from load_hippo import app, canvas

import hippo

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

events = ntc.createNTuple ( full_path, hdus[1] )

print "Names of the columns"
labels = events.getLabels()
print labels

from hippo import Display

#
# Create a 1D histogram
#
hist = Display ( "Histogram", events, ('GLON', ) )
canvas.addDisplay ( hist )

#
# Create a cut and set its range
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
## datarep = hist.getDataRep()
## datarep.applyCuts ( cuts )
hist.applyCuts ( cuts )

#
# If applying only one cut one would do
# datarep.applyCut ( cut1 )
#
print "Added multiple cuts to one display"

