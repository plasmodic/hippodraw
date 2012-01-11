""" -*- mode:python -*-

Demonstrates implementing a function in Python derived from the C++
abstract base class.  Also demonstrates use of the fitter from Python,
without using the GUI

@author Paul F. Kunz <paul_kunz@slac.stanford.edu>

"""
#
#$Id: fitting.py,v 1.11 2007/06/26 22:12:10 pfkeb Exp $
#

from load_hippo import app, canvas

import random

num = None
try:
    import numpy as num
    d_type = 'd'
except:
    import numarray as num
    d_type = num.double

slope = 2.
intercept = 1.
sigma = 2.

from hippo import FunctionBase

#
# Define a class in Python that is derived from C++ base class
#
class Linear ( FunctionBase ) :
    def __init__ ( self ) :
        FunctionBase.__init__( self )
        self.initialize ()

    def initialize ( self ) :
        self.setName ( 'linear' )
        self.setParmNames ( [ 'Intercept', 'Slope' ] )
        self.setParameters ( [ intercept, slope ] )

    def dimensions ( self ) :
        return 1
    
    def valueAt ( self, x ) :
        parms = self.getParameters ()
        
        return x * parms[1] + parms[0]

    def derivByParm ( self, i, x ) :
        if i == 0 :
            return 1.0
        if i == 1 :
            return x

gauss = random.gauss

# Generate some data using numarray
npts = 20
x = num.array(num.arange(npts), d_type)
y = num.array( [gauss(slope*xx + intercept, sigma) for xx in x] )
xerr = num.ones ( npts, d_type )
yerr = sigma*num.ones(npts)

# Create a DataArray and register it so that the Inspector can display it.
# The DataArray is will wrap a DataSource of type NTuple.

from hippo import DataArray
da = DataArray ( 'NTuple' ) # use NTuple to store data
da.register ( 'randomized line' ) # name the data source

# Fill the contents by adding named columns in the order expected by
# the fitter (more later)

da['x'] =  x
da['y'] =  y
da['xerr'] = xerr
da['yerr'] = yerr

# Create an XY plot, ignoring xerr, and add it to the canvas.
from hippo import Display
disp = Display ('XY Plot', da, ('x', 'y', 'nil', 'yerr' ) )
canvas.addDisplay ( disp )

# Get the name of the available fitters from the factory
from hippo import FitterFactory
factory = FitterFactory.instance()
fitters = factory.names()
print fitters

# Create a fitter, in this case Minuit Migrad algorithm with Chi Sq
migrad = factory.create ( fitters[1] )
print migrad.name()


# create a function
f = Linear ()
print f.name()
print f.parmNames()

# Get the FCN so we can set it up
fcn = migrad.getFCN () 
fcn.setFunction ( f ) 
fcn.setDataSource ( da.dataSource() )
fcn.setUseErrors ( True )


# Since we are using Minuit, we can set limits on the parameters.  If
# we wanted to do that we would uncomment the following.

#migrad.setLimits ( 0, -2., -1. )

# Minimize the Chi Sq
migrad.minimize()

# Print the resulting parameters
print f.getParameters()

# Add the function to the display of the data
fl = Linear ()

disp.addFunction ( f )

# Add the function to the factory so it can be used from the GUI
# One might just do this instead of using the minimizers directly
from hippo import FunctionFactory
ff = FunctionFactory.instance()
ff.add ( f )

# Create an DataArray with columns not in order expected by the fitter


da2 = DataArray ( 'NTuple' ) # use NTuple to store data
da2.register ( 'randomized line 2' ) # name the data source

# Fill the contents by adding named columns in the order expected by
# the fitter (more later)

da2['y'] =  y
da2['x'] =  x
da2['yerr'] = yerr
da2['xerr'] = xerr

# Teach the FCN the columns to use, ignoring xerr.  The `1' indicates
# the dimension of the coordinate space

fcn.setDataSource ( da2.dataSource(), 1, [ 1, 0, -1, 2 ] )

f.setParameters ( [ intercept, slope ] )
migrad.minimize ()
print f.getParameters ()
