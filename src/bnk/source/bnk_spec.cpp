
/* ---------- short Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableSH;
        static std::vector< databank_short* > gBnkRecordSH;
    }
    int add_bank(databank_short* bank);
    databank_short* get_bank_short(int index);
    int find_short(const std::string key);
    bool is_def_short(const std::string key);
}
    
bool bnk::is_def_short(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableSH.find(key) ==  gBnkIndexTableSH.end() ) return false;
    return true;
}
int bnk::add_bank(databank_short* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordSH.size(); gBnkRecordSH.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "short") );
    gBnkIndexTableSH[bank->Key()] = index; return BNK_OK;
}
bnk::databank_short* bnk::get_bank_short(int index)
{
    return bnk_private::gBnkRecordSH[index];
}
template<> int bnk::define<short>(const std::string key, const int size)
{
    if( bnk::is_def_short(key) == true ) return BNK_NG;
    add_bank( new databank_short(key, size) ); return BNK_OK;
}
int bnk::find_short(const std::string key)
{
    if ( bnk::is_def_short(key) == false ) return -1;
    return bnk_private::gBnkIndexTableSH[ key ];
}
template<> int bnk::put<short>(const std::string key, const short& value)
{
    int index = bnk::find_short(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_short(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<short>(const std::string key, const std::vector<short>& in, int begin, int end)
{
    int index = bnk::find_short(key); if ( index<0 ) return BNK_NG;
    //((databank_short*)get_bank_short(index))->Put(in, begin, end);
    (get_bank_short(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> short bnk::get<short>(const std::string key)
{
    short out = 0; int index = bnk::find_short(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_short( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<short> bnk::getv<short>(const std::string key, int begin, int end)
{
    int index = bnk::find_short(key); std::vector<short> out; if ( index<0 ) return out; 
    //((databank_short*)get_bank_short(index))->Get(&out, begin, end);
    //(get_bank_short(index))->Get(&out, begin, end);
    return *((get_bank_short(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<short>(const std::string key, const int size)
{
    int index = bnk::find_short(key);if( index<0 ) return BNK_NG;
    //((databank_short*)get_bank_short(index))->Resize(size); return BNK_OK;
    (get_bank_short(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<short>(const std::string& key)
{
    int index = bnk::find_short(key);if( index<0 ) return "";
    //return ((databank_short*)get_bank_short(index))->GetKeyToSize();
    return (get_bank_short(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<short>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_short(key);if( index<0 ) return BNK_NG;
    //((databank_short*)get_bank_short(index))->SetKeyToSize(key_to_size);
    (get_bank_short(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<short>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordSH ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<short>* bnk::getvecptr<short>(const std::string& key)
{
    int index = bnk::find_short(key); if( index<0 ) return nullptr;
    //return ((databank_short*)get_bank_short(index))->GetVecPtr();
    return (get_bank_short(index))->GetVecPtr();
}

template<> short* bnk::getptr<short>(const std::string key)
{
    int index = bnk::find_short(key); if( index<0 ) return nullptr;
    //return ((databank_short*)get_bank_short(index))->GetPtr();
    return (get_bank_short(index))->GetPtr();
}

/* ---------- ushort Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableUS;
        static std::vector< databank_ushort* > gBnkRecordUS;
    }
    int add_bank(databank_ushort* bank);
    databank_ushort* get_bank_ushort(int index);
    int find_ushort(const std::string key);
    bool is_def_ushort(const std::string key);
}
    
bool bnk::is_def_ushort(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableUS.find(key) ==  gBnkIndexTableUS.end() ) return false;
    return true;
}
int bnk::add_bank(databank_ushort* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordUS.size(); gBnkRecordUS.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "unsigned short") );
    gBnkIndexTableUS[bank->Key()] = index; return BNK_OK;
}
bnk::databank_ushort* bnk::get_bank_ushort(int index)
{
    return bnk_private::gBnkRecordUS[index];
}
template<> int bnk::define<unsigned short>(const std::string key, const int size)
{
    if( bnk::is_def_ushort(key) == true ) return BNK_NG;
    add_bank( new databank_ushort(key, size) ); return BNK_OK;
}
int bnk::find_ushort(const std::string key)
{
    if ( bnk::is_def_ushort(key) == false ) return -1;
    return bnk_private::gBnkIndexTableUS[ key ];
}
template<> int bnk::put<unsigned short>(const std::string key, const unsigned short& value)
{
    int index = bnk::find_ushort(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_ushort(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<unsigned short>(const std::string key, const std::vector<unsigned short>& in, int begin, int end)
{
    int index = bnk::find_ushort(key); if ( index<0 ) return BNK_NG;
    //((databank_ushort*)get_bank_ushort(index))->Put(in, begin, end);
    (get_bank_ushort(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> unsigned short bnk::get<unsigned short>(const std::string key)
{
    unsigned short out = 0; int index = bnk::find_ushort(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_ushort( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<unsigned short> bnk::getv<unsigned short>(const std::string key, int begin, int end)
{
    int index = bnk::find_ushort(key); std::vector<unsigned short> out; if ( index<0 ) return out; 
    //((databank_ushort*)get_bank_ushort(index))->Get(&out, begin, end);
    //(get_bank_ushort(index))->Get(&out, begin, end);
    return *((get_bank_ushort(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<unsigned short>(const std::string key, const int size)
{
    int index = bnk::find_ushort(key);if( index<0 ) return BNK_NG;
    //((databank_ushort*)get_bank_ushort(index))->Resize(size); return BNK_OK;
    (get_bank_ushort(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<unsigned short>(const std::string& key)
{
    int index = bnk::find_ushort(key);if( index<0 ) return "";
    //return ((databank_ushort*)get_bank_ushort(index))->GetKeyToSize();
    return (get_bank_ushort(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<unsigned short>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_ushort(key);if( index<0 ) return BNK_NG;
    //((databank_ushort*)get_bank_ushort(index))->SetKeyToSize(key_to_size);
    (get_bank_ushort(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<unsigned short>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordUS ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<unsigned short>* bnk::getvecptr<unsigned short>(const std::string& key)
{
    int index = bnk::find_ushort(key); if( index<0 ) return nullptr;
    //return ((databank_ushort*)get_bank_ushort(index))->GetVecPtr();
    return (get_bank_ushort(index))->GetVecPtr();
}

template<> unsigned short* bnk::getptr<unsigned short>(const std::string key)
{
    int index = bnk::find_ushort(key); if( index<0 ) return nullptr;
    //return ((databank_ushort*)get_bank_ushort(index))->GetPtr();
    return (get_bank_ushort(index))->GetPtr();
}

/* ---------- uint Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableUI;
        static std::vector< databank_uint* > gBnkRecordUI;
    }
    int add_bank(databank_uint* bank);
    databank_uint* get_bank_uint(int index);
    int find_uint(const std::string key);
    bool is_def_uint(const std::string key);
}
    
bool bnk::is_def_uint(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableUI.find(key) ==  gBnkIndexTableUI.end() ) return false;
    return true;
}
int bnk::add_bank(databank_uint* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordUI.size(); gBnkRecordUI.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "unsigned int") );
    gBnkIndexTableUI[bank->Key()] = index; return BNK_OK;
}
bnk::databank_uint* bnk::get_bank_uint(int index)
{
    return bnk_private::gBnkRecordUI[index];
}
template<> int bnk::define<unsigned int>(const std::string key, const int size)
{
    if( bnk::is_def_uint(key) == true ) return BNK_NG;
    add_bank( new databank_uint(key, size) ); return BNK_OK;
}
int bnk::find_uint(const std::string key)
{
    if ( bnk::is_def_uint(key) == false ) return -1;
    return bnk_private::gBnkIndexTableUI[ key ];
}
template<> int bnk::put<unsigned int>(const std::string key, const unsigned int& value)
{
    int index = bnk::find_uint(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_uint(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<unsigned int>(const std::string key, const std::vector<unsigned int>& in, int begin, int end)
{
    int index = bnk::find_uint(key); if ( index<0 ) return BNK_NG;
    //((databank_uint*)get_bank_uint(index))->Put(in, begin, end);
    (get_bank_uint(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> unsigned int bnk::get<unsigned int>(const std::string key)
{
    unsigned int out = 0; int index = bnk::find_uint(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_uint( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<unsigned int> bnk::getv<unsigned int>(const std::string key, int begin, int end)
{
    int index = bnk::find_uint(key); std::vector<unsigned int> out; if ( index<0 ) return out; 
    //((databank_uint*)get_bank_uint(index))->Get(&out, begin, end);
    //(get_bank_uint(index))->Get(&out, begin, end);
    return *((get_bank_uint(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<unsigned int>(const std::string key, const int size)
{
    int index = bnk::find_uint(key);if( index<0 ) return BNK_NG;
    //((databank_uint*)get_bank_uint(index))->Resize(size); return BNK_OK;
    (get_bank_uint(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<unsigned int>(const std::string& key)
{
    int index = bnk::find_uint(key);if( index<0 ) return "";
    //return ((databank_uint*)get_bank_uint(index))->GetKeyToSize();
    return (get_bank_uint(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<unsigned int>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_uint(key);if( index<0 ) return BNK_NG;
    //((databank_uint*)get_bank_uint(index))->SetKeyToSize(key_to_size);
    (get_bank_uint(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<unsigned int>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordUI ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<unsigned int>* bnk::getvecptr<unsigned int>(const std::string& key)
{
    int index = bnk::find_uint(key); if( index<0 ) return nullptr;
    //return ((databank_uint*)get_bank_uint(index))->GetVecPtr();
    return (get_bank_uint(index))->GetVecPtr();
}

template<> unsigned int* bnk::getptr<unsigned int>(const std::string key)
{
    int index = bnk::find_uint(key); if( index<0 ) return nullptr;
    //return ((databank_uint*)get_bank_uint(index))->GetPtr();
    return (get_bank_uint(index))->GetPtr();
}

/* ---------- int Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableI;
        static std::vector< databank_int* > gBnkRecordI;
    }
    int add_bank(databank_int* bank);
    databank_int* get_bank_int(int index);
    int find_int(const std::string key);
    bool is_def_int(const std::string key);
}
    
bool bnk::is_def_int(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableI.find(key) ==  gBnkIndexTableI.end() ) return false;
    return true;
}
int bnk::add_bank(databank_int* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordI.size(); gBnkRecordI.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "int") );
    gBnkIndexTableI[bank->Key()] = index; return BNK_OK;
}
bnk::databank_int* bnk::get_bank_int(int index)
{
    return bnk_private::gBnkRecordI[index];
}
template<> int bnk::define<int>(const std::string key, const int size)
{
    if( bnk::is_def_int(key) == true ) return BNK_NG;
    add_bank( new databank_int(key, size) ); return BNK_OK;
}
int bnk::find_int(const std::string key)
{
    if ( bnk::is_def_int(key) == false ) return -1;
    return bnk_private::gBnkIndexTableI[ key ];
}
template<> int bnk::put<int>(const std::string key, const int& value)
{
    int index = bnk::find_int(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_int(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<int>(const std::string key, const std::vector<int>& in, int begin, int end)
{
    int index = bnk::find_int(key); if ( index<0 ) return BNK_NG;
    //((databank_int*)get_bank_int(index))->Put(in, begin, end);
    (get_bank_int(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> int bnk::get<int>(const std::string key)
{
    int out = 0; int index = bnk::find_int(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_int( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<int> bnk::getv<int>(const std::string key, int begin, int end)
{
    int index = bnk::find_int(key); std::vector<int> out; if ( index<0 ) return out; 
    //((databank_int*)get_bank_int(index))->Get(&out, begin, end);
    //(get_bank_int(index))->Get(&out, begin, end);
    return *((get_bank_int(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<int>(const std::string key, const int size)
{
    int index = bnk::find_int(key);if( index<0 ) return BNK_NG;
    //((databank_int*)get_bank_int(index))->Resize(size); return BNK_OK;
    (get_bank_int(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<int>(const std::string& key)
{
    int index = bnk::find_int(key);if( index<0 ) return "";
    //return ((databank_int*)get_bank_int(index))->GetKeyToSize();
    return (get_bank_int(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<int>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_int(key);if( index<0 ) return BNK_NG;
    //((databank_int*)get_bank_int(index))->SetKeyToSize(key_to_size);
    (get_bank_int(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<int>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordI ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<int>* bnk::getvecptr<int>(const std::string& key)
{
    int index = bnk::find_int(key); if( index<0 ) return nullptr;
    //return ((databank_int*)get_bank_int(index))->GetVecPtr();
    return (get_bank_int(index))->GetVecPtr();
}

template<> int* bnk::getptr<int>(const std::string key)
{
    int index = bnk::find_int(key); if( index<0 ) return nullptr;
    //return ((databank_int*)get_bank_int(index))->GetPtr();
    return (get_bank_int(index))->GetPtr();
}

/* ---------- long Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableL;
        static std::vector< databank_long* > gBnkRecordL;
    }
    int add_bank(databank_long* bank);
    databank_long* get_bank_long(int index);
    int find_long(const std::string key);
    bool is_def_long(const std::string key);
}
    
bool bnk::is_def_long(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableL.find(key) ==  gBnkIndexTableL.end() ) return false;
    return true;
}
int bnk::add_bank(databank_long* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordL.size(); gBnkRecordL.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "long") );
    gBnkIndexTableL[bank->Key()] = index; return BNK_OK;
}
bnk::databank_long* bnk::get_bank_long(int index)
{
    return bnk_private::gBnkRecordL[index];
}
template<> int bnk::define<long>(const std::string key, const int size)
{
    if( bnk::is_def_long(key) == true ) return BNK_NG;
    add_bank( new databank_long(key, size) ); return BNK_OK;
}
int bnk::find_long(const std::string key)
{
    if ( bnk::is_def_long(key) == false ) return -1;
    return bnk_private::gBnkIndexTableL[ key ];
}
template<> int bnk::put<long>(const std::string key, const long& value)
{
    int index = bnk::find_long(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_long(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<long>(const std::string key, const std::vector<long>& in, int begin, int end)
{
    int index = bnk::find_long(key); if ( index<0 ) return BNK_NG;
    //((databank_long*)get_bank_long(index))->Put(in, begin, end);
    (get_bank_long(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> long bnk::get<long>(const std::string key)
{
    long out = 0; int index = bnk::find_long(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_long( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<long> bnk::getv<long>(const std::string key, int begin, int end)
{
    int index = bnk::find_long(key); std::vector<long> out; if ( index<0 ) return out; 
    //((databank_long*)get_bank_long(index))->Get(&out, begin, end);
    //(get_bank_long(index))->Get(&out, begin, end);
    return *((get_bank_long(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<long>(const std::string key, const int size)
{
    int index = bnk::find_long(key);if( index<0 ) return BNK_NG;
    //((databank_long*)get_bank_long(index))->Resize(size); return BNK_OK;
    (get_bank_long(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<long>(const std::string& key)
{
    int index = bnk::find_long(key);if( index<0 ) return "";
    //return ((databank_long*)get_bank_long(index))->GetKeyToSize();
    return (get_bank_long(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<long>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_long(key);if( index<0 ) return BNK_NG;
    //((databank_long*)get_bank_long(index))->SetKeyToSize(key_to_size);
    (get_bank_long(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<long>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordL ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<long>* bnk::getvecptr<long>(const std::string& key)
{
    int index = bnk::find_long(key); if( index<0 ) return nullptr;
    //return ((databank_long*)get_bank_long(index))->GetVecPtr();
    return (get_bank_long(index))->GetVecPtr();
}

template<> long* bnk::getptr<long>(const std::string key)
{
    int index = bnk::find_long(key); if( index<0 ) return nullptr;
    //return ((databank_long*)get_bank_long(index))->GetPtr();
    return (get_bank_long(index))->GetPtr();
}

/* ---------- llong Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableLL;
        static std::vector< databank_llong* > gBnkRecordLL;
    }
    int add_bank(databank_llong* bank);
    databank_llong* get_bank_llong(int index);
    int find_llong(const std::string key);
    bool is_def_llong(const std::string key);
}
    
bool bnk::is_def_llong(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableLL.find(key) ==  gBnkIndexTableLL.end() ) return false;
    return true;
}
int bnk::add_bank(databank_llong* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordLL.size(); gBnkRecordLL.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "long long") );
    gBnkIndexTableLL[bank->Key()] = index; return BNK_OK;
}
bnk::databank_llong* bnk::get_bank_llong(int index)
{
    return bnk_private::gBnkRecordLL[index];
}
template<> int bnk::define<long long>(const std::string key, const int size)
{
    if( bnk::is_def_llong(key) == true ) return BNK_NG;
    add_bank( new databank_llong(key, size) ); return BNK_OK;
}
int bnk::find_llong(const std::string key)
{
    if ( bnk::is_def_llong(key) == false ) return -1;
    return bnk_private::gBnkIndexTableLL[ key ];
}
template<> int bnk::put<long long>(const std::string key, const long long& value)
{
    int index = bnk::find_llong(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_llong(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<long long>(const std::string key, const std::vector<long long>& in, int begin, int end)
{
    int index = bnk::find_llong(key); if ( index<0 ) return BNK_NG;
    //((databank_llong*)get_bank_llong(index))->Put(in, begin, end);
    (get_bank_llong(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> long long bnk::get<long long>(const std::string key)
{
    long long out = 0; int index = bnk::find_llong(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_llong( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<long long> bnk::getv<long long>(const std::string key, int begin, int end)
{
    int index = bnk::find_llong(key); std::vector<long long> out; if ( index<0 ) return out; 
    //((databank_llong*)get_bank_llong(index))->Get(&out, begin, end);
    //(get_bank_llong(index))->Get(&out, begin, end);
    return *((get_bank_llong(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<long long>(const std::string key, const int size)
{
    int index = bnk::find_llong(key);if( index<0 ) return BNK_NG;
    //((databank_llong*)get_bank_llong(index))->Resize(size); return BNK_OK;
    (get_bank_llong(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<long long>(const std::string& key)
{
    int index = bnk::find_llong(key);if( index<0 ) return "";
    //return ((databank_llong*)get_bank_llong(index))->GetKeyToSize();
    return (get_bank_llong(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<long long>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_llong(key);if( index<0 ) return BNK_NG;
    //((databank_llong*)get_bank_llong(index))->SetKeyToSize(key_to_size);
    (get_bank_llong(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<long long>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordLL ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<long long>* bnk::getvecptr<long long>(const std::string& key)
{
    int index = bnk::find_llong(key); if( index<0 ) return nullptr;
    //return ((databank_llong*)get_bank_llong(index))->GetVecPtr();
    return (get_bank_llong(index))->GetVecPtr();
}

template<> long long* bnk::getptr<long long>(const std::string key)
{
    int index = bnk::find_llong(key); if( index<0 ) return nullptr;
    //return ((databank_llong*)get_bank_llong(index))->GetPtr();
    return (get_bank_llong(index))->GetPtr();
}

/* ---------- float Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableF;
        static std::vector< databank_float* > gBnkRecordF;
    }
    int add_bank(databank_float* bank);
    databank_float* get_bank_float(int index);
    int find_float(const std::string key);
    bool is_def_float(const std::string key);
}
    
bool bnk::is_def_float(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableF.find(key) ==  gBnkIndexTableF.end() ) return false;
    return true;
}
int bnk::add_bank(databank_float* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordF.size(); gBnkRecordF.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "float") );
    gBnkIndexTableF[bank->Key()] = index; return BNK_OK;
}
bnk::databank_float* bnk::get_bank_float(int index)
{
    return bnk_private::gBnkRecordF[index];
}
template<> int bnk::define<float>(const std::string key, const int size)
{
    if( bnk::is_def_float(key) == true ) return BNK_NG;
    add_bank( new databank_float(key, size) ); return BNK_OK;
}
int bnk::find_float(const std::string key)
{
    if ( bnk::is_def_float(key) == false ) return -1;
    return bnk_private::gBnkIndexTableF[ key ];
}
template<> int bnk::put<float>(const std::string key, const float& value)
{
    int index = bnk::find_float(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_float(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<float>(const std::string key, const std::vector<float>& in, int begin, int end)
{
    int index = bnk::find_float(key); if ( index<0 ) return BNK_NG;
    //((databank_float*)get_bank_float(index))->Put(in, begin, end);
    (get_bank_float(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> float bnk::get<float>(const std::string key)
{
    float out = 0.0; int index = bnk::find_float(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_float( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<float> bnk::getv<float>(const std::string key, int begin, int end)
{
    int index = bnk::find_float(key); std::vector<float> out; if ( index<0 ) return out; 
    //((databank_float*)get_bank_float(index))->Get(&out, begin, end);
    //(get_bank_float(index))->Get(&out, begin, end);
    return *((get_bank_float(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<float>(const std::string key, const int size)
{
    int index = bnk::find_float(key);if( index<0 ) return BNK_NG;
    //((databank_float*)get_bank_float(index))->Resize(size); return BNK_OK;
    (get_bank_float(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<float>(const std::string& key)
{
    int index = bnk::find_float(key);if( index<0 ) return "";
    //return ((databank_float*)get_bank_float(index))->GetKeyToSize();
    return (get_bank_float(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<float>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_float(key);if( index<0 ) return BNK_NG;
    //((databank_float*)get_bank_float(index))->SetKeyToSize(key_to_size);
    (get_bank_float(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<float>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordF ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<float>* bnk::getvecptr<float>(const std::string& key)
{
    int index = bnk::find_float(key); if( index<0 ) return nullptr;
    //return ((databank_float*)get_bank_float(index))->GetVecPtr();
    return (get_bank_float(index))->GetVecPtr();
}

template<> float* bnk::getptr<float>(const std::string key)
{
    int index = bnk::find_float(key); if( index<0 ) return nullptr;
    //return ((databank_float*)get_bank_float(index))->GetPtr();
    return (get_bank_float(index))->GetPtr();
}

/* ---------- double Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableD;
        static std::vector< databank_double* > gBnkRecordD;
    }
    int add_bank(databank_double* bank);
    databank_double* get_bank_double(int index);
    int find_double(const std::string key);
    bool is_def_double(const std::string key);
}
    
bool bnk::is_def_double(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableD.find(key) ==  gBnkIndexTableD.end() ) return false;
    return true;
}
int bnk::add_bank(databank_double* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordD.size(); gBnkRecordD.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "double") );
    gBnkIndexTableD[bank->Key()] = index; return BNK_OK;
}
bnk::databank_double* bnk::get_bank_double(int index)
{
    return bnk_private::gBnkRecordD[index];
}
template<> int bnk::define<double>(const std::string key, const int size)
{
    if( bnk::is_def_double(key) == true ) return BNK_NG;
    add_bank( new databank_double(key, size) ); return BNK_OK;
}
int bnk::find_double(const std::string key)
{
    if ( bnk::is_def_double(key) == false ) return -1;
    return bnk_private::gBnkIndexTableD[ key ];
}
template<> int bnk::put<double>(const std::string key, const double& value)
{
    int index = bnk::find_double(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_double(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<double>(const std::string key, const std::vector<double>& in, int begin, int end)
{
    int index = bnk::find_double(key); if ( index<0 ) return BNK_NG;
    //((databank_double*)get_bank_double(index))->Put(in, begin, end);
    (get_bank_double(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> double bnk::get<double>(const std::string key)
{
    double out = 0.0; int index = bnk::find_double(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_double( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<double> bnk::getv<double>(const std::string key, int begin, int end)
{
    int index = bnk::find_double(key); std::vector<double> out; if ( index<0 ) return out; 
    //((databank_double*)get_bank_double(index))->Get(&out, begin, end);
    //(get_bank_double(index))->Get(&out, begin, end);
    return *((get_bank_double(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<double>(const std::string key, const int size)
{
    int index = bnk::find_double(key);if( index<0 ) return BNK_NG;
    //((databank_double*)get_bank_double(index))->Resize(size); return BNK_OK;
    (get_bank_double(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<double>(const std::string& key)
{
    int index = bnk::find_double(key);if( index<0 ) return "";
    //return ((databank_double*)get_bank_double(index))->GetKeyToSize();
    return (get_bank_double(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<double>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_double(key);if( index<0 ) return BNK_NG;
    //((databank_double*)get_bank_double(index))->SetKeyToSize(key_to_size);
    (get_bank_double(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<double>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordD ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<double>* bnk::getvecptr<double>(const std::string& key)
{
    int index = bnk::find_double(key); if( index<0 ) return nullptr;
    //return ((databank_double*)get_bank_double(index))->GetVecPtr();
    return (get_bank_double(index))->GetVecPtr();
}

template<> double* bnk::getptr<double>(const std::string key)
{
    int index = bnk::find_double(key); if( index<0 ) return nullptr;
    //return ((databank_double*)get_bank_double(index))->GetPtr();
    return (get_bank_double(index))->GetPtr();
}

/* ---------- bool Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableB;
        static std::vector< databank_bool* > gBnkRecordB;
    }
    int add_bank(databank_bool* bank);
    databank_bool* get_bank_bool(int index);
    int find_bool(const std::string key);
    bool is_def_bool(const std::string key);
}
    
bool bnk::is_def_bool(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableB.find(key) ==  gBnkIndexTableB.end() ) return false;
    return true;
}
int bnk::add_bank(databank_bool* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordB.size(); gBnkRecordB.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "bool") );
    gBnkIndexTableB[bank->Key()] = index; return BNK_OK;
}
bnk::databank_bool* bnk::get_bank_bool(int index)
{
    return bnk_private::gBnkRecordB[index];
}
template<> int bnk::define<bool>(const std::string key, const int size)
{
    if( bnk::is_def_bool(key) == true ) return BNK_NG;
    add_bank( new databank_bool(key, size) ); return BNK_OK;
}
int bnk::find_bool(const std::string key)
{
    if ( bnk::is_def_bool(key) == false ) return -1;
    return bnk_private::gBnkIndexTableB[ key ];
}
template<> int bnk::put<bool>(const std::string key, const bool& value)
{
    int index = bnk::find_bool(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_bool(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<bool>(const std::string key, const std::vector<bool>& in, int begin, int end)
{
    int index = bnk::find_bool(key); if ( index<0 ) return BNK_NG;
    //((databank_bool*)get_bank_bool(index))->Put(in, begin, end);
    (get_bank_bool(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> bool bnk::get<bool>(const std::string key)
{
    bool out = false; int index = bnk::find_bool(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_bool( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<bool> bnk::getv<bool>(const std::string key, int begin, int end)
{
    int index = bnk::find_bool(key); std::vector<bool> out; if ( index<0 ) return out; 
    //((databank_bool*)get_bank_bool(index))->Get(&out, begin, end);
    //(get_bank_bool(index))->Get(&out, begin, end);
    return *((get_bank_bool(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<bool>(const std::string key, const int size)
{
    int index = bnk::find_bool(key);if( index<0 ) return BNK_NG;
    //((databank_bool*)get_bank_bool(index))->Resize(size); return BNK_OK;
    (get_bank_bool(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<bool>(const std::string& key)
{
    int index = bnk::find_bool(key);if( index<0 ) return "";
    //return ((databank_bool*)get_bank_bool(index))->GetKeyToSize();
    return (get_bank_bool(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<bool>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_bool(key);if( index<0 ) return BNK_NG;
    //((databank_bool*)get_bank_bool(index))->SetKeyToSize(key_to_size);
    (get_bank_bool(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<bool>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordB ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<bool>* bnk::getvecptr<bool>(const std::string& key)
{
    int index = bnk::find_bool(key); if( index<0 ) return nullptr;
    //return ((databank_bool*)get_bank_bool(index))->GetVecPtr();
    return (get_bank_bool(index))->GetVecPtr();
}


/* ---------- string Implementation ---------- */
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTableS;
        static std::vector< databank_string* > gBnkRecordS;
    }
    int add_bank(databank_string* bank);
    databank_string* get_bank_string(int index);
    int find_string(const std::string key);
    bool is_def_string(const std::string key);
}
    
bool bnk::is_def_string(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTableS.find(key) ==  gBnkIndexTableS.end() ) return false;
    return true;
}
int bnk::add_bank(databank_string* bank)
{
    using namespace bnk_private;
    int index = gBnkRecordS.size(); gBnkRecordS.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "std::string") );
    gBnkIndexTableS[bank->Key()] = index; return BNK_OK;
}
bnk::databank_string* bnk::get_bank_string(int index)
{
    return bnk_private::gBnkRecordS[index];
}
template<> int bnk::define<std::string>(const std::string key, const int size)
{
    if( bnk::is_def_string(key) == true ) return BNK_NG;
    add_bank( new databank_string(key, size) ); return BNK_OK;
}
int bnk::find_string(const std::string key)
{
    if ( bnk::is_def_string(key) == false ) return -1;
    return bnk_private::gBnkIndexTableS[ key ];
}
template<> int bnk::put<std::string>(const std::string key, const std::string& value)
{
    int index = bnk::find_string(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_string(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<std::string>(const std::string key, const std::vector<std::string>& in, int begin, int end)
{
    int index = bnk::find_string(key); if ( index<0 ) return BNK_NG;
    //((databank_string*)get_bank_string(index))->Put(in, begin, end);
    (get_bank_string(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> std::string bnk::get<std::string>(const std::string key)
{
    std::string out = ""; int index = bnk::find_string(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_string( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<std::string> bnk::getv<std::string>(const std::string key, int begin, int end)
{
    int index = bnk::find_string(key); std::vector<std::string> out; if ( index<0 ) return out; 
    //((databank_string*)get_bank_string(index))->Get(&out, begin, end);
    //(get_bank_string(index))->Get(&out, begin, end);
    return *((get_bank_string(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<std::string>(const std::string key, const int size)
{
    int index = bnk::find_string(key);if( index<0 ) return BNK_NG;
    //((databank_string*)get_bank_string(index))->Resize(size); return BNK_OK;
    (get_bank_string(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<std::string>(const std::string& key)
{
    int index = bnk::find_string(key);if( index<0 ) return "";
    //return ((databank_string*)get_bank_string(index))->GetKeyToSize();
    return (get_bank_string(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<std::string>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_string(key);if( index<0 ) return BNK_NG;
    //((databank_string*)get_bank_string(index))->SetKeyToSize(key_to_size);
    (get_bank_string(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<std::string>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecordS ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<std::string>* bnk::getvecptr<std::string>(const std::string& key)
{
    int index = bnk::find_string(key); if( index<0 ) return nullptr;
    //return ((databank_string*)get_bank_string(index))->GetVecPtr();
    return (get_bank_string(index))->GetVecPtr();
}

template<> std::string* bnk::getptr<std::string>(const std::string key)
{
    int index = bnk::find_string(key); if( index<0 ) return nullptr;
    //return ((databank_string*)get_bank_string(index))->GetPtr();
    return (get_bank_string(index))->GetPtr();
}
