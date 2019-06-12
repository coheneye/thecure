#ifndef _THECURE_CRYPTO_H_
#define _THECURE_CRYPTO_H_


#include <string>

using namespace std;

/**
 * This namespace is all about:
 * 1) hash
 * 2) random
 * 3) cryption
 * 4) sign
 */
namespace crypto{

    /** init library */
    bool init();

    std::string version();

     // ============= reversable hash functions =============
   
    std::string bin2hex(const unsigned char* bin, const size_t bin_len);

    int hex2bin(unsigned char*buf, const size_t buf_size, const char* hex, const size_t hex_len);

    std::string b64enc(const char* bin, const size_t bin_len, const int variant=1);

    int b64dec(char* bin, size_t bin_len, const char* b64, const size_t b64_len, const int variant=1);

    /**
     * @return: hex of random buf with length crypto_box_NONCEBYTES
     */
    std::string gen_nonce();

    // ============= keys used for hash or encryption =============
    std::string gen_key_hash();

    std::string gen_key_hash_short();

    std::string gen_key_hash_hmac256();

    std::string gen_key_hash_hmac512();

    /**
     * @return: random integer
     */
    uint32_t random();

    uint32_t random_uniform(const uint32_t upper_bound=0xFFFFFFFF);

    /**
     * 随机缓冲区内容 用于生成各种 key
     */
    void  random_buf(unsigned char* buf, const size_t buf_len);

    /**
     * 生成一个 hex 格式的字符串, 非内部使用。
     */
    std::string random_str(const size_t len);

    /**
     *  close fd /dev/urandom
     */
    int random_free();

    //
    void random_seed_reset();

    /**
     * 返回值(passwd_hashed) 可以直接存库
     */
    std::string hash_argon2(const char* passwd);

    /**
     * 密码验证。
     */
    bool hash_argon2_verify(const char* passwd, const char* passwd_hashed);

    #define hash_passwd hash_argon2
    #define hash_passwd_verify hash_argon2_verify

    /**
     * 返回 hex 格式的 hash。
     */
    std::string hash_sha256(const char* data, const size_t data_len);

    std::string hash_sha512(const char* data, const size_t data_len);

    /**
     * auth
     * 
     * @key: 必须长度为 crypto_auth_hmacsha256_KEYBYTES 的 bin 格式
     * 
     */
    std::string hash_hmac256(const char* data, const size_t data_len, const char* key);

    /**
     * auth
     * 
     * @key: 必须长度为 crypto_auth_hmacsha512_KEYBYTES 的 bin 格式
     * 
     */
    std::string hash_hmac512(const char* data, const size_t data_len, const char* key);

    #define sha256 hash_sha256
    #define sha512 hash_sha512
    #define hmac256 hash_hmac256
    #define hmac512 hash_hmac512

    /**
     *  极速 hash， 取代 md5
     * @key: 长度 crypto_generichash_KEYBYTES
     */
    std::string hash(const char* data, const size_t data_len, const char* key=nullptr);
    /**
     *  64bit 输出，可构建 算法类 hash
     * @key: 长度必须为 crypto_shorthash_KEYBYTES
     */
    std::string hash_short(const char* data, const size_t data_len, const char* key);

    std::string sym_gen_key();
    /**
     *  对称加密
     * 
     * @data, @data_len: 待加密数据
     * @nonce: 外部传入值，为 gen_nonce() 的返回值。
     * @key: 长度 crypto_secretbox_KEYBYTES 
     * 
     * @return: hex 格式的加密数据
     */
    std::string sym_enc(const char* data, const size_t data_len, const char* nonce, 
        const char* key);

    /**
     *  验证并返回解密数据
     * 
     * @decrypted: 输出值，解密数据，长度调用方保证。
     * @crypted: 待解密数据。hex 格式。
     * @nonce: gen_nonce() 返回值
     * @key: 长度 crypto_secretbox_KEYBYTES 
     * 
     * @return: 验证成功或失败。
     */
    bool sym_verify(char* decrypted, const char* crypted, const char* nonce, const char* key);

    /**
     * 生成公钥私钥对
     * 
     * @priv: hex 格式 private key
     * @pub: hex 格式 public key
     * 
     * @return 
     */
    bool asym_gen_key_pair(std::string& priv, std::string& pub);

    /**
     * 非对称加密
     * 
     * @data, @data_len: 待加密原始数据
     * @nonce: gen_nonce 返回值
     * @pub_key: 解密方公钥
     * @priv_key: 
     * 
     * @return: hex formated data
     */
    std::string asym_enc(const char* data, int data_len, const char* nonce, const char* pub_key, const char* priv_key);

    /**
     * 解密
     * 
     * @decrypted: 解密后数据
     * @ciphered: 待解密数据 hex
     * @nonce: 同上
     * @pub_key: 同上
     * @priv_key:
     * 
     * @return 验证成功失败
     */
    bool asym_verify(char* decrypted, const char* ciphered, const char* nonce, const char* pub_key, const char* priv_key);

    bool sign_gen_key_pair(std::string& priv, std::string& pub);
    /**
     *  数字签名
     *  
     */
    std::string sign_auth(const char* data, const size_t data_len, const char* priv_key);

    /**
     * 签名验证
     */
    bool sign_verify(char* unsigned_data, const char* signed_data, const char* pub_key);
};


#endif
