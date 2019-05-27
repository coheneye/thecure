#include "hub.h"
#include <uv.h>
#include <memory>


Hub::Hub(){
    m_hol = malloc(sizeof(uv_loop_t));
    uv_loop_init((uv_loop_t*)m_hol);
}


Hub::~Hub(){
    if(m_hol){
        uv_loop_close((uv_loop_t*)m_hol);
        free(m_hol);
        m_hol = 0;
    }
}


void * Hub::handle() const { 
    return m_hol; 
}


int Hub::serve_forever(){
    return uv_run((uv_loop_t*)m_hol, UV_RUN_DEFAULT);
}

int Hub::stop()
{
    uv_stop((uv_loop_t*)m_hol);
}
