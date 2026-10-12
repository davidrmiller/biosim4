import sys

import matplotlib.pyplot as plt
#import numpy as np
import os

# name of the dir where the simulation data is located
outputDir = "outputScript"

# list which will contain arrays/lists of data points
boxplotDataList = []

# tickLabels contain the experimentFolderNames
tickLabels = []


# get data necessary for the plots and store it in arrays
def getData(logName):
    os.chdir(outputDir)
    boxplotDataList.clear()
    experimentDataList = []

    # get all experiment dirs in the output dir
    for experimentDir in os.scandir():
        if experimentDir.is_dir():
            tickLabels.append(experimentDir.name)
            os.chdir(experimentDir.name)

            # get the replicates in the experiment dir and gather the data
            for replicateDir in os.scandir():
                if replicateDir.is_dir():
                    os.chdir(replicateDir.name)

                    # find and open the logfile from the replicate and print the last line for now
                    with open("logs/epoch-log.txt") as logFile:
                        print(os.getcwd())
                        # logSpecifierIndex shows at which position in the line the logValue is stored
                        logSpecifierIndex = 0

                        # add specific data point to array
                        generationData = logFile.readlines()

                        # split the first line into separate values
                        separateValuesName = generationData[0].split(" ")

                        # get the index of the logName
                        logSpecifierIndex = separateValuesName.index(logName)

                        # access the last line and get the value
                        separateValuesLast = generationData[len(generationData) - 1].split(" ")
                        value = separateValuesLast[logSpecifierIndex]

                        # !This line adds data to the list
                        experimentDataList.append(int(value))
                        logFile.close()


                    os.chdir("..")
            # At the end of the loop add experimentData to boxplotData
            boxplotDataList.append(experimentDataList.copy())
            # and empty the experimentDataList
            experimentDataList.clear()

            os.chdir("..")
    os.chdir("..")


# visualize the box plots
def visualizePlot(logName):
    print("Visualizing boxplots...")
    print(boxplotDataList)

    completeData = []
    ticks=[]
    tickCount=1
    for singleExperimentData in boxplotDataList:
        ticks.append(tickCount)
        tickCount+=1
        completeData.append(singleExperimentData)


    plt.figure(figsize=(tickCount + 2, 6))
    plt.boxplot(completeData, sym='') # sym specifies a symbol for outliers
    plt.title(logName)
    plt.xticks(ticks=ticks, labels=tickLabels)
    #plt.savefig(logName + ".png")
    plt.show()
    tickLabels.clear()


def main(argv):
    for logName in argv:
        if argv.index(logName) > 0:
            getData(logName)
            visualizePlot(logName)


if __name__ == "__main__":
    main(sys.argv)

