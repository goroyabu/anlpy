/**
   @date 2020/03/30
**/

#include <bnk.hpp>

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

namespace bnk
{
    template<typename T> class databank;
    using databank_short = databank<short>;
    using databank_ushort = databank<unsigned short>;
    using databank_uint = databank<unsigned int>;
    using databank_int = databank<int>;
    using databank_long = databank<long>;
    using databank_llong = databank<long long>;
    using databank_float = databank<float>;
    using databank_double = databank<double>;
    using databank_string = databank<std::string>;
    using databank_bool = databank<bool>;
    
    class databank_base
    {
    protected:
	databank_base(const std::string name, const int size = 1)
	    : m_name(name), m_alloc_size(size), m_used_size(0), m_attribute(0),
	      m_file_descripter(""), m_save_file(false),
	      m_num_put(0), m_num_get(0), m_key_to_size("") {}
	std::string m_name;
	int m_alloc_size;
	int m_used_size;
	int m_attribute;
	std::string m_file_descripter;
	bool m_save_file;
	double m_num_put;
	double m_num_get;
	std::string m_type;
	std::string m_key_to_size;
	
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
	void SetKeyToSize(const std::string& key) { m_key_to_size = key; }
	std::string GetKeyToSize() const { return m_key_to_size; }
    };
    
    /**
       @class databank
    **/
    template<typename T> class databank : public databank_base
    {
    protected:
	//std::vector<T> m_array;
	std::vector<T>* m_array_ptr;
	
    public:
	databank(const std::string name, int size = 1)
	    : databank_base(name, size), m_array_ptr(nullptr)
	{
	    // if(size<=1){ m_array.resize(1); }
	    // else m_array.reserve(size);
	    m_array_ptr = new std::vector<T>;
	    if(size<=1){ m_array_ptr->resize(1); }
	    else m_array_ptr->reserve(size);
	    m_key_to_size = std::to_string(size);
	}
	~databank()
	{
	    delete m_array_ptr;
	}

	int Put(const T& in)
	{
	    //m_array[0] = in; m_used_size = 1;
	    m_array_ptr->at(0) = in; m_used_size = 1;
	    ++m_num_put; return BNK_OK;
	}
	int Put(const std::vector<T>& vec, int begin, int end)
	{
	    if(begin==0 && end==-1){
		// for(auto elem : vec) m_array.emplace_back(elem);
		for(auto elem : vec) m_array_ptr->emplace_back(elem);
		m_used_size = (int)vec.size(); ++m_num_put;
		return BNK_OK;
	    }
	    
	    int size = (int)vec.size();
	    if( begin<0 || size<=begin ) return BNK_NG;
	    if( end<0 || size<end ) end = size;

	    if( end<begin ) return BNK_NG;
	    size = end - begin;
	    if( m_alloc_size<size ) return BNK_NG;
	    
	    // m_array.clear();
	    m_array_ptr->clear();

	    // for(int i=begin; i<end; ++i) m_array.emplace_back(vec[i]);
	    for(int i=begin; i<end; ++i) m_array_ptr->emplace_back(vec[i]);
	    
	    m_used_size = size; ++m_num_put;

	    return BNK_OK;
	}
	int Get(T* out)
	{
	    // *out = m_array[0]; ++m_num_get;
	    *out = m_array_ptr->at(0); ++m_num_get;
	    return BNK_OK;
	} 
        int Get(std::vector<T>* out, int begin, int end)
	{
	    if ( !out ) return BNK_NG;
	    if ( m_array_ptr->capacity()>m_alloc_size )
		m_alloc_size = m_array_ptr->size();
	    if ( m_array_ptr->size()!=m_used_size ) 
		m_used_size = m_array_ptr->size();
	    
	    out->clear();
	    //std::cout << begin << "=b, " << end << "=e" << std::endl;
	    // if( begin<0 || m_used_size<=begin ) begin = 0;//return BNK_NG;
	    // if( end<0 || m_used_size<end ) end = m_used_size;
	    // if( end<begin ) return BNK_NG; 
	    	    
	    // for(int i=begin; i<end; ++i) out->emplace_back(m_array[i]);
	    //for(int i=begin; i<end; ++i) out->emplace_back(m_array_ptr->at(i));
	    // for(int i=0; i<m_used_size; ++i)
	    //out->emplace_back(m_array_ptr->at(i));
	    
	    // std::cout << "bnk get size=" << m_array_ptr->size() << "." << std::endl;
	    for ( auto elem : *m_array_ptr )
		out->emplace_back(elem);	    
	    ++m_num_get;
	    // std::cout << "osize=" << out->size() << std::endl;
	    
	    return BNK_OK;
	}
	std::vector<T>* Get(int begin, int end)
	{
	    if ( m_array_ptr->capacity()>m_alloc_size )
		m_alloc_size = m_array_ptr->size();
	    if ( m_array_ptr->size()!=m_used_size ) 
		m_used_size = m_array_ptr->size();	    
	    ++m_num_get;
	    return m_array_ptr;
	}
	
	T* GetPtr()
	{
	    // return m_array.data();
	    return m_array_ptr->data();
	}
	std::vector<T>* GetVecPtr()
	{
	    //std::vector<T>* vp = &m_array;
	    //return &m_array;
	    return m_array_ptr;
	}
	void Resize(int size=0)
	{
	    // m_array.resize(size);
	    m_array_ptr->resize(size);
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
		// cout << m_array[i];
		cout << m_array_ptr->at(i);
		if(i < m_used_size-1) cout << ",";
	    }
	    cout << ")" << endl;
	}
	virtual std::string ClassName() { return "databank"; }
	//virtual std::string ValueType() const { return value_type<T>(); }
    };

}
