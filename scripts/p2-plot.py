#!/usr/bin/env python

import sys
import numpy
import pandas
import math
import matplotlib.pylab as pylab
import matplotlib.font_manager as fm
from matplotlib.font_manager import FontProperties
from mpl_toolkits.mplot3d import Axes3D
import pvplot as pv
import logging


def readInFile1(filename):
    # Define data types in file, by specifying local name for each column
    # and the format of the data in each column
    dtype = numpy.dtype({'names': ['queueType', 'load', 'winSize', 'queSize', 'goodputT1', 'goodputT2', 'goodputU'], 
                         'formats': [object, 'i', 'i', 'i', 'i', 'i', 'i']})

    # Get data from file with datatypes=dtype and skip the first line
    data = numpy.genfromtxt(filename,
                            dtype=dtype,
                            skip_header=1)
    return data


def readInFile2(filename):
    # Define data types in file, by specifying local name for each column
    # and the format of the data in each column

    dtype = numpy.dtype({'names': ['queueType', 'load', 'winSize', 'queSize', 'minth', 'maxth', 'maxp', 'wq', 'qlen', 'goodputT1', 'goodputT2', 'goodputU'], 
                         'formats': [object, 'i', 'i', 'i', 'f', 'f', 'f', 'f', 'i', 'i', 'i', 'i']})

    # Get data from file with datatypes=dtype and skip the first line
    data = numpy.genfromtxt(filename,
                            dtype=dtype,
                            skip_header=1)
    return data


def getFont(family='serif', color='black', weight='normal', size='12'):
    font = {'family' : family,
            'color'  : color,
            'weight' : weight,
            'size'   : size,}
    return font


def plotData1():
    # get data from file
    d = readInFile1('/home/pete/myRepos/ns3-sims/data/p2-droptail.data')

    # get unique data
    loadSizes = numpy.unique(d['load'])
    queSizes = numpy.unique(d['queSize'])

    # convert data to pandas DataFrame for convenience
    data = pandas.DataFrame(d)

    # get a font
    font = getFont('serif', 'darkred', 'normal', 14)

    # make queue length sizes the categories
    for l in range(len(loadSizes)):

        # get data corresponding to tcp protocol and window size
        tempData = data.ix[l*4:l*4+3,:]#.ix[(data.load == 60)]

        # set bar categories, x-data and y-data
        categories = ['Tcp1', 'Tcp2', 'Udp1']
        xdata = queSizes    # x data
        ydata = {}

        for i in range(3):
            ydata[i] = tempData.ix[:,i+4]
            print 'ydata[i]',ydata[i]

        chart = pv.BarChart()
        # chart.setLogLevel(logging.DEBUG)
        chart.setData(xdata, ydata, categories)
        chart.setFont(font)
        chart.setTitle('DropTail Goodput Using a load of {}%'.format(loadSizes[l]))
        chart.setXLabel('Queue Length (Bytes)')
        chart.setYLabel('Goodput (Bytes/sec)')
        chart.setLegend('Flow Type', 'center right', 0.0, FontProperties(), (1.124, 0.9))
        chart.savePlotAs('p2-droptail-load{}.png'.format(loadSizes[l]))
        chart.plot()



def plotData2():
    # get data from file
    d = readInFile2('/home/pete/myRepos/ns3-sims/data/p2-red.data')

    # get unique data
    loadSizes = numpy.unique(d['load'])
    queSizes = numpy.unique(d['queSize'])

    # convert data to pandas DataFrame for convenience
    data = pandas.DataFrame(d)

    # get a font
    font = getFont('serif', 'darkred', 'normal', 14)

    # make queue length sizes the categories
    for l in range(1):

        # get data corresponding to tcp protocol and window size
        tempData = data

        # set bar categories, x-data and y-data
        categories = ['Tcp1', 'Tcp2', 'Udp1']
        xdata = loadSizes    # x data
        ydata = {}

        for i in range(3):
            ydata[i] = tempData.ix[:,i+9]
            print 'ydata', ydata[i]

        chart = pv.BarChart()
        # chart.setLogLevel(logging.DEBUG)
        chart.setData(xdata, ydata, categories)
        chart.setFont(font)
        chart.setTitle('RED Goodputs With Varying Loads')
        chart.setXLabel('Load (%)')
        chart.setYLabel('Goodput (Bytes/sec)')
        chart.setLegend('Flow Type', 'center right', 0.0, FontProperties(), (1.124, 0.9))
        chart.savePlotAs('p2-red-loads.png')
        chart.plot()

if __name__ == '__main__':
    plotData2()
