/**
   @file evs.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.0
   @detail This file describes a C++ version of EVS. The Origin of this file is EVS Ver.3.4.
**/
#ifndef evs_hpp
#define evs_hpp

#include <string>

/**
   @namespace evs
   @brief 
**/
namespace evs{
    
    static const int EVS_OK = 0;
    static const int EVS_NG = -1;
    static const int EVS_TRUE = 1;
    static const int EVS_FALSE = 0;
    
    bool define(const std::string& key);
    int find(const std::string& key);
    bool is_defined(const std::string& key);
    
    void clear(const std::string& key);
    void reset(const std::string& key);
    
    int set(const std::string& key);
    int set_logic(const std::string& key, const bool logic);    
    int put(const std::string& key, const double value);
    int add(const std::string& key, const double value);
    
    bool is_set(const std::string& key);
    double get(const std::string& key);
    double integral(const std::string& key);
    
    int ndef();
    void accumulate();
    
    void clear_all();
    void reset_all();
    
    void output();
}

// /** **/
// int EvsIz();

// /** **/
// int EvsEnd();

// /** **/
// void EvsClrAll();

// /** **/
// void EvsRstAll();

// /** **/
// void EvsAcm();

// /** **/
// void EvsOut();

// /** @brief Return a number of EVS definition. **/
// int EvsNdf();

// /** @brief Define new EventSelection. Return -1 if already exist. **/
// int EvsDef(const string key);

// /** @brief Find index of key. Return -1 if NOT defined. **/
// int EvsKey(const string key, int *index);

// /** **/
// int EvsRst(const string key);

// /** @brief Return -1 if NOT defined **/
// int EvsIsDef(const string key);

// /** **/
// int EvsVal(const string key, const bool logic);

// /** **/
// int EvsSet(const string key);

// /** **/
// int EvsClr(const string key);

// /** **/
// int Evs(const string key);

// /** **/
// int EvsPut(const string key, const double val);

// /** **/
// int EvsAdd(const string key, const double val);

// /** **/
// int EvsGet(const string key, double *val);

// /** **/
// int EvsNum(const string key, double *num);

#endif
    
