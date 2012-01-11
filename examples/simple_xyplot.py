"""
   Demonstrates making simple XY plot.  

   author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
"""
#
# load the HippoDraw module
#
from load_hippo import app, canvas

from hippo import Display

# Create list of data
energy = [90.74, 91.06, 91.43, 91.50, 92.16, 92.22, 92.96, 89.24, 89.98, 90.35]
sigma  = [ 29.0, 30.0,  28.40, 28.80, 21.95, 22.90, 13.50,  4.50, 10.80, 24.20]
errors = [  5.9,  3.15,  3.0,   5.8,  7.9,   3.1,   4.6,    3.5,   4.6,   3.6]

# make a plot to test it.
xy = Display ( "XY Plot", [ energy, sigma, errors ],
               ['Energy', 'Sigma', 'nil', 'error' ] )

canvas.addDisplay ( xy )

xy.setTitle ( 'Mark II Z0 scan' )

print "An XY plot is now displayed.   You can use the Inspector dialog"
print "to modify the appearance or fit a function to it."
