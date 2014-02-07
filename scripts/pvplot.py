#!/usr/bin/env python

# Plotting imports
import matplotlib.pylab as pylab
from matplotlib.font_manager import FontProperties
# Logging imports
import logging as log

class BarChart:

    def __init__(self):
        # Appearance variables
        self._font = {'family':'serif', 'color':'black', 'weight':'normal', 'size':'12',}
        self._colors = 'rgbcmyk'
        # Labels
        self._title = 'Figure'
        self._xLabel = 'X-Axis'
        self._yLabel = 'Y-Axis'
        self._saveName = 'fig.png'
        # Data
        self._xData = {}
        self._yData = {}
        self._categoryData = {}
        # Legend
        self._usingLegend = False
        self._legendTitle = ''
        self._legendLocation = 'center right'
        self._legendLabelSpacing = 0.0
        self._legendFontProp = FontProperties()
        self._legendBboxToAnchor = None
        log.debug('Finished BarChart __init__()')

    def setColors(self, colors):
        self._colors = colors
        log.debug('Setting bar colors')

    def setLegend(self, legendTitle='Legend', legendLocation='center top',
                  labelspacing=0.0, fontProp=FontProperties(), bbox_to_anchor=None):
        self._usingLegend = True
        self._legendTitle = legendTitle
        self._legendLocation = legendLocation
        self._legendLabelSpacing = labelspacing
        self._legendFontProps = fontProp
        self._legendBboxToAnchor = bbox_to_anchor
        log.debug('Set legend parameters')

    def setFont(self, font):
        self._font = font
        log.debug('Setting font')

    def setXData(self, xData):
        self._xData = xData
        log.debug('Setting x-data')

    def setYData(self, yData):
        self._yData = yData
        log.debug('Setting y-data')

    def setCategoryData(self, categoryData):
        self._categoryData = categoryData
        log.debug('Setting category data')

    def setData(self, xData, yData, categoryData):
        self.setXData(xData)
        self.setYData(yData)
        self.setCategoryData(categoryData)
        log.debug('Setting x, y and category data')

    def setTitle(self, title):
        self._title = title
        log.debug('Setting plot title')

    def setXLabel(self, xLabel):
        self._xLabel = xLabel
        log.debug('Setting x-axis label')

    def setYLabel(self, yLabel):
        self._yLabel = yLabel
        log.debug('Setting y-axis label')

    def setTitleAndLabels(self, title, xLabel, yLabel):
        self.setTitle(title)
        self.setXLabel(xLabel)
        self.setYLabel(yLabel)
        log.debug('Setting title and axis labels')

    def setSaveName(self, saveName):
        self._saveName = saveName
        log.debug('Setting save name')

    def savePlotAs(self, imageName):
        pylab.savefig(imageName)
        log.debug('Saveing plot as ', imageName)

    def readyToPlot(self):
        if (self._xData == {} or self._yData == {} or self._categoryData == {}):
            return False
        else:
            return True

    def plot(self):
        print 'plotting'
        colors = self._colors[:(len(self._categoryData))]
        ind = pylab.arange(len(self._xData))
        bar_width = 1.0 / (len(self._categoryData) + 1)
        bar_groups = []

        for c in range(len(self._categoryData)):
            bars = pylab.bar(ind+c*bar_width, self._yData[c], bar_width, color=colors[c % len(colors)])
            bar_groups.append(bars)

        pylab.xticks(ind+bar_width, self._xData)
        if (self._usingLegend):
            pylab.legend((b[0] for b in bar_groups), self._categoryData,
                         title = self._legendTitle, loc = self._legendLocation,
                         labelspacing = self._legendLabelSpacing, 
                         prop = self._legendFontProps, bbox_to_anchor = self._legendBboxToAnchor)

        pylab.xlabel(self._xLabel, fontdict=self._font)
        pylab.ylabel(self._yLabel, fontdict=self._font)
        pylab.title(self._title, fontdict=self._font)

        pylab.show()
        
class LineChart:

    def __init__(self):

