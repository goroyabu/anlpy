/**
   @file bnk.hpp
   @author Goro Yabu
   @date 2018/11/23
   @date 2019/06/05 v1.2 Add bnk_resize()
   @version 1.2
**/
#ifndef bnk_hpp
#define bnk_hpp

#include <map>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <memory>
#include <iostream>
#include <iomanip>

/**
   @namespace bnk
**/
namespace bnk
{
    const int BNK_OK = 0;
    const int BNK_NG = -1;
    const int BNK_TRUE = 1;
    const int BNK_FALSE = 0;

    //template<typename T>
    //std::string value_type(){ return "template"; }
    
    class databank_base
    {
    protected:
	databank_base(const std::string name, const int size = 1)
	    : m_name(name), m_alloc_size(size), m_used_size(0), m_attribute(0),
	      m_file_descripter(""), m_save_file(false),
	      m_num_put(0), m_num_get(0) {}
	std::string m_name;
	int m_alloc_size;
	int m_used_size;
	int m_attribute;
	std::string m_file_descripter;
	bool m_save_file;
	double m_num_put;
	double m_num_get;
	std::string m_type;
	
    public:
	std::string GetName() const { return m_name; }
	int GetAllocSize() const { return m_alloc_size; }
	int GetUsedSize() const { return m_used_size; }
	int GetAttribute() const { return m_attribute; }
	std::string GetFileIO() const { return m_file_descripter; }
	double GetNumPut() const { return m_num_put; }
	double GetNumGet() const { return m_num_get; }

	std::string Key() const { return GetName(); }
	
	virtual void List() = 0;
	virtual void Export() = 0;
	virtual std::string ClassName() { return "databank_base"; }
	//virtual std::string ValueType() const { return "template"; }
    };
    
    /**
       @class databank
    **/
    template<typename T> class databank : public databank_base
    {
    protected:
	std::vector<T> m_array;
	
    public:
	databank(const std::string name, int size = 1)
	    : databank_base(name, size)
	{
	    if(size<0) size = 1;
	    m_array.reserve(size);
	}
	~databank() {}

	int Put(const T& in)
	{
	    m_array[0] = in; m_used_size = 1;
	    ++m_num_put; return BNK_OK;
	}
	int Put(const std::vector<T>& vec, int begin, int end)
	{
	    if(begin==0 && end==-1){
		for(auto elem : vec) m_array.emplace_back(elem);
		m_used_size = (int)vec.size(); ++m_num_put;
		return BNK_OK;
	    }
	    
	    int size = (int)vec.size();
	    if( begin<0 || size<=begin ) return BNK_NG;
	    if( end<0 || size<end ) end = size;

	    if( end<begin ) return BNK_NG;
	    size = end - begin;
	    if( m_alloc_size<size ) return BNK_NG;
	    
	    m_array.clear();

	    //for(int i=begin; i<end; ++i) m_array.push_back(vec[i]);
	    for(int i=begin; i<end; ++i) m_array.emplace_back(vec[i]);
	    
	    m_used_size = size; ++m_num_put;

	    return BNK_OK;
	}
	int Get(T* out)
	{
	    *out = m_array[0]; ++m_num_get;
	    return BNK_OK;
	} 
        int Get(std::vector<T>* out, int begin, int end)
	{
	    if( !out ) return BNK_NG;
	    out->clear();
	    //std::cout << begin << "=b, " << end << "=e" << std::endl;
	    if( begin<0 || m_used_size<=begin ) return BNK_NG;
	    if( end<0 || m_used_size<end ) end = m_used_size;
	    if( end<=begin ) return BNK_NG; 
	    
	    //for(int i=begin; i<end; ++i) out->push_back(m_array[i]);
	    for(int i=begin; i<end; ++i) out->emplace_back(m_array[i]);
	    //++m_num_get; std::cout << "osize=" << out->size() << std::endl;
	    
	    return BNK_OK;
	}
	T* GetPtr()
	{
	    return m_array.data();
	}
	void Resize(int size=0)
	{
	    m_array.resize(size);
	    m_used_size=size;
	}
	
	virtual void List()
	{
	    using namespace std;
	    cout << left << setw(31) << m_name;
	    cout << right << setw(10) << m_alloc_size;
	    cout << right << setw(9) << m_used_size;
	    cout << right << setw(10) << m_num_put;
	    cout << right << setw(10) << m_num_get;
	    cout << endl;
	}
	virtual void Export()
	{
	    using namespace std;
	    cout << setw(20) << m_name << "[";
	    cout << setw(3) << m_alloc_size << "] (";
	    for(int i=0; i<m_used_size; ++i){
		cout << m_array[i];
		if(i < m_used_size-1) cout << ",";
	    }
	    cout << ")" << endl;
	}
	virtual std::string ClassName() { return "databank"; }
	//virtual std::string ValueType() const { return value_type<T>(); }
    };

    int add_bank(databank_base* bank);
    databank_base* get_bank(int index); 
    
    int bnk_init();
    int bnk_end();
    int bnk_list();
    int bnk_export_all();
    int bnk_connect(std::string server);
    int bnk_server(int port);
    int bnk_ndef();
    
    /** @brief Return -1 if NOT defined. **/
    int bnk_is_def(const std::string key);

    /** @brief Define a new buffer. **/
    template<typename T>
    int bnk_def(const std::string key, const int size = 1)
    {
	if( bnk_is_def(key) == BNK_OK ) return BNK_NG;
	add_bank( new databank<T>(key, size) );
	return BNK_OK;
    }
    
    /** @brief Find index of key. **/
    int bnk_key(const std::string key, int* index);

    int bnk_equiv(const std::string new_key, const std::string old_key);

    /** @brief Put data. **/
    template<typename T> int bnk_put(const std::string key, const T& value)
    {
	int index; if( bnk_key(key, &index) == BNK_NG ) return BNK_NG;
	if( get_bank(index)->GetAllocSize()!=1 ) return BNK_NG;
	((databank<T>*)get_bank(index))->Put(value);
	return BNK_OK;
    }
    template<typename T> int bnk_put(const std::string key, const std::vector<T>& in, int begin, int end)
    {
	int index; if( bnk_key(key, &index) == BNK_NG ) return BNK_NG;
	((databank<T>*)get_bank(index))->Put(in, begin, end);
	return BNK_OK;
    }

    //modify on 2019/03/19
    template<typename T> T bnk_get(const std::string key)
    {
	int index;
	if( bnk_key(key, &index) == BNK_NG ){
	    std::cout << "Error in bnk::get() " << key << " is not defined" << std::endl; 
	    return 0;
	}
	if( get_bank(index)->GetAllocSize()!=1 ){
	    std::cout << "Error in bnk::get() " << key << " is array" << std::endl; 
	    return 0;
	}
	T out; if( ((databank<T>*)get_bank(index))->Get(&out) == BNK_NG ) return 0;
	return out;
    }
    template<typename T> int bnk_get(const std::string key, std::vector<T>* out, int begin=0, int end=-1)
    {
	int index; if( bnk_key(key, &index) == BNK_NG ) return BNK_NG;
	return ((databank<T>*)get_bank(index))->Get(out, begin, end);
    }

    template<typename T> T* bnk_ptr(const std::string key)
    {
	int index; if( bnk_key(key, &index) == BNK_NG ) return nullptr;
	return ((databank<T>*)get_bank(index))->GetPtr();
    }
    template<typename T> int bnk_resize(const std::string key, const int size=0)
    {
	int index; if( bnk_key(key, &index) == BNK_NG ) return BNK_NG;
	((databank<T>*)get_bank(index))->Resize(size);
	return BNK_OK;
    }
    
    /** **/
    int bnk_net_update(const int index);

    /** **/
    int bnk_export(const std::string key);

    /**
       For Compatibility
    **/
    //int BnkShmCreate(int buffer_size, char *shm_file);
    //int BnkShmOpen(char *shm_file);
    //int BnkShmClose(void);
    int BnkIni(int buffer_size);
    int BnkEnd(void);
    template<typename T> int BnkDef(char *key, int size)
    {
	return bnk_def<T>((std::string)key, size);
    }
    
    int BnkKey(char *key, int *index);
    int BnkIsDef(char *key);
    int BnkNdf(void);
    int BnkEqv(char *new_key, int size, char *old_key, int start);
    template<typename T> int BnkPut(char *key, int size, T *ptr)
    {
	if(size==1) return bnk_put<T>((std::string)key, *ptr);
	std::vector<T> vec;
	for(int i=0; i<size; ++i) vec.emplace_back(ptr[i]);
	return bnk_put<T>((std::string)key, vec, 0, size);
    }
    template<typename T> int BnkGet(char *key, int size, int *used, T *ptr)
    {
	if(size==1){
	    *ptr = bnk_get<T>((std::string)key);
	    *used = 1; return BNK_OK;
	}
	std::vector<T> vec;
	bnk_get<T>((std::string)key, &vec);
	*used = 0;
	for( auto elem : vec ){
	    ptr[*used] = elem; ++(*used);
	    if(*used>size) break;
	}
	return BNK_OK;
    }
    //int BnkfPut(char *key, int *index_ptr, int size, void *ptr);
    //int BnkfGet(char *key, int *index_ptr, int size, int *used, void *ptr);
    void BnkLst(void);
    int BnkConnect(char *server);
    int BnkExport(char *key);
    int BnkExportAll(void);
    int BnkServer(int port);
};
#endif
