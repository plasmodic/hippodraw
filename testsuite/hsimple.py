""" -*- mode: python -*-
A HippoDraw-based version of the hsimple.py histogramming example
from the PyRoot project.

@author J. Chiang <jchiang@slac.stanford.edu>
"""
#
#$Id: hsimple.py.in,v 1.8 2003/09/29 15:15:46 jchiang Exp $
#

import sys
sys.path.reverse()
sys.path.append('../python')
sys.path.append('../python')
sys.path.reverse()

from setPath import *
import hippoplotter as plot
from random import gauss, uniform

#
# Create new NTuple with columns (x, y, x^2 + y^2, uniform deviate, index)
#
nt = plot.newNTuple( ([], [], [], [], []), ("x", "y", "z", "unif", "i") )

hist = plot.Histogram( nt, "x" )
xyhist = plot.XYHist( nt, "x", "y" )
profile = plot.Profile( nt, "x", "z" )

mean = 0.
sigma = 1.
npts = 25000

nt.setIntervalCount ( 1000 )
nt.setIntervalEnabled ( 1 )
for i in range(npts):
    x, y = gauss(mean, sigma), gauss(mean, sigma)
    z = x*x + y*y
    nt.addRow( (x, y, z, uniform(0,1), i) )

nt.setIntervalEnabled ( 0 )
