""" -*- python -*-

This script tests and demonstrates appending to NTuple

Copyright (C) 2006   The Board of Trustees of The Leland Stanford
Junior University.  All Rights Reserved.

Author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: append_ntuple.py,v 1.4 2007/06/22 21:15:23 pfkeb Exp $

"""
#
# Import hippo and create application and canvas
#
from load_hippo import app, canvas

from hippo import *
num = None
d_type = None
#
# Set array module and double type for it
#
try:
    import numarray as num
    d_type = num.double 
except:
    import numpy as num
    d_type = 'd'
#
# Create empty arrays
#
da1 = DataArray ()
da2 = DataArray ()

#
# labels of columns that will be used often
#
data = 'data'
sqdata = 'sqrt(data)'

#
# Create and fill two columns
#
da1 [ data ] = num.array ( range ( 10),  d_type )
da1 [ sqdata ] = num.sqrt ( da1 [ data ] )

#
# Plot them
#
xyplot = Display ( 'XY Plot', da1, ( data, sqdata, 'nil', 'nil' ) )
canvas.addDisplay ( xyplot )

#
# Create a similar array
#
da2 [ data ] = num.array ( range ( 10, 20, 1 ), d_type)
da2 [ sqdata ] = num.sqrt ( da2 [ data ] )

#
# Append second to first
#
da1.append ( da2 )


print "Should see 20 data points, the result of appending two DataArrays"

