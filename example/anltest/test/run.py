#!/usr/bin/env python3

import anlpy
import anltest

def run_analysis() :
    
    anl = anlpy.ANLmanager()
    
    mod1 = anltest.TimeGetter()
    mod1.SetParameter("interval_sec", 1)
    anl.AddModule(mod1)
    
    mod2 = anltest.LoggerText().SetParameter("output_file", "otameshi.txt")
    anl.AddModule(mod2)

    anl.ShowAnalysis()
    anl.ReadData(10, 1)

if __name__ == '__main__':

    run_analysis()
