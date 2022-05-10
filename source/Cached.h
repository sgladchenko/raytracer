#ifndef __CACHED
#define __CACHED

#include <memory>
#include <functional>

namespace raytracer
{
    template <typename T>
    class Cached
    {
    // This class is mostly needed for the values that are dependent on some other values,
    // and so we want to sync them, but we want to avoid unecessarry re-calculations.
    public:
        Cached(const std::function<void(T&)>& _update) : update{_update} {}
        Cached(std::function<void(T&)>&& _update) : update{std::move(_update)} {}

        const T& get() 
        {
            if (!actual) update(content);
            return content;
        } 
        
    private:
        T content;
        bool actual = false;
        std::function<void(T&)> update;
    };
}

#endif // __CACHED