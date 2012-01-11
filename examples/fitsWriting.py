""" -*- mode:python -*-

Demonstrate using FitsController to create a new FITS file

author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

"""

from hippo import FitsController, Display
fc = FitsController.instance()

import sys
fullname = sys.path[0] + '/' + "SimEvents.fits"

print "Opening file ", fullname
hdu_names = fc.getNTupleNames ( fullname  )
print "Names of the HDUs are ", hdu_names

darray = fc.createDataArray ( fullname, hdu_names[1] ) # pass the empty image

print "Number of columns = ", darray.columns
print "Column labels are ..."
print darray.getLabels()

print "Number of rows = ", darray.rows

import hippo
app = hippo.HDApp()
canvas = app.canvas()

plot = Display ( "Color Plot", darray, ('GLON', 'GLAT', 'nil', 'nil' ) )
canvas.addDisplay ( plot )



#
# Calculate Log(energy) and add it as new column
#
import numarray
darray[ 'LogE' ] = numarray.log ( darray [ 'energy' ] )

lplot = Display ( 'Histogram', darray, ( 'LogE', ) )
lplot.setLog ( 'y', True )
canvas.addDisplay ( lplot )

#
# Compare it with logrithmic binning
#
clplot = Display ( 'Histogram', darray, ( 'energy',) )
canvas.addDisplay ( clplot )
clplot.setLog ( 'x', True )
clplot.setLog ( 'y', True )

#
# Apply a cut to the displays
#
cut = hippo.Cut ( darray, ( 'time', ) )
canvas.addDisplay ( cut )
cut.setCutRange ( 5600., 14000., 'x' )
cut.addTargets ( (plot, lplot, clplot) )

#
# Add array object to column
# 
glon = darray['GLON']
glat = darray['GLAT']
a2d = numarray.array ( [ glon, glat ] )
a2d.transpose()

print "Now have"
print "rank  = ", a2d.rank
print "shape = ", a2d.shape
print a2d

darray [ 'glon-glat' ] = a2d
darray.setTitle ( 'Lat events modified' )

print "\nCurrent columns are"
print darray.getLabels()

#
# Write the modified FITS binary table to a file
# 
fc.writeToFile ( darray, 'sim2.fits' )

#
# Write to FITS file only rows passing cuts and only certain columns
#
columns = [ 'GLON', 'GLAT', 'energy', 'time', 'LogE', 'glon-glat' ]
fc.writeToFile ( darray, 'sim3.fits', (cut,), columns )

#
# Verify the written file
#
names2 = fc.getNTupleNames ( 'sim3.fits' )
print "\nHDU names are ", names2

darray2 = fc.createDataArray ( 'sim3.fits', names2[1] )
print "\nColumn labels are", darray2.getLabels()

#
# Display data, should appear same as original under cut
#
plot2 = Display ( "Color Plot", darray2, ('GLON', 'GLAT', 'nil', 'nil' ) )
canvas.addDisplay ( plot2 )

lplot2 = Display ( 'Histogram', darray2, ( 'LogE', ) )
lplot2.setLog ( 'y', True )
canvas.addDisplay ( lplot2 )

#
# Get the array column
#
glonglat = darray2 [ 'glon-glat' ]
print "\ncheck the array variable"
print "\Rank = ", glonglat.rank
print "shape = ", glonglat.shape
print "And the data is under the cut"
print glonglat

