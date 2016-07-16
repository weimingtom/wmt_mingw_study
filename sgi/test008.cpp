// 编译命令行：
// g++ test008.cpp

//《C++程序设计语言（特别版）》
// 19.4.2 一个用户定义分配器
//演示标准库分配器allocator接口的简单实现

#include <cstddef>
#include <vector>
#include <iostream>
#include <iterator>

//class Pool定义一个简单接口的小内存块内存池
//这个内存池一次只能分配恒定大小的小内存块（由构造函数参数指定）
class Pool 
{
private:
	//用于分配和释放内存的链表
	//比Chunk的粒度小，多个Link分割一个Chunk
	//分配前Link的next域用于计算下一个分配位置
	//分配后Link只是void*内存块的头部
	struct Link
	{
		Link *next;
	};
	
	// 底层内存分配区的分区块
	// 一个Pool内有多个依次链接的Chunk
	//每个Chunk被多个Link分割成更小的内存块
	//（但所有Link的排列次序不一定依照其所在Chunk块的排列次序），
	//但因为内存是连续的，所以在增量分配和全体释放时速度会较快
	struct Chunk
	{
		//让一个Chunk不超过8K且略小于8K
		enum { SIZE = 8 * 1024 - 16 }; 
		
		unsigned char mem[SIZE];
		Chunk *next; //next应该不会超过16个字节
	};
	
	//多个Chunk前后组成一个链表（好像一个栈）
	Chunk *chunks; 
	//每次alloc()返回的内存块最大大小，
	//但不能比sizeof(Link)小，也不能比Chunk::SIZE大
	const unsigned int esize;
	//当前空闲Chunk块内的空闲Link头部
	Link *head; 
	
	//禁用默认的复制构造函数和复制赋值，
	//所以这里不需要定义方法体
private:
	Pool(Pool& sz);
	void operator=(Pool&);
	
private:
	void grow();
	
public:
	Pool(unsigned int n);//指定每次alloc()返回的内存块大小
	~Pool();
	void* alloc(); //不需要指定内存块大小（在构造函数中指定）
	void free(void* b);
};

//从head链表的头部取出一个Link
//如果空闲Chunk块用完，需要分配新的Chunk块，
//然后用Link链表分割这个新的空闲Chunk块。
inline void* Pool::alloc()
{
	if (head == 0)
	{
		grow();
	}
	Link *p = head;
	head = p->next;
	std::cout << "Pool::alloc():  p == " << p << ", head == " << head << std::endl;
	return p;
}

//把要释放的b放回head链表的头部
inline void Pool::free(void* b)
{
	Link* p = static_cast<Link*>(b);
	p->next = head;
	std::cout << "Pool::free(): p == " << p << ", head == " << head << std::endl;
	head = p;
}

Pool::Pool(unsigned int sz)
: esize(sz < sizeof(Link) ? sizeof(Link) : sz)
{
	head = 0;
	chunks = 0;
}

Pool::~Pool()
{
	Chunk *n = chunks;
	while (n)
	{
		Chunk *p = n;
		n = n->next;
		delete p;
	}
}

//增量分配内存
//虽然Link*所在的小内存块是连续的，
//但alloc的分配次序不一定是连续的
//（因为free()可能放回旧的内存块）
void Pool::grow()
{
	Chunk *n = new Chunk();
	n->next = chunks;
	chunks = n;
	
	std::cout << "Pool::grow(): esize == " << esize << std::endl;
	const int nelem = Chunk::SIZE / esize;
	unsigned char *start = n->mem;
	unsigned char *last = &start[(nelem - 1) * esize];
	for (unsigned char *p = start; p < last; p += esize)
	{
		reinterpret_cast<Link*>(p)->next = reinterpret_cast<Link*>(p + esize);
	}
	reinterpret_cast<Link*>(last)->next = 0; //见alloc()的if (head == 0)
	head = reinterpret_cast<Link*>(start);
}


//template Pool_alloc把class Pool适配到C++标准库的分配器接口
//不完整，因为有些接口没有实现，而有些接口只实现部分功能
template<class T>
class Pool_alloc
{
private:
	static Pool mem; //全局，以提高效率
	
public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	
	typedef T *pointer;
	typedef const T *const_pointer;
	
	typedef T &reference;
	typedef const T&const_reference;

	Pool_alloc();
	
	T* allocate(size_type n, void*);
	void deallocate(pointer p, size_type n);
};

//构造静态的mem域
//因为Pool只能分配固定大小的小内存块，
//而且Pool_alloc没有实现rebind方法
//所以这里指定每次分配的连续内存区都必须是sizeof(T) * 128，
//虽然造成浪费，但确保分配的内存是连续的。
//没有超过Chunk块的8K上限，所以是安全的。
template <class T> 
Pool Pool_alloc<T>::mem(sizeof(T) * 128);

template <class T> 
Pool_alloc<T>::Pool_alloc()
{
	
}

template<class T>
T* Pool_alloc<T>::allocate(size_type n, void* = 0)
{
	if (n <= 128)
	{
		void *p = mem.alloc();
		std::cout << "allocate : " << n << ", " << p << std::endl;
		return static_cast<T*>(p);
	} else {
		//TODO:
		std::cout << "allocate : " << n << std::endl;
		throw "allocate error"; //出错
	}
}

template<class T>
void Pool_alloc<T>::deallocate(pointer p, size_type n)
{
	if (n <= 128)
	{
		std::cout << "deallocate : " << n << ", " << p << std::endl;
		mem.free(p);
		return;
	} else {
		//TODO:
		std::cout << "deallocate : " << n << std::endl;
		throw "deallocate error"; //出错
	}
}

int main(int argc, const char *argv[])
{
	{
		std::vector<int> V1;
		V1.push_back(1);
		V1.push_back(2);
		V1.push_back(3);
		V1.push_back(4);
		std::cout << "V1:"<< std::endl;
		std::copy(V1.begin(), V1.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	{
		std::cout << "==================="<< std::endl;
		try
		{
			std::vector< int, Pool_alloc<int> > V2;
			std::cout << "V2.push_back(1);" << std::endl;
			V2.push_back(1);
			std::cout << "V2.push_back(2);" << std::endl;
			V2.push_back(2);
			std::cout << "V2.push_back(3);" << std::endl;
			V2.push_back(3);
			V2.push_back(4);
			std::cout << "V2:"<< std::endl;
			std::copy(V2.begin(), V2.end(),
				std::ostream_iterator<int>(std::cout, " "));
			std::cout << std::endl;
		}
		catch (const char *&str)
		{
			std::cout << str << std::endl;
		}
	}
	return 0;
}

/**
运行结果：
$ g++ -g test008.cpp
$ ./a.exe
V1:
1 2 3 4
===================
V2.push_back(1);
Pool::grow(): esize == 512
Pool::alloc():  p == 0x3e4d68, head == 0x3e4f68
allocate : 1, 0x3e4d68
V2.push_back(2);
Pool::alloc():  p == 0x3e4f68, head == 0x3e5168
allocate : 2, 0x3e4f68
deallocate : 1, 0x3e4d68
Pool::free(): p == 0x3e4d68, head == 0x3e5168
V2.push_back(3);
Pool::alloc():  p == 0x3e4d68, head == 0x3e5168
allocate : 4, 0x3e4d68
deallocate : 2, 0x3e4f68
Pool::free(): p == 0x3e4f68, head == 0x3e5168
V2:
1 2 3 4
deallocate : 4, 0x3e4d68
Pool::free(): p == 0x3e4d68, head == 0x3e4f68
*/


