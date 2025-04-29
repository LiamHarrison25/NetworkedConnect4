//
// Created by Scott Barrett on 4/14/25.
// Edited by Liam Harrison
//

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <iostream>
#include "endianness.h"
#include "raylib.h"

template <typename T>
T to_net_endian(const T& obj) {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    static_assert(sizeof(obj) == 1 || sizeof(obj) == 2 || sizeof(obj) == 4 || sizeof(obj) == 8,
        "T must be an 8, 16, 32, or 64 bit number.");
    union {
        T value;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
    } pun;
    pun.value = obj;
    switch (sizeof(obj)) {
    case 1:
        break;
    case 2:
        pun.u16 = user_htons(pun.u16);
        break;
    case 4:
        pun.u32 = user_htonl(pun.u32);
        break;
    case 8:
        pun.u64 = user_htonll(pun.u64);
        break;
    }
    return pun.value;
}

template <typename T>
T to_host_endian(const T& value) {
    static_assert(std::is_arithmetic_v<T>, "T must be arithmetic type");
    static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
        "T must be an 8, 16, 32, or 64 bit number.");
    union {
        T value;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
    } pun;
    pun.value = value;
    switch (sizeof(value)) {
    case 2:
        pun.u16 = user_ntohs(pun.u16);
        break;
    case 4:
        pun.u32 = user_ntohl(pun.u32);
        break;
    case 8:
        pun.u64 = user_ntohll(pun.u64);
        break;
    case 1:
        break;
    }
    return pun.value;
}

template <typename T>
void stream_write(std::ostream& os, const T& obj) {
    const T value = to_net_endian(obj);
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

template <>
void stream_write(std::ostream& os, const Vector2& obj);

template <>
void stream_write(std::ostream& os, const Color& obj);

template <typename T>
void stream_read(std::istream& is, T& value) {
    is.read(reinterpret_cast<char*>(&value), sizeof(T));
    value = to_host_endian(value);
}

template <>
void stream_read(std::istream& is, Vector2& value);

template <>
void stream_read(std::istream& is, Color& value);

// Alternate interface to stream_read
template <typename T>
T stream_read(std::istream& is) {
    T value;
    stream_read(is, value);
    return value;
}

#endif //SERIALIZATION_H
