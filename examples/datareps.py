""" -*- python -*-

This script demonstrates  the creation of a displays with multiple data reps

Author: Paul_Kunz@slac.stanford.edu

$Id: datareps.py,v 1.10 2005/07/30 16:31:57 pfkeb Exp $

"""

from load_hippo import app, canvas

from hippo import NTupleController
# Create NTuple with its's controller.
ntc = NTupleController.instance()

#
# create full path to the file in case this script is not run from this
# directory
#
import sys
full_path = sys.path[0] + '/' + 'aptuple.tnt'
nt = ntc.createNTuple ( full_path )

from hippo import Display

# Create a histogram
hist = Display ("Histogram", nt, ('Age', ) )
canvas.addDisplay( hist )

# Overlay another histogram.
hist.addDataRep ( 'Histogram', nt, ['Service'] )

#
# Set the line style and color
#
reps = hist.getDataReps ()
from hippo import Line
print "The available line styles are ..."
print Line.values
print ""
try :
    reps[1].set ( Line.dash )
except RuntimeError, detail :
    print detail
print "done"

#
# Set the color
#
from hippo import ColorValue
print "The available color values are ..."
print ColorValue.values
print ""
reps[1].set ( ColorValue.red )

reps[0].setBinWidth ( 'x', 1. )
reps[1].setBinWidth ( 'x', 2. )

full_path = sys.path[0] + '/' + 'Z0Strip.data'
nt2 = ntc.createNTuple ( full_path )

# Create a strip chart display.
strip = Display ( "Strip Chart", nt2, ('Energy', 'Sigma' ))
canvas.addDisplay ( strip )

# Overlay strip chart with an XY plot.
strip.addDataRep ( "XY Plot", nt2, ['Energy', 'Sigma', 'nil', 'error'] )

#
# Get the second data representation to set its symbol and size
#
reps = strip.getDataReps ()
from hippo import Symbol
print "The available symbols are ..."
print Symbol.values
print ""
print "about to try"
try :
    reps[1].set ( Symbol.opencircle )
    print "set symbol"
    reps[1].setSize ( 8. )
except RuntimeError, detail :
    print detail

# Create 2D histogram as contour plot.
color = Display ( "Contour Plot", nt, ('Age', 'Cost' ))
color.setBinWidth ( 'x', 1.0 )
canvas.addDisplay ( color )
print "created contour display"
# Overlay it with 1D profile plot.
color.addDataRep ( 'Profile', nt, [ 'Age', 'Cost' ] )

print "Overlayed various kinds of data representations"

