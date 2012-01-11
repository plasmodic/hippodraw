""" -*- mode: python -*-

Create an NTuple with power-law distribution and histogram it on log log scale

@author J. Chiang <jchiang@slac.stanford.edu>
"""

from load_hippo import app, canvas

import random, math
shoot = random.uniform

# Create empty NTuple of 4 columns that we can fill by row.
from hippo import NTuple, NTupleController
nt = NTuple ()
nt.setLabels ( [ 'x' ] )

xmin = 1.
xmax = 100.
gamma = 2.1
xpmax = math.pow ( xmax, 1. - gamma )
xpmin = math.pow ( xmin, 1. - gamma )

nsamp = 10000
for i in range(nsamp):
    x = shoot(0, 1)
    xx = math.pow ( x*( xpmax - xpmin ) + xpmin, 1./ (1. - gamma) )
    nt.addRow ( (xx, ) )

from hippo import Display

hist = Display ( 'Histogram', nt, ( 'x', ) )
canvas.addDisplay ( hist )

# Set the x and y axis on log scale
hist.setLog ( 'x', True )
hist.setLog ( 'y', True )

# fit to power law function
from hippo import Function
datarep = hist.getDataRep ()
powerlaw = Function ( "PowerLaw", datarep )
powerlaw.addTo( hist )
powerlaw.fit()
