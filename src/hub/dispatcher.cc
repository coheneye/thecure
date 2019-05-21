#include "dispatcher.h"
#include "session.h"


IDispatcher::IDispatcher()
{

}


IDispatcher::~IDispatcher()
{

}


void EchoDispatcher::dispatch(uv_stream_t* s, char* data, size_t data_len) 
{
        
}