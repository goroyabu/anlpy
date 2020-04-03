#ifndef VANL_Parameters_hpp
#define VANL_Parameters_hpp

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include <iterator> 

#include "ANLStatus.hpp"

namespace anl
{
    template<typename T> class VANL_Parameters;    
    using ANL_Iparameters = VANL_Parameters<int>;
    using ANL_Dparameters = VANL_Parameters<double>;
    using ANL_Sparameters = VANL_Parameters<std::string>;
    using ANL_Bparameters = VANL_Parameters<bool>;
    
    template<typename T>
    class VANL_Parameters
    {
    private:
	int                      nparams;
	std::vector<std::string> keys;
	std::vector<T>           values;

    public:
        VANL_Parameters() : nparams(0) {}
	VANL_Parameters(const VANL_Parameters& other)
	{
	    nparams = other.nparams;
	    std::copy( other.keys.begin(), other.keys.end(), std::back_inserter(keys) );
	    std::copy( other.values.begin(), other.values.end(), std::back_inserter(values) );
	}
	VANL_Parameters& operator = (const VANL_Parameters& other)
	{
	    nparams = other.nparams;
	    std::copy( other.keys.begin(), other.keys.end(), std::back_inserter(keys) );
	    std::copy( other.values.begin(), other.values.end(), std::back_inserter(values) );	    
	    return *this;
	}
	
	int index(const std::string& key) const
	{
	    auto it = std::find( keys.begin(), keys.end(), key );
	    return (int)std::distance( keys.begin(), it );    
	}
	
	bool exist(const std::string& key) const
	{
	    auto id = index(key);
	    if ( id == (int)keys.size() ) return false;
	    return true;	    
	}

	int define(const std::string& key, const T& defvalue)
	{
	    if ( exist(key)==true ) {
		std::cout << "*Warning* : A parameter " << key  << " is already defined" << std::endl;
		return -1;
	    }
	    keys.emplace_back(key);
	    values.emplace_back(defvalue);
	    ++nparams; return 0;
	}
	
	void set(const std::string& key, const T& value)
	{
	    if ( exist(key)==true )
		values[ index(key) ] = value;
	    else
		std::cout << "*Warning* : A parameter " << key << " is not found" << std::endl; 
	}

	T get(const std::string& key) const
	{
	    if ( exist(key)==false )
		std::cout << "***Error*** : A parameter " << key << " is not found" << std::endl;
	    return values[ index(key) ];
	}
	
	std::vector<std::tuple<std::string, T>> show()
	{
	    std::vector<std::tuple<std::string, T>> out;
	    for ( int i=0; i<nparams; ++i ) {
		out.emplace_back( std::make_tuple(keys[i], values[i]) );
	    }
	    return out;
	}
    };    
}

#endif
