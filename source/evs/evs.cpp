/**
   @file evs.cpp
   @author Goro Yabu
   @data 2018/11/21
   @version 1.0
**/
#include "evs.hpp"
namespace evs{
    static std::map<string, int> mIndexTable;
    static std::vector<EventSelection*> mRecord;
}
/**
   @func EvsIz
   @brief Initialize Evs
   @detail 現状、何もしない
**/
int evs::EvsIz()
{
    return EVS_OK;
}
/**
   @func EvsEnd
   @brief End Evs
   @detail 現状、何もしない
**/
int evs::EvsEnd()
{
    return EVS_OK;
}
/**
   @func EvsClrAll
   @brief すべてのEvent Selection のFlagを0にする
**/
void evs::EvsClrAll()
{
    for(auto itr : mRecord) itr->Clear();
}
/**
   @func EvsRstAll()
**/
void evs::EvsRstAll()
{
    for(auto itr : mRecord) itr->Reset();
}
/**
   @func EvsAcm
   @brief すべてのEvent Selection のFlagをNumに移す
**/
void evs::EvsAcm()
{
    for(auto itr : mRecord) itr->Accumulate();
}
/**
   @func EvsOut
   @brief すべてのEvent Selection のNumをprintする
**/
void evs::EvsOut()
{
    std::cout << "*** results of Event selection *** < Number of selects : " << std::setw(4) << EvsNdf() << " >" << std::endl;
    for(auto itr : mRecord) itr->Out();
}
/**
   @func EvsNdef
   @return define されているEvent Selectionの数
**/
int evs::EvsNdf()
{
    return mRecord.size();
}
/**
   @func EvsDef
   @brief Event Selection "key" を define する
**/
int evs::EvsDef(const string key)
{
    if( EvsIsDef(key) == EVS_OK ) return EVS_NG;
    mRecord.push_back( new EventSelection(key) );
    mIndexTable[key] = mRecord.size() - 1;
    return EVS_OK;
}
/**
   @func EvsKey 
   @detail Event Selection "key" の index を探す. Userは使わないと思う.
**/
int evs::EvsKey(const string key, int *index)
{
    *index = mIndexTable.find(key)->second;
    if( EvsIsDef(key) == EVS_NG ) return EVS_NG;
    return EVS_OK;
}
/**
   @func EvsRst
   @brief Event Selection "key" のFlagとNumを0にする
**/
int evs::EvsRst(const string key)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Reset();
    return EVS_OK;
}
int evs::EvsIsDef(const string key)
{
    if( mIndexTable.find(key) ==  mIndexTable.end() ) return EVS_NG;
    return EVS_OK;
}
/**
   @func EvsVal
   @brief logic がtrue のとき、Event Selection "key" のFlagを1にする.
**/
int evs::EvsVal(const string key, const bool logic)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Value(logic);
    return EVS_OK;
}
/**
   @func EvsSet
   @brief Event Selection "key" のFlag を1にする.
**/
int evs::EvsSet(const string key)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Set();
    return EVS_OK;
}
/**
   @func EvsClr 
   @brief Event Selection "key" のFlagを0にする.
**/
int evs::EvsClr(const string key)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Clear();
    return EVS_OK;
}
/**
   @func Evs
   @brief Event Selection "key" のFlagが0でないとき、trueを返す.
**/
int evs::Evs(const string key)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    if( mRecord[index]->GetFlag() == 0.0 ) return EVS_FALSE;
    else return EVS_TRUE;
}
int evs::EvsPut(const string key, double val)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Set(val);
    return EVS_OK;
}
int evs::EvsAdd(const string key, double val)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    mRecord[index]->Add(val);
    return EVS_OK;
}
int evs::EvsGet(const string key, double *val)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    *val = mRecord[index]->GetFlag();
    return EVS_OK;
}
int evs::EvsNum(const string key, double *num)
{
    int index;
    if( EvsKey(key, &index) == EVS_NG ) return EVS_NG;
    *num = mRecord[index]->GetNum();
    return EVS_OK;
}
