""" -*- mode:python -*-

Demo of doing a Fast Fourier Transform on simulated white noise.

author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

"""

from load_hippo import  app, canvas
import hippo, hippofft

num = None
try:
    import numpy as num
    import numpy.random as ra
except :
    import numarray as num
    import num.random_array as ra


canvas.setPlotMatrix ( 1, 3 )

#
# generate a random ordered sequence
#
npts = 400000
times = num.sort ( ra.uniform ( 1e9, 1e10, (npts, ) ) )

ds = hippo.NumArrayTuple ()
ds.addColumn ( 'times', times )

#
# Use the hippo FFT utility to get binned light curve display the
# normalized contents of the bins
#
lightcurve, bintimes = hippofft.simpleLightCurve( ds,'times', numberbins=4096 )
canvas.addDisplay ( lightcurve )

#
# Calculate the power spectrum by giving hippo the range and unit of
# time
#
range = lightcurve.getRange ( 'x' )
onetick = 1. / 20.0e6
spectrum = hippofft.powerSpectrum ( bintimes, range, onetick )

#
# The spectrum return is ntuple of two columns which can be used
# directly to plot it as an XY plot
#
labels = spectrum.getLabels()
xyplot = hippo.Display ( 'XY Plot', spectrum, labels )
canvas.addDisplay ( xyplot )

#
# Check that the power distribution is correct by first histogramming
# it
#
hist = hippo.Display ( 'Histogram', spectrum, (labels[1], ) )
hist.setLog ( 'y', True )
canvas.addDisplay ( hist )

#
# and then fitting it to an exponential
#
datarep = hist.getDataRep()
exp = hippo.Function ( 'Exponential', datarep )
exp.addTo ( hist )

