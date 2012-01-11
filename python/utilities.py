"""
Utility functions.

@author J. Chiang <jchiang@slac.stanford.edu>

"""
#
# $Id: utilities.py,v 1.2 2003/10/05 16:30:40 jchiang Exp $
#
import math, numarray

def locate(x, xx):
    """
    Binary search algorithm.
    usage: indx = locate(x, xx)
    x = input array; it must be in ascending order.
    xx = the search value
    indx = the index such that x[indx] < xx < x[indx+1];
           0 if xx <= x[0]
           len(x) - 2 if xx >= x[-1]
    """
    imin = 0
    imax = len(x) - 1
    if (xx <= x[imin]): return imin
    if (xx >= x[-1]): return imax-1
    while (x[imin] < xx < x[imax]):
        inew = (imin + imax)/2
        if (xx > x[inew]): imin = inew
        if (xx == x[inew] or imax-imin == 1): return inew
        if (xx < x[inew]): imax = inew
    return inew

def interpolate(x, y, xx):
    """
    Linear interpolation.
    usage: yy = interpolate(x, y, xx)
    x = abscissa array
    y = ordinate array
    xx = desired abscissa value
    yy = desired ordinate value
    """
    i = locate(x, xx)
    return (xx - x[i])/(x[i+1] - x[i])*(y[i+1] - y[i]) + y[i]

class gaussian:
    """
    1D Gaussian function class.
    usage: f = gaussian(mean, sigma);
           y = f(x), where x is a single value or a list and y returns
           as a like object.
           yint = f.integral(xmin, xmax)
    """
    import numarray
    def __init__(self, mean, sigma):
        self.mean = mean
        self.sigma = sigma
    def __call__(self, x):
        if (type(x) == type(1) or type(x) == type(1.)):
            return self.value(x)
        elif (type(x) == type([])):
            my_list = []
            for xx in x:
                my_list.append(self.value(xx))
            return my_list
        elif (type(x) == numarray.NumArray):
            return (numarray.exp(-numarray.power((x-self.mean)/self.sigma, 2)/2.)
                    /numarray.sqrt(2.*numarray.pi)/self.sigma)
    def value(self, x):
        return (math.exp( -math.pow((x - self.mean)/self.sigma, 2)/2. )
                /math.sqrt(2.*math.pi)/self.sigma)
    def integral(self, xmin, xmax):
        zmin = (xmin - self.mean)/math.sqrt(2.)/self.sigma
        zmax = (xmax - self.mean)/math.sqrt(2.)/self.sigma
        return (self.erfcc(zmin) - self.erfcc(zmax))/2.
    def erfcc(self, x):
        z = abs(x)
        t = 1./(1. + z/2.)
        ans = ( t*math.exp(-z*z-1.26551223+t*(1.00002368+
                t*(0.37409196+t*(0.09678418+t*(-0.18628806+
                t*(0.27886807+t*(-1.13520398+t*(1.48851587+
                t*(-0.82215223+t*0.17087277))))))))) )
        if x >= 0:
            return ans
        else:
            return 2. - ans

class histogram:
    """
    A simple histogramming class.
    usage: my_hist = histogram(xmin=0, xmax=1, nbins=30)
           my_hist.fill(x)           # add an entry
           x = my_hist.abscissa()    # return a numarray of x-axis values
           y = my_hist[i]            # index operator for accessing bin values
           my_hist.min()             # minimum and maximum bin values
           my_hist.max()
    """
    def __init__(self, xmin=0, xmax=1, nbins=30):
        self.xmin = xmin
        self.xmax = xmax
        self.nbins = nbins
        self.xstep = float(xmax - xmin)/float(nbins)
        self.hist = numarray.zeros(nbins)
    def fill(self, x):
        if x > self.xmin and x < self.xmax:
            indx = int((x - self.xmin)/self.xstep)
            if indx < self.nbins: self.hist[indx] = self.hist[indx] + 1
    def abscissa(self):
        return (numarray.arange(self.nbins)*self.xstep + self.xstep/2.
                + self.xmin)
    def __getitem__(self, index):
        return self.hist[index]
    def min(self):
        return min(self.hist)
    def max(self):
        return max(self.hist)
