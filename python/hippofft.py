""" -*- mode:python -*-

Utilty functions for doing FFT analysis

author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>
with help from  Warren Focke <focke@slac.stanford.edu>

"""

from load_hippo import  app, canvas


num = None
try:
    import numpy as num
    d_type = 'd'
except:
    import numarray as num
    d_type = num.double

import hippo

import spectral

def simpleLightCurve ( ntuple, column, numberbins = 1024 ) :
    hist = hippo.Display ( "Histogram", ntuple, ( column, ) )
    ticks = ntuple.getColumn ( column )

    hist.setRange ( 'x', ticks[0], ticks[-1] )
    
    width = ( ticks[-1] - ticks[0] ) / numberbins
    hist.setBinWidth ( 'x', width )
    
    hdata = hist.createNTuple()
    bintimes = num.array ( hdata.getColumn(1) )
    widths = hdata.getColumn ( 2 )
    bintimes *= widths
    bintimes *= 2.0
    return hist, bintimes

def powerSpectrum ( bintimes, range, onetick ) :
    average = num.add.reduce ( bintimes ) / len ( bintimes )
    number = num.sum ( bintimes )
    bintimes -= average

    power = spectral.multispec ( bintimes )

    power *= 2. / ( number -1 )
    
    spectrum = hippo.DataArray ( 'NTuple' )
    spectrum.setName ( 'Power spectrum' )
    ntc = hippo.NTupleController.instance ()
    ntc.registerNTuple ( spectrum.dataSource () )

    time_range = onetick * ( range[1] - range[0] )
    freqs = num.arange ( len ( power ) ) / time_range

    freq_label = 'frequency'
    spec_label = 'power spectrum'
    spectrum[freq_label ] = freqs
    spectrum[spec_label] = power

    return spectrum
