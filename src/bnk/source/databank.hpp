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
    using databank_int = databank<int>;
    using databank_double = databank<double>;
    using databank_string = databank<std::string>;
    using databank_bool = databank<bool>;
    
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
	    if(size<=1){ m_array.resize(1); }
	    else m_array.reserve(size);
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

}
