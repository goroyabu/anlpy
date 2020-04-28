/**
   @date 2020/03/27
**/

#include "bnk.hpp"
#include "databank.hpp"

#include <map>
#include <iostream>
#include <iomanip>

namespace bnk
{
    // static int n_buffer=0;
    // std::map<std::string, int> list_of_key;
    //int long double string

    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTable;
        static std::vector< databank_base* > gBnkRecord;
	static std::vector<std::pair<std::string, std::string>> gListOfAllBnk;
	
	// static std::map<std::string, int> gBnkIndexTableI;
	// static std::map<std::string, int> gBnkIndexTableD;
	// static std::map<std::string, int> gBnkIndexTableS;
	// static std::map<std::string, int> gBnkIndexTableB;

	// static std::vector< databank_int* > gBnkRecordI;
	// static std::vector< databank_double* > gBnkRecordD;
	// static std::vector< databank_string* > gBnkRecordS;
	// static std::vector< databank_bool* > gBnkRecordB;
    }

    //int add_bank(databank_base* bank);
    // int add_bank(databank_int* bank);
    // int add_bank(databank_double* bank);
    // int add_bank(databank_string* bank);
    // int add_bank(databank_bool* bank);

    // //databank_base* get_bank(int index);
    // databank_int* get_bank_int(int index);
    // databank_double* get_bank_double(int index);
    // databank_string* get_bank_string(int index);
    // databank_bool* get_bank_bool(int index);

    // // template<> class databank<int>;
    // // template<> class databank<double>;
    // // template<> class databank<std::string>;
    // // template<> class databank<bool>;
    // // using databank_int = databank<int>;
    // // using databank_double = databank<double>;
    // // using databank_string = databank<std::string>;
    // // using databank_bool = databank<bool>;
    // int find_int(const std::string key);
    // int find_double(const std::string key);
    // int find_string(const std::string key);
    // int find_bool(const std::string key);

    // bool is_def_int(const std::string key);
    // bool is_def_double(const std::string key);
    // bool is_def_string(const std::string key);
    // bool is_def_bool(const std::string key);
}

#include "bnk_spec.cpp"

// int bnk::add_bank(databank_base* bank)
// {
//     using namespace bnk_private;
//     gBnkRecord.push_back( bank );
//     int index = bnk::ndef() - 1;
//     gBnkIndexTable[bank->Key()] = index;
//     return BNK_OK;
// }

// int bnk::add_bank(databank_int* bank)
// {
//     using namespace bnk_private;
//     int index = gBnkRecordI.size(); gBnkRecordI.push_back( bank );
//     gBnkIndexTableI[bank->Key()] = index; return BNK_OK;
// }
// int bnk::add_bank(databank_double* bank)
// {
//     using namespace bnk_private;
//     int index = gBnkRecordD.size(); gBnkRecordD.push_back( bank );
//     gBnkIndexTableD[bank->Key()] = index; return BNK_OK;
// }
// int bnk::add_bank(databank_string* bank)
// {
//     using namespace bnk_private;
//     int index = gBnkRecordS.size(); gBnkRecordS.push_back( bank );
//     gBnkIndexTableS[bank->Key()] = index; return BNK_OK;
// }
// int bnk::add_bank(databank_bool* bank)
// {
//     using namespace bnk_private;
//     int index = gBnkRecordB.size(); gBnkRecordB.push_back( bank );
//     gBnkIndexTableB[bank->Key()] = index; return BNK_OK;
// }

// bnk::databank_base* bnk::get_bank(int index)
// {
//     return bnk_private::gBnkRecord[index];
// }

// bnk::databank_int* bnk::get_bank_int(int index)
// {
//     return bnk_private::gBnkRecordI[index];
// }
// bnk::databank_double* bnk::get_bank_double(int index)
// {
//     return bnk_private::gBnkRecordD[index];
// }
// bnk::databank_string* bnk::get_bank_string(int index)
// {
//     return bnk_private::gBnkRecordS[index];
// }
// bnk::databank_bool* bnk::get_bank_bool(int index)
// {
//     return bnk_private::gBnkRecordB[index];
// }

int bnk::init()
{
    return 0;
}
int bnk::end()
{
    return 0;
}
int bnk::list()
{
    using namespace std;
    cout << "BNK: (data storge system) Ver.1.2" << endl;    
    cout << "# of key    : " << bnk::ndef() << endl;       
    cout << "------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Name                            Allocated     Used      #Put      #Get  Attrib" << endl;
    cout << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    
    //for( auto itr : bnk_private::gBnkRecord ) itr->List();
    for( auto itr : bnk_private::gBnkRecordSH ) itr->List();
    for( auto itr : bnk_private::gBnkRecordUS ) itr->List();
    for( auto itr : bnk_private::gBnkRecordUI ) itr->List();
    for( auto itr : bnk_private::gBnkRecordI ) itr->List();
    for( auto itr : bnk_private::gBnkRecordL ) itr->List();
    for( auto itr : bnk_private::gBnkRecordF ) itr->List();
    for( auto itr : bnk_private::gBnkRecordD ) itr->List();
    for( auto itr : bnk_private::gBnkRecordB ) itr->List();
    for( auto itr : bnk_private::gBnkRecordS ) itr->List();
    
    cout << "------------------------------------------------------------------------------" << endl;
    
    return BNK_OK;
}
// int bnk::bnk_export_all()
// {
//     for( auto itr : bnk_private::gBnkRecord ) itr->Export();
//     return BNK_OK;
// }
int bnk::connect(std::string server)
{
    return server.size();
}
// int bnk::bnk_key(const std::string key)
// {
//     if( bnk_is_def(key) == BNK_NG ) return -1;
//     return bnk_private::gBnkIndexTable.find(key)->second;
// }
int bnk::export_all()
{
    return 0;
}
// int bnk::connect(std::string server)
// {
//     return 0;
// }
int bnk::server(int port)
{
    return 0;
}
int bnk::ndef()
{
    return
	bnk_private::gBnkRecordSH.size()
	+ bnk_private::gBnkRecordUS.size()
	+ bnk_private::gBnkRecordUI.size()
	+ bnk_private::gBnkRecordI.size()
	+ bnk_private::gBnkRecordL.size()
	+ bnk_private::gBnkRecordF.size()
	+ bnk_private::gBnkRecordD.size()
	+ bnk_private::gBnkRecordB.size()
	+ bnk_private::gBnkRecordS.size();
    //return bnk_private::gBnkRecord.size();
    //return n_buffer;
}
/** @brief Return -1 if NOT defined. **/
// bool bnk::is_def(const std::string key)
// {
//     using namespace bnk_private;
//     if( gBnkIndexTable.find(key) ==  gBnkIndexTable.end() ) return false;
//     return true;
// }

// bool bnk::is_def_int(const std::string key)
// {
//     using namespace bnk_private;
//     if( gBnkIndexTableI.find(key) ==  gBnkIndexTableI.end() ) return false;
//     return true;
// }
// bool bnk::is_def_double(const std::string key)
// {
//     using namespace bnk_private;
//     if( gBnkIndexTableD.find(key) ==  gBnkIndexTableD.end() ) return false;
//     return true;
// }
// bool bnk::is_def_string(const std::string key)
// {
//     using namespace bnk_private;
//     if( gBnkIndexTableS.find(key) ==  gBnkIndexTableS.end() ) return false;
//     return true;
// }
// bool bnk::is_def_bool(const std::string key)
// {
//     using namespace bnk_private;
//     if( gBnkIndexTableB.find(key) ==  gBnkIndexTableB.end() ) return false;
//     return true;
// }


/** @brief Define a new buffer. **/
template<typename T>
int bnk::define(const std::string key, const int size)
{
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    // if( bnk::is_def(key) == BNK_OK ) return BNK_NG;
    // add_bank( new databank<T>(key, size) );
    return BNK_NG;
}

// template<> int bnk::define<int>        (const std::string key, const int size)
// {
//     if( bnk::is_def_int(key) == true ) return BNK_NG;
//     add_bank( new databank_int(key, size) ); return BNK_OK;
// }
// template<> int bnk::define<double>     (const std::string key, const int size)
// {
//     if( bnk::is_def_double(key) == true ) return BNK_NG;
//     add_bank( new databank_double(key, size) ); return BNK_OK;
// }
// template<> int bnk::define<std::string>(const std::string key, const int size)
// {
//     if( bnk::is_def_string(key) == true ) return BNK_NG;
//     add_bank( new databank_string(key, size) ); return BNK_OK;
// }
// template<> int bnk::define<bool>       (const std::string key, const int size)
// {
//     if( bnk::is_def_bool(key) == true ) return BNK_NG;
//     add_bank( new databank_bool(key, size) ); return BNK_OK;
// }

/** @brief Find index of key. **/
int bnk::find(const std::string key)
{
    return -1;
}

// int bnk::find_int(const std::string key)
// {
//     if ( bnk::is_def_int(key) == false ) return -1;
//     return bnk_private::gBnkIndexTableI[ key ];
// }
// int bnk::find_double(const std::string key)
// {
//     if ( bnk::is_def_double(key) == false ) return -1;
//     return bnk_private::gBnkIndexTableD[ key ];
// }
// int bnk::find_string(const std::string key)
// {
//     if ( bnk::is_def_string(key) == false ) return -1;
//     return bnk_private::gBnkIndexTableS[ key ];
// }
// int bnk::find_bool(const std::string key)
// {
//     if ( bnk::is_def_bool(key) == false ) return -1;
//     return bnk_private::gBnkIndexTableB[ key ];
// }


int bnk::equiv(const std::string new_key, const std::string old_key)
{
    return BNK_OK;
}

/** @brief Put data. **/
template<typename T> int bnk::put(const std::string key, const T& value)
{
    // int index = bnk::find(key); if ( index<0 ) return BNK_NG;    
    // if( get_bank(index)->GetAllocSize()!=1 ) return BNK_NG;
    // ((databank<T>*)get_bank(index))->Put(value);
    // return BNK_OK;
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    return BNK_NG;
}

// template<> int bnk::put<int>(const std::string key, const int& value)
// {
//     int index = bnk::find_int(key); if ( index<0 ) return BNK_NG;
//     auto bank = get_bank_int(index);
//     if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
//     bank->Put(value); return BNK_OK;
// }
// template<> int bnk::put<double>(const std::string key, const double& value)
// {
//     int index = bnk::find_double(key); if ( index<0 ) return BNK_NG;
//     auto bank = get_bank_double(index);
//     if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
//     bank->Put(value); return BNK_OK;
// }
// template<> int bnk::put<std::string>(const std::string key, const std::string& value)
// {
//     int index = bnk::find_string(key); if ( index<0 ) return BNK_NG;
//     auto bank = get_bank_string(index);
//     if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
//     bank->Put(value); return BNK_OK;
// }
// template<> int bnk::put<bool>(const std::string key, const bool& value)
// {
//     int index = bnk::find_bool(key); if ( index<0 ) return BNK_NG;
//     auto bank = get_bank_bool(index);
//     if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
//     bank->Put(value); return BNK_OK;
// }

template<typename T> int bnk::put(const std::string key, const std::vector<T>& in, int begin, int end)
{
    // int index = bnk::find(key); if ( index<0 ) return BNK_NG;
    // ((databank<T>*)get_bank(index))->Put(in, begin, end);
    // return BNK_OK;
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    return BNK_NG;
}

// template<> int bnk::put<int>(const std::string key, const std::vector<int>& in, int begin, int end)
// {
//     int index = bnk::find_int(key); if ( index<0 ) return BNK_NG;
//     ((databank_int*)get_bank_int(index))->Put(in, begin, end);
//     return BNK_OK;
// }
// template<> int bnk::put<double>(const std::string key, const std::vector<double>& in, int begin, int end)
// {
//     int index = bnk::find_double(key); if ( index<0 ) return BNK_NG;
//     ((databank_double*)get_bank_double(index))->Put(in, begin, end);
//     return BNK_OK;
// }
// template<> int bnk::put<std::string>(const std::string key, const std::vector<std::string>& in, int begin, int end)
// {
//     int index = bnk::find_string(key); if ( index<0 ) return BNK_NG;
//     ((databank_string*)get_bank_string(index))->Put(in, begin, end);
//     return BNK_OK;
// }
// template<> int bnk::put<bool>(const std::string key, const std::vector<bool>& in, int begin, int end)
// {
//     int index = bnk::find_bool(key); if ( index<0 ) return BNK_NG;
//     ((databank_bool*)get_bank_bool(index))->Put(in, begin, end);
//     return BNK_OK;
// }

//modify on 2019/03/19
template<typename T> T bnk::get(const std::string key)
{
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    T out;
    return out;
    // T out;
    // int index = bnk::find(key);
    // if( index<0 ){
    // 	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl; 
    // 	return out;
    // }
    // if( get_bank(index)->GetAllocSize()!=1 ){
    // 	std::cout << "Error in bnk::get() " << key << " is array" << std::endl; 
    // 	return out;
    // }
    // if( ((databank<T>*)get_bank(index))->Get(&out) == BNK_NG ) return out;
    // return out;
}

// template<> int bnk::get<int>(const std::string key)
// {
//     int out = 0; int index = bnk::find_int(key);
//     if( index<0 ) { 
// 	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
// 	return out;
//     }
//     auto bank = get_bank_int( index );
//     if( !bank || bank->GetAllocSize()!=1 ) {
// 	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
// 	return out;
//     }
//     if( bank->Get(&out) == BNK_NG ) {
// 	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
//     }
//     return out;
// }
// template<> double bnk::get<double>(const std::string key)
// {
//     double out = 0.0; int index = bnk::find_double(key);
//     if( index<0 ) {
// 	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
// 	return out;
//     }
//     auto bank = get_bank_double(index);
//     if( !bank || bank->GetAllocSize()!=1 ) {
// 	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
// 	return out;
//     }
//     if( bank->Get(&out) == BNK_NG ) {
// 	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
//     }
//     return out;
// }
// template<> std::string bnk::get<std::string>(const std::string key)
// {
//     std::string out = ""; int index = bnk::find_string(key);
//     if( index<0 ) {
// 	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
// 	return out;
//     }
//     auto bank = get_bank_string(index);
//     if( !bank || bank->GetAllocSize()!=1 ) {
// 	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
// 	return out;
//     }
//     if( bank->Get(&out) == BNK_NG ) {
// 	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
//     }
//     return out;
// }
// template<> bool bnk::get<bool>(const std::string key)
// {
//     bool out = false; int index = bnk::find_bool(key);
//     if( index<0 ) {
// 	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
// 	return out;
//     }
//     auto bank = get_bank_bool(index);
//     if( !bank || bank->GetAllocSize()!=1 ) {
// 	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
// 	return out;
//     }
//     if( bank->Get(&out) == BNK_NG ) {
// 	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
//     }
//     return out;
// }

template<typename T> std::vector<T> bnk::getv(const std::string key, int begin, int end)
{
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    std::vector<T> out; return out;
}
// template<> std::vector<int> bnk::getv<int>(const std::string key, int begin, int end)
// {
//     int index = bnk::find_int(key); std::vector<int> out; if ( index<0 ) return out; 
//     ((databank_int*)get_bank_int(index))->Get(&out, begin, end);
//     return out;
// }
// template<> std::vector<double> bnk::getv<double>(const std::string key, int begin, int end)
// {
//     int index = bnk::find_double(key); std::vector<double> out; if ( index<0 ) return out;    
//     ((databank_double*)get_bank_double(index))->Get(&out, begin, end);
//     return out;
// }
// template<> std::vector<std::string> bnk::getv<std::string>(const std::string key, int begin, int end)
// {
//     int index = bnk::find_string(key); std::vector<std::string> out; if ( index<0 ) return out;    
//     ((databank_string*)get_bank_string(index))->Get(&out, begin, end);
//     return out;
// }
// template<> std::vector<bool> bnk::getv<bool>(const std::string key, int begin, int end)
// {
//     int index = bnk::find_bool(key); std::vector<bool> out; if ( index<0 ) return out;    
//     ((databank_bool*)get_bank_bool(index))->Get(&out, begin, end);
//     return out;
// }

template<typename T> T* bnk::getptr(const std::string key)
{
    // int index = bnk::find(key); if( index<0 ) return nullptr;
    // return ((databank<T>*)get_bank(index))->GetPtr();
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    return nullptr;
}
// template<> int* bnk::getptr<int>(const std::string key)
// {
//     int index = bnk::find_int(key); if( index<0 ) return nullptr;
//     return ((databank_int*)get_bank_int(index))->GetPtr();
// }
// template<> double* bnk::getptr<double>(const std::string key)
// {
//     int index = bnk::find_double(key); if( index<0 ) return nullptr;
//     return ((databank_double*)get_bank_double(index))->GetPtr();
// }
// template<> std::string* bnk::getptr<std::string>(const std::string key)
// {
//     int index = bnk::find_string(key); if( index<0 ) return nullptr;
//     return ((databank_string*)get_bank_string(index))->GetPtr();
// }

// template<> bool* bnk::getptr<bool>(const std::string key)
// {
//     int index = bnk::find(key); if( index<0 ) return nullptr;
//     return ((databank_bool*)get_bank(index))->GetPtr();
// }

template<typename T> int bnk::resize(const std::string key, const int size)
{
    // int index = bnk::find(key);if( index<0 ) return BNK_NG;
    // ((databank<T>*)get_bank(index))->Resize(size); return BNK_OK;
    std::cout << "***Error*** : This type of BNK is not allocated" << std::endl;
    return BNK_NG;
}
// template<> int bnk::resize<int>(const std::string key, const int size)
// {
//     int index = bnk::find_int(key);if( index<0 ) return BNK_NG;
//     ((databank_int*)get_bank_int(index))->Resize(size); return BNK_OK;
// }
// template<> int bnk::resize<double>(const std::string key, const int size)
// {
//     int index = bnk::find_double(key);if( index<0 ) return BNK_NG;
//     ((databank_double*)get_bank_double(index))->Resize(size); return BNK_OK;
// }
// template<> int bnk::resize<std::string>(const std::string key, const int size)
// {
//     int index = bnk::find_string(key);if( index<0 ) return BNK_NG;
//     ((databank_string*)get_bank_string(index))->Resize(size); return BNK_OK;
// }
// template<> int bnk::resize<bool>(const std::string key, const int size)
// {
//     int index = bnk::find_bool(key);if( index<0 ) return BNK_NG;
//     ((databank_bool*)get_bank_bool(index))->Resize(size); return BNK_OK;
// }

std::vector<std::pair<std::string, std::string>> bnk::get_list_of_buffers()
{
    return bnk_private::gListOfAllBnk;
}

/** **/
int bnk::net_update(const int index)
{
    return BNK_OK;
}
/** **/
int bnk::export_buffer(const std::string key)
{
    return BNK_OK;
}

