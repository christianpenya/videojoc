#include <memory>

template<typename T>
struct CReleaser
{
	void operator()(T* obj)
	{
		obj->Release();
	}
};

template<typename T>
using releaser_ptr = std::unique_ptr < T, CReleaser<T> >;