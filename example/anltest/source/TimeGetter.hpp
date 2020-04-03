/**
   @date 2020/04/02
**/

#include "VANL_Module.hpp"

#include <string>

class TimeGetter : public anl::VANL_Module
{
public:
    TimeGetter();
    
    int mod_init() override;
    int mod_ana() override;

private:
    std::string input_file;
    int interval_sec;
};
