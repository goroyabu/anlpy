/**
   @file ANLStatus.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.0
   @brief Original = anl_status.hh
**/
#ifndef _ANL_STATUS_H_
#define _ANL_STATUS_H_

namespace anlcross {
    const int ANL_OK      =  0;
    const int ANL_NG      = -1;
    
    const int ANL_YES     =  1;
    const int ANL_NO      =  0;
    
    const int ANL_TRUE    =  1;
    const int ANL_FALSE   =  0;
    
    const int ANL_ENA     =  1;
    const int ANL_DIS     =  0;
    
    const int ANL_ON      =  1;
    const int ANL_OFF     =  0;
    
    const int ANL_QUIT    = -1;
    const int ANL_SKIP    = -3;
    const int ANL_LOOP    = -4;
    // flow control (new) 
    const int ANL_ENDLOOP =  1;
    const int ANL_DISCARD =  2;
    const int ANL_NOCOUNT =  4;
    const int ANL_NEWROOT =  8;
}
#endif // _ANL_STATUS_H_
    
