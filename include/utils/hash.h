#ifndef _THECURE_HASH_H_
#define _THECURE_HASH_H_

#include <string>


std::string md5(const char* s);
std::string sha256(const char* s);
std::string base64_enc(const char* s);
std::string base64_dec(const char* s);


#endif
