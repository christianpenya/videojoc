#include <memory>

template<typename T>
struct CReleaser
{
    void operator()(T* obj)
    {
        if (obj)
        {
            obj->Release();
            obj = nullptr;
        }
    }
};

template<typename T>
using releaser_ptr = std::unique_ptr < T, CReleaser<T> >;