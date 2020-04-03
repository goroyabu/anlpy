/**
   @file evs.cpp
   @author Goro Yabu
   @data 2018/11/21
   @version 1.0
**/

#include "evs.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <iomanip>

namespace evs
{
    
    /**
       @class EventSelection
    **/
    class EventSelection
    {
    private:
	std::string mName;
	double mFlag;
	double mNum;
	
    public:
	EventSelection(const std::string& name)
	    : mName(name), mFlag(0.0), mNum(0.0) {}
	~EventSelection(){}
	
	std::string GetName() const { return mName; }
	double GetFlag() const { return mFlag; }
	double GetNum() const { return mNum; }
	
	void Set( const double val = 1.0 ) { mFlag = val; }
	void Clear() { Set(0.0); }
	void Reset() { mNum = 0.0; Set(0.0); }
	void Accumulate() { mNum += mFlag; Set(0.0); }
	void Value( const bool logic )
	{
	    if(logic) Set(1.0); else Set(0.0);
	}
	void Out() const
	{
	    std::cout << std::setw(10) << mNum << " : " << mName << std::endl;
	}
	void Add( const double val ) { Set(mFlag + val); }
    };

    static std::map<std::string, int> mIndexTable;
    static std::vector<EventSelection*> mRecord;
}

// /**
//    @func EvsIz
//    @brief Initialize Evs
//    @detail 現状、何もしない
// **/
// int evs::EvsIz()
// {
//     return EVS_OK;
// }
// /**
//    @func EvsEnd
//    @brief End Evs
//    @detail 現状、何もしない
// **/
// int evs::EvsEnd()
// {
//     return EVS_OK;
// }

/**
   @func EvsClrAll
   @brief すべてのEvent Selection のFlagを0にする
**/
void evs::clear_all()
{
    for(auto itr : mRecord) itr->Clear();
}
/**
   @func EvsRstAll()
**/
void evs::reset_all()
{
    for(auto itr : mRecord) itr->Reset();
}
/**
   @func EvsAcm
   @brief すべてのEvent Selection のFlagをNumに移す
**/
void evs::accumulate()
{
    for(auto itr : mRecord) itr->Accumulate();
}
/**
   @func EvsOut
   @brief すべてのEvent Selection のNumをprintする
**/
void evs::output()
{
    std::cout << "*** results of Event selection *** < Number of selects : ";
    std::cout << std::setw(4) << ndef() << " >" << std::endl;
    for(auto itr : mRecord) itr->Out();
}
/**
   @func EvsNdef
   @return define されているEvent Selectionの数
**/
int evs::ndef()
{
    return mRecord.size();
}
/**
   @func EvsDef
   @brief Event Selection "key" を define する
**/
bool evs::define(const std::string& key)
{
    if( is_defined(key) == true ) return false;
    mRecord.push_back( new EventSelection(key) );
    mIndexTable[key] = mRecord.size() - 1;
    return true;
}
/**
   @func EvsKey 
   @detail Event Selection "key" の index を探す. Userは使わないと思う.
**/
int evs::find(const std::string& key)
{
    if( is_defined(key) == false ) return -1;
    return mIndexTable.find(key)->second;
}
/**
   @func EvsRst
   @brief Event Selection "key" のFlagとNumを0にする
**/
void evs::reset(const std::string& key)
{
    int index = find(key);
    if( index > 0 ) mRecord[index]->Reset();
}
bool evs::is_defined(const std::string& key)
{
    if( mIndexTable.find(key) ==  mIndexTable.end() ) return false;
    return true;
}
/**
   @func EvsVal
   @brief logic がtrue のとき、Event Selection "key" のFlagを1にする.
**/
int evs::set_logic(const std::string& key, const bool logic)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    mRecord[index]->Value(logic);
    return EVS_OK;
}
/**
   @func EvsSet
   @brief Event Selection "key" のFlag を1にする.
**/
int evs::set(const std::string& key)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    mRecord[index]->Set();
    return EVS_OK;
}
/**
   @func EvsClr 
   @brief Event Selection "key" のFlagを0にする.
**/
void evs::clear(const std::string& key)
{
    int index = find(key);
    if( index >= 0 ) mRecord[index]->Clear();
}
/**
   @func Evs
   @brief Event Selection "key" のFlagが0でないとき、trueを返す.
**/
bool evs::is_set(const std::string& key)
{
    int index = find(key); if( index < 0 ) return false;
    if( mRecord[index]->GetFlag() == 0.0 ) return false;
    else return true;
}
int evs::put(const std::string& key, const double val)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    mRecord[index]->Set(val);
    return EVS_OK;
}
int evs::add(const std::string& key, double val)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    mRecord[index]->Add(val);
    return EVS_OK;
}
double evs::get(const std::string& key)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    return mRecord[index]->GetFlag();
}
double evs::integral(const std::string& key)
{
    int index = find(key); if( index < 0 ) return EVS_NG;
    return mRecord[index]->GetNum();
}
