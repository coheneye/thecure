#ifndef _THECURE_SESSION_H_
#define _THECURE_SESSION_H_

#include "server.h"
#include "buffer.h"
#include "dispatcher.h"


#define DEF_RECEIVE_BUF_SIZE 1600
#define DEF_SEND_BUF_SIZE DEF_RECEIVE_BUF_SIZE


typedef struct {
    uv_write_t wh;
    uv_buf_t b;
}writer_t;

/**
 * Session: represents a TCP connection.
 * 
 */
class Session {
public:
    explicit Session(Server* s);
    ~Session();
    // called in listener
    int accept(uv_stream_t* s);

    int connect(const struct sockaddr* dest, int* dest_len);

    // stream mode
    int start_read();
    int send(const char* buf, int size);
    // close session. but not free memeory.
    int close();
    
    // session id.
    // unique within single process.
    int id();

public:
    
protected:
    IDispatcher* m_disp;
    uv_tcp_t m_hot;         // used in server/client mode
    writer_t m_writer;
    uv_connect_t m_conn;    // used in client mode

    int m_buf_tracer;

    Buffer m_read_buf;
    Buffer m_send_buf;

    static uint64_t ms_stat_received;   // total received bytes
    static uint64_t ms_stat_sent;       // total send bytes
};

#endif