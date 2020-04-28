#!/bin/sh

function speci_impl ()
{
    typesymbol=$1
    typename=$2
    typename2=$2
    defvalue=$3

    if [ $typesymbol == "S" ]
    then
	typename2="std::string"
	
    elif [ $typesymbol == "UI" ]
    then
	typename2="unsigned int"
	
    elif [ $typesymbol == "US" ]
    then
	typename2="unsigned short"
	
    elif [ $typesymbol == "LL" ]
    then
	typename2="long long"
    fi

    echo 
    echo "/* ---------- ${typename} Implementation ---------- */"
    
cat <<EOF
namespace bnk
{
    namespace bnk_private
    {
        static std::map<std::string, int> gBnkIndexTable${typesymbol};
        static std::vector< databank_${typename}* > gBnkRecord${typesymbol};
    }
    int add_bank(databank_${typename}* bank);
    databank_${typename}* get_bank_${typename}(int index);
    int find_${typename}(const std::string key);
    bool is_def_${typename}(const std::string key);
}
    
bool bnk::is_def_${typename}(const std::string key)
{
    using namespace bnk_private;
    if( gBnkIndexTable${typesymbol}.find(key) ==  gBnkIndexTable${typesymbol}.end() ) return false;
    return true;
}
int bnk::add_bank(databank_${typename}* bank)
{
    using namespace bnk_private;
    int index = gBnkRecord${typesymbol}.size(); gBnkRecord${typesymbol}.push_back( bank );
    gListOfAllBnk.emplace_back( std::make_pair(bank->Key(), "${typename2}") );
    gBnkIndexTable${typesymbol}[bank->Key()] = index; return BNK_OK;
}
bnk::databank_${typename}* bnk::get_bank_${typename}(int index)
{
    return bnk_private::gBnkRecord${typesymbol}[index];
}
template<> int bnk::define<${typename2}>(const std::string key, const int size)
{
    if( bnk::is_def_${typename}(key) == true ) return BNK_NG;
    add_bank( new databank_${typename}(key, size) ); return BNK_OK;
}
int bnk::find_${typename}(const std::string key)
{
    if ( bnk::is_def_${typename}(key) == false ) return -1;
    return bnk_private::gBnkIndexTable${typesymbol}[ key ];
}
template<> int bnk::put<${typename2}>(const std::string key, const ${typename2}& value)
{
    int index = bnk::find_${typename}(key); if ( index<0 ) return BNK_NG;
    auto bank = get_bank_${typename}(index);
    if( !bank || bank->GetAllocSize()!=1 ) return BNK_NG;
    bank->Put(value); return BNK_OK;
}
template<> int bnk::put<${typename2}>(const std::string key, const std::vector<${typename2}>& in, int begin, int end)
{
    int index = bnk::find_${typename}(key); if ( index<0 ) return BNK_NG;
    //((databank_${typename}*)get_bank_${typename}(index))->Put(in, begin, end);
    (get_bank_${typename}(index))->Put(in, begin, end);
    return BNK_OK;
}
template<> ${typename2} bnk::get<${typename2}>(const std::string key)
{
    ${typename2} out = ${defvalue}; int index = bnk::find_${typename}(key);
    if( index<0 ) { 
	std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl;
	return out;
    }
    auto bank = get_bank_${typename}( index );
    if( !bank || bank->GetAllocSize()!=1 ) {
	std::cout << "Error in bnk::get() " << key << " is array" << std::endl;
	return out;
    }
    if( bank->Get(&out) == BNK_NG ) {
	std::cout << "***Error*** : bnk::get() is failed" << std::endl;
    }
    return out;
}
template<> std::vector<${typename2}> bnk::getv<${typename2}>(const std::string key, int begin, int end)
{
    int index = bnk::find_${typename}(key); std::vector<${typename2}> out; if ( index<0 ) return out; 
    //((databank_${typename}*)get_bank_${typename}(index))->Get(&out, begin, end);
    //(get_bank_${typename}(index))->Get(&out, begin, end);
    return *((get_bank_${typename}(index))->Get(begin, end));
    // return out;
}
template<> int bnk::resize<${typename2}>(const std::string key, const int size)
{
    int index = bnk::find_${typename}(key);if( index<0 ) return BNK_NG;
    //((databank_${typename}*)get_bank_${typename}(index))->Resize(size); return BNK_OK;
    (get_bank_${typename}(index))->Resize(size); return BNK_OK;
}
template<> std::string bnk::getkeytosize<${typename2}>(const std::string& key)
{
    int index = bnk::find_${typename}(key);if( index<0 ) return "";
    //return ((databank_${typename}*)get_bank_${typename}(index))->GetKeyToSize();
    return (get_bank_${typename}(index))->GetKeyToSize();
}
template<> int bnk::setkeytosize<${typename2}>(const std::string& key, const std::string& key_to_size)
{
    int index = bnk::find_${typename}(key);if( index<0 ) return BNK_NG;
    //((databank_${typename}*)get_bank_${typename}(index))->SetKeyToSize(key_to_size);
    (get_bank_${typename}(index))->SetKeyToSize(key_to_size);
    return BNK_OK;
}
template<> std::vector<std::string> bnk::getbufferlist<${typename2}>()
{
    using namespace bnk_private;
    std::vector<std::string> list;
    for ( auto bnk : gBnkRecord${typesymbol} ) {
    	if ( !bnk ) continue;
    	list.emplace_back( bnk->GetName() );
    }
    return list;
}
template<> std::vector<${typename2}>* bnk::getvecptr<${typename2}>(const std::string& key)
{
    int index = bnk::find_${typename}(key); if( index<0 ) return nullptr;
    //return ((databank_${typename}*)get_bank_${typename}(index))->GetVecPtr();
    return (get_bank_${typename}(index))->GetVecPtr();
}

EOF

if [ $typesymbol != "B" ]
then
cat<<EOF
template<> ${typename2}* bnk::getptr<${typename2}>(const std::string key)
{
    int index = bnk::find_${typename}(key); if( index<0 ) return nullptr;
    //return ((databank_${typename}*)get_bank_${typename}(index))->GetPtr();
    return (get_bank_${typename}(index))->GetPtr();
}
EOF
fi

}

filename="bnk_spec.cpp"
rm $filename
speci_impl SH "short"  0       >> $filename
speci_impl US "ushort" 0       >> $filename
speci_impl UI "uint"   0       >> $filename
speci_impl I  "int"    0       >> $filename
speci_impl L  "long"   0       >> $filename
speci_impl LL "llong"  0       >> $filename 
speci_impl F  "float"  0.0     >> $filename
speci_impl D  "double" 0.0     >> $filename
speci_impl B  "bool"   "false" >> $filename
speci_impl S  "string" '""'    >> $filename
