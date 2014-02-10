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

"""
@brief Creates a bar chart.
@param categories Categories for each set of bars (data)
@param xdata X-axis data (data)
@param ydata Y-axis data. 2D array, categories by data (data)
@param title Title of plot (string)
@param xlabel X-axis label (string)
@param ylabel Y-axis label (string)
@param font Font for the plot's labels and title (font)
@param plot Whether or not to plot the figure (bool)
@param saveImage Whether or not to save the image (bool)
@param imageName Name of saved png image
"""
def bar_chart(categories, xdata, ydata,
              title, xlabel, ylabel,
              font={'family':'serif','color':'black','weight':'normal','size':12,},
              plot=True, saveImage=False, imageName='fig.png'):

    colors = 'rgbcmyk'
    colors = colors[:(len(categories))]

    ind  = pylab.arange(len(xdata))
    bar_width = 1.0 / (len(categories) + 1)
    bar_groups = []

    # loop through categories and plot one bar in each category every loop (ie., one color at a time.)
    fig = pylab.figure()
    for c in range(len(categories)):
        bars = pylab.bar(ind+c*bar_width, ydata[c], bar_width, color=colors[c % len(colors)])
        bar_groups.append(bars)

    fontP = FontProperties()
    fontP.set_size('small')
    pylab.xticks(ind+bar_width, xdata)
    pylab.legend([b[0] for b in bar_groups], categories, 
                 loc='center right', title='Flow #', labelspacing=0,
                 prop=fontP, bbox_to_anchor=(1.125, .7))
    pylab.xlabel(xlabel, fontdict=font)
    pylab.ylabel(ylabel, fontdict=font)
    pylab.title(title, fontdict=font)

    # save the figure
    if saveImage:
        pylab.savefig(imageName)

    # plot the figure
    if plot:
        pylab.show()


def readInFile(filename, numBulkSendApps):
    # Define data types in file, by specifying local name for each column
    # and the format of the data in each column
    if(numBulkSendApps == 1):
        dtype = numpy.dtype({'names': ['tcpType', 'winSize', 'queSize', 'segSize', 'goodput'], 
                             'formats': [object, 'i', 'i', 'i', 'i', 'i']})
    elif (numBulkSendApps == 10):
        dtype = numpy.dtype({'names': ['tcpType', 'winSize', 'queSize', 'segSize', 'goodput0', 'goodput1', 'goodput2', 'goodput3', 'goodput4', 'goodput5', 'goodput6', 'goodput7', 'goodput8', 'goodput9'], 
                             'formats': [object, 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i', 'i']})

    else:
        print "Unexpected number of bulksendapps: ", numBulkSendApps
        exit(-1)

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
    d = readInFile('/home/pete/myRepos/ns3-sims/data/p1-1_BulkSendApp.data', 1)

    # get unique data
    winSizes = numpy.unique(d['winSize'])
    queSizes = numpy.unique(d['queSize'])
    tcpTypes = numpy.unique(d['tcpType'])
    segSizes = numpy.unique(d['segSize'])

    # convert data to pandas DataFrame for convenience
    data = pandas.DataFrame(d)

    # get a font
    font = getFont('serif', 'darkred', 'normal', 14)

    # make segment sizes the categories
    for t in range(len(tcpTypes)):
        for w in range(len(winSizes)):
            # get data corresponding to tcp protocol and window size
            tempData = data.ix[(data.tcpType == tcpTypes[t]) & (data.winSize == winSizes[w])]

            # set bar categories, x-data and y-data
            categories = numpy.unique(tempData.segSize)
            xdata = queSizes    # x data
            ydata = {}
            print tempData
            for s, i in zip(segSizes, range(len(segSizes))):
                td = tempData.ix[(tempData.segSize == s)].goodput
                ydata[i] = td

            exit()
            # plot bar chart
            title = 'TCP Goodput Using {} with a Window Size of {}'.format(tcpTypes[t], winSizes[w])
            xlabel= 'Queue Size (Bytes)'
            ylabel= 'Goodput (Bytes/sec)'
            if(winSizes[w] == 2000):
                s='2000'
            elif (winSizes[w] == 8000):
                s='8000'
            elif(winSizes[w] == 32000):
                s='32000'
            elif(winSizes[w] == 64000):
                s='64000'
            imageName = tcpTypes[t] + "-win" + s + ".png"
            bar_chart(categories, xdata, ydata,
                      title, xlabel, ylabel, font,
                      True, True, imageName)


def plotData2():
    # get data from file
    d = readInFile('/home/pete/myRepos/ns3-sims/data/p1-10_BulkSendApps.data', 10)

    # get unique data
    winSizes = numpy.unique(d['winSize'])
    queSizes = numpy.unique(d['queSize'])
    tcpTypes = numpy.unique(d['tcpType'])
    segSizes = numpy.unique(d['segSize'])

    # convert data to pandas DataFrame for convenience
    data = pandas.DataFrame(d)

    # get a font
    font = getFont('serif', 'darkred', 'normal', 14)

    # make segment sizes the categories
    for t in range(len(tcpTypes)):
        for w in range(2):
            for q in range(2):
                # get data corresponding to tcp protocol and window size
                tempData = data.ix[(data.tcpType == tcpTypes[t]) &
                                   (data.winSize == winSizes[w]) &
                                   (data.queSize == queSizes[q])]

                # set bar categories, x-data and y-data
                categories = range(10)
                xdata = segSizes    # x data
                ydata = {}
                
                for i in range(10):
                    td = tempData.ix[:,i+4]
                    ydata[i] = td

                # plot bar chart
                title = '{} Goodput Using a Window Size of {} and Queue Size of {}'.format(tcpTypes[t], winSizes[w], queSizes[q])
                xlabel= 'Segment Size (Bytes)'
                ylabel= 'Goodput (Bytes/sec)'
                if(winSizes[w] == 2000):
                    s='2000'
                elif (winSizes[w] == 8000):
                    s='8000'
                elif(winSizes[w] == 32000):
                    s='32000'
                elif(winSizes[w] == 64000):
                    s='64000'
                imageName = tcpTypes[t] + "-win" + s + ".png"
                # print 'xdata: \n', xdata
                print '\n\nydata: \n', ydata
                # print '\n\ncat: \n', categories
                chart = pv.BarChart()
                chart.setData(xdata, ydata, categories)
                chart.setFont(font)
                chart.setTitleAndLabels('Test Title', 'Segment Size (Bytes)', 'Goodput (Bytes/sec)')
                chart.setLegend('Flow #', 'center right', 0.0, FontProperties(), (1.124, 0.7))
                chart.plot()
                exit()
                bar_chart(categories, xdata, ydata,
                          title, xlabel, ylabel, font,
                          True, True, imageName)
                w += 2
                q += 2


if __name__ == '__main__':
    plotData2()
