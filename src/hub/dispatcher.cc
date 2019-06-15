#include <hub/dispatcher.h>
#include <hub/session.h>

#include <stdexcept>
#include <iostream>
#include <utils/logger.h>
#include <utils/function.h>


IDispatcher::IDispatcher() : m_need(sizeof(uint32_t)), m_received(0), m_header(true), m_buf(nullptr)
{

}


IDispatcher::~IDispatcher()
{
    if(m_buf){
        free(m_buf);
        m_buf = 0;
    }
}


/** @buf: 为输出参数 */
void IDispatcher::lend(char** buf, unsigned int* len)
{
    if(!m_buf){
        m_buf = (char*)malloc(m_need * sizeof(char));
    }
    *len = m_need - m_received;
    *buf = m_buf + m_received;    
}


void IDispatcher::remand(char* buf, unsigned int len)
{
}


void IDispatcher::on_data_read(ISession* ses, char* data, int data_len)
{
    m_received += data_len;

    if(m_received == m_need){   // received OK.
        if(m_header){
            memcpy(&m_need, m_buf, sizeof(uint32_t));
        }else{
            this->dispatch(ses, m_buf, m_need);
            m_need = sizeof(uint32_t);
        }

        m_received = 0;
        m_header = !m_header;

        m_buf = (char*)realloc(m_buf, m_need);
    }
}


void IDispatcher::dispatch(ISession* ses, char* data, int data_len)
{
    
}