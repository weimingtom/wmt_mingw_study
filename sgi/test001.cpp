// 编译命令行：
// g++ -Wno-deprecated test001.cpp

// 演示STL基本组件
//construct
//destroy
//uninitialized_copy/uninitialized_copy_n
//uninitialized_fill/uninitialized_fill_n
//get_temporary_buffer/return_temporary_buffer

#include <iostream>
#include <string>
#include <memory>
// algo.h的construct和destroy不属于标准，尽量不要使用
// 因为algo.h会导致名字空间std被using
#include <algo.h> 

class Int 
{
public:
	Int(int x):val(x) {}
	int get() { return val; }
private:
	int val;
};

int main(int argc, const char *argv[])
{
	std::cout << "Test STL basic functions" << std::endl;
	
	// 底层函数construct，不需要std::
	// 用于使用malloc构造类对象
	std::string *sptr = (std::string *)malloc(sizeof(std::string));
	construct(sptr, "test");
	assert(strcmp(sptr->c_str(), "test") == 0);
	std::cout << "sptr->c_str() == \"test\"" << std::endl;
	
	// 底层函数destroy，不需要std::
	// 用于调用数组的某指针到某指针之间的所有对象的析构函数
	//（但内存并没有真正释放）
	Int A[] = { Int(1), Int(2), Int(3), Int(4) };
	destroy(A, A + 4);
	construct(A, Int(10));
	construct(A + 1, Int(11));
	construct(A + 2, Int(12));
	construct(A + 3,  Int(13));
	for(int i1 = 0; i1 < 4; i1++)
	{
		std::cout << "A[" << i1 << "] = " << A[i1].get() << std::endl;
	}
	
	// 底层函数uninitialized_copy/uninitialized_copy_n
	// 用于对一个数组批量执行construct
	// 相当于执行N次construct(&*(result + (i - first)), *i) 
	int A1[] = {1, 2, 3, 4, 5, 6, 7};
	const int N = sizeof(A1) / sizeof(int);
	Int* A2 = (Int*) malloc(N * sizeof(Int));
	std::uninitialized_copy(A1, A1 + N, A2);
	//or
	//uninitialized_copy_n(A1, N, A2);
	for(int i2 = 0; i2 < N; i2++)
	{
		std::cout << "A2[" << i2 << "] = " << A2[i2].get() << std::endl;
	}
	
	
	// 底层函数uninitialized_fill/uninitialized_fill_n
	// 用于对一个数组批量执行相同的construct
	// 相当于执行N次construct(&*i, x)
	const int N3 = 7;
	Int val(46);
	Int* A3 = (Int*) malloc(N3 * sizeof(Int));
	std::uninitialized_fill(A3, A3 + N3, val);
	//or
	//uninitialized_fill_n(A3, N3, val);
	for(int i3 = 0; i3 < N3; i3++)
	{
		std::cout << "A3[" << i3 << "] = " << A3[i3].get() << std::endl;
	}
	
	// 底层函数get_temporary_buffer/return_temporary_buffer
	// 用于开辟临时内存区，供:
	// uninitialized_copy/uninitialized_copy_n/uninitialized_fill/uninitialized_fill_n
	// 这些底层函数使用
	// sgi的文档例子在mingw测试时有问题：
	// 如果你不指明模板参数<int>，g++会报以下编译错误
	// test001.cpp:75: error: no matching function for call to `get_temporary_buffer(int)'
	// see http://www.cplusplus.com/reference/std/memory/get_temporary_buffer/
	// 另外，新版本的get_temporary_buffer可以多接一个初始化参数
	// pair<int*, ptrdiff_t> P = get_temporary_buffer<int>(10000, (int*) 0);
	std::pair<int*, ptrdiff_t> P = std::get_temporary_buffer<int>(10000);
	//拿到刚开辟的临时内存区和大小
	int* buf = P.first;
	ptrdiff_t N4 = P.second;
	//全部填充42
	std::uninitialized_fill_n(buf, N4, 42);
	//寻找不等于42的位置
	int* result = std::find_if(buf, buf + N4, std::bind2nd(std::not_equal_to<int>(), 42));
	//毫无疑问，指向内存区最后的下一个位置
	assert(result == buf + N4);
	std::cout << "result == buf + N4" << std::endl;
	//归还临时内存
	std::return_temporary_buffer(buf);
	
	return 0;
}
