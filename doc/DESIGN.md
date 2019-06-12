# Piece of Design

## zookeeper

/thecure
    -- settings
        --tax:5
        --logics
            -- 20101 : {"enabled":true, "name":"ddz", "lowest":30, "highest":100, "unit":1,
                   "rooms":3, "tables_per_room":100, "tax":5} [persist]
    -- mysqls:
        -- 1: {} [persist]

    -- nats
        -- 1: {} [persist]

    -- redis : 
        -- 1: {"ip":"192.168.0.33", "port":6333, "requirePass":"abc"} [persist]

    -- logins
        -- 1: {"ip":"192.168.0.2", "port":2183} [persist]

    -- event servers
        -- 1: {"ip":"192.168.0.2", "port":2183} [ephemeral]
        -- 2: {"ip":"192.168.0.2", "port":2183} [ephemeral]

    -- http servers
        -- 1: {"ip":"192.168.0.2", "port":2183, "sign":""} [ephemeral]
        -- 2: {"ip":"192.168.0.2", "port":2183, "sign":""} [ephemeral]

    -- agents
        -- 1 : {"ip":"192.168.0.2", "port":2183} [ephemeral]
        -- 2 : {"ip":"192.168.0.2", "port":2183} [ephemeral]

    -- logics
        -- 10701 : {"ip":"192.168.0.2", "port":2183} [ephemeral]
        -- 20101 : {"ip":"192.168.0.2", "port":2183} [ephemeral]

## redis

user:<user_id> [hash] <user metadata>
