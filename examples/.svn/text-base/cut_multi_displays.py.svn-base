""" -*- python -*-

This example demonstrates applying one cut to muliple displays.

Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: cut_multi_displays.py,v 1.4 2006/02/27 19:02:37 pfkeb Exp $

"""
import hippo
from load_hippo import app, canvas


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
# Create a 1D histograms
#
hist1 = Display ( "Histogram", events, ('GLON', ) )
canvas.addDisplay ( hist1 )

hist2 = Display ( "Histogram", events, ('GLAT', ) )
canvas.addDisplay ( hist2 )

#
# Create a cut
#
cut = hippo.Cut ( events, ( 'zenith_angle', ) )
canvas.addDisplay ( cut )
cut.setCutRange ( 80., 90., 'x' )
#
# Add this cut to multiple displays ( a Python tuple of displays ).
#
cut.addTargets ( ( hist1, hist2 ) )

# if only one target than
# >>> cut.addTarget ( hist1 )
#
print "Added one cut to mulitple displays"
print "Now try manipulating the cut range,"
print "use yellow buttons on the tools bar to select the mode."
