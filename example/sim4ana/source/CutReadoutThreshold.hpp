/**
   @file   CutReadoutThreshold.hpp
   @date   2020/08/24
   @author 
   @detail Automatically generated by make_anlpy_project.sh 1.0.0
**/

#ifndef CutReadoutThreshold_hpp
#define CutReadoutThreshold_hpp

#include <VANL_Module.hpp>

class CutReadoutThreshold : public anl::VANL_Module
{
public:
    CutReadoutThreshold();
    ~CutReadoutThreshold();

    int mod_bgnrun() override;
    int mod_ana() override;
    int mod_endrun() override;
};

#endif
