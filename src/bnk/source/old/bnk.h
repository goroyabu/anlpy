/**
   @file bnk.hpp
   @author Goro Yabu
   @date 2018/11/23
   @date 2019/06/05 v1.2 Add bnk_resize()
   @version 1.2
**/
#ifndef bnk_h
#define bnk_h

int BnkShmCreate(int buffer_size, char *shm_file);
int BnkShmOpen(char *shm_file);
int BnkShmClose(void);
int BnkIni(int buffer_size);
int BnkEnd(void);
template<typename T> int BnkDef(char *key, int size);
    
int BnkKey(char *key, int *index);
int BnkIsDef(char *key);
int BnkNdf(void);
int BnkEqv(char *new_key, int size, char *old_key, int start);
template<typename T> int BnkPut(char *key, int size, T *ptr);
template<typename T> int BnkGet(char *key, int size, int *used, T *ptr);
int BnkfPut(char *key, int *index_ptr, int size, void *ptr);
int BnkfGet(char *key, int *index_ptr, int size, int *used, void *ptr);
void BnkLst(void);
int BnkConnect(char *server);
int BnkExport(char *key);
int BnkExportAll(void);
int BnkServer(int port);

#endif
