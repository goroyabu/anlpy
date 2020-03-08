/**
   @file bnk.hpp
   @author Goro Yabu
   @date 2018/11/23
   @version 1.2
**/
#include "bnk.hpp"
namespace bnk
{
    namespace bnk_private
    {
	static std::map<std::string, int> gBnkIndexTable;
	static std::vector< databank_base* > gBnkRecord;
    }
}
int bnk::add_bank(databank_base* bank)
{
    using namespace bnk_private;
    gBnkRecord.push_back( bank );
    int index = bnk_ndef() - 1;
    gBnkIndexTable[bank->Key()] = index;
    return BNK_OK;
}
bnk::databank_base* bnk::get_bank(int index)
{
    return bnk_private::gBnkRecord[index];
}

int bnk::bnk_init()
{
    return BNK_OK;
}
int bnk::bnk_end()
{
    return BNK_OK;
}
int bnk::bnk_list()
{
    using namespace std;
    cout << "BNK: (data storge system) Ver.1.2" << endl;
     
    cout << "# of key    : " << bnk_ndef() << endl;
    
    //cout << "buffer size : " << endl;
    
    //cout << "buffer used : " << endl;
    
    cout << "------------------------------------------------------------------------------" << endl;
    cout << endl;
    cout << "Name                            Allocated     Used      #Put      #Get  Attrib" << endl;
    cout << endl;
    cout << "------------------------------------------------------------------------------" << endl;
    
    for( auto itr : bnk_private::gBnkRecord ) itr->List();
    cout << "------------------------------------------------------------------------------" << endl;
    
    return BNK_OK;
}
int bnk::bnk_export_all()
{
    for( auto itr : bnk_private::gBnkRecord ) itr->Export();
    return BNK_OK;
}
int bnk::bnk_connect(std::string server)
{
    return server.size();
}
int bnk::bnk_ndef()
{
    return bnk_private::gBnkRecord.size();
}
int bnk::bnk_key(const std::string key, int* index)
{
    *index = bnk_private::gBnkIndexTable.find(key)->second;
    if( bnk_is_def(key) == BNK_NG ) return BNK_NG;
    return BNK_OK;
}
int bnk::bnk_is_def(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTable.find(key) ==  gBnkIndexTable.end() ) return BNK_NG;
    return BNK_OK;
}
int bnk::bnk_export(const std::string key)
{
    int	index;
    if( bnk_key(key, &index) == BNK_NG ) return BNK_NG;
    get_bank(index)->Export();
    return BNK_OK;
}
//template<> std::string bnk::value_type<int>(){ return "int"; }
//template<> std::string bnk::value_type<float>(){ return "float"; }
//template<> std::string bnk::value_type<double>(){ return "double"; }

int bnk::BnkIni([[maybe_unused]] int buffer_size)
{
    return bnk_init();
}
int bnk::BnkEnd(void)
{
    return bnk_end();
}
int bnk::BnkKey(char* key, int* index)
{
    return bnk_key((std::string)key, index);
}
int bnk::BnkIsDef(char* key)
{
    return bnk_is_def((std::string)key);
}
int bnk::BnkNdf(void)
{
    return bnk_ndef();
}
/*
int bnk::BnkEqv(char* new_key, int size, char* old_key, int start)
{
    return 0;
}
*/
void bnk::BnkLst(void)
{
    bnk_list();
}
int bnk::BnkConnect(char* server)
{
    return bnk_connect((std::string)server);
}
int bnk::BnkExport(char* key)
{
    return bnk_export((std::string)key);
}
int bnk::BnkExportAll(void)
{
    return bnk_export_all();
}
int bnk::BnkServer(int port)
{
    return port;
}
/*
int bnk::BnkNetUpdate(const int index)
{
    return index;
}
*/
//template int bnk::BnkDef<int>(const std::string key, const int size = 1);
//template int bnk::BnkDef<float>(const std::string key, const int size = 1);
