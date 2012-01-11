""" -*- mode:python -*-

   This example shows how to do 2 dimension fitting by talking to the
   fitter and its FCN directly.  It also shows the optional setting
   for the fitting process that are only available with a Minuit based
   fitter.

   author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
   
   $Id: fitting2.py,v 1.16 2007/06/26 22:12:10 pfkeb Exp $
   
"""

import random
num = None
d_type = None
try:
    import numpy as num
    d_type = 'd'
except:
    import numarray as num
    d_type = num.double

import hippo

from hippo import FunctionBase

#
# Declare a function that take is 2 dimensional.
#
class Planar ( FunctionBase ) :
    def __init__ ( self, other = None ) :
        if other :
            FunctionBase.__init__( self, other )
        else :
            FunctionBase.__init__( self )
            self.initialize ()

    def initialize ( self ) :
        self.setName ( 'Planar' )
        self.setParmNames ( [ 'Interscept', 'SlopeX', 'SlopeY' ] )
        self.setParameters ( [ 0., 1., 1. ] ) # initial parameters

    def valueAt ( self, x, y ) :
        parms = self.getParameters ()
        return parms[0] + parms[1] * x + parms[2] * y

#
# Generate some data.
#
slope = 2.
intercept = 1.
mean = 10.
sigma = 4.

gauss = random.gauss

def plane ( x, y ) :
    return x + y

npts = 20
x = num.arange(npts*npts)
y = num.arange(npts*npts)
z = num.arange(npts*npts)
zerr = num.arange(npts*npts)
k = 0

for i in range(npts) :
    for j in range(npts) :
        x[k] = xx = i
        y[k] = yy = j
        z[k] = plane ( xx, yy ) + gauss ( 0., 2. )
        zerr[k] = sigma
        k += 1
        
#
# Store the data in NTuple
#
nt = hippo.NTuple () # empty ntuple
nt.addColumn ( 'x', x ) # add column
nt.addColumn ( 'y', y ) 
nt.addColumn ( 'z', z )
nt.addColumn ( 'zerr', zerr )

#
# Get the list of available fitters so one can verify that Minuit is
# available.
#
from hippo import FitterFactory
factory = FitterFactory.instance()
fitters = factory.names()
print "Show the fitters available"
print fitters

#
# Create a Minuit Migrad fitter
#
migrad = factory.create ( fitters[1] )

#
# Create an instance of our custom function and verify its names
#
f = Planar ()
print "Show the functin name and its parameter names"
print f.name()
print f.parmNames()

#
# Get the objective function so we can set the function and data to be used
#
fcn = migrad.getFCN ()
fcn.setFunction ( f )

#
# Tell the FCN how to bind to the NTuple
# columns are                x  y  z        zerr
fcn.setDataSource ( nt, 2, [ 0, 1, 2, -1, -1, 3 ] ) # no xerr or yerr
fcn.setUseErrors ( False ) # do not use errors on z value

migrad.minimize()

chisq = fcn.objectiveValue ()
print "Show ChiSq after minimization with no Z error"
print "ChiSq =", chisq
print "Degrees of Freedom = ", fcn.degreesOfFreedom()

fcn.setUseErrors ( True ) # use error on z value.
chisq = fcn.objectiveValue ()
print "Show ChiSq after minimization using Z error"
print "ChiSq =", chisq
print "Degrees of Freedom = ", fcn.degreesOfFreedom()

#
# Parameters are returned as a tuple, so 
# get a copy of thie parameter values by creating a list.
#
parms = list(f.getParameters())
print "Show the parameter values"
print parms

#
# Set a parameter value and have it fixed in the fit
#
parms[1] = 1.0
f.setParameters ( parms )
migrad.setFixedFlags ( ( 0, 1, 0 ) )
migrad.minimize ()

print "Show parameter values with 2nd one fixed"
print f.getParameters()

#
# Try setting limits, only available with Minuit fitter
#
migrad.setLimits ( 2, 1.1, 1.2 )
migrad.minimize()
print "Show parameter values after setting limits on 3rd one."
print f.getParameters ()

print "There's no need to display the data"
