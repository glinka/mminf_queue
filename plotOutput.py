import os
import numpy as np
import matplotlib.pyplot as plt

from mpltools import style
from mpltools import layout

style.use('ggplot')

def getData(filename, **kwargs):
    path_to_file = os.path.realpath(filename)
    data = np.genfromtxt(path_to_file, delimiter=",", skip_header=0, **kwargs)
    print 'done importing data from', filename
    return data

def plotQueue(times, queueSizes, ax=None):
    if ax is not None:
        print times.shape, queueSizes.shape
        ax.scatter(times, queueSizes, lw=0, alpha=0.7)
    else:
        fig = plt.figure()
        ax = fig.add_subplot(111)
        ax.scatter(times, queueSizes, lw=0, alpha=0.7)
        plt.show(fig)

if __name__=="__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('inputFiles', nargs='+')
    args = parser.parse_args()
    fig = plt.figure()
    ax = fig.add_subplot(111)
    timeData = None
    for f in args.inputFiles:
        if 'time' in f:
            timeData = getData(f)
    for f in args.inputFiles:
        if 'yData' in f:
             plotQueue(timeData, getData(f), ax)
    plt.savefig('queueResults.png')

