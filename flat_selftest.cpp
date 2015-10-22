#include "flat_set.h"
#include "flat_map.h"
#include <stdio.h>

/*
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

static int nTestNum;

#define TEST(a) { nTestNum ++; \
	if (!(a)) {printf("Test %d FAILED! (%s)\n", nTestNum, #a); return nTestNum; } }

int flat_test()
{
	nTestNum = 0;

	flat_map<int, double> map1;

	map1.insert(10, 1.);
	map1.insert(10, -999.99);
	map1.insert(11, 100.);
	map1.insert(12, 29.);
	map1.insert(13, 66.);
	map1.insert(14, 44.);
	map1.insert(12, 40.);
	map1.insert(12, 50.);

	TEST(map1.size() == 5);
	TEST(map1.count(12) == 1);

	int nCount = 0;
	{
		for (flat_map<int, double>::iterator it = map1.begin(); it != map1.end(); it++)
		{
			//printf(" ( %d, %g )\n", it->first, it->second);
			nCount++;
		}
	}
	TEST(nCount == 5);

	map1.erase(11);

	TEST(map1.size() == 4);

	flat_map<int, double>::iterator it2 = map1.find(12);
	TEST(it2 != map1.end());
	TEST(it2->first == 12);
	TEST(it2->second == 50);

	flat_multimap<int, double> map2;
	map2.insert(10, 1.);
	map2.insert(10, -999.99);
	map2.insert(11, 100.);
	map2.insert(12, 29.);
	map2.insert(13, 66.);
	map2.insert(14, 44.);
	map2.insert(12, 40.);
	map2.insert(12, 50.);

	TEST(map2.size() == 8);
	TEST(map2.count(12) == 3);
	map2.erase(11);
	TEST(map2.size() == 7);

	flat_multimap<int, double>::iterator_pair itp = map2.equal_range(12);
	nCount = 0;
	while (itp.first != itp.second)
	{
		nCount++;
		++itp.first;
	}
	TEST(nCount == 3);

	flat_set<int> set1;

	set1.insert(33);
	set1.insert(22);
	set1.insert(11);
	set1.insert(55);
	set1.insert(1);
	set1.insert(100);
	set1.insert(100);

	TEST(set1.size() == 6);

	nCount = 0;
	{
		for (flat_set<int>::iterator it = set1.begin(); it != set1.end(); it++)
		{
			nCount++;
			//printf(" ( %d )\n", *it);
		}
	}
	TEST(nCount == 6);

	set1.erase(22);
	TEST(set1.size() == 5);
	flat_set<int>::iterator it_set1 = set1.find(22);
	TEST(it_set1 == set1.end());
	it_set1 = set1.find(11);
	TEST(it_set1 != set1.end());
	TEST(*it_set1 == 11);

	flat_multiset<int> set2;

	set2.insert(33);
	set2.insert(22);
	set2.insert(11);
	set2.insert(11);
	set2.insert(55);
	set2.insert(1);
	set2.insert(11);
	set2.insert(100);
	set2.insert(100);
	set2.insert(100);
	set2.insert(1);

	TEST(set2.size() == 11);
	set2.erase(11);
	TEST(set2.size() == 8);

	TEST(set2.count(100) == 3);

	nCount = 0;
	{
		for (flat_multiset<int>::iterator it = set2.begin(); it != set2.end(); it++)
		{
			nCount++;
		}
	}
	TEST(nCount == 8);

	flat_multiset<int>::iterator_pair itset12 = set2.equal_range(11);
	TEST(itset12.first == itset12.second);
	itset12 = set2.equal_range(100);
	TEST(itset12.first != set2.end());
	TEST(*itset12.first == 100);

	int nDist = static_cast<int>(itset12.second - itset12.first);
	TEST(nDist == 3);
	nCount = 0;
	while (itset12.first != itset12.second)
	{
		//printf(" *( %d )\n", *itset12.first);
		nCount++;
		++itset12.first;
	}
	TEST(nCount == 3);

	flat_multiset<int> set22;
	set22.swap(set2);

	TEST(set2.empty());
	TEST(set22.size()==8);

	//flat_multiset<int> set23(std::move(set22));
	//TEST(set23.size() == 8);

	return 0;
}

int main (int argc, char **argv)
{
	int fi = flat_test();
	if (fi != 0)
	{
		printf("ftal_test() failed at test #%d\n", fi);
		return -1;
	}
	return 0;
}
