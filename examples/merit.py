""" -*- mode:python -*-

Demo of reading ROOT file with function, cuts, and calculation

This examples depends on a ROOT file not included in the distribution
because of its size.  It can be downloaded from here...

ftp://ftp-glast.slac.stanford.edu/glast.u11/InstrumentAnalysis/MC/EngineeringModel-v5r0608p7/2Tower_surface_muons/surface_muons_4M_merit.root

author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

$Id: merit.py,v 1.9 2006/09/12 18:35:19 pfkeb Exp $

"""

from hippo import RootController, HDApp, Display
rc = RootController.instance()

filename = "/nfs/farm/g/glast/u33/InstrumentAnalysis/MC/EngineeringModel-v6r070329p28/Surface_muons/surface_muons_4M_merit.root"

# Get the top level tree names from the file
ntuple_names = rc.getNTupleNames ( filename  )
print "In this file, tree names are ", ntuple_names

# Create a data array from the first tree
ntuple = rc.createDataArray ( filename, 'MeritTuple' )

# Pprint some information about this tree
print "Number of columns = ", ntuple.columns
labels = ntuple.getLabels()
print "First ten column labels are ... ", labels[:10]
print "Number of rows = ", ntuple.rows

# Create a histogram for one of the columns and add it to the canvas
hist = Display ( "Histogram", ntuple, ('TkrEnergy', ) )

# Up to now, we didn't need the HippoDraw application running.
# Now we do in order to view the data.
app = HDApp()
canvas = app.canvas()
canvas.addDisplay ( hist )

# Set the Y axis on log scale of better viewing
hist.setLog ( 'y', True )

# Add a cut from data in another column
from hippo import Cut
hits_cut = Cut ( ntuple, ('TkrTotalHits',) )
canvas.addDisplay ( hits_cut )
hits_cut.setLog ( 'y', True )

hits_cut.addTarget ( hist )
hits_cut.setCutRange ( 4, 110, 'x' )

# Change the range of the displayed data
hist.setRange ( 'x', 40, 700 )

# fit a function to the histogram
from hippo import Function
datarep = hist.getDataRep ()

exp1 = Function ( "Exponential", datarep )
exp1.addTo ( hist )

exp1.fit ()

# Print the results of the fit
pnames = exp1.parmNames ()
print pnames

parms = exp1.parameters ()
print parms

# add another function to the histogram and fit the linear sum
exp2 = Function ( "Exponential", datarep )
exp2.addTo ( hist )

exp1.fit() # always fit to linear sum

# Build an array which is the sum of two columns and add it to the ntuple
label = "Raw sum"
ntuple [ label ] = ntuple [ 'TkrEnergy' ] + ntuple [ 'CalEnergyCorr' ]

# Histogram the new column
sum_hist = Display ( 'Histogram', ntuple, (label, ) )
canvas.addDisplay ( sum_hist )

sum_hist.setLog ( 'x', True )
sum_hist.setLog ( 'y', True )

# Compare it with the official sum of energies
merit_sum = Display ( 'Histogram', ntuple, ( 'EvtEnergyCorr', ) )
canvas.addDisplay ( merit_sum )

merit_sum.setLog ( 'x', True )
merit_sum.setLog ( 'y', True )

sum_hist.setRange ( 'x', 1.0, 1e+06 )
