import numpy as np
import sys
from scipy.stats import beta

toint = {'a': 1, 'b':2, 'c':3, 'd':4}
np.random.seed(int(sys.argv[-1]) + toint[sys.argv[4]])
if len(sys.argv) <= 1: 
    print "enter: python beta.py [a] [b] [count] [filename]"
    exit()
a, b, num = float(sys.argv[1]), float(sys.argv[2]), int(sys.argv[3])
filename = sys.argv[4]
s = beta.rvs(a, b, size=num)
#random_state=toint[sys.argv[4]])
if filename == 'a':
    #       km  / m/s * 60
    s = [ (3.0 * 0.27778) / (e * 6.53 + 2.32) * 60 for e in s]
elif filename == 'b':
    s = [ (3.5 * 0.27778) / (e * 6.53 + 2.32) * 60 for e in s]
elif filename == 'c':
    s = [ (4.0 * 0.27778) / (e * 6.53 + 2.32) * 60 for e in s]
elif filename == 'd':
    s = [ (2.0 * 0.27778) / (e * 6.53 + 2.32) * 60 for e in s]
print "pathtime ", np.array(s)
with open('pathtime/' + filename, 'w') as outf:
    for each in s:
        outf.write(str(each) + "\n")
    


