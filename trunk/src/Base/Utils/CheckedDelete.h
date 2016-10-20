#ifndef __H_CHECKED_DELETE__
#define __H_CHECKED_DELETE__

#include <vector>
#include <list>
#include <set>

namespace base{
	namespace utils{
		template < class T > void CheckedDelete(std::set< T > & ar_set)
		{
			for (std::set< T >::iterator itb = ar_set.begin(), ite = ar_set.end(); itb != ite; ++itb)
			{
				CheckedDelete(*itb);
			}
			ar_set.clear();
		}

		template < class T > void CheckedDelete(std::list< T > & ar_list)
		{
			for (std::list< T >::iterator itb = ar_list.begin(), ite = ar_list.end(); itb != ite; ++itb)
			{
				CheckedDelete(*itb);
			}
			ar_list.clear();
		}

		template < class T > void CheckedDelete(std::vector< T > & ar_vec)
		{
			for (size_t i = 0; i < ar_vec.size(); ++i)
			{
				CheckedDelete(ar_vec[i]);
			}
			ar_vec.clear();
		}

		template< class T > inline void CheckedDelete(T*& p_ptr)
		{
			if (p_ptr)
			{
				delete p_ptr;
				p_ptr = nullptr;
			}
		}

		template< class T > inline void DeleteArray(T*& p_ptr)
		{
			delete[] p_ptr;
			p_ptr = nullptr;
		}
	}
}

#endif // __H_CHECKED_DELETE__
