//
// Created by Scott Barrett on 4/14/25.
// Edited by Liam Harrison
//

#include "serialization.h"

template <>
void stream_write(std::ostream& os, const Vector2& obj) {
    stream_write(os, obj.x);
    stream_write(os, obj.y);
}

template <>
void stream_write(std::ostream& os, const Color& obj) {
    stream_write(os, obj.r);
    stream_write(os, obj.g);
    stream_write(os, obj.b);
    stream_write(os, obj.a);
}

template <>
void stream_read(std::istream& is, Vector2& value) {
    stream_read(is, value.x);
    stream_read(is, value.y);
}

template <>
void stream_read(std::istream& is, Color& value) {
    stream_read(is, value.r);
    stream_read(is, value.g);
    stream_read(is, value.b);
    stream_read(is, value.a);
}