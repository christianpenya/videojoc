#ifndef __H_CHECKED_RELEASE__
#define __H_CHECKED_RELEASE__

#include <vector>
#include <list>
#include <set>

namespace base
{
namespace utils
{
template < class T > void CheckedRelease(std::set< T > & ar_set)
{
    for (std::set< T >::iterator itb = ar_set.begin(), ite = ar_set.end(); itb != ite; ++itb)
    {
        CheckedRelease(*itb);
    }
    ar_set.clear();
}

template < class T > void CheckedRelease(std::list< T > & ar_list)
{
    for (std::list< T >::iterator itb = ar_list.begin(), ite = ar_list.end(); itb != ite; ++itb)
    {
        CheckedRelease(*itb);
    }
    ar_list.clear();
}

template < class T > void CheckedRelease(std::vector< T > & ar_vec)
{
    for (size_t i = 0; i < ar_vec.size(); ++i)
    {
        CheckedRelease(ar_vec[i]);
    }
    ar_vec.clear();
}

template< class T > inline void CheckedRelease(T*& p_ptr)
{
    if (p_ptr)
    {
        p_ptr->Release();
        p_ptr = nullptr;
    }
}
}
}

#endif // __H_CHECKED_RELEASE__
