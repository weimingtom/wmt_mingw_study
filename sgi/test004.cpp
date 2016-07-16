// 编译命令行：
// g++ test004.cpp

// 演示STL排序/查找算法

// for cout
#include <iostream>
// for ostream_iterator
#include <iterator>
//函数对象
#include <functional>
//算法
#include <algorithm>
// for string
#include <string>
// for vector
#include <vector>
// for is_sorted
#include<ext/algorithm>

inline bool lt_nocase(char c1, char c2) 
{ 
	return tolower(c1) < tolower(c2); 
}

//分治法排序，即stable_sort（经修改）
template<class BidirectionalIter>
void mergesort(BidirectionalIter first, BidirectionalIter last)
{
	typename std::iterator_traits<BidirectionalIter>::difference_type n = 
		std::distance(first, last);
	if(n == 0 || n == 1)
		return ;
	else
	{
		BidirectionalIter mid = first + n / 2;
		mergesort(first, mid);
		mergesort(mid, last);
		std::inplace_merge(first, mid, last);
	}
}

int main(int argc, const char *argv[])
{
	std::cout << "Test STL sort functions" << std::endl;
	
	{
		//sort，用于非稳定排序
		//不保证等价元素（即x既不小于y，y也不小于x）的相对次序
		int A[] = {1, 4, 2, 8, 5, 7};
		const int N = sizeof(A) / sizeof(int);
		//从小到大排序
		std::sort(A, A + N);
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;	
		
		//判断是否排序成功
		assert(__gnu_cxx::is_sorted(A, A + N));
		
		//还可以接受第三个参数，以判断大小或指定排序的顺序
		//下面从大到小对字符串排序
		const char *A2[] = {"apple", "banana", "pear"};
		const int N2 = sizeof(A2) / sizeof(const char *);
		std::sort(A2, A2 + N2, std::greater<std::string>());
		//or
		//std::sort(A2, A2 + N2, std::greater<const char*>());
		std::copy(A2, A2 + N2,
			std::ostream_iterator<const char*>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//stable_sort，用于稳定排序
		//保证等价元素（即x既不小于y，y也不小于x）的相对次序
		char A[] = "fdBeACFDbEac";
		const int N = sizeof(A) - 1;
		std::stable_sort(A, A+N, lt_nocase);
		std::cout << A << std::endl;
	}
	
	{
		//partial_sort/partial_sort_copy，用于原地或容器间复制的局部排序
		//即把某个区间的前n个最小元素排序后移到区间左边
		//如果第二和第三参数相同，则等价于sort
		//即partial_sort(A, A+N, A+N)相当于sort(A, A+N)
		int A[] = {7, 2, 6, 11, 9, 3, 12, 10, 8, 4, 1, 5};
		const int N = sizeof(A) / sizeof(int);
		//只对最小的前5个数排序
		std::partial_sort(A, A + 5, A + N);
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout <<std::endl;
		
		std::vector<int> V(4);
		//只对最小的前4个数排序
		std::partial_sort_copy(A, A + N, V.begin(), V.end());
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout <<std::endl;
	}
	
	{
		//nth_element，用于把某个区间的前n个最小元素不排序，但移到区间左边
		int A[] = {7, 2, 6, 11, 9, 3, 12, 10, 8, 4, 1, 5};
		const int N = sizeof(A) / sizeof(int);
		//前6个最小值
		std::nth_element(A, A + 6, A + N);
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//is_sorted，用于判断序列是否已经被排序
		int A[] = {1, 4, 2, 8, 5, 7};
		const int N = sizeof(A) / sizeof(int);
		assert(!__gnu_cxx::is_sorted(A, A + N));
		std::cout << "A is not sorted" << std::endl;
		std::sort(A, A + N);
		assert(__gnu_cxx::is_sorted(A, A + N));
		std::cout << "A is sorted" << std::endl;
	}
	
	{
		//binary_search，用于判断已排序序列内是否存在i，用二分法查找
		//基于已排序序列
		int A[] = { 1, 2, 3, 3, 3, 5, 8 };
		const int N = sizeof(A) / sizeof(int);
		for (int i = 1; i <= 10; ++i) 
		{
			std::cout << "Searching for " << i << ": "
				<< (std::binary_search(A, A + N, i) ? "present" : "not present") 
				<< std::endl;
		}
	}
	
	{
		//lower_bound/upper_bound/equal_range，用于找到不破坏原有顺序的第一个/最后一个插入位置
		//（其中equal同时计算两个值）
		//基于已排序序列
		int A[] = { 1, 2, 3, 3, 3, 5, 8 };
		const int N = sizeof(A) / sizeof(int);
		for (int i = 1; i <= 10; ++i) 
		{
			int* p = std::lower_bound(A, A + N, i);
			std::cout << "Searching for " << i << ".  ";
			std::cout << "Result: index = " << p - A << ", ";
			if (p != A + N)
				std::cout << "A[" << p - A << "] == " << *p << std::endl;
			else
				std::cout << "which is off-the-end." << std::endl;
		}
		
		for (int i2 = 1; i2 <= 10; ++i2) 
		{
			int* p = std::upper_bound(A, A + N, i2);
			std::cout << "Searching for " << i2 << ".  ";
			std::cout << "Result: index = " << p - A << ", ";
			if (p != A + N)
				std::cout << "A[" << p - A << "] == " << *p << std::endl;
			else
				std::cout << "which is off-the-end." << std::endl;
		}
		
		int A3[] = { 1, 2, 3, 3, 3, 5, 8 };
		const int N3 = sizeof(A) / sizeof(int);
		for (int i3 = 2; i3 <= 4; ++i3) 
		{
			std::pair<int*, int*> result = std::equal_range(A3, A3 + N3, i3);
			std::cout << std::endl;
			std::cout << "Searching for " << i3 << std::endl;
			std::cout << "  First position where " << i3 << " could be inserted: "
				<< result.first - A3 << std::endl;
			std::cout << "  Last position where " << i3 << " could be inserted: "
				<< result.second - A3 << std::endl;
			if (result.first < A3 + N3)
				std::cout << "  *result.first = " << *result.first << std::endl;
			if (result.second < A3 + N3)
				std::cout << "  *result.second = " << *result.second << std::endl;
		}
	}
	
	{
		//merge，用于合并两个已排序序列成为一个排序序列
		int A1[] = { 1, 3, 5, 7 };
		int A2[] = { 2, 4, 6, 8 };
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int);
		std::merge(A1, A1 + N1, A2, A2 + N2, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//inplace_merge，用于对同一序列的两个已排序区间进行合并，成为一个排序序列
		int A[] = { 1, 3, 5, 7, 2, 4, 6, 8 };
		std::inplace_merge(A, A + 4, A + 8);
		std::copy(A, A + 8, 
			std::ostream_iterator<int>(std::cout, " "));  
		std::cout << std::endl;
		
		//使用inplace_merge实现分治法排序
		int A2[] = { 1, 3, 5, 7, 2, 4, 6, 8 }; 
		const int N2 = sizeof(A2) / sizeof(int);
		mergesort(A2, A2 + N2);
		std::cout << "inplace_merge sort : " << std::endl;
		std::copy(A2, A2 + N2, 
			std::ostream_iterator<int>(std::cout, " "));  
		std::cout << std::endl;
	}
	
	{
		//includes，用于判断一个排序序列是否包含在另一个排序序列中
		int A1[] = { 1, 2, 3, 4, 5, 6, 7 };
		int A2[] = { 1, 4, 7 };
		int A3[] = { 2, 7, 9 };
		int A4[] = { 1, 1, 2, 3, 5, 8, 13, 21 };
		int A5[] = { 1, 2, 13, 13 };
		int A6[] = { 1, 1, 3, 21 };
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int);
		const int N3 = sizeof(A3) / sizeof(int);
		const int N4 = sizeof(A4) / sizeof(int);
		const int N5 = sizeof(A5) / sizeof(int);
		const int N6 = sizeof(A6) / sizeof(int);
		std::cout << "A2 contained in A1: " 
			<< (std::includes(A1, A1 + N1, A2, A2 + N2) ? "true" : "false") 
			<< std::endl;
		std::cout << "A3 contained in A1: " 
			<< (std::includes(A1, A1 + N2, A3, A3 + N3) ? "true" : "false") 
			<< std::endl;
		std::cout << "A5 contained in A4: " 
			<< (std::includes(A4, A4 + N4, A5, A5 + N5) ? "true" : "false") 
			<< std::endl;
		std::cout << "A6 contained in A4: " 
			<< (std::includes(A4, A4 + N4, A6, A6 + N6) ? "true" : "false") 
			<< std::endl;
	}
	
	{
		//set_union，用于求两个排序序列的并集
		int A1[] = {1, 3, 5, 7, 9, 11};
		int A2[] = {1, 1, 2, 3, 5, 8, 13};  
		char A3[] = {'a', 'b', 'B', 'B', 'f', 'H'};
		char A4[] = {'A', 'B', 'b', 'C', 'D', 'F', 'F', 'h', 'h'};
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int); 
		const int N3 = sizeof(A3);
		const int N4 = sizeof(A4);
		std::cout << "Union of A1 and A2: ";
		std::set_union(A1, A1 + N1, A2, A2 + N2,
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl 
			<< "Union of A3 and A4: ";
		std::set_union(A3, A3 + N3, A4, A4 + N4, 
			std::ostream_iterator<char>(std::cout, " "),
			lt_nocase);
		std::cout << std::endl;
	}
	
	{
		//set_intersection，用于求两个排序序列的交集
		int A1[] = {1, 3, 5, 7, 9, 11};
		int A2[] = {1, 1, 2, 3, 5, 8, 13};  
		char A3[] = {'a', 'b', 'b', 'B', 'B', 'f', 'h', 'H'};
		char A4[] = {'A', 'B', 'B', 'C', 'D', 'F', 'F', 'H' };
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int); 
		const int N3 = sizeof(A3);
		const int N4 = sizeof(A4);
		std::cout << "Intersection of A1 and A2: ";
		std::set_intersection(A1, A1 + N1, A2, A2 + N2,
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl 
		   << "Intersection of A3 and A4: ";
		std::set_intersection(A3, A3 + N3, A4, A4 + N4, 
			std::ostream_iterator<char>(std::cout, " "),
			lt_nocase);
		std::cout << std::endl;
	}
	
	{
		//set_difference，用于求两个排序序列的差集
		int A1[] = {1, 3, 5, 7, 9, 11};
		int A2[] = {1, 1, 2, 3, 5, 8, 13};  
		char A3[] = {'a', 'b', 'b', 'B', 'B', 'f', 'g', 'h', 'H'};
		char A4[] = {'A', 'B', 'B', 'C', 'D', 'F', 'F', 'H' };
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int); 
		const int N3 = sizeof(A3);
		const int N4 = sizeof(A4);
		std::cout << "Difference of A1 and A2: ";
		std::set_difference(A1, A1 + N1, A2, A2 + N2,
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl 
			<< "Difference of A3 and A4: ";
		std::set_difference(A3, A3 + N3, A4, A4 + N4, 
			std::ostream_iterator<char>(std::cout, " "),
			lt_nocase);
		std::cout << std::endl;
	}
	
	{
		//set_symmetric_difference，用于求两个排序序列的对称差集
		int A1[] = {1, 3, 5, 7, 9, 11};
		int A2[] = {1, 1, 2, 3, 5, 8, 13};  
		char A3[] = {'a', 'b', 'b', 'B', 'B', 'f', 'g', 'h', 'H'};
		char A4[] = {'A', 'B', 'B', 'C', 'D', 'F', 'F', 'H' };
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int); 
		const int N3 = sizeof(A3);
		const int N4 = sizeof(A4);
		std::cout << "Symmetric difference of A1 and A2: ";
		std::set_symmetric_difference(A1, A1 + N1, A2, A2 + N2,
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl 
			<< "Symmetric difference of A3 and A4: ";
		std::set_symmetric_difference(A3, A3 + N3, A4, A4 + N4, 
			std::ostream_iterator<char>(std::cout, " "),
			lt_nocase);
		std::cout << std::endl;
	}
	
	{
		//make_heap/sort_heap/is_heap，用于把一个序列变成堆/进行堆排序/判断是否已经变成堆
		int A[] = {1, 4, 2, 8, 5, 7};
		const int N = sizeof(A) / sizeof(int);
		assert(!__gnu_cxx::is_heap(A, A+N));
		std::make_heap(A, A+N);
		assert(__gnu_cxx::is_heap(A, A+N));
		std::copy(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::sort_heap(A, A+N);
		std::copy(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//push_heap/pop_heap，用于扩大（压入新的值）/缩小(把最大值移到区间外)堆的范围
		//输入序列必须是堆
		int A[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::make_heap(A, A + 9);
		std::cout << "[A, A + 9)  = ";
		std::copy(A, A + 9, 
			std::ostream_iterator<int>(std::cout, " "));  
		std::push_heap(A, A + 10);
		std::cout << std::endl << "[A, A + 10) = ";
		std::copy(A, A + 10, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		int A2[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
		std::make_heap(A2, A2 + 10);
		std::cout << "[A2, A2 + 10) = ";
		std::copy(A2, A2 + 10, 
			std::ostream_iterator<int>(std::cout, " "));  		
		std::pop_heap(A2, A2 + 10);
		std::cout << std::endl << "[A2, A2 + 9)  = ";
		std::copy(A2, A2 + 9, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	return 0;
}
