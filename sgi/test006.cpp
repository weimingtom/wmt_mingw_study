// 编译命令行：
// g++ test006.cpp

// 演示STL函数对象
#include <math.h>
#include <assert.h>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <iterator>
#include <functional>
// for iota / transform
#include <ext/numeric>
// for hash, or #include <ext/hash_set>
#include <ext/hash_map>

struct sine:public std::unary_function<double, double> 
{
    double operator()(double x) 
    { 
		return sin(x); 
	}
};

struct exponentiate : public std::binary_function<double, double, double> 
{
    double operator()(double x, double y) 
    { 
		return pow(x, y); 
	}
};


struct B 
{
	virtual void print() = 0;
};

struct D1 : public B 
{
	void print() 
	{ 
		std::cout << "I'm a D1" << std::endl; 
	}
};

struct D2 : public B 
{
	void print() 
	{ 
		std::cout << "I'm a D2" << std::endl; 
	}
};

struct Operation 
{
	virtual double eval(double) = 0;
};

struct Square : public Operation 
{
	double eval(double x) 
	{ 
		return x * x; 
	}
};

struct Negate : public Operation 
{
	double eval(double x) 
	{ 
		return -x; 
	}
};

int main(int argc, const char *argv[])
{
	{
		//unary_function<Arg, Result>，用于作为基类声明一元函数对象（一元仿函数，实际上是个类）
		//binary_function<Arg1, Arg2, Result>，用于作为基类声明二元函数对象
		
	}
	
	{
		//plus<T> / minus<T> / multiplies<T> / divides<T> / modulus<T>，
		//序列相加/相减/相乘/相除/取模
		//V3 = V1 + V2
		const int N = 1000;
		std::vector<double> V1(N);
		std::vector<double> V2(N);
		std::vector<double> V3(N);
		__gnu_cxx::iota(V1.begin(), V1.end(), 1);
		std::fill(V2.begin(), V2.end(), 75);
		assert(V2.size() >= V1.size() && V3.size() >= V1.size());
		std::transform(V1.begin(), V1.end(), V2.begin(), V3.begin(),
			std::plus<double>());
	}
	
	{
		//negate<T>，序列取相反数
		//V2 = -V1
		const int N = 1000;
		std::vector<int> V1(N);
		std::vector<int> V2(N);
		__gnu_cxx::iota(V1.begin(), V1.end(), 1);
		assert(V2.size() >= V1.size());
		std::transform(V1.begin(), V1.end(), V2.begin(),
			std::negate<int>());
	}
	
	{
		//equal_to<T> / not_equal_to<T> / less<T> / greater<T> / less_equal<T> / greater_equal<T>
		//条件操作，判断是否等于/不等于/小于/大于/小于等于/大于等于某个数
		int A[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0};
		const int N = sizeof(A)/sizeof(int);
		//把0移到最左边的区间
		std::partition(A, A + N,
			std::bind2nd(std::equal_to<int>(), 0));
		std::copy(A, A + N, 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;

		std::list<int> L(1000);
		std::fill(L.begin(), L.end(), 75);
		//查找满足条件的位置
		std::list<int>::iterator first_nonzero = 
			std::find_if(L.begin(), L.end(), 
				std::bind2nd(std::not_equal_to<int>(), 0));
		assert(first_nonzero == L.end() || *first_nonzero != 0);
	}
	
	{
		//logical_and<T> / logical_or<T> / logical_not<T>
		//条件操作，判断两个条件的与/或/非
		std::list<int> L;
		std::fill(L.begin(), L.end(), 75);
		std::list<int>::iterator in_range = 
			std::find_if(L.begin(), L.end(),
				__gnu_cxx::compose2(std::logical_and<bool>(),
					std::bind2nd(std::greater_equal<int>(), 1),
					std::bind2nd(std::less_equal<int>(), 10)));
		assert(in_range == L.end() || (*in_range >= 1 && *in_range <= 10));
		
		char str[] = "Hello world!";
		const int MAXLEN = sizeof(str) / sizeof(char);
		const char* wptr = 
			std::find_if(str, str + MAXLEN,
				__gnu_cxx::compose2(std::logical_or<bool>(),
					std::bind2nd(std::equal_to<char>(), ' '),
					std::bind2nd(std::equal_to<char>(), '\n')));
		assert(wptr == str + MAXLEN || *wptr == ' ' || *wptr == '\n');
	
		std::vector<bool> V;
		V.push_back(true);
		V.push_back(false);
		std::transform(V.begin(), V.end(), V.begin(), 
			std::logical_not<bool>());
	}
	
	{
		//identity<T>，原值返回
		int x = 137;
		__gnu_cxx::identity<int> id;
		assert(x == id(x));
	}
	
	{
		//project1st<Arg1, Arg2>，忽略第二参数
		std::vector<int> v1(10, 137);
		std::vector<char*> v2(10, (char*) 0);
		std::vector<int> result(10);
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(),
			__gnu_cxx::project1st<int, char*>());
		assert(std::equal(v1.begin(), v1.end(), result.begin()));
	}
	
	{
		//project2nd<Arg1, Arg2>，忽略第一参数
		std::vector<char*> v1(10, (char*) 0);
		std::vector<int> v2(10, 137);
		std::vector<int> result(10);
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(),
			__gnu_cxx::project2nd<char*, int>());
		assert(std::equal(v2.begin(), v2.end(), result.begin()));
	}
	
	{
		//select1st<Pair>，pair或pair相同接口（如map）的类的第一参数
		//select2nd<Pair>，pair或pair相同接口（如map）的类的第二参数
		std::map<int, double> M;
		M[1] = 0.3;
		M[47] = 0.8;
		M[33] = 0.1;
		// 输出1 33 47.
		std::transform(M.begin(), M.end(), 
			std::ostream_iterator<int>(std::cout, " "),
			__gnu_cxx::select1st<std::map<int, double>::value_type>());
		std::cout << std::endl;
		// 输出0.3 0.1 0.8
		std::transform(M.begin(), M.end(), 
			std::ostream_iterator<double>(std::cout, " "),
			__gnu_cxx::select2nd<std::map<int, double>::value_type>());
		std::cout << std::endl;
	}
	
	{
		//hash<T>，返回哈希值
		__gnu_cxx::hash<const char*> H;
		std::cout << "foo -> " << H("foo") << std::endl;
		std::cout << "bar -> " << H("bar") << std::endl;
	}
	
	{
		//subtractive_rng，伪随机数生成器，减去法
		__gnu_cxx::subtractive_rng R;
		//生成0至4之间的伪随机数
		for (int i = 0; i < 20; ++i)
			std::cout << R(5) << ' ';
		std::cout << std::endl;
	}
	
	{
		//mem_fun_t<Result, X>，用于遍历调用多态的虚函数，容器元素是指针
		//mem_fun1_t<Result, X, Arg>，传入容器的元素作为参数，遍历调用多态的虚函数，容器元素是指针
		//const_mem_fun_t / const_mem_fun1_t，用于const的成员函数，用法类似 
		std::vector<B*> V;
		V.push_back(new D1);
		V.push_back(new D2);
		V.push_back(new D2);
		V.push_back(new D1);
		std::for_each(V.begin(), V.end(), 
			std::mem_fun(&B::print));
		
		std::vector<Operation*> operations;
		std::vector<double> operands;
		operations.push_back(new Square);
		operations.push_back(new Square);
		operations.push_back(new Negate);
		operations.push_back(new Negate);
		operations.push_back(new Square);
		operands.push_back(1);
		operands.push_back(2);
		operands.push_back(3);
		operands.push_back(4);
		operands.push_back(5);
		std::transform(operations.begin(), operations.end(),
			operands.begin(),
			std::ostream_iterator<double>(std::cout, "\n"),
			std::mem_fun(&Operation::eval));
	}
	
	{
		//mem_fun_ref_t<Result, X>，用于遍历调用多态的虚函数，容器元素是引用
		//mem_fun1_ref_t<Result, X, Arg>，传入容器的元素作为参数，遍历调用多态的虚函数，容器元素是引用
		//const_mem_fun_ref_t / const_mem_fun1_ref_t，用于const的成员函数，用法类似
		std::vector<D1> V;
		V.push_back(D1());
		V.push_back(D1());
		std::for_each(V.begin(), V.end(), 
			std::mem_fun_ref(&B::print));
			
		int A1[5] = {1, 2, 3, 4, 5};
		int A2[5] = {1, 1, 2, 3, 5};
		int A3[5] = {1, 4, 1, 5, 9};
		std::vector<std::vector<int> > V2;
		V2.push_back(std::vector<int>(A1, A1 + 5));
		V2.push_back(std::vector<int>(A2, A2 + 5));
		V2.push_back(std::vector<int>(A3, A3 + 5));
		int indices[3] = {0, 2, 4};
		int& (std::vector<int>::*extract)(std::vector<int>::size_type);
		//使用&，否则编译器报错
		//输出1 2 9
		extract = &std::vector<int>::operator[];
		std::transform(V2.begin(), V2.end(), indices,
			std::ostream_iterator<int>(std::cout, "\n"),
			std::mem_fun_ref(extract));
	}
	
	{
		//binder1st<AdaptableBinaryFunction>，用于把第一参数设为常数
		std::list<int> L;
		std::fill(L.begin(), L.end(), 75);
		std::list<int>::iterator first_nonzero = 
			std::find_if(L.begin(), L.end(), 
				std::bind1st(std::not_equal_to<int>(), 0));
		assert(first_nonzero == L.end() || *first_nonzero != 0);
	}
	
	{
		//binder2nd<AdaptableBinaryFunction>，用于把第二参数设为常数
		std::list<int> L;
		std::fill(L.begin(), L.end(), 75);
		std::list<int>::iterator first_positive = 
			std::find_if(L.begin(), L.end(), 
				std::bind2nd(std::greater<int>(), 0));
		assert(first_positive == L.end() || *first_positive > 0);
	}
	
	{
		//pointer_to_unary_function<Arg, Result>，用于把一个参数的C函数转为函数对象，
		//pointer_to_binary_function<Arg1, Arg2, Result>，同上，把两个参数的C函数转为函数对象
		//常用于叠加的函数对象如compose1，
		//如果没有叠加，可以直接使用C函数如fabs，则不需要std::ptr_fun()的辅助
		const int N = 1000;
		std::vector<double> V1(N);
		__gnu_cxx::iota(V1.begin(), V1.end(), 1);
		//如果出现错误：
		//error: expected primary-expression before ',' token
		//请检查函数对象如std::negate<double>()后面的括号是否遗漏
		//
		//直接使用fabs时不需要使用ptr_fun
		//std::transform(V1.begin(), V1.end(), V1.begin(), fabs);
		//
		std::transform(V1.begin(), V1.end(), V1.begin(),
			__gnu_cxx::compose1(std::negate<double>(), std::ptr_fun(fabs)));
			
		//使用两个参数C函数strcmp进行条件查找
		char *str = "OK";
		const int N2 = 1000;
		std::list<char*> L(N2);
		//使用fill之前需要小心容器的个数（需要在创建时指定，而非缺省）
		std::fill(L.begin(), L.end(), str);
		//如果出现这样的错误
		//error: missing template arguments before '(' token
		//提示binder2nd缺少模板参数，
		//可以输入一个错误的模板参数类型，如<int>
		//然后看错误提示
		assert(!strcmp(L.front(), str));
		//
		//查找第一个等于"OK"的位置
		//
		std::list<char*>::iterator item = 
			std::find_if(L.begin(), L.end(),
				std::not1(
					std::binder2nd< std::pointer_to_binary_function<const char*, const char*, int> >(
						std::ptr_fun(strcmp), "OK")));
		assert(!strcmp(*item, "OK"));
		assert(item == L.end() || strcmp(*item, "OK") == 0);
	}
	
	{
		//unary_negate<AdaptablePredicate>，用于一元函数逻辑非
		//binary_negate<AdaptableBinaryPredicate>，用于二元函数的逻辑非
		const int N = 1000;
		std::list<int> L(N);
		std::fill(L.begin(), L.end(), 75);
		std::list<int>::iterator in_range = 
			std::find_if(L.begin(), L.end(),
				std::not1(
					__gnu_cxx::compose2(std::logical_and<bool>(),
						std::bind2nd(std::greater_equal<int>(), 1),
						std::bind2nd(std::less_equal<int>(), 10))));
		assert(in_range == L.end() || !(*in_range >= 1 && *in_range <= 10));
		
		char str[] = "Hello, world!";
		const int MAXLEN = sizeof(str) / sizeof(char);
		const char* wptr = find_if(str, str + MAXLEN,
			__gnu_cxx::compose2(std::not2(std::logical_or<bool>()),
				std::bind2nd(std::equal_to<char>(), ' '),
				std::bind2nd(std::equal_to<char>(), '\n')));
		assert(wptr != str + MAXLEN);
		assert(wptr == str + MAXLEN || !(*wptr == ' ' || *wptr == '\n')); 
	}
	
	{
		//unary_compose<AdaptableUnaryFunction1,AdaptableUnaryFunction2>
		//用于把两个一元函数f(x),g(x)组合成f(g(x))
		//binary_compose<AdaptableBinaryFunction,AdaptableUnaryFunction1,AdaptableUnaryFunction2>
		//用于把两个二元函数f(x),g(x)组合成f(g(x))
		std::vector<double> angles;
		std::vector<double> sines(100);
		const double pi = 3.14159265358979323846;
		for(int i = 0; i <90; i++)
		{
			angles.push_back(i);
		}
		assert(sines.size() >= angles.size());
		std::transform(angles.begin(), angles.end(), sines.begin(),
			__gnu_cxx::compose1(std::negate<double>(),
				__gnu_cxx::compose1(std::ptr_fun(sin),
					std::bind2nd(std::multiplies<double>(), pi / 180.))));
		
		std::list<int> L(100);
		std::fill(L.begin(), L.end(), 75);
		std::list<int>::iterator in_range = 
			std::find_if(L.begin(), L.end(),
				__gnu_cxx::compose2(std::logical_and<bool>(),
					std::bind2nd(std::greater_equal<int>(), 1),
					std::bind2nd(std::less_equal<int>(), 10)));
		assert(in_range == L.end() || (*in_range >= 1 && *in_range <= 10));
		
		std::list<double> L2(100);
		std::fill(L2.begin(), L2.end(), 75.0);
		double DBL_MIN = 1.0;
		//计算sin(x)/(x + DBL_MIN)
		std::transform(L2.begin(), L2.end(), L2.begin(),
			__gnu_cxx::compose2(std::divides<double>(),
				std::ptr_fun(sin),
				std::bind2nd(std::plus<double>(), DBL_MIN)));
	}
	
	//标准C++允许main不返回，但标准C要求必须返回
	return 0;
}

