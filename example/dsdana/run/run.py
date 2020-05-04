#!/usr/local/bin/python3

import anlpy
import dsdana

if __name__ == '__main__':

    anl = anlpy.ANLmanager()

    tmpReadTTree = dsdana.ReadTTree()\
        .SetParameter("file_name", "example_data_cc.root")\
        .SetParameter("tree_name", "eventtree")\
        .SetParameter("branch_ignore", "chflag*:cmn*_ex:ref*")
    anl.AddModule(tmpReadTTree)

    tmpDSDdatabase = dsdana.DSDdatabase()\
        .SetParameter("file_name", "database.txt")
    anl.AddModule(tmpDSDdatabase)

    tmpApplyDatabase = dsdana.ApplyDatabase()
    anl.AddModule(tmpApplyDatabase)

    tmpMergeAdjacent = dsdana.MergeAdjacent()
    anl.AddModule(tmpMergeAdjacent)

    tmpCoupleHit = dsdana.CoupleHit()
    anl.AddModule(tmpCoupleHit)

    tmpWriteTTree = dsdana.WriteTTree()\
        .SetParameter("file_name", "example_output.root")\
        .SetParameter("tree_name", "hittree")\
        .SetParameter("branch_ignore", "adc*:cmn*:index*:hitnum*")
    anl.AddModule(tmpWriteTTree)

    anl.ShowAnalysis()
    anl.ReadData(10000000, 1000)

    print("Test run of dsdana is succeeded !")
