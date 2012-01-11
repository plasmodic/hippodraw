"""
Read in a series of FITS table files and make them accessible as
numarrays, optionally creating a HippoDraw NTuple.

@author J. Chiang <jchiang@slac.stanford.edu>
"""
#
# $Id: FitsNTuple.py,v 1.12 2007/06/25 19:28:16 jchiang Exp $
#
import sys, pyfits
try:
    import numarray as num
except ImportError:
    import numpy as num

class FitsNTuple:
    def __init__(self, fitsfiles, extension=1):
        cat = num.concatenate
        #
        # If fitsfile is not a list or tuple of file names, assume
        # it's a single file name and put it into a single element
        # tuple.
        #
        if type(fitsfiles) != type([]) and type(fitsfiles) != type(()):
            fitsfiles = (fitsfiles, )
        #
        # Process each file named in the list or tuple.
        #
        columnData = {}
        for i, file in zip(xrange(sys.maxint), fitsfiles):
            #print "adding", file
            table = pyfits.open(file.strip(" "))
            if i == 0: 
                self.names = table[extension].columns.names
            for name in self.names:
                if i == 0:
                    columnData[name] = table[extension].data.field(name)
                else:
                    columnData[name] = cat((columnData[name],
                                            table[extension].data.field(name)))
        #
        # Add these columns to the internal dictionary.
        #
        self.__dict__.update(columnData)
        
    def makeNTuple(self, name=None, useNumArray=1):
        import hippo
        if useNumArray:
            nt = hippo.NumArrayTuple()
        else:
            nt = hippo.NTuple()
        if name != None:
            nt.setTitle(name)
        ntc = hippo.NTupleController.instance()
        ntc.registerNTuple(nt)
        for name in self.names:
            if len(self.__dict__[name].shape) > 1: # have multicolumn variable
                columns = self.__dict__[name]
                columns.transpose()
                for i, col in enumerate(columns):
                    colname = "%s%i" % (name, i)
                    nt.addColumn(colname, col)
            else:
                try:
                    nt.addColumn(name, self.__dict__[name])
                except TypeError:
                    pass
        return nt
