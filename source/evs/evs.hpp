/**
   @file evs.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.0
   @detail This file describes a C++ version of EVS. The Origin of this file is EVS Ver.3.4.
**/
#ifndef evs_hpp
#define evs_hpp

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

/**
   @namespace evs
   @brief 
**/
namespace evs{

    using string = std::string;
    static const int EVS_OK = 0;
    static const int EVS_NG = -1;
    static const int EVS_TRUE = 1;
    static const int EVS_FALSE = 0;
    
    /**
       @class EventSelection
    **/
    class EventSelection
    {
    private:
	string mName;
	double mFlag;
	double mNum;
	
    public:
	EventSelection(const string name) : mName(name), mFlag(0.0), mNum(0.0) {}
	~EventSelection(){}
	
	string GetName() const { return mName; }
	double GetFlag() const { return mFlag; }
	double GetNum() const { return mNum; }
	
	void Set( const double val = 1.0 ) { mFlag = val; }
	void Clear() { Set(0.0); }
	void Reset() { mNum = 0.0; Set(0.0); }
	void Accumulate() { mNum += mFlag; Set(0.0); }
	void Value( const bool logic ) { if(logic) Set(1.0); else Set(0.0); }
	void Out() const { std::cout << std::setw(10) << mNum << " : " << mName << std::endl; }
	void Add( const double val ) { Set(mFlag + val); }
    };

    /** **/
    int EvsIz();

    /** **/
    int EvsEnd();

    /** **/
    void EvsClrAll();

    /** **/
    void EvsRstAll();

    /** **/
    void EvsAcm();

    /** **/
    void EvsOut();

    /** @brief Return a number of EVS definition. **/
    int EvsNdf();
    
    /** @brief Define new EventSelection. Return -1 if already exist. **/
    int EvsDef(const string key);
    
    /** @brief Find index of key. Return -1 if NOT defined. **/
    int EvsKey(const string key, int *index);
    
    /** **/
    int EvsRst(const string key);
    
    /** @brief Return -1 if NOT defined **/
    int EvsIsDef(const string key);
    
    /** **/
    int EvsVal(const string key, const bool logic);
    
    /** **/
    int EvsSet(const string key);
    
    /** **/
    int EvsClr(const string key);
    
    /** **/
    int Evs(const string key);
    
    /** **/
    int EvsPut(const string key, const double val);
    
    /** **/
    int EvsAdd(const string key, const double val);
    
    /** **/
    int EvsGet(const string key, double *val);
    
    /** **/
    int EvsNum(const string key, double *num);
}
#endif
    
