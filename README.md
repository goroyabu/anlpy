
## 1. Installation

Requirements of ANLpy are
 - C++ compiler ( e.g. gcc, clang++ )
 - pybind11
 - cmake
 - make or ninja

```sh
 $ git clone https://github.com/goroyabu/anlpy
 $ pip3 install anlpy/
```

## 2. Getting Started

### 2.1 Compile Sample Program

```sh
 $ pip3 install anlpy/example/anltest/
```

### 2.2 How to Run Sample Program

```sh
 $ python3 test_run.py
```

`test_run.py`
```python
#!/usr/bin/env python3

import anlpy
import anltest

anl = anlpy.ANLmanager()
read_module = anltest.TimeGetter()
read_module.SetParameter("interval_sec", 1)
write_module = anltest.LoggerText().SetParameter("output_file", "otameshi.txt")
anl.AddModule(read_module)
anl.AddModule(write_module)

anl.ShowAnalysis()
anl.ReadData(10, 1)

```

```
 $ python3 test_run.py # or ./test_run.py

      ***************************
      ***** Analysis chain ******
      ***************************

    0   TimeGetter              0.1       
    1   LoggerText              1.0       

 ##### TimeGetter #####
  Type   Name = Value
  int    interval_sec = 1
  double scale_factor = 2
  string module_name = TimeGetter
  string module_version = 0.1
  string copyid =
  string input_file = input.txt

 ##### LoggerText #####
  Type   Name = Value
  double scale_factor = 2
  string module_name = LoggerText
  string module_version = 1.0
  string copyid =
  string output_file = otameshi.txt


 ##### ANLmanager #####
  Type   Name = Value
  int    display_interval_msec = 1000000
  string module_name = ANLmanager
  string module_version = 1.0
  string copyid =
  string program_name = ANL

Opening otameshi.txt is succeeded!
0/10(0%)
1/10(10%)
2/10(20%)
3/10(30%)
4/10(40%)
5/10(50%)
6/10(60%)
7/10(70%)
8/10(80%)
9/10(90%)

      ***************************
      ***** Analysis chain ******
      ***************************

               PUT: 10
                |
     [ 0]  TimeGetter                  version 0.1
                | OK: 10/10
     [ 1]  LoggerText                  version 1.0
                | OK: 8/10                      -------> SKIP: 2
               GET: 8

*** results of Event selection *** < Number of selects : 2    >
10         : multiple_of_1
3          : multiple_of_3

BNK: (data storge system) Ver.1.2
# of key    : 3
------------------------------------------------------------------------------

Name                            Allocated     Used      #Put      #Get  Attrib

------------------------------------------------------------------------------
seconds                                 1        1        10        10
ANL_eventid                             1        1        11         0
time_stamp                              1        1        10        10
------------------------------------------------------------------------------
```

## 3. How to Run DSDANA

Install via pip3 by the following command.
```sh
 $ pip3 install anlpy/example/dsdana/
```

### 3.1 Use 5-degree polynominal as calibration curve

```sh
cd example/dsdana/run/poly5
./run.py
```

### 3.2 Use 3-degree spline curve as calibration curve

```sh
cd example/dsdana/run/spline3
./analysis_hittree.py
```
