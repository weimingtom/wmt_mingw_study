// 编译命令行：
// g++ test003.cpp

// 演示STL algorithm functions (motifications)，
// 包括部分数学运算算法
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

//for random_sample
#include <ext/algorithm>

struct string_length_exceeds
{
	string_length_exceeds(int i):N(i){}
	int N;
	bool operator()(const char * str) const
	{
		return strlen(str) > N;
	}
};

inline bool eq_nocase(char c1, char c2) 
{ 
	return tolower(c1) == tolower(c2); 
}

inline bool lt_nocase(char c1, char c2) 
{ 
	return tolower(c1) < tolower(c2); 
}

struct eq_div
{
	eq_div(int i):N(i){}
	int N;
	//这里有两个括号
	bool operator()(int a, int b) const 
	{ 
		return (a/N) == (b/N); 
	}
};

template <class BidirectionalIterator> 
void snail_sort(BidirectionalIterator first, BidirectionalIterator last)
{
	//循环比较，执行O(n!)排序法，直至排序结束，回滚到第一个序列
	//（即返回最小的那个序列）
	//这个函数还可接受第三个参数（比较规则函数）
	while (std::next_permutation(first, last)) 
	{
		
	}
}

int main(int argc, const char *argv[])
{
	std::cout << "Test STL algorithm functions (motifications)" << std::endl;
	
	//33
	{
		//copy，用于复制序列（即使容器不同类）（指定目标区间的左范围）
		std::vector<int> V(5);
		//生成一个相对于value的增一序列
		__gnu_cxx::iota(V.begin(), V.end(), 1);
		std::list<int> L(V.size());
		std::copy(V.begin(), V.end(), L.begin());
		assert(std::equal(V.begin(), V.end(), L.begin()));
		std::cout << "V == L" << std::endl;
			
		//copy，还可用于输出
		char A[] = "Hello";
		std::vector<char> V2(A, A + strlen(A));
		__gnu_cxx::slist<char> L2(V2.size());
		std::copy(V2.begin(), V2.end(), L2.begin());
		assert(std::equal(V2.begin(), V2.end(), L2.begin()));
		
		std::vector<char> V3;
		//V3没有元素，需要使用back_inserter
		std::copy(V2.begin(), V2.end(), std::back_inserter(V3));
		//以int的形式输出到控制台
		copy(V3.begin(), V3.end(),
			std::ostream_iterator<int>(std::cout, "\n"));
	}
	
	{
		//copy_backward，类似于copy，用于指定目标区间的右范围的复制
		//把[first, last)复制到[result - (last - first), result)
		std::vector<int> V(15);
		__gnu_cxx::iota(V.begin(), V.end(), 1);
		std::copy_backward(V.begin(), V.begin() + 10, V.begin() + 15);
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//swap，用于交换两个变量的值
		int x = 1;
		int y = 2;
		std::cout << x << "," << y << std::endl;
		std::swap(x, y);
		std::cout << x << "," << y << std::endl;
	}

	{
		//iter_swap，用于交换两个指针对应变量的值
		int x = 1;
		int y = 2;
		std::cout << x << "," << y << std::endl;
		std::iter_swap(&x, &y);
		std::cout << x << "," << y << std::endl;
	}
	
	{
		//swap_ranges，用于交换两个容器的区间
		std::vector<int> V1, V2;
		V1.push_back(1);
		V1.push_back(2);
		V2.push_back(3);
		V2.push_back(4);
		std::cout << "before swap_ranges:"<< std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V2.begin(), V2.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::swap_ranges(V1.begin(), V1.end(), V2.begin());
		std::cout << "after swap_ranges:"<< std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V2.begin(), V2.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//transform用于原地变换或不同容器的变换
		//对序列取负
		const int N = 10;
		double A[N];
		__gnu_cxx::iota(A, A+N, 1);
		std::cout << "before transform:"<< std::endl;
		std::copy(A, A + N,
			std::ostream_iterator<double>(std::cout, " "));
		std::cout << std::endl;
		std::transform(A, A+N, A, std::negate<double>());
		std::cout << "after transform:"<< std::endl;
		std::copy(A, A + N,
			std::ostream_iterator<double>(std::cout, " "));
		std::cout << std::endl;
		
		// V3 = V1 + V2
		//const int N = 10;
		std::vector<int> V1(N);
		std::vector<int> V2(N);
		std::vector<int> V3(N);
		__gnu_cxx::iota(V1.begin(), V1.end(), 1);
		std::fill(V2.begin(), V2.end(), 75);
		assert(V2.size() >= V1.size() && V3.size() >= V1.size());
		std::transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),
			std::plus<int>());
		std::cout << "V1, V2, V3, ostream_iterator:"<< std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V2.begin(), V2.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V3.begin(), V3.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		//直接输出到控制台
		std::transform(V1.begin(), V1.end(), V2.begin(),
			std::ostream_iterator<int>(std::cout, " "),
			std::plus<int>());
		std::cout << std::endl;
	}
	
	{
		//replace，用于原地替换
		std::vector<int> V;
		V.push_back(1);
		V.push_back(2);
		V.push_back(3);
		V.push_back(1);
		std::cout << "befor replace:" << std::endl;
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		//把1换成99
		std::replace(V.begin(), V.end(), 1, 99);
		std::cout << "after replace:" << std::endl;
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//replace_if，用于原地条件替换
		std::vector<int> V;
		V.push_back(1);
		V.push_back(-3);
		V.push_back(2);
		V.push_back(-1);
		std::cout << "before replace_if:" << std::endl;
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::replace_if(V.begin(), V.end(), std::bind2nd(std::less<int>(), 0), -1);
		std::cout << "after replace_if:" << std::endl;
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//也可用于字符串
		const char * A[] = {"apple", "banana", "pear", "unknown"};
		const int N = sizeof(A) / sizeof(char *);
		std::cout << "before replace_if:" << std::endl;
		std::copy(A, A + N, 
			std::ostream_iterator<const char *>(std::cout, " "));
		std::cout << std::endl;
		std::replace_if(A, A+N,
			string_length_exceeds(6),
			"******");
		std::cout << "after replace_if:" << std::endl;
		std::copy(A, A + N, 
			std::ostream_iterator<const char *>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//replace_copy，用于两容器间复制后替换
		std::vector<int> V1;
		V1.push_back(1);
		V1.push_back(2);
		V1.push_back(3);
		V1.push_back(1);
		std::vector<int> V2(4);
		std::replace_copy(V1.begin(), V1.end(), V2.begin(), 1, 99);
		std::cout << "V1, V2:" << std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V2.begin(), V2.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//replace_copy_if，用于两容器间复制后条件替换
		std::vector<int> V1;
		V1.push_back(1);
		V1.push_back(-1);
		V1.push_back(-5);
		V1.push_back(2);
		std::vector<int> V2(4);
		std::replace_copy_if(V1.begin(), V1.end(), V2.begin(),
			std::bind2nd(std::less<int>(), 0),
			0);
		std::cout << "V1, V2:" << std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::copy(V2.begin(), V2.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//fill/fill_n，用于元素填充（后者常用于插入）
		std::vector<double> V(4);
		std::fill(V.begin(), V.end(), 137);
		assert(V[0] == 137 && V[1] == 137 && V[2] == 137 && V[3] == 137);
		std::cout << "V == 137" << std::endl;
		
		//从后面插入4个42
		std::vector<double> V2;
		std::fill_n(std::back_inserter(V2), 4, 42);
		assert(V2.size() == 4 && V2[0] == 42 && V2[1] == 42 && V2[2] == 42 && V2[3] == 42);
		std::cout << "V2 == 42" << std::endl;
	}
	
	{
		//generate/generate_n，用于按根据函数（没有参数）的结果产生序列
		std::vector<int> V(5);
		std::generate(V.begin(), V.end(), rand);
		std::cout << "rand:" << std::endl;
		std::copy(V.begin(), V.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//generate_n常用于插入
		std::generate_n(std::ostream_iterator<int>(std::cout, " "), 5, rand);
		std::cout << std::endl;
	}
	
	{
		//remove，用于原地删除指定值，但真正的删除是发生在erase后
		std::vector<int> V;
		V.push_back(3);
		V.push_back(1);
		V.push_back(4);
		V.push_back(1);
		V.push_back(5);
		V.push_back(9);
		std::cout << "before remove" << std::endl;
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		//删除值为1的元素
		std::vector<int>::iterator new_end = 
			std::remove(V.begin(), V.end(), 1);
		std::cout << "after remove" << std::endl;
		std::copy(V.begin(), new_end, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		V.erase(new_end, V.end());
		std::cout << "after erase" << std::endl;
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//remove_if，用于原地删除指定条件值，但真正的删除是发生在erase后
		std::vector<int> V;
		V.push_back(1);
		V.push_back(4);
		V.push_back(2);
		V.push_back(8);
		V.push_back(5);
		V.push_back(7);
		std::cout << "before remove_if" << std::endl;
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		//删除偶数
		std::vector<int>::iterator new_end = 
			std::remove_if(V.begin(), V.end(), 
				__gnu_cxx::compose1(std::bind2nd(std::equal_to<int>(), 0),
					std::bind2nd(std::modulus<int>(), 2)));
		V.erase(new_end, V.end());
		std::cout << "after remove_if" << std::endl;
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//remove_copy，用于容器之间的复制后删除指定值，常用于插入
		std::vector<int> V;
		V.push_back(-2);
		V.push_back(0);
		V.push_back(-1);
		V.push_back(0);
		V.push_back(1);
		V.push_back(2);
		//删除0
		std::remove_copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "),
			0);
		std::cout << std::endl;
	}
	
	{
		//remove_copy_if，用于容器之间的复制后删除指定条件的值，常用于插入
		//copy_if(first, last, result, pred)等价于
		//remove_copy_if(first, last, result, not1(pred))
		std::vector<int> V1;
		V1.push_back(-2);
		V1.push_back(0);
		V1.push_back(-1);
		V1.push_back(0);
		V1.push_back(1);
		V1.push_back(2);
		std::vector<int> V2;
		//插入复制后删除负数
		std::remove_copy_if(V1.begin(), V1.end(), 
			std::back_inserter(V2),
			std::bind2nd(std::less<int>(), 0));
		std::copy(V2.begin(), V2.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//unique，用于原地删除相邻重复
		//如果配合sort使用就是删除所有重复的值
		std::vector<int> V;
		V.push_back(1);
		V.push_back(3);
		V.push_back(3);
		V.push_back(3);
		V.push_back(2);
		V.push_back(2);
		V.push_back(1);
		std::vector<int>::iterator new_end = std::unique(V.begin(), V.end());
		// 1 3 2 1
		std::copy(V.begin(), new_end, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//配合sort使用，就变成删除所有重复项
		const char init[] = "The Standard Template Library";
		std::vector<char> V2(init, init + strlen(init));
		std::sort(V2.begin(), V2.end(), lt_nocase);
		std::copy(V2.begin(), V2.end(), 
			std::ostream_iterator<char>(std::cout));
		std::cout << std::endl;
		std::vector<char>::iterator new_end2 = std::unique(V2.begin(), V2.end(), eq_nocase);
		std::copy(V2.begin(), new_end2, 
			std::ostream_iterator<char>(std::cout));
		std::cout << std::endl;
	}
	
	{
		//unique_copy，用于容器间复制后删除相邻重复
		const int A[] = {2, 7, 7, 7, 1, 1, 8, 8, 8, 2, 8, 8};
		std::unique_copy(A, A + sizeof(A) / sizeof(int), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//还可以指定相等的特定规则函数
		//下面是复制后删除相邻除于10相等的值
		const int A2[] = {2, 7, 7, 7, 11, 11, 18, 18, 18, 12, 18, 18};
		std::unique_copy(A2, A2 + sizeof(A2) / sizeof(int),
			std::ostream_iterator<int>(std::cout, " "),
			eq_div(10));
		std::cout << std::endl;			
	}
	
	{
		//reverse，用于原地反序
		std::vector<int> V;
		V.push_back(0);
		V.push_back(1);
		V.push_back(2);
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;	
		std::reverse(V.begin(), V.end());
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;	
	}
	
	{
		//reverse_copy，用于容器间复制后反序
		std::vector<int> V;
		V.push_back(0);
		V.push_back(1);
		V.push_back(2);
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::list<int> L(V.size());
		std::reverse_copy(V.begin(), V.end(), L.begin());
		copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//rotate，用于两个子串原地互换位置
		char alpha[] = "abcdefghijklmnopqrstuvwxyz";
		std::rotate(alpha, alpha + 13, alpha + 26);
		std::cout << alpha << std::endl;
	}
	
	{
		//rotate_copy，用于两个容器复制后两个子串互换位置
		const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
		std::rotate_copy(alpha, alpha + 13, alpha + 26, 
			std::ostream_iterator<char>(std::cout));
		std::cout << std::endl;
	}
	
	{
		//next_permutation/prev_permutation，向前或向后获取n!排列
		
		//O(n!)排序法
		int A[] = {8, 3, 6, 1, 2, 5, 7, 4};
		//int A[] = {1, 2, 3};
		const int N = sizeof(A) / sizeof(int);
		snail_sort(A, A+N);
		std::copy(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		// 输出n!个排列
		int A2[] = {1, 2, 3};
		const int N2 = 3;
		do
		{
			std::copy(A2, A2 + N2,
				std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		} while(std::next_permutation(A2, A2 + N2));
		
		// 获取前或后的n!排列
		int A3[] = {2, 3, 4, 5, 6, 1};
		const int N3 = sizeof(A3) / sizeof(int);
		std::cout << "Initially:              ";
		std::copy(A3, A3 + N3, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::prev_permutation(A3, A3 + N3);
		std::cout << "After prev_permutation: ";
		std::copy(A3, A3 + N3, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::next_permutation(A3, A3 + N3);
		std::cout << "After next_permutation: ";
		std::copy(A3, A3 + N3, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//partition，用于把符合条件的值移到区间的最左边
		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		const int N = sizeof(A)/sizeof(int);
		//把偶数都移到区间左边
		std::partition(A, A + N,
			__gnu_cxx::compose1(std::bind2nd(std::equal_to<int>(), 0),
				std::bind2nd(std::modulus<int>(), 2)));
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//stable_partition，用于把符合条件的值移到区间的最左边，然后排序
		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		const int N = sizeof(A)/sizeof(int);
		std::stable_partition(A, A + N,
			__gnu_cxx::compose1(std::bind2nd(std::equal_to<int>(), 0),
				std::bind2nd(std::modulus<int>(), 2)));
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//random_shuffle，用于产生均匀分布的随机N!排列
		//可提供第三参数用于乱数生成器
		const int N = 8;
		int A[] = {1, 2, 3, 4, 5, 6, 7, 8};
		std::random_shuffle(A, A + N);
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//random_sample/random_sample_n，从N个数中取n个数，概率相等
		//可提供第三参数用于乱数生成器
		const int N = 10;
		const int n = 4;
		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		int B[n];
		__gnu_cxx::random_sample(A, A+N, B, B+n);
		// 10选4
		std::copy(B, B + n, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//插入随机序列（10选4）
		__gnu_cxx::random_sample_n(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "), 4);
		std::cout << std::endl;
	}
	
	{
		//accumulate，常用于累加或连乘
		//需要指定一个初始值
		int A[] = {1, 2, 3, 4, 5};
		const int N = sizeof(A) / sizeof(int);
		//累加
		std::cout << "The sum of all elements in A is " 
			<< std::accumulate(A, A + N, 0)
			<< std::endl;
		//连乘
		std::cout << "The product of all elements in A is "
			<< std::accumulate(A, A + N, 1, std::multiplies<int>())
			<< std::endl;
	}
	
	{
		//inner_product，用于计算内积
		int A1[] = {1, 2, 3};
		int A2[] = {4, 1, -2};
		const int N1 = sizeof(A1) / sizeof(int);
		// 1 * 4 + 2 * 1 + 3 * (-2)
		std::cout << "The inner product of A1 and A2 is " 
		   << std::inner_product(A1, A1 + N1, A2, 0)
		   << std::endl;
	}
	
	{
		//partial_sum，用于计算部分总和序列
		//（部分总和，指每步相加的和）
		const int N = 10;
		int A[N];
		std::fill(A, A+N, 1);
		std::cout << "A:                 ";
		std::copy(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::cout << "Partial sums of A: ";
		std::partial_sum(A, A+N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//adjacent_difference，用于生成差分序列，
		//即相邻元素的差的序列
		//（如果是第一个，就直接取第一个元素的值）
		//用partial_sum可还原为原来的序列
		int A[] = {1, 4, 9, 16, 25, 36, 49, 64, 81, 100};
		const int N = sizeof(A) / sizeof(int);
		int B[N];
		std::cout << "A[]:         ";
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::adjacent_difference(A, A + N, B);
		std::cout << "Differences: ";
		std::copy(B, B + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		//用partial_sum还原为原来的序列
		std::cout << "Reconstruct: ";
		std::partial_sum(B, B + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	return 0;
}
