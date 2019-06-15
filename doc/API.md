# API

## Namespaces

- log
- tc
- utils

## log

- log.trace(string)
- log.debug(string)
- log.info(string)
- log.warn(string)
- log.error(string)
- log.critical(string)

## tc

### tc.hub  

 userdata, used to create other objects as a parameter.

### tc.timer

- tc.timer(hub)   // constructor

- tc.timer:start()
- tc.timer:stop()
- tc.timer:again()
- tc.timer:set_interval()
- tc.timer:get_interval()
- tc.timer:is_active()

### tc.task

- tc.task(hub)    // constructor
- tc.task:run()
- tc.task:cancel()
- tc.task:send()

### tc.redis_res

redis 回调返回值，不能构造

- tc.redis_res:type()
- tc.redis_res:array_size()
- tc.redis_res:value_of_int()
- tc.redis_res:value_of_str()
- tc.redis_res:value_of_arr()

### tc.redis

- tc.redis:is_connected()
- tc.redis:exec()

### utils.crypto

- utils.crpto.b64enc()
- utils.crpto.b64dec()

- utils.crpto.gen_nonce()
- utils.crpto.get_hash_key()
- utils.crpto.get_hmac256_key()
- utils.crpto.get_hmac512_key()
- utils.crpto.sha256()
- utils.crpto.sha512()
- utils.crpto.hmac256()
- utils.crpto.hmac512()
- utils.crpto.hash()
- utils.crpto.random()
- utils.crpto.random_uniform()
- utils.crpto.random_str()

### utils.time

- utils.time.now_in_sec()
- utils.time.now_in_mill()
- utils.time.now_in_str()
- utils.time.today_in_str()

### utils.time_counter

- utils.time_counter()    // constructor
- utils.time_counter:elapse_secs()
- utils.time_counter:elapse_mills()
- utils.time_counter:reset()
