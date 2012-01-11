"""

Demonstraton of static versus dynamic histograms.  Also demonstrates
taking difference between two histograms and plotting it.

 @author Paul F. Kunz <Paul_Kunz@slacs.stanford.edu>

"""
import random
from time import sleep

from load_hippo import app, canvas

from hippo import Display, NTuple, NTupleController

# Create static histogram and add it to canvas.
sthist = Display ( "Static Histogram" )
sthist.setTitle ( "Gaussian Distribution (static hist)" )
sthist.setLabel ( 'x', 'X' )
sthist.setRange ( 'x', 0, 100)
sthist.setBinWidth ( 'x', 1. )
canvas.addDisplay ( sthist )

# Create second static histogram and add it to canvas
sthists = Display ( "Static Histogram" )
sthists.setTitle ( "Gaussian Distribution (low statistics)" )
sthists.setLabel ( 'x', 'X' )
sthists.setRange ( 'x', 0, 100)
sthists.setBinWidth ( 'x', 1. )
canvas.addDisplay ( sthists )

# Create empty NTuple and set the column label.
# Setting the column labels sets the number of columns
ntuple = NTuple ( )
ntuple.setTitle ( 'Gaussian Distribution' )
ntuple.setLabels ( ('X value', ) )

# Register the NTuple so the Inspector can see it.
NTupleController.instance().registerNTuple ( ntuple )

# Create dynamic histogram and attach it to NTuple.
# It will automatically adjust its range, title, and labels
dyhist = Display ( "Histogram", ntuple, ('X value',  ) )
canvas.addDisplay ( dyhist )

mean = 45
sigma = 10
gauss = random.gauss

# Generate some data,  fill the static histograms and NTuple.
for i in range(10000):
    x = gauss( mean, sigma )
    sthist.addValues ( (x, ) )
    ntuple.addRow ( (x, ) )
    if i < 1000 :
        sthists.addValues ( (x, ) )  # only fill with first 1000


# Print some statistics from static histogram
# Could do same for dynamic
datarep = sthist.getDataRep()

print "Histogram :"
print "  Title : " + sthist.getTitle()
print "  Entries : %i" % sthist.numberOfEntries()
print "  Mean = %f" % datarep.getMean ( 'x' )
print "  Rms  = %f" % datarep.getRMS ( 'x' )

# Print the average X value on the display
canvas.selectDisplay ( sthist)
canvas.addTextRep ( sthist, 'averagex' )
canvas.selectDisplay ( dyhist)
canvas.addTextRep ( dyhist, 'averagex' )

# Get the contents of the bins as a DataArray
high = sthist.createDataArray()
low  = sthists.createDataArray ()

# Take difference with high statistics one scaled down, and a column
# to the low one.
low[ 'diff' ] = high[ 'Entries / bin' ] / 10 - low[ 'Entries / bin' ]

# Create an XY plot to see the difference
xyplot = Display ( "XY Plot", low, ( 'X', 'diff', 'nil', 'Error' ) )
canvas.addDisplay ( xyplot )
low.register ('difference ntuple' )

print "Static histogram is on the left, dynamic one on the below it."
print "You can changing the binning of the dynamic one with the Inspector"
print "\nOn the right we have low statistics gaussian with difference"
print "between it and high statistics plotted below it"

