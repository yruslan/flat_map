#ifndef _FLAT_SET_H_INCLUDED_2015_01_19
#define _FLAT_SET_H_INCLUDED_2015_01_19

#include <vector>
#include <algorithm>

/* 
 * Minimalistic set C++ template based on vector
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

template<typename T>
class flat_set
{
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef std::pair<iterator, iterator> iterator_pair;

	flat_set() : m_bSorted(true) {};
#ifdef ENABLE_MOVE_SEMANTICS
	flat_set(const flat_set& rhs) = default;
	flat_set(flat_set&& rhs) NOEXCEPT { swap(rhs); };
	flat_set &operator=(const flat_set &rhs) = default;	
#endif

	void clear();
	void reserve(size_t size);
	void insert(const T &p);
	iterator find(const T &k);
	iterator lower_bound(const T &k);
	iterator upper_bound(const T &k);
	iterator_pair equal_range(const T &k);
	size_t count(const T &k);
	size_t size();
	bool empty();
	iterator erase(const T &k);
	iterator erase(iterator i0);
	iterator erase(iterator i0, iterator i1);
	void swap(flat_set& other) NOEXCEPT;

	iterator begin();
	iterator end();

	void sort(bool bPriorityFirstUnique = false);

private:
	std::vector<T> ar;
	bool m_bSorted;
};

template<typename T>
class flat_multiset
{
public:
	typedef typename std::vector<T>::iterator iterator;
	typedef std::pair<iterator, iterator> iterator_pair;

	flat_multiset() : m_bSorted(true) {};
#ifdef ENABLE_MOVE_SEMANTICS
	flat_multiset(const flat_multiset& rhs) = default;
	flat_multiset(flat_multiset&& rhs) NOEXCEPT{ swap(rhs); };
	flat_multiset &operator=(const flat_multiset &rhs) = default;	
#endif

	void clear();
	void reserve(size_t size);
	void insert(const T &p);
	iterator find(const T &k);
	iterator lower_bound(const T &k);
	iterator upper_bound(const T &k);
	iterator_pair equal_range(const T &k);
	size_t count(const T &k);
	size_t size();
	bool empty();
	iterator erase(const T &k);
	iterator erase(iterator i0);
	iterator erase(iterator i0, iterator i1);
	void swap(flat_multiset& other) NOEXCEPT;

	iterator begin();
	iterator end();

	void sort();

private:
	std::vector<T> ar;
	bool m_bSorted;
};


//------------------------------------- flat_set -----------------------------------------

template<typename T>
inline void flat_set<T>::clear()
{
	ar.clear();
	m_bSorted = true;
}

template<typename T>
inline void flat_set<T>::reserve(size_t size)
{
	ar.reserve(size);
}

template<typename T>
inline void flat_set<T>::insert(const T &v)
{
	ar.push_back(v);
	m_bSorted = false;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::find(const T &v)
{
	if (!m_bSorted) sort();
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), v);
	if (pit.first == pit.second)
		return ar.end();
	return pit.first;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::lower_bound(const T &v)
{
	if (!m_bSorted) sort();
	iterator it = std::lower_bound(ar.begin(), ar.end(), v);
	return it;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::upper_bound(const T &v)
{
	if (!m_bSorted) sort();
	iterator it = std::upper_bound(ar.begin(), ar.end(), v);
	return it;
}

template<typename T>
inline typename flat_set<T>::iterator_pair flat_set<T>::equal_range(const T &v)
{
	if (!m_bSorted) sort();
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), v);
	return pit;
}

template<typename T>
inline size_t flat_set<T>::count(const T &v)
{
	if (!m_bSorted) sort();
	bool b = std::binary_search(ar.begin(), ar.end(), v);
	//bool b = std::binary_search(&(*ar.begin()), &(*ar.begin())+ar.size(), v); // binary_search() on C arrays is faster then on vector
	if (!b) return 0;
	return 1;
}

template<typename T>
inline size_t flat_set<T>::size()
{
	if (!m_bSorted) sort();
	return ar.size();
}

template<typename T>
inline bool flat_set<T>::empty()
{
	return ar.size()==0;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::erase(const T &v)
{
	iterator it = ar.erase(std::remove(ar.begin(), ar.end(), v));
	return it;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::erase(iterator i0)
{
	iterator it = ar.erase(i0);
	return it;
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::erase(iterator i0, iterator i1)
{
	iterator it = ar.erase(i0, i1);
	return it;
}

template<typename T>
inline void flat_set<T>::swap(flat_set<T>& other) NOEXCEPT
{
	std::swap(m_bSorted, other.m_bSorted);
	std::swap(ar, other.ar);
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::begin()
{
	if (!m_bSorted) sort();
	return ar.begin();
}

template<typename T>
inline typename flat_set<T>::iterator flat_set<T>::end()
{
	if (!m_bSorted) sort();
	return ar.end();
}

template<typename T>
void inline flat_set<T>::sort(bool bPriorityFirstUnique /*= false*/)
{
	if (ar.size() < 2)
	{
		m_bSorted = true;
		return;
	}
	iterator i0 = ar.begin();
	iterator i1 = ar.end();
	std::stable_sort(i0, i1);

	// reversing first and last unique values
	if (!bPriorityFirstUnique)
	{
		int nLast = static_cast<int>(ar.size()) - 1;
		int nFirstUnique = nLast;
		int nLastUnique = nLast;
		for (int i = nLast - 1; i >= 0; i--)
		{
			if (ar[i] == ar[nLastUnique])
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
	ar.erase(std::unique(i0, i1), i1);

	m_bSorted = true;
}

//------------------------------------- flat_multiset -----------------------------------------

template<typename T>
inline void flat_multiset<T>::clear()
{
	ar.clear();
	m_bSorted = true;
}

template<typename T>
inline void flat_multiset<T>::reserve(size_t size)
{
	ar.reserve(size);
}

template<typename T>
inline void flat_multiset<T>::insert(const T &v)
{
	ar.push_back(v);
	m_bSorted = false;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::find(const T &v)
{
	if (!m_bSorted) sort();
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), v);
	if (pit.first == pit.second)
		return ar.end();
	return pit.first;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::lower_bound(const T &v)
{
	if (!m_bSorted) sort();
	iterator it = std::lower_bound(ar.begin(), ar.end(), v);
	return it;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::upper_bound(const T &v)
{
	if (!m_bSorted) sort();
	iterator it = std::upper_bound(ar.begin(), ar.end(), v);
	return it;
}

template<typename T>
inline typename flat_multiset<T>::iterator_pair flat_multiset<T>::equal_range(const T &v)
{
	if (!m_bSorted) sort();
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), v);
	return pit;
}

template<typename T>
inline size_t flat_multiset<T>::count(const T &v)
{
	if (!m_bSorted) sort();
	iterator_pair pit = std::equal_range(ar.begin(), ar.end(), v);
	return std::distance(pit.first, pit.second);
}

template<typename T>
inline size_t flat_multiset<T>::size()
{
	return ar.size();
}

template<typename T>
inline bool flat_multiset<T>::empty()
{
	return ar.size()==0;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::erase(const T &v)
{
	iterator it = ar.erase(std::remove(ar.begin(), ar.end(), v), ar.end());
	return it;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::erase(iterator i0)
{
	iterator it = ar.erase(i0);
	return it;
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::erase(iterator i0, iterator i1)
{
	iterator it = ar.erase(i0, i1);
	return it;
}

template<typename T>
inline void flat_multiset<T>::swap(flat_multiset<T>& other) NOEXCEPT
{
	std::swap(m_bSorted, other.m_bSorted);
	std::swap(ar, other.ar);
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::begin()
{
	if (!m_bSorted) sort();
	return ar.begin();
}

template<typename T>
inline typename flat_multiset<T>::iterator flat_multiset<T>::end()
{
	if (!m_bSorted) sort();
	return ar.end();
}

template<typename T>
void inline flat_multiset<T>::sort()
{
	if (ar.size() < 2)
	{
		m_bSorted = true;
		return;
	}
	iterator i0 = ar.begin();
	iterator i1 = ar.end();
	std::sort(i0, i1);
	m_bSorted = true;
}

#ifdef ENABLE_MOVE_SEMANTICS
#undef ENABLE_MOVE_SEMANTICS
#endif
#undef NOEXCEPT

#endif // _FLAT_SET_H_INCLUDED_2015_01_19
