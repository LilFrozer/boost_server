//
// Created by Алексей Подоплелов on 26.05.2026.
//

#ifndef MIL_GROUPDATA_H
#define MIL_GROUPDATA_H

#include <vector>
#include <cstdint>

namespace TCP_DATA
{
enum class DataTypes : uint16_t
{
    TestStruct = 0x0
};

struct TestStruct
{
    std::string a{""};
    int b{0};
    double c{0.00};
    std::vector<int> d{};

    /**
     * @brief Сериализация в std::vector<uint8_t>
     */
    std::vector<uint8_t> serilize() const
    {
        std::vector<uint8_t> buffer;

        // !!!htonl -> into big_endian!!!

        // len(str) + str
        uint32_t str_len = htonl(a.length());
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&str_len),
                    reinterpret_cast<uint8_t*>(&str_len) + 4);
        buffer.insert(buffer.end(), a.begin(), a.end());

        // int
        int32_t b_net = htonl(b);
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&b_net),
                    reinterpret_cast<uint8_t*>(&b_net) + 4);

        // double
        double this_c = c;
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&this_c),
                    reinterpret_cast<uint8_t*>(&this_c) + 8);

        // size(vector) + vector
        uint32_t vec_size = htonl(d.size());
        buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&vec_size),
                    reinterpret_cast<uint8_t*>(&vec_size) + 4);

        for( int val : d )
        {
            int32_t val_net = htonl(val);
            buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&val_net),
                        reinterpret_cast<uint8_t*>(&val_net) + 4);
        }

        return buffer;
    }

    /**
     * @brief Десериализация из const uint8_t
     */
    static TestStruct deserialize(const uint8_t* data)
    {
        TestStruct result;
        size_t offset = 0;

        // !!!ntohl -> from big_endian!!!

        // lent(str) + str
        uint32_t str_len;
        memcpy(&str_len, data + offset, 4);
        str_len = ntohl(str_len);
        offset += 4;

        result.a.assign(reinterpret_cast<const char*>(data + offset), str_len);
        offset += str_len;

        // int
        memcpy(&result.b, data + offset, 4);
        result.b = ntohl(result.b);
        offset += 4;

        // double
        memcpy(&result.c, data + offset, 8);
        offset += 8;

        // size(vector) + vector
        uint32_t vec_size;
        memcpy(&vec_size, data + offset, 4);
        vec_size = ntohl(vec_size);
        offset += 4;

        result.d.resize(vec_size);
        for( size_t i=0;i<vec_size;++i )
        {
            memcpy(&result.d[i], data + offset, 4);
            result.d[i] = ntohl(result.d[i]);
            offset += 4;
        }

        return result;
    }
};
}

#endif //MIL_GROUPDATA_H