import numpy as np
import sys

np.random.seed(int(sys.argv[-1]))
if len(sys.argv) <= 1: 
    print "enter: python exponential.py [mean] [count] [filename]"
    exit()
mean, num = float(sys.argv[1]), int(sys.argv[2])
filename = sys.argv[3]
s = np.random.exponential(mean, num)
print "exponential ", s
with open('create/' + filename, 'w') as outf:
    for each in s:
        outf.write(str(each) + "\n")
    


