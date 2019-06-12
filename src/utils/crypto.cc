#include <utils/crypto.h>
#include <cassert>
#include <string.h>
#include <sodium.h>


namespace crypto{
    static size_t strlen_s(const char* p)
    {
        if(!p){
            return 0;
        }
        return strlen(p);
    }

    bool init()
    {
        return 0 == sodium_init();
    }


    std::string version()
    {
        return sodium_version_string();
    }


    std::string bin2hex(const unsigned char* bin, const size_t bin_len)
    {
        size_t hex_len = bin_len * 2 + 1;
        std::string rtv(bin_len * 2 + 1, '0');
        sodium_bin2hex((char*)rtv.data(), hex_len, bin, bin_len);
        return rtv;
    }


    int hex2bin(unsigned char*bin, const size_t bin_len, const char* hex, const size_t hex_len)
    {
        if(!sodium_hex2bin(bin, bin_len, hex, hex_len, nullptr, 0, nullptr)){
            return -1;
        }
        return 0;
    }


    std::string b64enc(const char* bin, const size_t bin_len, const int variant)
    {
        size_t max_len = sodium_base64_encoded_len(bin_len, variant);
        std::string rtv(max_len, '0');
        sodium_bin2base64((char*)rtv.data(), max_len, (unsigned char*)bin, bin_len, variant);
        return rtv;
    }


    int b64dec(char* bin, size_t bin_len, const char* b64, const size_t b64_len, const int variant)
    {
        return sodium_base642bin((unsigned char*)bin, bin_len, b64, b64_len, nullptr, nullptr, nullptr, variant);
    }

    
    std::string gen_nonce()
    {
        unsigned char nonce[crypto_box_NONCEBYTES] = {0};
        randombytes_buf(nonce, crypto_box_NONCEBYTES);
        return bin2hex(nonce, crypto_box_NONCEBYTES);
    }


    std::string gen_key_hash()
    {
        return random_str(crypto_generichash_KEYBYTES);
    }


    std::string gen_key_hash_short()
    {
        return random_str(crypto_shorthash_KEYBYTES);
    }


    std::string gen_key_hash_hmac256()
    {
        return random_str(crypto_auth_hmacsha256_KEYBYTES);
    }


    std::string gen_key_hash_hmac512()
    {
        return random_str(crypto_auth_hmacsha512_KEYBYTES);
    }


    std::string sym_gen_key()
    {
        return random_str(crypto_secretbox_KEYBYTES);
    }


    uint32_t random()
    {
        return randombytes_random();
    }


    uint32_t random_uniform(const uint32_t upper_bound)
    {
        return randombytes_uniform(upper_bound);
    }


    void  random_buf(unsigned char* buf, const size_t buf_len)
    {
        return randombytes_buf(buf, buf_len);
    }

    
    std::string random_str(const size_t len)
    {
        assert(len > 0);
        std::string rtv(len, '0');
        randombytes_buf((void*)rtv.data(), len);
        return bin2hex((unsigned char*)rtv.data(), len);
    }

   
    int random_free()
    {
        return randombytes_close();
    }

    
    void random_seed_reset()
    {
        return randombytes_stir();
    }

    
    std::string hash_passwd(const char* passwd)
    {
        char hashed[crypto_pwhash_STRBYTES] = {0};
        if(crypto_pwhash_str(hashed, passwd, strlen_s(passwd), crypto_pwhash_OPSLIMIT_SENSITIVE, crypto_pwhash_MEMLIMIT_SENSITIVE)){
            return "";
        }
        return hashed;
    }

    
    bool hash_passwd_verify(const char* passwd, const char* passwd_hashed)
    {
        return 0 == crypto_pwhash_str_verify(passwd_hashed, passwd, strlen_s(passwd));
    }

    
    std::string hash_sha256(const char* data, const size_t data_len)
    {
        unsigned char hash[crypto_hash_sha256_BYTES] = {0};
        crypto_hash_sha256(hash, (unsigned char*)data, data_len);
        return bin2hex(hash, crypto_hash_sha256_BYTES);
    }


    std::string hash_sha512(const char* data, const size_t data_len)
    {
        unsigned char hash[crypto_hash_sha512_BYTES] = {0};
        crypto_hash_sha256(hash, (unsigned char*)data, data_len);
        return bin2hex(hash, crypto_hash_sha512_BYTES);
    }


    std::string hash_hmac256(const char* data, const size_t data_len, const char* key)
    {
        unsigned char hash[crypto_auth_hmacsha256_BYTES] = {0};

        unsigned char* bin_key = nullptr;
        unsigned char bin_key_body[crypto_auth_hmacsha256_KEYBYTES] = {0};
        if(key){
            bin_key = bin_key_body;
            hex2bin(bin_key, crypto_auth_hmacsha256_KEYBYTES, key, strlen_s(key));
        }
        
        crypto_auth_hmacsha256(hash, (unsigned char*)data, data_len, bin_key);

        return bin2hex(hash, crypto_auth_hmacsha256_BYTES);
    }

    std::string hash_hmac512(const char* data, const size_t data_len, const char* key)
    {
        unsigned char hash[crypto_auth_hmacsha512_BYTES] = {0};

        unsigned char* bin_key = nullptr;
        unsigned char bin_key_body[crypto_auth_hmacsha512_KEYBYTES] = {0};
        if(key){
            bin_key = bin_key_body;
            hex2bin(bin_key, crypto_auth_hmacsha512_KEYBYTES, key, strlen_s(key));
        }

        crypto_auth_hmacsha256(hash, (unsigned char*)data, data_len, bin_key);

        return bin2hex(hash, crypto_auth_hmacsha512_BYTES);
    }

   
    std::string hash(const char* data, const size_t data_len, const char* key)
    {
        unsigned char dest[crypto_generichash_BYTES] = {0};

        unsigned char* bin_key = nullptr;
        size_t key_len = 0;
        unsigned char bin_key_body[crypto_generichash_KEYBYTES] = {0};
        if(key){
            key_len = crypto_generichash_KEYBYTES;
            bin_key = bin_key_body;
            hex2bin(bin_key, crypto_generichash_KEYBYTES, key, strlen_s(key));
        }
        
        crypto_generichash(dest, crypto_generichash_BYTES, (unsigned char*)data, data_len, bin_key, key_len);

        return bin2hex(dest, crypto_generichash_BYTES);
    }

    
    std::string hash_short(const char* data, const size_t data_len, const char* key)
    {
        unsigned char dest[crypto_shorthash_BYTES] = {0};

        unsigned char bin_key[crypto_shorthash_KEYBYTES] = {0};
   
        hex2bin(bin_key, crypto_shorthash_KEYBYTES, key, strlen_s(key));

        crypto_shorthash(dest, (unsigned char*)data, data_len, bin_key);

        return bin2hex(dest, crypto_shorthash_BYTES);
    }

    
    std::string sym_enc(const char* data, const size_t data_len, const char* nonce, 
        const char* key)
    {
        std::string ciphered(crypto_secretbox_MACBYTES + data_len, '0');
    
        unsigned char bin_nonce[crypto_secretbox_NONCEBYTES] = {0};
        hex2bin(bin_nonce, crypto_secretbox_NONCEBYTES, nonce, strlen_s(nonce));
        
        unsigned char bin_key[crypto_secretbox_KEYBYTES] = {0};

        hex2bin(bin_key, crypto_secretbox_KEYBYTES, key, strlen_s(key));     

        crypto_secretbox_easy((unsigned char*)ciphered.data(), (unsigned char*)data, data_len, bin_nonce, bin_key);

        return bin2hex((unsigned char*)ciphered.data(), crypto_secretbox_MACBYTES + data_len);
    }

    
    bool sym_verify(char* decrypted, const char* crypted, const char* nonce, const char* key)
    {
        size_t bin_crypted_len = strlen_s(crypted) / 2;
        std::string bin_crypted(bin_crypted_len, '0');
        hex2bin((unsigned char*)bin_crypted.data(), bin_crypted_len, crypted, strlen_s(crypted));

        unsigned char bin_nonce[crypto_secretbox_NONCEBYTES] = {0};
        hex2bin(bin_nonce, crypto_secretbox_NONCEBYTES, nonce, strlen_s(nonce));

        unsigned char bin_key[crypto_secretbox_KEYBYTES] = {0};
        hex2bin(bin_key, crypto_secretbox_KEYBYTES, key, strlen_s(key));

        return 0 == crypto_secretbox_open_easy((unsigned char*)decrypted, (unsigned char*)bin_crypted.data(), bin_crypted_len, bin_nonce, bin_key);
    }


    bool asym_gen_key_pair(std::string& priv, std::string& pub)
    {
        unsigned char pub_key[crypto_box_PUBLICKEYBYTES] = {0};
        unsigned char priv_key[crypto_box_SECRETKEYBYTES] = {0};

        if (0 != crypto_box_keypair(pub_key, priv_key)){
            return false;
        }

        pub = bin2hex(pub_key, crypto_box_PUBLICKEYBYTES);
        priv = bin2hex(priv_key, crypto_box_SECRETKEYBYTES);
        
        return true;
    }

    bool sign_gen_key_pair(std::string& priv, std::string& pub)
    {
        unsigned char pub_key[crypto_sign_PUBLICKEYBYTES] = {0};
        unsigned char priv_key[crypto_sign_SECRETKEYBYTES] = {0};

        if (0 != crypto_sign_keypair(pub_key, priv_key)){
            return false;
        }

        pub = bin2hex(pub_key, crypto_sign_PUBLICKEYBYTES);
        priv = bin2hex(priv_key, crypto_sign_SECRETKEYBYTES);
        
        return true;
    }
    
    std::string asym_enc(const char* data, int data_len, const char* nonce, const char* pub_key, const char* priv_key)
    {
        unsigned char bin_pub[crypto_box_PUBLICKEYBYTES] = {0};
        unsigned char bin_priv[crypto_box_SECRETKEYBYTES] = {0};
        unsigned char bin_nonce[crypto_box_NONCEBYTES] = {0};
        std::string ciphered(crypto_box_MACBYTES + data_len, '0');

        hex2bin(bin_nonce, crypto_box_NONCEBYTES, nonce, strlen_s(nonce));
        hex2bin(bin_pub, crypto_box_PUBLICKEYBYTES, pub_key, strlen_s(pub_key));
        hex2bin(bin_priv, crypto_box_SECRETKEYBYTES, priv_key, strlen_s(priv_key));

        if(0 != crypto_box_easy((unsigned char*)ciphered.data(), (unsigned char*)data, data_len, bin_nonce, bin_pub, bin_priv)){
            return "";
        }
        return bin2hex((unsigned char*)ciphered.data(), crypto_box_MACBYTES + data_len);
    }

    
    bool asym_verify(char* decrypted, const char* ciphered, const char* nonce, const char* pub_key, const char* priv_key)
    {
        unsigned char bin_pub[crypto_box_PUBLICKEYBYTES] = {0};
        unsigned char bin_priv[crypto_box_SECRETKEYBYTES] = {0};
        unsigned char bin_nonce[crypto_box_NONCEBYTES] = {0};

        size_t ciphered_len = strlen_s(ciphered);
        size_t bin_ciphered_len = ciphered_len / 2;
        std::string bin_ciphered(bin_ciphered_len, '0');

        hex2bin(bin_nonce, crypto_box_NONCEBYTES, nonce, strlen_s(nonce));
        hex2bin(bin_pub, crypto_box_PUBLICKEYBYTES, pub_key, strlen_s(pub_key));
        hex2bin(bin_priv, crypto_box_SECRETKEYBYTES, priv_key, strlen_s(priv_key));

        hex2bin((unsigned char*)bin_ciphered.data(), bin_ciphered_len, ciphered, ciphered_len);

        return 0 == crypto_box_open_easy((unsigned char*)decrypted, (unsigned char*)bin_ciphered.data(), bin_ciphered_len, bin_nonce, bin_pub, bin_priv);
    }

    std::string sign_auth(const char* data, const size_t data_len, const char* priv_key)
    {
        unsigned char bin_priv[crypto_sign_SECRETKEYBYTES] = {0};
        std::string signed_data(crypto_sign_BYTES + data_len, '0');
        unsigned long long signed_len = 0;

        hex2bin(bin_priv, crypto_sign_SECRETKEYBYTES, priv_key, strlen_s(priv_key));
        crypto_sign((unsigned char*)signed_data.data(), &signed_len, (unsigned char*)data, data_len, bin_priv);

        signed_data[signed_len] = 0;
        return bin2hex((const unsigned char*)signed_data.data(), signed_len);
    }


    bool sign_verify(char* unsigned_data, const char* signed_data, const char* pub_key)
    {
        size_t signed_data_len = strlen_s(signed_data);
        size_t bin_signed_data_len = signed_data_len / 2;
        std::string bin_signed_data(bin_signed_data_len, '0');
        hex2bin((unsigned char*)bin_signed_data.data(), bin_signed_data_len, signed_data, signed_data_len);

        unsigned char bin_pub[crypto_sign_PUBLICKEYBYTES] = {0};
        hex2bin(bin_pub, crypto_sign_PUBLICKEYBYTES, pub_key, strlen_s(pub_key));

        unsigned long long unsigned_len = 0;
        return 0 == crypto_sign_open((unsigned char*)unsigned_data, &unsigned_len, (unsigned char*)bin_signed_data.data(), bin_signed_data_len, bin_pub);
    }
};
