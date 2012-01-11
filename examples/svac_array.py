""" -*- mode:python -*-

Demo of reading ROOT SVAC data set a with numarrray and adding
variables to your own ntuple.

Numarray functions used in this script are described here...
<http://stsdas.stsci.edu/numarray/numarray-1.5.html/node21.html>
and node22.html and node38.html


author: Paul F. Kunz <Paul_Kunz@slac.stanford.edu

"""

#
# Get numarrays:
#
import numarray

#
# Get canvas:
#
import hippo
app = hippo.HDApp()
canvas = app.canvas ()

#
# HippoDraw Root interface:
#
from hippo import RootController, Display
rc = RootController.instance()

#
# SVAC ntuple we want to look at:
#
filenameSvac = "/nfs/farm/g/glast/u27/Integration/rootData/135005389/v5r0703p6/calib-v1r0/svacTuple/emRootv0r0/svacTuple-v3r4p4_135005389_svac_svac.root"


print "Opening SVAC file ", filenameSvac
print "Ignore the warning and error messages which come from ROOT"
#
# SVAC Ntuple tree.  The ROOT file could have multiple trees, we
# probably want the first one.
#

tree_names = rc.getNTupleNames ( filenameSvac )
print "In the SVAC file the tree names are: ", tree_names
nameSvac = tree_names[0]

#
# Get the SVAC DataArray: It is called 'daSvac' in this script
# The dimensinality will be
#   number of columns = number of TBranch's
#   number of rows = number of events
#
daSvac = rc.createDataArray ( filenameSvac, nameSvac )

#
# Read in the trigger vectors.  The `GemTkrVector' variable in this column
# is a vector of size 16
#
# The first method we use a boolean expression, and then convert it to
# integer array.   The second method we use the where function
#
# The dimensions of the resulting numarrays are
#   number of rows = number of events
#   number of columns = size of the vector (16)
#
print "Verify shape of ROOT array variable"
na = daSvac['GemTkrVector']
print na.shape

import time
tstart = time.time()
print "start"
TkrTriggered =  ( daSvac [ 'GemTkrVector'] == 1).astype(numarray.Int32)
print "Verify shape of result of vector expression"
print TkrTriggered.shape

CalLeTriggered = numarray.where ( daSvac ['GemCalLeVector'] == 1, 1, 0 )

#
# The labels of the columns we will add to the data set
#
label_TkrTriggered = 'Number of Towers with Tkr Triggers'
label_CalLeTriggered = 'Number of Towers with CalLe Triggers'
label_TowerTkrTrigGemCond = 'Number of Towers with Tkr Triggers after Cut'
label_TowerCalLeTrigGemCond = 'Number of Towers with CalLe Triggers after Cut'

#
# For each event, take the sum of the columns.
# Second argument in sum() is 0 for sum of row and 1 for column
# Then add resulting vector as a new column to the data set
#
nbrTkrTriggered = numarray.sum ( TkrTriggered, 1 )
print "Verify shape of result of sum"
print nbrTkrTriggered.shape

daSvac [ label_TkrTriggered ]  = nbrTkrTriggered
nbrCalLeTriggered = numarray.sum ( CalLeTriggered, 1 )
daSvac [ label_CalLeTriggered ] = nbrCalLeTriggered
#
# Get the branch as an array and create a numarray withere the value
# is equal to 7
#
t = daSvac [ 'GemConditionsWord' ] == 7.0

#
# If GemConditionsWord == 7., then fill element with `nbrTkrTriggered',
# otherwise with -1
#
daSvac [ label_TowerTkrTrigGemCond ] = \
       numarray.choose ( t, ( nbrTkrTriggered, -1 ) )
daSvac [ label_TowerCalLeTrigGemCond ] = \
       numarray.choose ( t, ( nbrCalLeTriggered, -1 ) )
tend = time.time()
print "Took %f seconds create the 4 new columns with 500,000 rows each" % \
      (tend -tstart)
        
#
# The rest is standard procedure
#
tkrtrighist = Display ("Histogram", daSvac, (label_TkrTriggered,) )
canvas.addDisplay ( tkrtrighist )
tkrtrighist.setLog ( 'y', True)

calletrighist = Display ("Histogram", daSvac, (label_CalLeTriggered, ) )
canvas.addDisplay ( calletrighist )
calletrighist.setLog ( 'y', True)

tkrtrighist_gemcond = Display ( "Histogram", daSvac,
                                (label_TowerTkrTrigGemCond, ) )
canvas.addDisplay ( tkrtrighist_gemcond )
tkrtrighist_gemcond.setRange ( 'x', 0, 16)
tkrtrighist_gemcond.setLog ( 'y', True)
print tkrtrighist_gemcond.numberOfEntries()

