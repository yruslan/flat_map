#ifndef _FLAT_MAP_H_INCLUDED_2015_01_17
#define _FLAT_MAP_H_INCLUDED_2015_01_17

#include <vector>
#include <algorithm>

/* 
 * Minimalistic map C++ template based on vector
 *
 * Flat map features
 * - Stores keys and values inside a vector, not in a binary tree
 * - Works faster for a work flow in which many adds follows many lookups
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Ruslan Yushchenko
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * For more information, please refer to <http://opensource.org/licenses/MIT>
 */

// Is noexcept supported?
#if defined(__clang__)
# if __has_feature(cxx_noexcept)
#    define NOEXCEPT noexcept
# endif // defined(__clang__)
#endif // __has_feature(cxx_noexcept)
#if	defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46
#    define NOEXCEPT noexcept
#endif // defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46
#if defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 180021114
#include <yvals.h>
#  ifdef _NOEXCEPT
#    define NOEXCEPT _NOEXCEPT
#  else
#    define NOEXCEPT noexcept
#  endif
#endif
#ifndef NOEXCEPT
#  define NOEXCEPT
#endif

// Is move semantics supported?
#if defined(__clang__)
# if __has_feature(__cxx_rvalue_references__)
#  define ENABLE_MOVE_SEMANTICS
# endif // __has_feature(__cxx_rvalue_references__)
#endif // defined(__clang__)
#if defined(__GNUC__)
# if ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#  if defined(__GXX_EXPERIMENTAL_CXX0X__)
#   define ENABLE_MOVE_SEMANTICS
#  endif // defined(__GXX_EXPERIMENTAL_CXX0X__)
# endif // ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) || (__GNUC__ > 4)
#endif // defined(__GNUC__)
#if (_MSC_VER >= 1700)
# define ENABLE_MOVE_SEMANTICS
#endif // (_MSC_VER >= 1700)

template<typename key_type, typename val_type>
class flat_map
{
public:
	typedef std::pair<key_type, val_type> pair_type;
	typedef typename std::vector<pair_type >::iterator iterator;
	typedef std::pair<iterator, iterator> iterator_pair;

	flat_map() : m_bSorted(true) {};
#ifdef ENABLE_MOVE_SEMANTICS
	flat_map(const flat_map& rhs) = default;
	flat_map(flat_map&& rhs) NOEXCEPT{ swap(rhs); };
	flat_map &operator=(const flat_map &rhs) = default;
#endif

	void clear();
	void reserve(size_t size);
	void insert(const pair_type &p);
	void insert(const key_type &k, const val_type &v);
	iterator find(const key_type &k);
	iterator lower_bound(const key_type &k);
	iterator upper_bound(const key_type &k);
	iterator_pair equal_range(const key_type &k);
	size_t count(const key_type &k);
	bool empty();
	size_t size();
	iterator erase(const key_type &k);
	iterator erase(iterator i0);
	iterator erase(iterator i0, iterator i1);
	void swap(flat_map& other) NOEXCEPT;

	iterator begin();
	iterator end();

	void sort(bool bPriorityFirstUnique = false);

private:
	template<class T>
	struct flat_map_less_key
	{
		bool operator() (const T& lhs, const T& rhs) const
		{
			return lhs.first < rhs.first;
		}
	};

	template<class T>
	struct flat_map_equal_key
	{
		bool operator() (const T& lhs, const T& rhs) const
		{
			return lhs.first == rhs.first;
		}
	};

	template<class T>
	struct flat_map_equal_key1
	{
		flat_map_equal_key1(const typename T::first_type& k) : m_key(k) {};

		bool operator() (const T& p) const
		{
			return p.first == m_key;
		}
		const typename T::first_type& m_key;
	};

	std::vector<pair_type> ar;
	bool m_bSorted;
};

template<typename key_type, typename val_type>
class flat_multimap
{
public:
	typedef std::pair<key_type, val_type> pair_type;
	typedef typename std::vector<pair_type >::iterator iterator;
	typedef std::pair<iterator, iterator> iterator_pair;

	flat_multimap() : m_bSorted(true) {};
#ifdef ENABLE_MOVE_SEMANTICS
	flat_multimap(const flat_multimap& rhs) = default;
	flat_multimap(flat_multimap&& rhs) NOEXCEPT{ swap(rhs); };
	flat_multimap &operator=(const flat_multimap &rhs) = default;	
#endif

	void clear();
	void reserve(size_t size);
	void insert(const pair_type &p);
	void insert(const key_type &k, const val_type &v);
	iterator find(const key_type &k);
	iterator lower_bound(const key_type &k);
	iterator upper_bound(const key_type &k);
	iterator_pair equal_range(const key_type &k);
	size_t count(const key_type &k);
	size_t size();
	bool empty();
	iterator erase(const key_type &k);
	iterator erase(iterator i0);
	iterator erase(iterator i0, iterator i1);
	void swap(flat_multimap& other) NOEXCEPT;

	iterator begin();
	iterator end();

	void sort();

private:
	template<class T>
	struct flat_multimap_less_key
	{
		bool operator() (const T& lhs, const T& rhs) const
		{
			return lhs.first < rhs.first;
		}
	};

	template<class T>
	struct flat_multimap_equal_key
	{
		bool operator() (const T& lhs, const T& rhs) const
		{
			return lhs.first == rhs.first;
		}
	};

	template<class T>
	struct flat_multimap_equal_key1
	{
		flat_multimap_equal_key1(const typename T::first_type& k) : m_key(k) {};

		bool operator() (const T& p) const
		{
			return p.first == m_key;
		}
		const typename T::first_type& m_key;
	};

	std::vector<pair_type> ar;
	int m_bSorted;
};

//------------------------------------- flat_map -----------------------------------------

template<typename key_type, typename val_type>
inline void flat_map<key_type, val_type>::clear()
{
	ar.clear();
	m_bSorted = true;
}

template<typename key_type, typename val_type>
inline void flat_map<key_type, val_type>::reserve(size_t size)
{
	ar.reserve(size);
}

template<typename key_type, typename val_type>
inline void flat_map<key_type, val_type>::insert(const pair_type &p)
{
	ar.push_back(p);
	m_bSorted = false;
}

template<typename key_type, typename val_type>
inline void flat_map<key_type, val_type>::insert(const key_type &k, const val_type &v)
{
	ar.push_back(std::make_pair(k, v));
	m_bSorted = false;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::find(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), p,
		flat_map_less_key<pair_type>());
	if (pit.first == pit.second)
		return ar.end();
	return pit.first;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::lower_bound(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator it = std::lower_bound(ar.begin(), ar.end(), p,
		flat_map_less_key<pair_type>());
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::upper_bound(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator it = std::upper_bound(ar.begin(), ar.end(), p,
		flat_map_less_key<pair_type>());
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator_pair flat_map<key_type, val_type>::equal_range(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), p,
		flat_map_less_key<pair_type>());
	return pit;
}

template<typename key_type, typename val_type>
inline size_t flat_map<key_type, val_type>::count(const key_type &k)
{
	pair_type p;
	p.first = k;
	bool b = std::binary_search(ar.begin(), ar.end(), p,
		flat_map_less_key<pair_type>());
	if (!b) return 0;
	return 1;
}

template<typename key_type, typename val_type>
inline size_t flat_map<key_type, val_type>::size()
{
	if (!m_bSorted) sort();
	return ar.size();
}

template<typename key_type, typename val_type>
inline bool flat_map<key_type, val_type>::empty()
{
	return ar.size()==0;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::erase(const key_type &k)
{
	flat_map_equal_key1<pair_type> pred(k);
	iterator it = ar.erase(std::remove_if(ar.begin(), ar.end(), pred));
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::erase(iterator i0)
{
	iterator it = ar.erase(i0);
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::erase(iterator i0, iterator i1)
{
	iterator it = ar.erase(i0, i1);
	return it;
}

template<typename key_type, typename val_type>
inline void flat_map<key_type, val_type>::swap(flat_map<key_type, val_type>& other) NOEXCEPT
{
	std::swap(m_bSorted, other.m_bSorted);
	std::swap(ar, other.ar);
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::begin()
{
	if (!m_bSorted) sort();
	return ar.begin();
}

template<typename key_type, typename val_type>
inline typename flat_map<key_type, val_type>::iterator flat_map<key_type, val_type>::end()
{
	if (!m_bSorted) sort();
	return ar.end();
}

template<typename key_type, typename val_type>
void inline flat_map<key_type, val_type>::sort(bool bPriorityFirstUnique /*= false*/)
{
	if (ar.size() < 2)
	{
		m_bSorted = true;
		return;
	}
	iterator i0 = ar.begin();
	iterator i1 = ar.end();
	std::stable_sort(i0, i1, flat_map_less_key<pair_type>());

	// reversing first and last unique values
	if (!bPriorityFirstUnique)
	{
		int nLast = static_cast<int>(ar.size()) - 1;
		int nFirstUnique = nLast;
		int nLastUnique = nLast;
		for (int i = nLast - 1; i >= 0; i--)
		{
			if (ar[i].first == ar[nLastUnique].first)
			{
				nLastUnique--;
				if (i == 0) std::swap(ar[nFirstUnique], ar[nLastUnique]);
				continue;
			}
			if (nFirstUnique != nLastUnique) std::swap(ar[nFirstUnique], ar[nLastUnique]);
			nFirstUnique = i;
			nLastUnique = i;
		}
	}
	ar.erase(std::unique(i0, i1, flat_map_equal_key<pair_type>()), i1);

	m_bSorted = true;
}

//----------------------------------- flat_multimap ---------------------------------------

template<typename key_type, typename val_type>
inline void flat_multimap<key_type, val_type>::clear()
{
	ar.clear();
	m_bSorted = true;
}

template<typename key_type, typename val_type>
inline void flat_multimap<key_type, val_type>::reserve(size_t size)
{
	ar.reserve(size);
}

template<typename key_type, typename val_type>
inline void flat_multimap<key_type, val_type>::insert(const pair_type &p)
{
	ar.push_back(p);
	m_bSorted = false;
}

template<typename key_type, typename val_type>
inline void flat_multimap<key_type, val_type>::insert(const key_type &k, const val_type &v)
{
	ar.push_back(std::make_pair(k, v));
	m_bSorted = false;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::find(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), p,
		flat_multimap<pair_type>());
	if (pit.first == pit.second)
		return ar.end();
	return pit.first;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::lower_bound(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator it = std::lower_bound(ar.begin(), ar.end(), p,
		flat_multimap_less_key<pair_type>());
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::upper_bound(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator it = std::upper_bound(ar.begin(), ar.end(), p,
		flat_multimap_less_key<pair_type>());
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator_pair flat_multimap<key_type, val_type>::equal_range(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), p,
		flat_multimap_less_key<pair_type>());
	return pit;
}


template<typename key_type, typename val_type>
inline size_t flat_multimap<key_type, val_type>::count(const key_type &k)
{
	if (!m_bSorted) sort();
	pair_type p;
	p.first = k;
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), p,
		flat_multimap_less_key<pair_type>());
	return std::distance(pit.first, pit.second);
}

template<typename key_type, typename val_type>
inline size_t flat_multimap<key_type, val_type>::size()
{
	return ar.size();
}

template<typename key_type, typename val_type>
inline bool flat_multimap<key_type, val_type>::empty()
{
	return ar.size()==0;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::erase(const key_type &k)
{
	flat_multimap_equal_key1<pair_type> pred(k);
	iterator it = ar.erase(std::remove_if(ar.begin(), ar.end(), pred), ar.end());
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::erase(iterator i0)
{
	iterator it = ar.erase(i0);
	return it;
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::erase(iterator i0, iterator i1)
{
	iterator it = ar.erase(i0, i1);
	return it;
}

template<typename key_type, typename val_type>
inline void flat_multimap<key_type, val_type>::swap(flat_multimap<key_type, val_type>& other) NOEXCEPT
{
	std::swap(m_bSorted, other.m_bSorted);
	std::swap(ar, other.ar);
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::begin()
{
	if (!m_bSorted) sort();
	return ar.begin();
}

template<typename key_type, typename val_type>
inline typename flat_multimap<key_type, val_type>::iterator flat_multimap<key_type, val_type>::end()
{
	if (!m_bSorted) sort();
	return ar.end();
}

template<typename key_type, typename val_type>
void inline flat_multimap<key_type, val_type>::sort()
{
	if (ar.size() < 2)
	{
		m_bSorted = true;
		return;
	}
	iterator i0 = ar.begin();
	iterator i1 = ar.end();
	std::stable_sort(i0, i1, flat_multimap_less_key<pair_type>());
	m_bSorted = true;
}

#ifdef ENABLE_MOVE_SEMANTICS
#undef ENABLE_MOVE_SEMANTICS
#endif
#undef NOEXCEPT

#endif // _FLAT_MAP_H_INCLUDED_2015_01_17
