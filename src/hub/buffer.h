#ifndef _THECURE_BUFFER_H_
#define _THECURE_BUFFER_H_

#include <stdlib.h>
#include <queue>


class Buffer final{
public:
    explicit Buffer(size_t size);
    ~Buffer();

    void resize(size_t size);

    char* buf();

    size_t size();

private:
    char* m_buf;
    size_t m_size;
};


class BufPool final {
public:
    BufPool(int bsize, int qsize);
    ~BufPool();

    char* get();
    void  put(char*);
private:
    std::queue<char*> m_q;
};


#endif