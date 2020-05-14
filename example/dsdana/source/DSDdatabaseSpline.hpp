/**
   @file DSDdatabaseSpline.hpp
   @date 20200506
   @author Goro Yabu
**/

#ifndef DSDdatabaseSpline_hpp
#define DSDdatabaseSpline_hpp

#include "DSDdatabase.hpp"
#include <TSpline.h>

class DSDdatabaseSpline : public DSDdatabase
{
public:
    DSDdatabaseSpline();
    ~DSDdatabaseSpline();

    int mod_bgnrun() override;

    float GetEPI(const int stripid, const float pha) override;

private:
    std::map<int, TSpline3*> gain_curve_spline3;    
};

#endif
