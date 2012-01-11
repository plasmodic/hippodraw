"""
Decorator class to convert ordinary Python functions to FunctionBase
objects for use by HippoDraw.

@author J. Chiang <jchiang@slac.stanford.edu>
"""
# $Id: FunctionBaseDecorator.py,v 1.5 2005/08/11 22:49:09 jchiang Exp $

from hippo import FunctionBase

_clones = []

class FunctionBaseDecorator(FunctionBase):
    _eps = 1e-5
    def __init__(self, func=None):
        if func is None:
            raise RuntimeError, ("FunctionBaseDecorator cannot have " +
                                 "a default constructor")
        FunctionBase.__init__(self)
        self.func = func
        argcount = func.func_code.co_argcount
        args = func.func_code.co_varnames[:argcount]
        defaults = func.func_defaults
        self.setName(str(func.__name__))
        self.setParmNames(args[-len(defaults):])
        self.setParameters(defaults)
        self._dims = len(args) - len(defaults)
    def __call__(self, *args):
        return self.valueAt(*args)
    def valueAt(self, *args):
        my_args = list(args)
        my_args.extend(self.getParameters())
        return self.func(*my_args)
    def derivByParm(self, i, *args):
        params = self.getParameters()
        f0 = self.valueAt(*args)
        new_params = list(params)
        if new_params[i] != 0:
            delta = self._eps*new_params[i]
        else:
            delta = self._eps
        my_args = list(args)
        my_args.extend(new_params)
        f1 = self.func(my_args)
        return (f1 - f0)/delta
    def dimensions(self):
        return self._dims
    def __setitem__(self, parname, value):
        values = list(self.getParameters())
        names = list(self.parmNames())
        try:
            values[names.index(parname)] = value
            self.setParameters(values)
        except ValueError:
            pass
    def clone(self):
        _clones.append(FunctionBaseDecorator(self.func))
        return _clones[-1]
    def register(self):
        from hippo import FunctionFactory
        FunctionFactory.instance().add(self)

if __name__ == '__main__':
    import hippoplotter as plot
    disp = plot.scatter([-2, 2], [-1, 2])
    f = FunctionBaseDecorator(lambda x, a=1, b=0: a*x + b)

    @FunctionBaseDecorator
    def g(x, a=1, b=0):
        return a*x**2 + b

    disp.addFunction(f)
    disp.addFunction(g)
