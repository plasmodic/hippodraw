"""
Python wrapper interface to HippoDraw's hippo module.

@author: J. Chiang <jchiang@slac.stanford.edu>

"""
#
# $Id: hippoplotter.py,v 1.76 2007/07/25 20:12:51 jchiang Exp $
#

import sys, math
try:
    import numarray
except ImportError:
    try:
        import Numeric as numarray
    except:
        import numpy as numarray
    
#print sys.path
import hippo

canvas = None
getDisplay = None
getDisplays = None

def Canvas():
    global canvas, getDisplay, getDisplays
    if canvas is None:
        canvas = hippo.HDApp.instance().canvas()
        getDisplay = canvas.getDisplay
        getDisplays = canvas.getDisplays
    return canvas

ntc = hippo.NTupleController.instance()
prf = hippo.PointRepFactory.instance()

#
# NTuple management
#
ntuples = {}
keynum = 0
registerNTuple = ntc.registerNTuple

def newNTuple(arrays, labels, name="", register=1):
    """
    Define and register an new hippo n-tuple.
    usage: nt = newNTuple( (x, y, ...), ("x", "y", ...), name="", register=1 )
    """
    nt = hippo.NTuple()
    for array, label in zip(arrays, labels):
        nt.addColumn(label, array)
    if name != "": nt.setName(name)
    if register: registerNTuple(nt)
    addNTuple(nt, key=name)
    return nt
    
def newDataArray(arrays, labels, name="", register=1):
    """
    Define and register an new hippo DataArray
    usage: nt = newDataArray((x, y, ...), ("x", "y", ...), name="", register=1)
    """
    nt = hippo.DataArray()
    for array, label in zip(arrays, labels):
        nt[label] = array
    if name != "":
        nt.setName(name)
    if register:
        nt.register(name)
    addNTuple(nt, key=name)
    return nt
    
def newNumArrayTuple(arrays, labels, name="", register=1):
    """
    Define and register an new hippo NumArray
    usage: nt = newNumArrayTuple( (x, y, ...), ("x", "y", ...),
                                  name="", register=1 )
    """
    nt = hippo.NumArrayTuple()
    for array, label in zip(arrays, labels):
        nt.addColumn(label, array)
    if name != "":
        nt.setName(name)
    if register:
        registerNTuple(nt)
    addNTuple(nt, key=name)
    return nt

def newRootNTuple(rootfile, branchName=None):
    """
    Define and register an new hippo RootNTuple
    usage: nt = newRootNTuple( (x, y, ...), ("x", "y", ...),
                               name="", register=1 )
    """
    rc = hippo.RootController.instance()
    names = rc.getNTupleNames(rootfile)
    if branchName == None:
        branchName = names[0]
        print "Returning the first branch named ", branchName
    if branchName in names:
        try:
            nt = rc.createDataArray(rootfile, branchName)
        except:
            nt = rc.createNTuple(rootfile, branchName)
        addNTuple(nt, key=': '.join((rootfile, branchName)))
        return nt
    else:
        message = "Branch %s not found in ntuple\n" % branchName
        message += "Valid names are\n"
        for name in names:
            message += name + "\n"
        raise LookupError, message
    
def addNTuple(nt, key=""):
    """
    Add an existing hippo n-tuple to hippoplotter's list.
    usage: nt = addNTuple(nt, key="")
    """
    global keynum
    if key == "":
        ntuples[keynum] = nt
        keynum = keynum + 1
    else:
        ntuples[key] = nt

#
# Display management
#

def displayHandler( displayType, nt, columns, logScales, axisLimits,
                    pointRep, title="", oplot=0, color=None, lineStyle=None,
                    addToCanvas=1, autoscale=0, select=1 ):
    """
    Wrapper for hippo.Display and the QtDisplay class set methods.
    """
    Canvas()
    if not oplot:
        #
        # Create a new display and set the various options.
        #
        display = hippo.Display( displayType, nt, columns )
        if addToCanvas:
            Canvas().addDisplay( display )
        Axes = ('x', 'y', 'z')
        for logScale, axis in zip(logScales, Axes):
            if logScale:
                display.setLog( axis, 1 )
        for limits, axis in zip(axisLimits, Axes):
            if len(limits) == 2:
                display.setRange( axis, limits[0], limits[1], 1 )
        display.setPointRep( prf.create(pointRep) )
        if title != "":
            display.setTitle(title)
        if color:
            rep = display.getDataRep()
            rep.setColor(color)
        if lineStyle:
            rep = display.getDataRep()
            rep.setLineStyle(lineStyle)
        if select:
            Canvas().selectDisplay(display)
        return display
    else:
        #
        # Add to the currently selected display if possible.
        #
        currentDisplay = Canvas().getDisplay()
        if currentDisplay:
            if not autoscale:
                #
                # Save the current axis ranges.
                #
                xr = currentDisplay.getRange('x')
                yr = currentDisplay.getRange('y')
            #
            # Create a new DataRep
            #
            rep = hippo.DataRep( displayType, nt, columns )
            rep.setPointRep( prf.create( pointRep ) )
            currentDisplay.addDataRep( rep )
            if color:
                rep.setColor(color)
            if lineStyle:
                rep.setLineStyle(lineStyle)
            if not autoscale:
                #
                # Restore the axis ranges
                #
                currentDisplay.setRange('x', xr[0], xr[1])
                currentDisplay.setRange('y', yr[0], yr[1])
            return rep
#
# Plotting functions
#
def Histogram(nt, xname, xlog=0, ylog=0, xrange=(), yrange=(),
              title="", pointRep="Column", oplot=0, color=None,
              addToCanvas=1, autoscale=0, lineStyle=None):
    """
    usage: display = Histogram(nt, xname, xlog=0, ylog=0, 
                               xrange=(), yrange=(), title="",
                               pointRep="Column", oplot=0, color=None,
                               addToCanvas=1, autoscale=0)
    """              
    return displayHandler( 'Histogram', nt, (xname,), (xlog, ylog), 
                           (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           addToCanvas=addToCanvas, autoscale=autoscale,
                           lineStyle=lineStyle)

def histogram(x, xname = "x", xlog=0, ylog=0, xrange=(), yrange=(),
              title="", pointRep="Column", oplot=0, color=None,
              autoscale=0, addToCanvas=1, lineStyle=None):
    """
    usage: display = histogram(x, xname = "x", xlog=0, ylog=0,
                               xrange=(), yrange=(), title="",
                               pointRep="Column", oplot=0, color=None,
                               autoscale=0, addToCanvas=1)
    """
    nt = newNTuple( (x, ), (xname, ), name=title)
    return Histogram( nt, xname, xlog=xlog, ylog=ylog, xrange=xrange,
                      yrange=yrange, pointRep=pointRep, title=title,
                      oplot=oplot, color=color, autoscale=autoscale,
                      addToCanvas=addToCanvas, lineStyle=lineStyle)

def YPlot(nt, yname, xlog=0, ylog=0, xrange=(), yrange=(),
          title="", pointRep="Symbol", oplot=0, color=None, autoscale=0):
    return displayHandler( 'Y Plot', nt, (yname, ), (xlog, ylog), 
                           (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           autoscale=autoscale)

def yplot(y, yname="y", xlog=0, ylog=0, xrange=(), yrange=(),
          title="", pointRep="Symbol", oplot=0, color=None, autoscale=0):
    nt = newNTuple( (y, ), (yname, ), name=title )
    return YPlot(nt, yname, xlog=xlog, ylog=ylog, xrange=xrange,
                 yrange=yrange, pointRep=pointRep, title=title,
                 oplot=oplot, color=color, autoscale=autoscale)

def Scatter(nt, xname, yname, xlog=0, ylog=0, xrange=(), yrange=(), 
            title="", pointRep="Symbol", oplot=0, color=None, lineStyle=None,
            autoscale=0):
    return displayHandler( 'Scatter Plot', nt, (xname, yname),
                           (xlog, ylog), (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           lineStyle=lineStyle, autoscale=autoscale )

def scatter(x, y, xname="x", yname="y", xlog=0, ylog=0,
            xrange=(), yrange=(), title="", pointRep="Symbol",
            oplot=0, color=None, lineStyle=None, autoscale=0):
    nt = newNTuple( (x, y), (xname, yname), name=title )
    return Scatter(nt, xname, yname, xlog=xlog, ylog=ylog,
                   xrange=xrange, yrange=yrange, 
                   title=title, pointRep=pointRep, oplot=oplot, color=color,
                   lineStyle=lineStyle, autoscale=autoscale)

def XYPlot(nt, xname, yname, xerr=None, yerr=None, xlog=0, ylog=0,
           xrange=(), yrange=(), title="", pointRep="Symbol", oplot=0,
           color=None, autoscale=0):
    if not xerr: xerr = 'nil'
    if not yerr: yerr = 'nil'
    disp = displayHandler( 'XY Plot', nt, (xname, yname, xerr, yerr),
                           (xlog, ylog), (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           autoscale=autoscale)
    try:
        disp.getDataRep().setSymbol('filled_square', 2)
    except AttributeError:
        disp.setSymbol('filled_square', 2)
    return disp

def xyplot(x, y, xerr=None, yerr=None, xname='x', yname='y',
           xerrname='xerr', yerrname='yerr', xlog=0, ylog=0,
           xrange=(), yrange=(), title="", pointRep="Symbol", 
           oplot=0, color=None, autoscale=0):
    if xerr is None:
        xerr = [0]*len(x)
    if yerr is None:
        yerr = [0]*len(y)
    nt = newNTuple( (x, y, xerr, yerr), (xname, yname, xerrname, yerrname),
                    name=title )
    return XYPlot(nt, xname, yname, xerrname, yerrname, xlog=xlog, ylog=ylog,
                  xrange=xrange, yrange=yrange, title=title, pointRep=pointRep,
                  oplot=oplot, color=color, autoscale=autoscale)

def StripChart(nt, xname, yname, xlog=0, ylog=0, xrange=(), yrange=(), 
               title="", pointRep="Line", oplot=0, color=None, autoscale=1):
    return displayHandler( "Strip Chart", nt, (xname, yname),
                           (xlog, ylog), (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           autoscale=autoscale )

def strip_chart(x, y, xname="x", yname="y", xlog=0, ylog=0,
                xrange=(), yrange=(), title="", pointRep="Line",
                oplot=0, color=None, autoscale=1):
    nt = newNTuple( (x, y), (xname, yname), name=title)
    return StripChart(nt, xname, yname, xlog=xlog, ylog=ylog,
                      xrange=xrange, yrange=yrange, title=title,
                      pointRep=pointRep, oplot=oplot, color=color,
                      autoscale=autoscale)

def Profile(nt, xname, yname, xlog=0, ylog=0, xrange=(), yrange=(),
            pointRep="Symbol", title="", oplot=0, color=None, autoscale=0,
            addToCanvas=1):
    return displayHandler( "Profile", nt, (xname, yname), 
                           (xlog, ylog), (xrange, yrange), pointRep,
                           title=title, oplot=oplot, color=color,
                           autoscale=autoscale, addToCanvas=addToCanvas )

def profile(x, y, xname="x", yname="y", xlog=0, ylog=0, xrange=(),
            yrange=(), pointRep="Symbol", title="", oplot=0, color=None,
            autoscale=0, addToCanvas=1):
    nt = newNTuple( (x, y), (xname, yname), name=title )
    return Profile(nt, xname, yname, xlog=xlog, ylog=ylog, xrange=xrange,
                   yrange=yrange, pointRep=pointRep, title=title,
                   oplot=oplot, color=color, autoscale=autoscale,
                   addToCanvas=addToCanvas )

def XYHist(nt, xname, yname, xbinsize=0, ybinsize=0,
           xrange=(), yrange=(), xlog=0, ylog=0, Aitoff=0, title="",
           pointRep="ColorBox", oplot=0, autoscale=0):
    display = displayHandler( "Color Plot", nt, (xname, yname),
                              (xlog, ylog), (xrange, yrange), pointRep,
                              title=title, oplot=oplot, autoscale=autoscale )
    if xbinsize > 0: display.setBinWidth('x', xbinsize, 1)
    if ybinsize > 0: display.setBinWidth('y', ybinsize, 1)
    if Aitoff: display.setTransform( "HammerAito" )
    return display

def xyhist(x, y, xname="x", yname="y", xbinsize=0, ybinsize=0,
           xrange=(), yrange=(), xlog=0, ylog=0, Aitoff=0, title="",
           pointRep="ColorBox", oplot=0, autoscale=0):
    nt = newNTuple( (x, y), (xname, yname), name=title)
    return XYHist(nt, xname, yname, xbinsize=xbinsize, ybinsize=ybinsize,
                  xrange=xrange, yrange=yrange, xlog=xlog, ylog=ylog,
                  Aitoff=Aitoff, title=title, oplot=oplot, autoscale=autoscale)

def XYZPlot(nt, xname, yname, zname, xrange=(), yrange=(), xlog=0, ylog=0,
            Aitoff=False, pointRep="ColorSymbol", title="", oplot=0,
            autoscale=0):
    display = displayHandler( "XYZ Plot", nt, (xname, yname, zname),
                              (xlog, ylog), (xrange, yrange), pointRep,
                              title=title, oplot=oplot, autoscale=autoscale )
    if Aitoff:
        display.setTransform( "HammerAito" )
    return display

#
# This function illustrates that Color Plots (XYHist) and Contour
# Plots differ only by their pointRep.
#
def Contour(nt, xname, yname, xbinsize=0, ybinsize=0,
            xrange=(), yrange=(), xlog=0, ylog=0, Aitoff=0, title="",
            pointRep="Contour", oplot=0, autoscale=0):
    display = displayHandler( "Color Plot", nt, (xname, yname),
                              (xlog, ylog), (xrange, yrange), pointRep,
                              title=title, oplot=oplot, autoscale=autoscale )
    if xbinsize > 0: display.setBinWidth('x', xbinsize, 1)
    if ybinsize > 0: display.setBinWidth('y', ybinsize, 1)
    if Aitoff: display.setTransform( "HammerAito" )
    return display

def contour(x, y, xname="x", yname="y", xbinsize=0, ybinsize=0,
            xrange=(), yrange=(), xlog=0, ylog=0, Aitoff=0, title="",
            pointRep="Contour", oplot=0, autoscale=0):
    nt = newNTuple( (x, y), (xname, yname), name=title)
    return XYHist(nt, xname, yname, xbinsize=xbinsize, ybinsize=ybinsize,
                  xrange=xrange, yrange=yrange, xlog=xlog, ylog=ylog,
                  Aitoff=Aitoff, title=title, oplot=oplot, autoscale=autoscale)

def uniq(xx):
    "Remove duplicates from a list and sort."
    dict = {}
    for x in xx:
        dict[x] = 1
    keys = dict.keys()
    keys.sort()
    return keys

def ZPlot(nt):
    registerNTuple(nt)
    image = hippo.Display("Image", nt, (nt.getLabel(2), ))
    axes = ("x", "y")
    labels = nt.getLabels()
    for axis, label in zip(axes, labels):
        array = uniq( nt.getColumn(label) )
        image.setNumberOfBins(axis, len(array))
        image.setBinWidth(axis, array[1]-array[0], 1)
        image.setRange(axis, min(array), max(array), 1)
        image.setOffset(axis, min(array))
    Canvas().addDisplay( image )
    return image

def fitsImage(fits_image, zlog=0, aspect=0, transform=None, title="",
              extension=0):
    if type(fits_image) == type(""):
        #
        # If fits_image is a string, assume it's a FITS image file name.
        #
        fitsImage = FitsImage(fits_image, extension=extension)
        addNTuple(fitsImage.nt)  # need this for NTuple persistence
    else:
        #
        # It is a FitsImage object.
        #
        fitsImage = fits_image
    image = Image(fitsImage.nt, zlog=zlog,
                  aspect=aspect, transform=transform, title=title)
    #
    # Assume, perhaps dangerously, that the default values of
    # the image dimensions are for a square matrix.
    #
    defaultSize = math.sqrt(fitsImage.nt.rows)
    nx = fitsImage.getKeyVal( "naxis1", default=defaultSize )
    ny = fitsImage.getKeyVal( "naxis2", nx )
    image.setNumberOfBins( "x", nx )
    image.setNumberOfBins( "y", ny )
    
    xstep = fitsImage.getKeyVal("cdelt1", 1)
    ystep = fitsImage.getKeyVal("cdelt2", 1)
    image.setBinWidth( "x", xstep, 1 )
    image.setBinWidth( "y", ystep, 1 )

    refpix1 = fitsImage.getKeyVal("crpix1", 1)
    refpix2 = fitsImage.getKeyVal("crpix2", 1)
    
    x0 = fitsImage.getKeyVal("crval1", 0)
    y0 = fitsImage.getKeyVal("crval2", 0)
    xoffset = -xstep*(refpix1 - 0.5) + x0
    yoffset = -ystep*(refpix2 - 0.5) + y0
    image.setOffset( "x", xoffset)
    image.setOffset( "y", yoffset)
    
    if xstep < 0 :
        image.setRange( 'x',
                        x0 + (nx + 0.5 - refpix1)*xstep,
                        x0 + (0.5 - refpix1)*xstep, 1 )
    else:
        image.setRange( 'x',
                        x0 + (0.5 - refpix1)*xstep,
                        x0 + (nx + 0.5 - refpix1)*xstep, 1 )
    image.setRange ( 'y',
                     y0 + (0.5 - refpix2)*ystep,
                     y0 + (ny + 0.5 - refpix2)*ystep, 1 )
    return image

def Image(nt, colname=None, x=None, y=None, xname='x', yname='y',
          zlog=0, aspect=0, transform=None, title=""):
    if colname is None:
        colname = nt.getLabel(0)
    image = hippo.Display("Image", nt, (colname, ))

    image.setLog( "z", zlog )
    image.setAspectRatio( aspect )
    if (transform):
        image.setTransform(transform)

    if x is not None:
        image.setNumberOfBins('x', len(x))
        xstep = x[1] - x[0]
        image.setBinWidth("x", xstep, 1)
        image.setOffset("x", x[0] - xstep)
        image.setRange('x', x[0], x[-1], 1)
        image.setLabel("x", xname)
        
    if y is not None:
        image.setNumberOfBins('y', len(y))
        ystep = y[1] - y[0]
        image.setBinWidth("y", ystep, 1)
        image.setOffset("y", y[0] - ystep)
        image.setRange('y', y[0], y[-1], 1)
        image.setLabel("y", yname)

    Canvas().addDisplay(image)
    return image

def image(zData, x=None, y=None, xname='x', yname='y', zname="z values",
          zlog=0, aspect=0, transform=None, title=""):
    import copy
    nt = newNTuple( (zData.flat, ), (zname, ) )
    
    disp = hippo.Display("Image", nt, (zname, ))
    
    disp.setLog( "z", zlog )
    disp.setAspectRatio( aspect )
    if (transform):
        disp.setTransform(transform)

    disp.setNumberOfBins('x', len(x))
    disp.setNumberOfBins('y', len(y))

    xstep = x[1]-x[0]
    ystep = y[1]-y[0]
    disp.setBinWidth("x", xstep, 1)
    disp.setBinWidth("y", ystep, 1)

    disp.setOffset("x", x[0] - xstep)
    disp.setOffset("y", y[0] - ystep)

    disp.setRange('x', x[0], x[-1], 1)
    disp.setRange('y', y[0], y[-1], 1)

    disp.setLabel("x", xname)
    disp.setLabel("y", yname)

    Canvas().addDisplay(disp)
    return disp

class FitsImage:
    """
    A FITS image class for plotting with hippoplotter.py
    """
    def __init__(self, file, extension=0):
        import copy
        import pyfits
        self.ext = extension
        #
        # Open the FITS image file.
        #
        self.image = pyfits.open(file)
        #
        # The primary header contains the coordinate data.
        #
        self.header = self.image[self.ext].header
        #
        # Create the image NTuple and register it with HippoDraw.
        #
        self.nt = hippo.NTuple()
        ntc = hippo.NTupleController.instance()
        ntc.registerNTuple(self.nt)
        #
        z = copy.deepcopy(self.image[self.ext].data)
        z.transpose()
        z = numarray.array(z).flat
        #
        # Use a place-holder name for now.
        #
        self.label = "Z values"
        self.nt.addColumn(self.label, z)
    def updateNTuple(self):
        z = self.image[self.ext].data
        z.transpose()
        z = numarray.array(z).flat
        self.nt.replaceColumn(self.label, z)
    def getKeyVal(self, keyword, default=None):
        #
        # Try to get the keyword value from the primary HDU.
        #
        return self.header.get(keyword, default)
    
def FitsNTuple(fitsFiles, extension=1, name=None, useNumArray=0):
    "Wrap the FitsNTuple class for use by hippoplotter"
    from FitsNTuple import FitsNTuple
    myTuple = FitsNTuple(fitsFiles, extension)
    if name is None:
        try:
            fitsFiles.swapcase
            name = "%s: %s" % (fitsFiles, extension)
        except:
            name = "%s: %s" % (fitsFiles[0], extension)
    nt = myTuple.makeNTuple(name=name, useNumArray=useNumArray)
    addNTuple(nt, key=name)
    return (nt, myTuple)

class StaticHist:
    def __init__(self, xrange=(-1, 1), xBinWidth = 0.1, title="", xlabel="",
                 color=None):
        self.display = hippo.Display( "Static Histogram" )
        self.display.setTitle(title)
        self.display.setLabel("x", xlabel)
        self.display.setRange("x", xrange[0], xrange[1], 1)
        self.display.setBinWidth("x", xBinWidth, 1)
        Canvas().addDisplay( self.display )
        if color:
            rep = self.display.getDataRep()
            rep.setColor(color)
    def __getattr__(self, attrname):
        return getattr(self.display, attrname)
    def __call__(self):
        return self.display

class Static2DHist:
    def __init__(self, xrange=(-1, 1), yrange=(-1, 1),
                 xBinWidth=0.1, yBinWidth=0.1,
                 title="", xlabel="", ylabel=""):
        self.display = hippo.Display( "Static 2D Histogram" )
        self.display.setTitle(title)
        self.display.setLabel("x", xlabel)
        self.display.setLabel("y", ylabel)
        self.display.setRange("x", xrange[0], xrange[1], 1)
        self.display.setRange("y", yrange[0], yrange[1], 1)
        self.display.setBinWidth("x", xBinWidth, 1)
        self.display.setBinWidth("y", yBinWidth, 1)
        Canvas().addDisplay( self.display )
    def __getattr__(self, attrname):
        return getattr(self.display, attrname)
    def __call__(self):
        return self.display

def Null():
    pass

def setCut(update=Null):
    newLimits = []
    while 1:
        while len(newLimits) < 2:
            pt = Canvas().mouseData()
            if not Canvas().getDisplay():
                return
            if (len(pt) == 3):
                newLimits.append(pt[:2])
        xrange = ( min(newLimits[0][0], newLimits[1][0]),
                   max(newLimits[0][0], newLimits[1][0]) )
        cut = Canvas().getCut()
        if cut:
            cut.setCutRange(xrange[0], xrange[1], 'x')
            update()
            newLimits =[]

def dragCut(update=Null):
    import math
    row = list(Canvas().mouseData())
    display = Canvas().getDisplay()
    while display:
        if (len(row) == 3):
            cut = Canvas().getCut()
            cutRange = cut.cutRange()
            if display.getLog('x'):
                halfWidth = math.log(cutRange[1]/cutRange[0])/2.
                cut.setCutRange(math.exp(math.log(row[0])-halfWidth),
                                math.exp(math.log(row[0])+halfWidth), 'x')
                update()
            else:
                halfWidth = (cutRange[1] - cutRange[0])/2.
                cut.setCutRange( row[0] - halfWidth, row[0] + halfWidth, 'x')
                update()
        row = list(Canvas().mouseData())
        display = Canvas().getDisplay()

def resize():
    newLimits = []
    while 1:
        while len(newLimits) < 2:
            pt = Canvas().mouseData()
            display = Canvas().getDisplay()
            if not display:
                return
            if (len(pt) == 3):
                newLimits.append(pt[:2])
        if display.numViews() == 0:
            display.saveView()
        xrange = ( min(newLimits[0][0], newLimits[1][0]),
                   max(newLimits[0][0], newLimits[1][0]) )
        yrange = ( min(newLimits[0][1], newLimits[1][1]),
                   max(newLimits[0][1], newLimits[1][1]) )
        display.setAutoRanging("x", 0)
        display.setAutoRanging("y", 0)
        display.setRange("x", xrange[0], xrange[1], 1)
        display.setRange("y", yrange[0], yrange[1], 1)
        newLimits = []
        display.saveView()

def fitData( display, funcName, fitter='ChiSq: Levenberg Marquart' ):
    try:
        rep = display.getDataRep()
    except AttributeError:
        rep = display
    func = hippo.Function( funcName, rep )
    try:
        func.addTo(display)
    except:    # big kluge here
        func.addTo(Canvas().getDisplay())
    func.setFitter(fitter)
    func.fit()
    paramDict = {}
    errorDict = {}
    names = func.parmNames()
    params = func.parameters()
    errors = func.principleErrors()
    for name, param, error in zip(names, params, errors):
        paramDict[name] = param
        errorDict[name] = error
    return (paramDict, errorDict, func)

def slopes():
    import math
    first = 1
    display = None
    while display or first:
        first = 0
        newLimits = []
        while len(newLimits) < 2:
            pt = Canvas().mouseData()
            if not Canvas().getDisplay():
                return None
            if (len(pt) == 3):
                newLimits.append(pt[:2])
        xrange = ( newLimits[0][0], newLimits[1][0] )
        yrange = ( newLimits[0][1], newLimits[1][1] )
        display = Canvas().getDisplay()
        xlog = 0; ylog = 0
        if display:
            if display.getLog("x"):
                xlog = 1
                xrange = (math.log(xrange[0]), math.log(xrange[1]))
            if display.getLog("y"):
                ylog = 1
                yrange = (math.log(yrange[0]), math.log(yrange[1]))
            slope = (yrange[1] - yrange[0])/(xrange[1] - xrange[0])
            intercept = yrange[1] - slope*xrange[1]
            print slope
            plotLine( display, slope, intercept, xlog, ylog )

def hline(y=None, color='black'):
    if y == None:
        pt = Canvas().mouseData()
        y = pt[1]
        print y
    display = Canvas().getDisplay()
    if display:
        plotLine(display, 0, y, 0, 0, lineStyle='Dot', color=color)

def vline(x=None, color='black'):
    if x == None:
        pt = Canvas().mouseData()
        x = pt[0]
        print x
    x = numarray.zeros(100) + x
    display = Canvas().getDisplay()
    yr = list(display.getRange("y"))
    if display.getBinWidth('x') > 0 and display.getBinWidth('y') < 0:
        yr[0] /= display.getBinWidth('x')
        yr[1] /= display.getBinWidth('x')
    ylog = display.getLog("y")
    if ylog:
        yr = ( numarray.log(yr[0]), numarray.log(yr[1]) )
    y = numarray.arange(100)/99.*(yr[1]-yr[0]) + yr[0]
    if ylog:
        y = numarray.exp(y)
    display.setAutoRanging("x", 0)
    display.setAutoRanging("y", 0)
    nt = newNTuple( (x, y), ('x', 'y'), register=0 )
    Scatter(nt, 'x', 'y', pointRep="Line", oplot=1, lineStyle='Dot',
            color=color)

def box(xmin, xmax, ymin, ymax, color='black'):
    x = [xmin, xmax, xmax, xmin, xmin]
    y = [ymin, ymin, ymax, ymax, ymin]
    scatter(x, y, oplot=1, pointRep='Line', lineStyle='Dot', color=color)

def plotLine(display, slope, intercept, xlog, ylog, lineStyle='Dot',
             color='black'):
    f = lambda x: slope*x + intercept
    xr = display.getRange("x")
    if xlog:
        xr = ( numarray.log(xr[0]), numarray.log(xr[1]) )
    xx = numarray.arange(100)/99.*(xr[1] - xr[0]) + xr[0]
    yy = numarray.array([f(x) for x in xx])
    if ylog:
        yy = numarray.exp(yy)
    if xlog:
        xx = numarray.exp(xx)
    ylabel = display.getLabel("y")
    xBinWidth = display.getDataRep().getBinWidth("x")
    if xBinWidth and ylabel == "Entries / bin":
        yy = yy/xBinWidth
    Canvas().selectDisplay( display )
    display.setAutoRanging("x", 0)
    display.setAutoRanging("y", 0)
    nt = newNTuple( (xx, yy), ('x', 'y'), register=0 )
    Scatter(nt, 'x', 'y', pointRep="Line", oplot=1, lineStyle=lineStyle,
            color=color)

def pickData():
    row = list(Canvas().mouseData())
    display = Canvas().getDisplay()
    dispId = display.plotterId()
    rep = display.getDataRep()
    xlabel = display.getLabel("x")
    labels = [xlabel]
    ylabel = display.getLabel("y")
    labels.append(ylabel)
    labels.append(display.getLabel("z"))
    #
    # Create phantom columns for plotting the pick points.  These
    # columns are needed for data that is binned on a linear x-axis
    # scaling.
    #
    xBinWidth = rep.getBinWidth("x")
    yBinWidth = rep.getBinWidth("y")
    labels.append("phantom_" + xlabel)
    labels.append("phantom_" + ylabel)
    nt = newNTuple( ([], [], [], [], []), labels, register=0 )
    Scatter(nt, labels[3], labels[4], oplot=1, color="red")
    while display:
        if len(row) == 3 and display.plotterId() == dispId:
            print row
            row.append(row[0])
            if xBinWidth and ylabel == "Entries / bin":
                yvalue = row[1]/xBinWidth
                row.append(yvalue)
            else:
                row.append(row[1])
            nt.addRow(row)
        row = list(Canvas().mouseData())
        display = Canvas().getDisplay()
    return nt

def clear():
    displays = Canvas().getDisplays()
    for display in displays:
        Canvas().removeDisplay(display)

def prompt(prompt=None):
    if (prompt):
        sys.stderr.write(prompt)
    else:
        sys.stderr.write("Hit return to continue: ")
    x = sys.stdin.readline()
    return x

def setParam(target, value, func):
    names = func.parmNames()
    vals = list(func.parameters())
    for i, name in zip(xrange(len(names)), names):
        if name == target:
            vals[i] = value
    func.setParameters(vals)

def setFixed(targets, func):
    names = func.parmNames()
    flags = []
    for name in names:
        if name in targets:
            flags.append(1)
        else:
            flags.append(0)
    func.setFixedFlags(flags)

def plot_test():
    import math
    phi = map(lambda x: x/49.*2.*math.pi, range(50))
    x = map(lambda t: math.cos(t), phi)
    y = map(lambda t: math.sin(t), phi)
    scatter(x, y, "x", "y", pointRep="Line")
    nt = newNTuple( (x, y), ("xx", "yy"), "My NTuple")
    Scatter(nt, "xx", "yy", pointRep="Line")
    scatter(x, y, "x", "y")

def add_titles():
    ntuples[0].setTitle("x vs y")
    ntuples["My NTuple"].setTitle("xx vs yy")
    ntuples[1].setTitle("none")
    Scatter(ntuples[1], "x", "y", pointRep="Line")

def run_tests():
    plot_test()
    add_titles()

if __name__ == "__main__":
    run_tests()
