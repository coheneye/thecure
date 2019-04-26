#include "server.h"


Server::Server():m_hol(nullptr){
    m_hol = uv_default_loop();
}


Server::~Server(){
    if(m_hol){
        uv_stop(m_hol);
    }
}


uv_loop_t * Server::handle() const { 
    return m_hol; 
}


int Server::serve_forever(){
    return uv_run(m_hol, UV_RUN_DEFAULT);
}
