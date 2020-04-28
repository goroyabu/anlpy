#!/usr/local/bin/python3

import anlpy
import dsdana

if __name__ == '__main__':
    anl = anlpy.ANLmanager()

    tmpReadTTree = dsdana.ReadTTree()
    #tmpReadTTree.SetParameter("file_name", " /Users/goroyabu/github/sim4-w-strip/test/sim_20200329_214825.root")
    #tmpReadTTree.SetParameter("tree_name", "tree")
    tmpReadTTree.SetParameter("file_name", "example_data_cc.root")
    tmpReadTTree.SetParameter("tree_name", "eventtree")
    tmpReadTTree.SetParameter("branch_ignore", "chflag*:cmn*_ex:ref*")
    anl.AddModule(tmpReadTTree)

    tmpDSDdatabase = dsdana.DSDdatabase()
    tmpDSDdatabase.SetParameter("file_name", "database.txt")
    anl.AddModule(tmpDSDdatabase)

    tmpApplyDatabase = dsdana.ApplyDatabase()
    # tmpApplyDatabase
    anl.AddModule(tmpApplyDatabase)

    tmpMergeAdjacent = dsdana.MergeAdjacent()
    anl.AddModule(tmpMergeAdjacent)

    tmpCoupleHit = dsdana.CoupleHit()
    anl.AddModule(tmpCoupleHit)

    tmpWriteTTree = dsdana.WriteTTree()
    tmpWriteTTree.SetParameter("tree_name", "tree")
    tmpWriteTTree.SetParameter("branch_ignore", "adc*:cmn*:index*:hitnum*:{live,unix}time")
    anl.AddModule(tmpWriteTTree)

    anl.ShowAnalysis()
    anl.ReadData(10000, 1000)

    print("Test run of dsdana is succeeded !")
