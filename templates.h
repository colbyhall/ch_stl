#pragma once

#include "types.h"

namespace ch {
    template <typename T> struct remove_reference        { using Type = T; };
    template <typename T> struct remove_reference<T&>    { using Type = T; };
    template <typename T> struct remove_reference<T&&>   { using Type = T; };
    
    template <typename T> inline T&& forward(typename remove_reference<T>::Type& t) { return static_cast<T&&>(t); }
    template <typename T> inline T&& forward(typename remove_reference<T>::Type&& t) { return static_cast<T&&>(t); }
    template <typename T> constexpr T&& move(T& t) { return static_cast<typename remove_reference<T>::Type&&>(t); }
}
