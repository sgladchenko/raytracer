#ifndef __CACHED
#define __CACHED

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
        : value{obj}, actual{true} {}
        Cached(T&& obj)
        : value{std::move(obj)}, actual{true} {}

    // The method to make the cached value up-to-date again
        void actualize(const T& obj) { value = obj; actual = true; }
        void actualize(T&& obj) { value = std::move(obj); actual = true; }
        void expire() { actual = false; }
    
    // Data and flag which tells if value is up-to-date
        T value;
        bool actual = true;
    };
}

#endif // __CACHED