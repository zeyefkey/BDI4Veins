from pandas import DataFrame, read_csv
import pandas as pd 
import sys 
import numpy
import scipy.stats as sp
import math
from pylab import *
import sys, getopt

def main(argv):
    try:
      opts, args = getopt.getopt(argv,"hi:o:",["input=","output="])
    except getopt.GetoptError:
        print('usage: checkOutliers.py -i <Results from simulation run> -o <Merged CSV file>')
        sys.exit(2)
    if len(sys.argv) < 2:
        print('usage: checkOutliers.py -i <Results from simulation run> -o <Merged CSV file>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-i", "--input"):
            inFile = str(arg)
        elif opt in ("-o", "--output"):
            outFile = str(arg)
    rdf = pd.read_csv(inFile)
    platoonSize = int(rdf[rdf.Agent == 0].PlatoonSize)
    #First, check if the first utility measures are zero
    firstMeasure = 100
    for a in range(0, platoonSize):
        firstMeasure *= float(rdf[rdf.Agent == a].Utility1)
    if firstMeasure == 0.0:
        print("MALFORMED,first measure of utility non existant!")
        sys.exit(1)
    try:
        mdf = pd.read_csv(outFile)
        mdf = mdf.append( rdf, ignore_index = True )
        mdf.to_csv(outFile,index=False)
        print("File exists")
    except FileNotFoundError:
        print("File not found")
        rdf.to_csv(outFile,index=False)
    print("CSV is well formed")
    sys.exit(0)

if __name__ == "__main__":
    main(sys.argv[1:])
