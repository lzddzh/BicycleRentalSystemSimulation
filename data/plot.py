import sys
import numpy as np
import matplotlib.pyplot as plt

fr = open(sys.argv[1])
s = [ float(line.strip()) for line in fr.readlines()]
print "mean", np.mean(s)
print "std dev", np.std(s)
hist, bins = np.histogram(s, bins=50)
width = 0.7 * (bins[1] - bins[0])
center = (bins[:-1] + bins[1:]) / 2
plt.bar(center, hist, align='center', width=width)
plt.title(sys.argv[1])
plt.show()
