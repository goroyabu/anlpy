#!/usr/bin/env python3

import os, sys
import time

import anlpy
import dsdana

def analysis_1data(input_filename):

    output_filename = input_filename.replace(".root", "_hittree.root")
    print(output_filename)

    anl = anlpy.ANLmanager()

    tmpReadTTree = dsdana.ReadTTree()\
        .SetParameter("file_name", input_filename)\
        .SetParameter("tree_name", "eventtree")\
        .SetParameter("branch_ignore", "chflag*:cmn*_ex:ref*")
    anl.AddModule(tmpReadTTree)

    tmpDSDdatabase = dsdana.DSDdatabaseSpline()\
        .SetParameter("file_name", "database_spline.txt")\
        .SetParameter("root_file", "calfunc_spline.root")
    anl.AddModule(tmpDSDdatabase)

    tmpApplyDatabase = dsdana.ApplyDatabase()
    anl.AddModule(tmpApplyDatabase)

    tmpMergeAdjacent = dsdana.MergeAdjacent()
    anl.AddModule(tmpMergeAdjacent)

    tmpCoupleHit = dsdana.CoupleHit()
    anl.AddModule(tmpCoupleHit)

    tmpWriteTTree = dsdana.WriteTTree()\
        .SetParameter("file_name", output_filename)\
        .SetParameter("tree_name", "hittree")\
        .SetParameter("branch_ignore", "adc*:cmn*:index*:hitnum*")
    anl.AddModule(tmpWriteTTree)

    anl.ShowAnalysis()
    anl.ReadData(10000000, 1000)
    anl.Quit()
    print("")

if __name__ == '__main__':

    args = sys.argv

    for i in range( 1, len(args) ):
        start = time.time()
        analysis_1data(args[i])
        elapsed_time = time.time() - start
        print ("elapsed_time:{0}".format(elapsed_time) + "[sec]")
