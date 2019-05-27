#pragma once

#include "ch_stl.h"

// @TODO(CHall): Find out who created this and give credit

template <typename F>
struct Priv_Defer {
    F f;
    Priv_Defer(F f) : f(f) {}
    ~Priv_Defer() { f(); }
};

template <typename F>
Priv_Defer<F> defer_func(F f) {
    return Priv_Defer<F>(f);
}

#define CH_DEFER_1(x, y)    x##y
#define CH_DEFER_2(x, y)    CH_DEFER_1(x, y)
#define CH_DEFER_3(x)       CH_DEFER_2(x, __COUNTER__)
#define defer(code)         auto CH_DEFER_3(_defer_) = defer_func([&](){code;})
