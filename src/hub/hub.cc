#include "hub.h"


Hub::Hub():m_hol(nullptr){
    m_hol = uv_default_loop();
}


Hub::~Hub(){
    if(m_hol){
        uv_stop(m_hol);
    }
}


uv_loop_t * Hub::handle() const { 
    return m_hol; 
}


int Hub::serve_forever(){
    return uv_run(m_hol, UV_RUN_DEFAULT);
}
