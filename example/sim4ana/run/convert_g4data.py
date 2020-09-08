#!/usr/local/bin/python3

import anlpy
import sim4ana

if __name__ == '__main__':
    anl = anlpy.ANLmanager()

    readTTree = sim4ana.ReadTTree()\
        .SetParameter("file_name", "test_data_sim.root")\
        .SetParameter("tree_name", "g4tree")
    anl.AddModule(readTTree)

    beginG4TreeAnalysis = sim4ana.BeginG4TreeAnalysis()
    anl.AddModule(beginG4TreeAnalysis)

    tmpDiffuseEnergyCharge = sim4ana.DiffuseEnergyCharge()
    #tempDiffuseEnergyCharge.SetParameter("copyid", "ss")
    anl.AddModule(tmpDiffuseEnergyCharge)

    tmpApplyEnergyResponse = sim4ana.ApplyEnergyResponse()
    #tempApplyEnergyResponse.SetParameter("copyid", "ss")
    anl.AddModule(tmpApplyEnergyResponse)

    tmpJudgeTrigger = sim4ana.JudgeTrigger()
    #tempJudgeTrigger.SetParameter("copyid", "ss")
    anl.AddModule(tmpJudgeTrigger)

    tmpCutReadoutThreshold = sim4ana.CutReadoutThreshold()
    #tempCutReadoutThreshold.SetParameter("copyid", "ss")
    anl.AddModule(tmpCutReadoutThreshold)

    tmpSelectHitPattern = sim4ana.SelectHitPattern()
    #tempSelectHitPattern.SetParameter("copyid", "ss")
    anl.AddModule(tmpSelectHitPattern)

    tmpCutAnalysisThreshold = sim4ana.CutAnalysisThreshold()
    #tempCutAnalysisThreshold.SetParameter("copyid", "ss")
    anl.AddModule(tmpCutAnalysisThreshold)

    tmpMergeAdjacentStrip = sim4ana.MergeAdjacentStrip()
    #tempMergeAdjacentStrip.SetParameter("copyid", "ss")
    anl.AddModule(tmpMergeAdjacentStrip)

    tmpCoupleHit = sim4ana.CoupleHit()
    #tempCoupleHit.SetParameter("copyid", "ss")
    anl.AddModule(tmpCoupleHit)

    writetree = sim4ana.WriteTTree()\
        .SetParameter("file_name", "out.root")\
        .SetParameter("tree_name", "hittree")\
        .SetParameter("branch_ignore", "detid:nmerged_raw_hits:strip_x:strip_y:dir_x:dir_y:dir_z:pos_x:pos_y:pos_z:edep:pixel_center_x:pixel_center_y")
    anl.AddModule(writetree)

    anl.ShowAnalysis()
    anl.ReadData(10000, 1000)

    print("Test run of sim4ana is succeeded !")
