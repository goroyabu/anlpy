/**
   @date 2020/04/02
**/

#include "VANL_Module.hpp"
#include <fstream>

class LoggerText : public anl::VANL_Module
{
public:
    LoggerText();
    int mod_init() override;
    int mod_ana() override;

private:
    std::string output_file;
    std::ofstream output_stream;
    double scale_factor;
};
