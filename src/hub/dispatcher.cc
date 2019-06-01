#include "dispatcher.h"
#include "session.h"

#include <stdexcept>
#include <iostream>


IDispatcher::IDispatcher()
{

}


IDispatcher::~IDispatcher()
{

}

/** @buf: 为输出参数 */
void IDispatcher::lend(char** buf, unsigned int* len)
{
    // default implement
    if(*len < 8){
        *len = 8;
    }
    *buf = (char*)malloc(*len);
    std::cout << "default lend() implementation." << std::endl;
    return;
}
void IDispatcher::remand(char* buf, unsigned int len)
{
    free(buf);
    std::cout << "default remand() implementation." << std::endl;
    buf = 0;
    return;
}


void IDispatcher::dispatch(ISession* ses, char* data, int data_len)
{
    std::cout << "not implemented yet." << std::endl;
    //throw std::runtime_error("not implement.");
}