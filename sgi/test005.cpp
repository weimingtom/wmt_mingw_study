// 编译命令行：
// g++ test005.cpp

// 演示STL迭代器
#include <list>
#include <vector>
#include <string>
#include <iterator>
#include <iostream>

class Int 
{
public:
	Int(int x):val(x) 
	{
	
	}
	int get() 
	{ 
		return val; 
	}
private:
	int val;
};    

int main(int argc, const char *argv[])
{
	{
		//front_insert_iterator<FrontInsertionSequence>，用于从前面依次插入元素
		std::list<int> L;
		L.push_front(3);
		std::front_insert_iterator< std::list<int> > ii(L);
		*ii++ = 0;
		*ii++ = 1;
		*ii++ = 2;
		//输出2 1 0 3
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//back_insert_iterator<BackInsertionSequence>，用于从后面依次插入元素
		//效果相当于
		//copy(L.begin(), L.end(), back_inserter(v2));
		//也相当于更快的方法（如果原来没有元素的话）
		//vector v1(L.begin(), L.end());
		std::list<int> L;
		L.push_front(3);
		std::back_insert_iterator< std::list<int> > ii(L);
		*ii++ = 0;
		*ii++ = 1;
		*ii++ = 2;
		// 输出3 0 1 2
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//insert_iterator<Container>，用于某个位置后面的空挡处插入元素
		std::list<int> L;
		L.push_front(3);
		std::insert_iterator< std::list<int> > ii(L, L.begin());
		*ii++ = 0;
		*ii++ = 1;
		*ii++ = 2;
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	//if(false)
	{
		//istream_iterator<T, Distance> / ostream_iterator<T>，用于把输入/输出流转换为迭代器
		fflush(stdin);
		std::vector<int> V;
		std::cout << "please input some number (. to end)" << std::endl;
		std::copy(std::istream_iterator<int>(std::cin), 
			std::istream_iterator<int>(),
			std::back_inserter(V)); 
		std::copy(V.begin(), V.end(), 
			std::ostream_iterator<int>(std::cout, "\n"));
		fflush(stdout);
	}
	
	//if(false)
	{
		//istreambuf_iterator / ostreambuf_iterator，用于把输入/输出流缓冲转换为迭代器
		fflush(stdin);
		std::istreambuf_iterator<char> first(std::cin);
		std::istreambuf_iterator<char> eos;
		std::cout << "please input string (Ctrl+Z to end)" << std::endl;
		std::vector<char> buffer(first, eos);
		std::copy(buffer.begin(), buffer.end(),
			std::ostreambuf_iterator<char>(std::cout));
		std::cout << std::endl;
		fflush(stdout);
	}
	
	{
		//reverse_iterator<RandomAccessIterator, T, Reference, Distance>，用于作为参数传入算法时，颠倒容器的前后次序
		std::string s = "This is a test.";
		std::copy(std::reverse_iterator<std::string::iterator>(s.end()),
			std::reverse_iterator<std::string::iterator>(s.begin()),
			std::ostream_iterator<char>(std::cout));
		std::cout << std::endl;
	}
	
	{
		//raw_storage_iterator<ForwardIterator, T>，和迭代器类似，但使用已经分配的内存而非用new分配。
		//在不分配内存的情况下只调用构造函数，即*r = x等效于construct(&*i, x).
		int A1[] = {1, 2, 3, 4, 5, 6, 7};
		const int N = sizeof(A1) / sizeof(int);
		Int* A2 = (Int*) malloc(N * sizeof(Int));     
		std::transform(A1, A1 + N, 
			std::raw_storage_iterator<Int*, int>(A2),
			std::negate<int>());
		for(int i = 0; i < N; i++)
		{
			std::cout << "A2[" << i << "] == " << A2[i].get() << std::endl;
		}
		std::cout << std::endl;
		free(A2);
	}
	
	//标准C++允许main不返回，但标准C要求必须返回
	return 0;
}

