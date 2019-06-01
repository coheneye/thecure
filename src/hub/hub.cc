#include "hub.h"
#include <uv.h>
#include <memory>


Hub::Hub(){
    m_hdl = malloc(sizeof(uv_loop_t));
    uv_loop_init((uv_loop_t*)m_hdl);
}


Hub::~Hub(){
    if(m_hdl){
        uv_loop_close((uv_loop_t*)m_hdl);
        free(m_hdl);
        m_hdl = 0;
    }
}


void * Hub::handle() const { 
    return m_hdl; 
}


int Hub::serve_forever(){
    return uv_run((uv_loop_t*)m_hdl, UV_RUN_DEFAULT);
}

int Hub::stop()
{
    uv_stop((uv_loop_t*)m_hdl);
}
