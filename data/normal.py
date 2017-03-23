import numpy as np
import sys

np.random.seed(int(sys.argv[-1]))

if len(sys.argv) <= 1: 
    print "enter: python normal.py [mu] [sigma] [count] [filename] [seed]"
    exit()
mu, sigma, num = float(sys.argv[1]), float(sys.argv[2]), int(sys.argv[3])
filename = sys.argv[4]
s = np.random.normal(mu, sigma, num)
print "normal: ", s
with open('playtime/' + filename, 'w') as outf:
    for each in s:
        outf.write(str(each) + "\n")
    


