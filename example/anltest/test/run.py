#!/usr/local/bin/python3

import anlpy
import anltest

anl = anlpy.ANLmanager()
mod1 = anltest.TimeGetter()
mod1.SetParameter("interval_sec", 1)
mod2 = anltest.LoggerText().SetParameter("output_file", "otameshi.txt")
#mod2.SetParameter("output_file", "testout.txt")
anl.AddModule(mod1)
anl.AddModule(mod2)

anl.ShowAnalysis()
anl.ReadData(10, 1)
