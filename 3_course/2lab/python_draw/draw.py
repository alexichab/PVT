#!/usr/bin/env python3

import os
import sys

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np

def draw(filenames, labels, dest_filename):

    plt.rcParams["legend.markerscale"] = 1.0
    # plt.rcParams['font.family'] = 'sans-serif'
    # plt.rcParams['font.sans-serif'] = ['PT Sans']
    plt.rcParams['font.size'] = '12'
    plt.rcParams["legend.loc"] = "upper left" # 'upper left', 'upper right', 'lower left', 'lower right', 'center'
    plt.rcParams["legend.fontsize"] = "7"
    plt.xticks([2, 4, 6, 8], color="black", size="7")
    cm = 1.4/2.54  # centimeters in inches
    fig = plt.figure(figsize=(10*cm, 7*cm))
    ax = fig.add_subplot(111)
    ax.set_title("")
    ax.set(xlabel="Processes", ylabel="Relative Speedup")
    ax.label_outer()
    ax.xaxis.set_major_locator(ticker.MultipleLocator(2))
    ax.yaxis.set_major_locator(ticker.MultipleLocator(2))
    #ax.xaxis.set_ticks(np.arange(min(x), max(x)+1, 1))
    #ax.yaxis.set_ticks(np.arange(min(y), max(y)+1, 1))
    ax.xaxis.set_tick_params(direction='in', which='both')
    ax.yaxis.set_tick_params(direction='in', which='both')
    #ax.loglog()

    ax.text(3.6, -0.6, "2x2")
    ax.text(7.6, -0.6, "2x4")
    ax.text(11.6, -0.6, "2x6")
    ax.text(15.6, -0.6, "2x8")
    X = [4, 6, 8, 10, 12, 14, 16]
    Y = [2, 3, 4, 5, 6, 7, 8]
    i = 0
    for (fname, datalabel) in zip(filenames, labels):
        data = np.loadtxt(fname)
        x = data[:, 0]
        y = data[:, 1]

        #ax.plot(x,y,'-o',markersize=1,c="green")
        ax.plot(x,y,'-o',markersize=1,linewidth=0.75,label=datalabel)
        #x.bar(x, y);
    # ax.plot(2,4,'--',c="black",linewidth=0.3,label="nthreads > ncores")
    # ax.vlines(4, 0.4, 8, color='black', linewidth=0.3, linestyle='--')
    ax.plot(X,Y,'-',c="blue",linewidth=0.5,label="Linear speedup")

    plt.tight_layout()
    ax.legend()
    fig.savefig(dest_filename, dpi=300)

if __name__ == "__main__":
    draw(["midpoint.dat"], ["n0=300000000"], "midpoint.png")
    draw(["montecarlo.dat", "montecarlo2.dat"], ["n=10^7","n=10^8"], "montecarlo.png")
    # draw(["mc_2n.dat"], ["N=100000000"], "mc_2n.png", 4, 17)
    # draw(["data/runge_n2.dat"], ["N=60000000"], "charts/runge_n2.png", 4, 17)
