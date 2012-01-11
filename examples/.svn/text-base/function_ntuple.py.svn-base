""" -*- python -*-

   This script adding functions and fitting.  It also demonstrates
   retreiving an ntuple from the histogram to do something with its
   contents.

Author: Paul_Kunz@slac.stanford.edu

$Id: function_ntuple.py,v 1.6 2006/04/17 17:32:09 pfkeb Exp $nt

"""
import sys


from load_hippo import app, canvas

#
# Create NTuple with its controller so Inspector can see it.
#
from hippo import NTupleController
ntc = NTupleController.instance()
#
# Create full path to example file in case this script is not run from
# this directory.
#
full_path = sys.path[0] + '/' + 'aptuple.tnt'

nt1 = ntc.createNTuple ( full_path )

canvas.setPlotMatrix ( 2, 3 )
from hippo import Display

hist = Display ( "Histogram", nt1, ("Cost", ) )
canvas.addDisplay( hist )

# Get the data representation so we can add function to it.
datarep1 = hist.getDataRep()
from hippo import Function
gauss = Function ( "Gaussian", datarep1 )
gauss.addTo ( hist )

# Get the function parameters and display them.
print "Before fitting"
parmnames = gauss.parmNames ( )
print parmnames

parms = gauss.parameters ( )
print parms

# Now do the fitting.
gauss.fit ( )

print "After fitting"
parms = gauss.parameters ( )
print parms

# Add another function.
gauss1 = Function ( "Gaussian", datarep1 )
gauss1.addTo ( hist )

# Do another fit, should fit to linear sum
gauss1.fit ()


# Add Chi-squared per d.f. display
canvas.addTextRep ( hist, 'Chi-squared' )

# Create an NTuple from the histogram.
# Calculate the residuals

result = hist.createNTuple ()
ntc.registerNTuple ( result )

coords = result.getColumn ( 'Cost' )
values = result.getColumn ( 'Density' )
res = []
for i in range ( result.rows ) :
    x = coords[i]
    diff = values[i] - gauss1.valueAt ( x )
    res.append ( diff )

# Add a column and display it.
result.addColumn ( 'residuals', res )
resplot=Display ( "XY Plot", result, ( 'Cost', 'residuals', 'nil', 'Error' ) )
canvas.addDisplay ( resplot )

print "The histogram was fitted to the sum of two gaussians."
print "Then histogram bins were retrieved to calculate "
print "the residuals.  These were then plotted as an XY Plot."
print 'One could have used the "Create residuals display" button on the'
print "Inspector, but that wouldn't have demonstrated anything."


