#ifndef __CACHED
#define __CACHED

#include <memory>
#include <functional>

namespace raytracer
{
    template <class T>
    struct Cached
    {
    // This class is mostly needed for the values that are dependent on some other values,
    // and so we want to sync them, but we want to avoid unecessarry re-calculations.
        Cached(const T& obj) : content{obj} {}
        Cached(T&& obj) : content{std::move(obj)} {}
        T content;
        bool actual = true;
        void request_update() { actual = false; }
    };
}

#endif // __CACHED