#!/usr/bin/env python3

import os

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

def draw(filenames, labels, dest_filename, title=None):
    plt.rcParams["legend.markerscale"] = 1.0
    plt.rcParams['font.family'] = 'sans-serif'
    plt.rcParams['font.sans-serif'] = ['PT Sans']
    plt.rcParams['font.size'] = '12'
    plt.rcParams["legend.loc"] = "upper left" # 'upper left', 'upper right', 'lower left', 'lower right', 'center'
    plt.rcParams["legend.fontsize"] = "6"
    cm = 1/2.54  # centimeters in inches
    fig = plt.figure(figsize=(11*cm, 8*cm))
    ax = fig.add_subplot(111)
    ax.set_title(title, fontsize="x-small", fontweight="bold")
    ax.set(xlabel="Threads", ylabel="Speedup")
    ax.label_outer()
    #ax.xaxis.set_ticks(np.arange(min(x), max(x)+1, 1))
    #ax.yaxis.set_ticks(np.arange(min(y), max(y)+1, 1))
    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    #ax.loglog()

    i = 0
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        x = data[:, 0]
        y = data[:, 1]

        #ax.plot(x,y,'-o',markersize=1,c="green")
        ax.plot(x,y,'-o',markersize=1,linewidth=0.75,label=datalabel)
        #x.bar(x, y);
    ax.plot(range(2,9),range(2,9),'-',c="blue",linewidth=0.5,label="Linear speedup")

    plt.tight_layout()
    ax.legend()
    # plt.title(title, fontsize="xx-small", loc="left", fontweight="bold")
    fig.savefig(dest_filename, dpi=300)

if __name__ == "__main__":
    draw(["1.dat", "2.dat", "3.dat"], ["N = 1 000 000 000", "N = 2 000 000 000", "N = 3 000 000 000"], "th_1000.png", "THRESHOLD = 1000")
