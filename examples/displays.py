""" -*- python -*-
This script demonstrates various types of Displays.

Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: displays.py,v 1.10 2006/09/15 17:12:11 pfkeb Exp $

"""
from load_hippo import app, canvas

from hippo import NTupleController

#
# Get full path to examplefile in case this script is not run from this
# directory.
#
import sys
full_path = sys.path[0] + '/' + 'aptuple.tnt'


# Read an ASCII NTuple file to create an NTuple.
ntc = NTupleController.instance()
nt1 = ntc.createNTuple ( full_path )

from hippo import Display

# Change the matrix from the default
canvas.setPlotMatrix ( 3, 4 )

# Create the  displays.

hist = Display ("Histogram", nt1, ('Cost', ) )
canvas.addDisplay( hist )

profile = Display ( "Profile", nt1, ('Age', 'Cost' ))
canvas.addDisplay ( profile )

color = Display ( "Color Plot", nt1, ('Age', 'Cost' ))
color.setBinWidth ( 'x', 1.0 )
canvas.addDisplay ( color )

contour = Display ( "Contour Plot", nt1, ('Age', 'Cost' ))
contour.setBinWidth ( 'x', 1.0 )
canvas.addDisplay ( contour )

profile2D = Display ( "Profile 2D", nt1, ('Age', 'Cost', 'Service' ))
canvas.addDisplay ( profile2D )

profileC = Display ( "Profile Contour", nt1, ('Age', 'Cost', 'Service' ))
canvas.addDisplay ( profileC )

scatter = Display ( "Scatter Plot", nt1, ('Age', 'Cost' ))
canvas.addDisplay ( scatter )

#
# Get full path to examplefile in case this script is not run from this
# directory
#
full_path = sys.path[0] + '/' + 'Z0Strip.data'
nt2 = ntc.createNTuple ( full_path )

strip = Display ( "Strip Chart", nt2, ('Energy', 'Sigma' ))
canvas.addDisplay ( strip )

xy = Display ( "XY Plot", nt2, ('Energy', 'Sigma', 'nil', 'error' ))
canvas.addDisplay ( xy )

y = Display ( "Y Plot", nt2, ('Energy', ) )
canvas.addDisplay ( y )

xyz = Display ( "XYZ Plot", nt1, ('Age', 'Service', 'Cost' ))
canvas.addDisplay ( xyz )

vm = Display ( "Variable Mesh", nt2,
               ( 'Energy', 'Sigma', 'Sigma', 'binsize', 'error' ) )
canvas.addDisplay ( vm )

print "Various type of displays were created"
