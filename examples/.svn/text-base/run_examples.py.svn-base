"""
Run example scripts.

@author J. Chiang <jchiang@slac.stanford.edu>
@author Paul F. Kunz <Paul_Kunz@slac.stanford.edu>

"""
#$Id: run_examples.py,v 1.13 2006/10/03 20:02:20 pfkeb Exp $

import sys

from load_hippo import app, canvas

prompt = 1

scripts = []
scripts.append('static_vs_dynamic')
scripts.append('loglog')
scripts.append('datareps')
scripts.append('append_ntuple')
scripts.append('cut_multi_displays')
scripts.append('cuts_one_display')
scripts.append('cuts_complex')
scripts.append('function_ntuple')
scripts.append('fitting')
scripts.append('fitting2')
scripts.append('simple_xyplot')
scripts.append('mainpage')
scripts.append('fft')
scripts.append('displays')

def prompt(prompt = None):
    if (prompt):
        sys.stderr.write(prompt)
    else:
        sys.stderr.write("Hit return to continue: ")
    x = sys.stdin.readline()
    return x


print "Hit return to run named script"

for name in scripts :
    prompt("Run %s: " % name)
    canvas.clear()
    command = 'import ' + name
    exec(command)

print "All done. Enjoy!"
