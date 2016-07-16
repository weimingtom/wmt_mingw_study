// 编译命令行：
// g++ test002.cpp

// 演示STL algorithm functions（无修改算法）
//

#include <iostream>
#include <iterator>
#include <list>
#include <vector>
#include <ext/slist>
#include <functional>
//#include <ext/functional>
// for iota
#include <ext/numeric>
//有时用function.h更方便
//因为mingw的compose1和select1st并不在std内
//而是在__gnu_cxx内
//不过这样做会引入std名字空间
//#include <function.h>

bool IsOdd (int i) 
{
	return ((i%2)==1);
}

//非模板的congruent
bool congruent1(int& a, int& b)
{
	return (a - b) % 10 == 0;
}

//模板化的congruent1
template<class Integer>
struct congruent
{
	congruent(Integer mod):N(mod){}
	//小心这里有两个括号，别漏了，否则会出现
	//error: no match for call to `(congruent) (int&, int&)'
	//参数可以写成引用（更快）
	bool operator()(Integer a, Integer b) const 
	{
		return (a - b) % N == 0;
	}
	Integer N;
};

bool eq_nosign(int x, int y) 
{ 
	return abs(x) == abs(y); 
}

void lookup(int* first, int* last, size_t count, int val) 
{
	std::cout << "Searching for a sequence of "
		<< count
		<< " '" << val << "'"
		<< (count != 1 ? "s: " : ":  ");
	int* result = std::search_n(first, last, count, val);
	if (result == last)
		std::cout << "Not found" << std::endl;
	else
		std::cout << "Index = " << result - first << std::endl;
}

void lookup_nosign(int* first, int* last, size_t count, int val) 
{
	std::cout << "Searching for a (sign-insensitive) sequence of "
		<< count
		<< " '" << val << "'"
		<< (count != 1 ? "s: " : ":  ");
	int* result = std::search_n(first, last, count, val, eq_nosign);
	if (result == last)
		std::cout << "Not found" << std::endl;
	else
		std::cout << "Index = " << result - first << std::endl;
}

// 模拟std::ostream_iterator
template<class T> 
struct print : public std::unary_function<T, void>
{
	print(std::ostream& out) : os(out), count(0) {}
	void operator() (T x) 
	{ 
		os << x << ' '; ++count; 
	}
	std::ostream& os;
	int count;
};

int main(int argc, const char *argv[])
{
	std::cout << "Test STL algorithm functions (no motifications)" << std::endl;
	
	//find，用于得到[first, last)范围内第一个满足*i == value的位置
	{
		std::list<int> L;
		L.push_back(3);
		L.push_back(1);
		L.push_back(7);
		std::list<int>::iterator result = std::find(L.begin(), L.end(), 7);
		assert(result == L.end() || *result == 7);
		std::cout << "result == L.end() || *result == 7" << std::endl;
		std::cout << "*result == " << *result << std::endl;
	}
	
	{
		//find_if，用于得到[first, last)范围内第一个满足pred(*i) == true的位置
		int A[] = {-3, 0, 3, -2};
		const int N = sizeof(A) / sizeof(int);
		int *result2 = std::find_if(A, A+N,
			std::bind2nd(std::greater<int>(), 0));
		// 或者干脆用一个C函数作为条件
		//int *result2 = std::find_if(A, A+N, IsOdd);    
		assert(result2 == A + N || *result2 > 0);
		std::cout << "result2 == A + N || *result2 > 0" << std::endl;
		std::cout << "*result2 == " << *result2 << std::endl;	
		
		//更复杂的find_if：
		typedef std::pair<int, char*> Pair;
		std::vector<Pair> V;
		V.push_back(Pair(3, "A"));
		V.push_back(Pair(2, "B"));
		std::vector<Pair>::iterator p = 
			std::find_if(V.begin(), V.end(), 
			__gnu_cxx::compose1(std::bind2nd(std::equal_to<int>(), 2), 
				__gnu_cxx::select1st<Pair>()));
		std::cout << p->first << " , " << p->second << std::endl;
	}
	
	{
		//adjacent_find，用于相邻重复查找，即第一个出现重复的位置
		int A2[] = {1, 2, 3, 3, 4, 5};
		const int N2 = sizeof(A2) / sizeof(int);
		const int* p2 = std::adjacent_find(A2, A2 + N2);
		std::cout << "*p2 == " << *p2 << std::endl; 
		std::cout << "*(p2 + 1) == " << *(p2 + 1) << std::endl;
		
		//adjacent_find，还可用于判断递增性
		int A3[] = {1, 2, 3, 4, 6, 5, 7, 8};
		const int N3 = sizeof(A3) / sizeof(int);
		const int* p3 = std::adjacent_find(A3, A3 + N3, std::greater<int>());
		std::cout << "Element " << p3 - A3 << " is out of order: "
			 << *p3 << " > " << *(p3 + 1) << "." << std::endl;
	}
	
	//find_first_of，用于条件为范围的查找
	//例如查找字符串中的\t或\n或空格
	{
		const char* WS = "\t\n ";
		const int n_WS = strlen(WS);
		char* s1 = "This sentence contains five words.";
		char* s2 = "OneWord";
		char* end1 = std::find_first_of(s1, s1 + strlen(s1),
								 WS, WS + n_WS); 
		char* end2 = std::find_first_of(s2, s2 + strlen(s2),
								 WS, WS + n_WS); 
		printf("First word of s1: %.*s\n", end1 - s1, s1); 
		printf("First word of s2: %.*s\n", end2 - s2, s2); 
	}
	
	{
		//search，用于子串的查找
		const char S1[] = "Hello, world!";
		const char S2[] = "world";
		const int N1 = sizeof(S1) - 1;
		const int N2 = sizeof(S2) - 1;
		const char* p = std::search(S1, S1 + N1, S2, S2 + N2);
		printf("Found subsequence \"%s\" at character %d of sequence \"%s\".\n",
			 S2, p - S1, S1);
			 
		//search，还可以用于连续的匹配
		//下面是查找个位连续为1,2,3的整数的位置
		int A[10] = {23, 46, 81, 2, 43, 19, 14, 98, 72, 5};
		int digits[3] = {1, 2, 3};
		int *seq = std::search(A, A + 10, digits, digits + 3, congruent<int>(10));
		std::cout << "Subsequent: ";
		std::copy(seq, seq + 3, std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//find_end，类似于search，但它查找最后匹配的子串
		char* s = "executable.exe";
		char* suffix = "exe";
		const int N = strlen(s);
		const int N_suf = strlen(suffix);
		char* location = std::find_end(s, s + N, 
			suffix, suffix + N_suf);
		if (location != s + N) 
		{
			std::cout << "Found a match for " << suffix << " within " << s << std::endl;
			std::cout << s << std::endl;
			int i;
			for (i = 0; i < (location - s); ++i)
				std::cout << ' ';
			for (i = 0; i < N_suf; ++i)
				std::cout << '^';
			std::cout << std::endl;
		}
		else
			std::cout << "No match for " << suffix << " within " << s << std::endl;
	}
	
	{
		//search_n，用于连续n次出现的子串匹配
		const int N = 10;
		int A[N] = {1, 2, 1, 1, 3, -3, 1, 1, 1, 1};
		//连续出现4个1的序列
		lookup(A, A+N, 4, 1);
		//连续出现2个绝对值为3的序列
		lookup_nosign(A, A+N, 2, 3);
	}
	
	{
		//count，用于计算某元素的个数
		int A[] = { 2, 0, 4, 6, 0, 3, 1, -7 };
		const int N = sizeof(A) / sizeof(int);
		std::cout << "Number of zeros: " 
			<< std::count(A, A + N, 0)
			<< std::endl;
	}
	
	{
		//count_if，用于计算满足某个条件的元素个数
		//下面是计算偶数的个数
		int A[] = { 2, 0, 4, 6, 0, 3, 1, -7 };
		const int N = sizeof(A) / sizeof(int);
		std::cout << "Number of even elements: " 
			<< std::count_if(A, A + N,
				__gnu_cxx::compose1(std::bind2nd(std::equal_to<int>(), 0), 
				std::bind2nd(std::modulus<int>(), 2)))
			<< std::endl;
	}
	
	{
		//for_each，用于以只读的函数遍历所有元素
		int A[] = {1, 4, 2, 8, 5, 7};
		const int N = sizeof(A) / sizeof(int);
		//最后的参数还可以直接传入一个C函数的指针，参数是容器的元素类型
		//这里的print<int>()是带状态的
		print<int> P = std::for_each(A, A + N, print<int>(std::cout));
		//利用返回值取出函数对象的状态值
		std::cout << std::endl << P.count << " objects printed." << std::endl;
	}
	
	{
		//equal，用于判断序列是否相等，true或false
		int A1[] = { 3, 1, 4, 1, 5, 9, 3 };
		int A2[] = { 3, 1, 4, 2, 8, 5, 7 };
		const int N = sizeof(A1) / sizeof(int);
		std::cout << "Result of comparison: " << (std::equal(A1, A1 + N, A2) ? "true" : "false") << std::endl;
	}
	
	{
		//mismatch，用于判断两个序列出现首个不同的位置
		int A1[] = { 3, 1, 4, 1, 5, 9, 3 };
		int A2[] = { 3, 1, 4, 2, 8, 5, 7 };
		const int N = sizeof(A1) / sizeof(int);
		std::pair<int*, int*> result = std::mismatch(A1, A1 + N, A2);
		std::cout << "The first mismatch is in position " << result.first - A1 << std::endl;
		std::cout << "Values are: " << *(result.first) << ", " << *(result.second) << std::endl;
	}
	
	{
		//lexicographical_compare，用于字典序列比较
		int A1[] = {3, 1, 4, 1, 5, 9, 3};
		int A2[] = {3, 1, 4, 2, 8, 5, 7};
		int A3[] = {1, 2, 3, 4};
		int A4[] = {1, 2, 3, 4, 5};
		const int N1 = sizeof(A1) / sizeof(int);
		const int N2 = sizeof(A2) / sizeof(int);
		const int N3 = sizeof(A3) / sizeof(int);
		const int N4 = sizeof(A4) / sizeof(int);
		bool C12 = std::lexicographical_compare(A1, A1 + N1, A2, A2 + N2);
		bool C34 = std::lexicographical_compare(A3, A3 + N3, A4, A4 + N4);
		std::cout << "A1[] < A2[]: " << (C12 ? "true" : "false") << std::endl;
		std::cout << "A3[] < A4[]: " << (C34 ? "true" : "false") << std::endl;
	}
	
	{
		//min/max，用于较小值/较大值
		const int x_min = std::min(3, 9);
		const int x_max = std::max(3, 9);
		std::cout << "x_min == " << x_min << std::endl;
		std::cout << "x_max == " << x_max << std::endl;
	}
	
	{
		//min_element/max_element，用于求一个序列的最小值/最大值
		std::list<int> L;
		std::generate_n(std::front_inserter(L), 1000, rand);
		std::list<int>::const_iterator it = std::min_element(L.begin(), L.end());
		std::list<int>::const_iterator it2 = std::max_element(L.begin(), L.end());
		std::cout << "The smallest element is " << *it << std::endl;
		std::cout << "The largest element is " << *it2 << std::endl;
	}
	
	return 0;
}
