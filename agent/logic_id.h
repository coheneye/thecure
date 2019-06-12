#ifndef _AGENT_LOGIC_ID_H_
#define _AGENT_LOGIC_ID_H_

/**
 * Logic Type Id    代表一种逻辑类型，比如斗地主逻辑。最大 MAX_TYPE_ID 支持100
 * Logic Group Id   代表一种逻辑的分组，一个分组可共享参数配置 。比如斗地主中级场。MAX_GROUP_ID 最大 支持100
 * Logic Sequence Id 代表分组下的一个实例，可以多个实例部署，用于负载均衡或容量扩展。MAX_SEQ_ID 最大支持100
 * 
 * 范围: [1, 100) 
 * 1 为大厅保留，
 * 所以取值范围为: (1, MAX)
 * 
 * 则 logic_id = type_id * 10000 + group_id * 100 + seq_id
 * 
 * eg.
 * type_id = 73
 * group_id = 1
 * seq_id = 1
 * 
 * logic_id = 73 * 100 * 100 + 1 * 100 + 1 = 730101
 */
#include <cstdint>
#include <cassert>


#define MIN_ID          1
#define MAX_TYPE_ID     100
#define MAX_GROUP_ID    100
#define MAX_SEQ_ID      100

typedef uint64_t id_t;

inline bool valid_type_id(id_t type_id)
{
    return type_id >= MIN_ID && type_id < MAX_TYPE_ID;
}

inline bool valid_group_id(id_t group_id)
{
    return group_id >= MIN_ID && group_id < MAX_GROUP_ID;
}

inline bool valid_seq_id(id_t seq_id)
{
    return seq_id >= MIN_ID && seq_id < MAX_SEQ_ID;
}

inline id_t make_logic_id(id_t type_id, id_t group_id, id_t seq_id)
{
    assert(valid_type_id(type_id));
    assert(valid_group_id(group_id));
    assert(valid_seq_id(seq_id));

    return type_id * (MAX_GROUP_ID * MAX_SEQ_ID) +
        group_id * MAX_SEQ_ID + 
        seq_id;
}

inline int parse(id_t logic_id, id_t* type_id, id_t* group_id, id_t* seq_id)
{
    id_t mul = MAX_GROUP_ID * MAX_SEQ_ID;
    *type_id = logic_id / mul;
    *group_id = logic_id % mul / MAX_SEQ_ID;
    *seq_id = logic_id % MAX_SEQ_ID;

    assert(valid_type_id(*type_id));
    assert(valid_group_id(*group_id));
    assert(valid_seq_id(*seq_id));
}

#endif
