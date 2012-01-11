""" -*- python -*-
This script demonstrates using FITS file as data source.

Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: mainpage.py,v 1.6 2005/11/22 22:23:53 pfkeb Exp $

"""
from load_hippo import app, canvas

from hippo import FitsController

#
# Gets the FITS controller which will do the reading of the file.
#
ntc = FitsController.instance()

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
# Create the  displays.
#

hist = Display ("Histogram", events, ('energy', ) )
hist.setLog ( 'x', True )
hist.setLog ( 'y', True )
canvas.addDisplay( hist )

color = Display ( "Color Plot", events, ('GLON', 'GLAT' ))
color.setRange ( 'z', 0.5, 150. )
color.setLog ( 'z', True )
canvas.addDisplay ( color )

contour = Display ( "Contour Plot", events, ('time', 'GLON' ))
canvas.addDisplay ( contour )

profile = Display ( "Profile", events, ('time', 'energy' ))
canvas.addDisplay ( profile )

print "Created the plots shown on the HippoDraw main page."

