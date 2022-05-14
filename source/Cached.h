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
        Cached()
        : actual{false} {}
        Cached(const T& obj)
        : content{obj}, actual{true} {}
    // Data and flag which data is up-to-data
        T content;
        bool actual = true;
    };
}

#endif // __CACHED