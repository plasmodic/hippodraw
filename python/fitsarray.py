#
# @brief Classes to provide numarray arrays with FITS image support
# using pyfits.
#
# @author J. Chiang <jchiang@slac.stanford.edu>
#
# $Id: fitsarray.py,v 1.3 2006/09/24 20:21:32 pfkeb Exp $
#

import os, sys, numarray, copy
sys.path.append('/u1/jchiang/python')
import pyfits, FitsAxis

class FitsImageArray:
    """
    This class wraps numarray.array objects, providing them with
    rudimentary pyfits support.  The constructor can take various
    arguments and will create the FitsImageArray object accordingly:

    <class 'numarray.NumArray'> Use the array argument as the FITS
                                primary HDU image and provide a
                                minimal header.
                              
    <type 'str'> Read in an existing FITS image file given by the
                 string argument.
               
    <type ['list', 'tuple']> Build an array of zeros, using the list or
                             tuple values as the array axis lengths.

    <type 'int'> Build an arg x arg array of zeros.

    Any other type for the argument will produce a 100x100 array of zeros.
    """
    def __init__(self, arg):
        self.name = "FitsImageArrayObject"          # default name
        self.axes = {}
        if (type(arg) == numarray.NumArray):
            # instantiate from a numarray and create a new hdu
            if (2 <= len(arg.shape) <= 3):
                self.array = arg
                self.hdu = pyfits.PrimaryHDU()
                self.hdu.data = self.array
            else:
                sys.stderr.write("The numarray object must have " +
                                 "2 <= dimensions <= 3.\n")
        elif type(arg) == type(""):
            # read in an existing FITS file
            fitsobj = pyfits.open(arg)
            self.hdu = fitsobj[0]
            self.array = self.hdu.data
            # set the name of this object using root name of FITS file
            indx = arg.find(".")
            if indx != -1:
                self.name = arg[:indx]
            else:
                self.name = arg
        elif type(arg) == type(()) or type(arg) == type([]):
            # build array of zeros using tuple or list values
            if (2 <= len(arg) <= 3):
                self.array = numarray.zeros(tuple(arg))
                self.hdu = pyfits.PrimaryHDU()
                self.hdu.data = self.array
            else:
                sys.stderr.write("The numarray object must have " +
                                 "2 <= dimensions <= 3.\n")
        elif type(arg) == type(1):
            # arg is of type int so build an arg x arg array filled with zeros
            self.array = numarray.zeros((arg, arg))
            self.hdu = pyfits.PrimaryHDU()
            self.hdu.data = self.array
        else:
            # create default array object
            sys.stderr.write("Invalid argument of %s\n" % type(arg))
            sys.stderr.write("Creating default 100x100 object.\n")
            self.array = numarray.zeros((100, 100))
            self.hdu = pyfits.PrimaryHDU()
            self.hdu.data = self.array

    def setName(self, name):
        self.name = `name`

    def fits_image_array(self, array, history = ""):
        my_array = FitsImageArray(array)
        my_array.hdu = pyfits.PrimaryHDU()
        my_array.hdu.header.ascard = copy.deepcopy(self.hdu.header.ascard)
        my_array.hdu.data = array
        if (history != ""):
            my_array.name = history   # for propagation in later history info
            my_array.hdu.header.update("HISTORY", history, before="END")
        return my_array

    def __getattr__(self, attrname):
        return getattr(self.array, attrname)

    def __getitem__(self, key):
        return FitsImageArray(self.array[key])

    def __repr__(self):
        return `self.array`

    def __add__(self, operand):
        if (type(operand) == type(self)):
            history = self.name + " + " + operand.name
            return self.fits_image_array(self.array + operand.array, history)
        else:
            history = self.name + " + " + self.operandString(operand)
            return self.fits_image_array(self.array + operand, history)

    def __radd__(self, operand):
        if (type(operand) == type(self)):
            history = operand.name + " + " + self.name
            return self.fits_image_array(self.array + operand.array, history)
        else:
            history = self.operandString(operand) + " + " + self.name
            return self.fits_image_array(self.array + operand, history)

    def __sub__(self, operand):
        if (type(operand) == type(self)):
            history = self.name + " - " + operand.name
            return self.fits_image_array(self.array - operand.array, history)
        else:
            history = self.name + " - " + self.operandString(operand)
            return self.fits_image_array(self.array - operand, history)

    def __mul__(self, operand):
        if (type(operand) == type(self)):
            history = self.name + "*" + operand.name
            return self.fits_image_array(self.array*operand.array, history)
        else:
            history = self.name + " * " + self.operandString(operand)
            return self.fits_image_array(self.array*operand, history)

    def __rmul__(self, operand):
        if (type(operand) == type(self)):
            history = operand.name + "*" + self.name
            return self.fits_image_array(self.array*operand.array, history)
        else:
            history = self.operandString(operand) + "*" + self.name
            return self.fits_image_array(self.array*operand, history)

    def __div__(self, operand):
        if (type(operand) == type(self)):
            history = self.name + "/" + operand.name
            return self.fits_image_array(self.array/operand.array, history)
        else:
            history = self.name + "/" + self.operandString(operand)
            return self.fits_image_array(self.array/operand, history)

    def operandString(self, operand):
        if type(operand) == numarray.NumArray:
            return 'numarrayObject'
        else:
            return `operand`

    def setAxis(self, axisObj, naxis):
        axisObj.updateFitsHeader(self.hdu.header, naxis)
        self.axes[naxis] = axisObj
        
    def writeto(self, filename):
        os.system("rm -f " + filename)
        self.fitsobj = pyfits.HDUList()
        self.fitsobj.append(self.hdu)
        self.fitsobj.writeto(filename)

if __name__ == "__main__":
    print "FitsAxis class tests: "
    fitsobj = pyfits.HDUList()
    hdu = pyfits.PrimaryHDU()
    x = FitsAxis("x-axis", 0., 0.2, 10)
    y = FitsAxis("y-axis", 0., 0.1, 20)
    hdu.data = numarray.zeros((y.naxis, x.naxis))    # note the ordering
    x.updateFitsHeader(hdu.header, 1)
    y.updateFitsHeader(hdu.header, 2)
    fitsobj.append(hdu)
    os.system("rm -f test.fits")
    fitsobj.writeto('test.fits')
    print "FitsAxis class tests completed.\n"

    print "FitsImageArray class tests:"
    x = FitsAxis("x-axis", 0., 0.2, 5)
    y = FitsAxis("y-axis", 0., 0.1, 10)
    array1 = numarray.ones((y.naxis, x.naxis))
    z = {}
    z[1] = FitsImageArray(array1)
    z[1].setAxis(x, 1)
    z[1].setAxis(y, 2)
    z[1].setName('z1')
    z[2] = z[1]/3.
    z[2].setName('z2')
    z[3] = z[1] + z[2]/2.
    z[3].setName('z3')
    os.system("rm -f z3.fits")
    z[3].writeto("z3.fits")
    z[4] = z[3][ :3, : ]
    z[4].setName('z4')
    z[5] = FitsImageArray("z3.fits")
    z[5].setName('z5')
    z[6] = z[2] - array1
    z[6].setName('z6')
    for key in z.keys():
        print "\nz["+`key`+"]:"
        print z[key].hdu.header.ascard
        print z[key]
    print "FitsImageArray class tests completed."
