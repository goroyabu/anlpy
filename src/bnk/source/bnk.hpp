/**
   @file bnk.hpp
   @author Goro Yabu
   @date 2018/11/23
   @date 2019/06/05 v1.2 Add bnk_resize()
   @version 1.2
**/
#ifndef bnk_hpp
#define bnk_hpp

#include <vector>
#include <string>

/**
   @namespace bnk
**/
namespace bnk
{
    const int BNK_OK = 0;
    const int BNK_NG = -1;
    const int BNK_TRUE = 1;
    const int BNK_FALSE = 0;

    /** @brief Initialize BNK **/
    int init();

    /** @brief End BNK **/
    int end();

    /** @brief Show the list of the defined buffers **/
    int list();

    int export_all();
    int connect(std::string server);
    int server(int port);

    /** @brief Return the number of buffers **/
    int ndef();
    
    /** @brief Return false if the buffer is NOT defined **/
    bool is_def(const std::string key);

    /** @brief Define a new buffer **/
    template<typename T> int define(const std::string key, const int size = 1);
    
    /** @brief Find an index of the key **/
    int find(const std::string key);//, int* index);
    
    /** @brief Make an alias of the key **/
    int equiv(const std::string new_key, const std::string old_key);

    /** @brief Put values to the buffer **/
    template<typename T> int put(const std::string key, const T& value);
    template<typename T> int put(const std::string key, const std::vector<T>& in, int begin=0, int end=-1);

    /** @brief Get values in the buffer **/
    template<typename T> T              get(const std::string key);
    template<typename T> std::vector<T> getv(const std::string key, int begin=0, int end=-1);
    
    template<typename T> T*  getptr(const std::string key);
    template<typename T> int resize(const std::string key, const int size=0);
    
    int net_update(const int index);

    int export_buffer(const std::string key);
}
#endif
