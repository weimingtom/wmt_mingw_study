// 编译命令行：
// g++ test007.cpp

//see http://www.cplusplus.com/reference/stl/

// 演示STL容器
#include <assert.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <list>
#include <ext/slist>
#include <deque>
#include <set>
#include <map>
#include <ext/hash_set>
#include <ext/hash_map>
#include <stack>
#include <queue>

//用于排序的函数对象
struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

//用于哈希查找的函数对象
struct eqstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};

//哈希查找
void lookup(const __gnu_cxx::hash_set<const char*, __gnu_cxx::hash<const char*>, eqstr>& Set,
    const char* word)
{
	__gnu_cxx::hash_set<const char*, __gnu_cxx::hash<const char*>, eqstr>::const_iterator it
		= Set.find(word);
	std::cout << word << ": "
		<< (it != Set.end() ? "present" : "not present")
		<< std::endl;
}

//哈希查找
void lookup(const __gnu_cxx::hash_multiset<const char*, __gnu_cxx::hash<const char*>, eqstr>& Set,
    const char* word)
{
	int n_found = Set.count(word);
	std::cout << word << ": "
		<< n_found << " "
		<< (n_found == 1 ? "instance" : "instances")
		<< std::endl;
}

// 使用typedef简化容器的类型名
typedef __gnu_cxx::hash_multimap<const char*, int, __gnu_cxx::hash<const char*>, eqstr> map_type;

//哈希查找
void lookup(const map_type& Map, const char* str)
{
	std::cout << str << ": ";
	std::pair<map_type::const_iterator, map_type::const_iterator> p =
		Map.equal_range(str);
	for (map_type::const_iterator i = p.first; i != p.second; ++i)
		std::cout << (*i).second << " ";
	std::cout << std::endl;
}

int main(int argc, const char *argv[])
{	
	{
		//vector<T, Alloc>
		//支持随机元素访问的动态数组（序列）
		std::vector<int> V;
		V.insert(V.begin(), 3);
		assert(V.size() == 1 && V.capacity() >= 1 && V[0] == 3);
		
		//扩大vector的容量到指定值，除非已经超过指定值
		std::cout << "size:" << V.size() << ",capacity:" << V.capacity() << std::endl;
		V.reserve(3);
		std::cout << "size:" << V.size() << ",capacity:" << V.capacity() << std::endl;
		
		//用构造函数复制vector，容量会自动压缩 
		std::vector<int> temp(V.begin(), V.end());
		std::cout << "Use vector's ctor copy:" << std::endl;
		std::cout << "size:" << temp.size() << ",capacity:" << temp.capacity() << std::endl;
		
		//用swap把序列转移到新的vector，容量不会改变，
		//而且原来的vector会被修改（交换了内容）
		std::vector<int> temp_swap;
		temp_swap.swap(V);
		std::cout << "Use vector's swap:" << std::endl;
		std::cout << "temp_swap's size:" << temp_swap.size() << ",capacity:" << temp_swap.capacity() << std::endl;
		std::cout << "V's size:" << V.size() << ",capacity:" << V.capacity() << std::endl;
		
		//把序列复制到vector，通常用于输入流
		//用法类似std::copy
		std::cout << "Please input double array (NaN is EOF):" << std::endl;
		std::istream_iterator<double> first(std::cin);
		std::istream_iterator<double> eof;
		std::vector<double> buf(first, eof);
		std::cout << "input double numbers are:" << std::endl;
		std::copy(buf.begin(), buf.end(), 
			std::ostream_iterator<double>(std::cout, "\n"));
		std::cout << std::flush;
	}
	
	{
		//list<T, Alloc>
		//双向链表
		std::list<int> L;
		L.push_back(0);
		L.push_front(1);
		L.insert(++L.begin(), 2);
		std::cout << "list:" << std::endl;
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//排序
		L.sort();
		std::cout << "sorted list:" << std::endl;
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//合并
		std::list<int> L2;
		L2.push_back(3);
		L2.push_back(2);
		L2.push_back(1);
		L.merge(L2);
		std::cout << "merged list:" << std::endl;
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//颠倒次序
		L.reverse();
		std::cout << "reversed list:" << std::endl;
		std::copy(L.begin(), L.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//slist<T, Alloc>
		//单向链表
		//从begin()插入
		__gnu_cxx::slist<int> L;
		L.push_front(0);
		L.push_front(1);
		L.insert_after(L.begin(), 2);
		std::cout << "slist, insert to begin:" << std::endl;
		std::copy(L.begin(), L.end(),
		   std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		//从end()插入
		__gnu_cxx::slist<int>::iterator back = L.previous(L.end());
		back = L.insert_after(back, 3); 
		back = L.insert_after(back, 4);
		back = L.insert_after(back, 5);
		std::cout << "slist, insert to end:" << std::endl;
		std::copy(L.begin(), L.end(),
		   std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//deque<T, Alloc>
		//双端队列，类似vector，
		//支持随机访问和双端插入，
		//但不支持capacity()和reserve()
		std::deque<int> Q;
		Q.push_back(3);
		Q.push_front(1);
		Q.insert(Q.begin() + 1, 2);
		Q[2] = 0;
		std::cout << "deque:" << std::endl;
		std::copy(Q.begin(), Q.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//set<Key, Compare, Alloc>
		//集合，不重复的序列，可执行交、并、差集
		const int N = 6;
		const char* a[N] = {"isomer", "ephemeral", "prosaic", 
						  "nugatory", "artichoke", "serif"};
		const char* b[N] = {"flat", "this", "artichoke",
						  "frigate", "prosaic", "isomer"};
		std::set<const char*, ltstr> A(a, a + N);
		std::set<const char*, ltstr> B(b, b + N);
		std::set<const char*, ltstr> C;
		
		std::cout << "Set A: ";
		std::copy(A.begin(), A.end(), 
			std::ostream_iterator<const char*>(std::cout, " "));
		std::cout << std::endl;
		std::cout << "Set B: ";
		std::copy(B.begin(), B.end(), 
			std::ostream_iterator<const char*>(std::cout, " "));   
		std::cout << std::endl;
		
		std::cout << "Union: ";
		std::set_union(A.begin(), A.end(), B.begin(), B.end(),
			std::ostream_iterator<const char*>(std::cout, " "),
			ltstr());   
		std::cout << std::endl;
		
		std::cout << "Intersection: ";
		std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
			std::ostream_iterator<const char*>(std::cout, " "),
			ltstr());    
		std::cout << std::endl;
		
		std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
			std::inserter(C, C.begin()),
			ltstr());
		std::cout << "Set C (difference of A and B): ";
		std::copy(C.begin(), C.end(), 
			std::ostream_iterator<const char*>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//map<Key, Data, Compare, Alloc>
		//关联数组，映射表，键不可重复
		std::map<const char*, int, ltstr> months;
		
		months["january"] = 31;
		months["february"] = 28;
		months["march"] = 31;
		months["april"] = 30;
		months["may"] = 31;
		months["june"] = 30;
		months["july"] = 31;
		months["august"] = 31;
		months["september"] = 30;
		months["october"] = 31;
		months["november"] = 30;
		months["december"] = 31;

		std::cout << "june -> " << months["june"] << std::endl;
		std::map<const char*, int, ltstr>::iterator cur  = months.find("june");
		std::map<const char*, int, ltstr>::iterator prev = cur;
		std::map<const char*, int, ltstr>::iterator next = cur;    
		++next;
		--prev;
		std::cout << "Previous (in alphabetical order) is " << (*prev).first << std::endl;
		std::cout << "Next (in alphabetical order) is " << (*next).first << std::endl;
		
		
		//另一种插入键值对的方法（可以判断是否插入成功）
		std::map<const char*, int, ltstr> M;
		std::pair<std::map<const char*, int>::iterator, bool> p = M.insert(std::make_pair("A", 17));
		//第二返回值表示插入是否成功
		if(p.second)
			std::cout << "insert pair is " << p.first->first << ", " << p.first->second << std::endl;
		//<< "pair:" << "," << p.second 
	}
	
	{
		//multiset<Key, Compare, Alloc>
		//允许元素重复的集合
		const int N = 10;
		int a[N] = {4, 1, 1, 1, 1, 1, 0, 5, 1, 0};
		int b[N] = {4, 4, 2, 4, 2, 4, 0, 1, 5, 5};

		std::multiset<int> A(a, a + N);
		std::multiset<int> B(b, b + N);
		std::multiset<int> C;

		std::cout << "Set A: ";
		std::copy(A.begin(), A.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		std::cout << "Set B: ";
		std::copy(B.begin(), B.end(), 
			std::ostream_iterator<int>(std::cout, " "));   
		std::cout << std::endl;
		
		std::cout << "Union: ";
		std::set_union(A.begin(), A.end(), B.begin(), B.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
		
		std::cout << "Intersection: ";
		std::set_intersection(A.begin(), A.end(), B.begin(), B.end(),
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;  

		std::set_difference(A.begin(), A.end(), B.begin(), B.end(),
			std::inserter(C, C.begin()));
		std::cout << "Set C (difference of A and B): ";
		std::copy(C.begin(), C.end(), 
			std::ostream_iterator<int>(std::cout, " "));
		std::cout << std::endl;
	}
	
	{
		//multimap<Key, Data, Compare, Alloc>
		//键允许重复的关联数组
		std::multimap<const char*, int, ltstr> m;
		
		m.insert(std::pair<const char* const, int>("a", 1));
		m.insert(std::pair<const char* const, int>("c", 2));
		m.insert(std::pair<const char* const, int>("b", 3));
		m.insert(std::pair<const char* const, int>("b", 4));
		m.insert(std::pair<const char* const, int>("a", 5));
		m.insert(std::pair<const char* const, int>("b", 6));
		
		std::cout << "Number of elements with key a: " << m.count("a") << std::endl;
		std::cout << "Number of elements with key b: " << m.count("b") << std::endl;
		std::cout << "Number of elements with key c: " << m.count("c") << std::endl;
		
		std::cout << "Elements in m: " << std::endl;
		for (std::multimap<const char*, int, ltstr>::iterator it = m.begin(); it != m.end(); ++it)
			std::cout << "  [" << (*it).first << ", " << (*it).second << "]" << std::endl;
	}
	
	{
		//hash_set<Key, HashFcn, EqualKey, Alloc>
		//支持哈希查找的集合，不允许元素重复
		__gnu_cxx::hash_set<const char*, __gnu_cxx::hash<const char*>, eqstr> Set;
		Set.insert("kiwi");
		Set.insert("plum");
		Set.insert("apple");
		Set.insert("mango");
		Set.insert("apricot");
		Set.insert("banana");

		lookup(Set, "mango");
		lookup(Set, "apple");
		lookup(Set, "durian");
	}
	
	{
		//hash_map<Key, Data, HashFcn, EqualKey, Alloc>
		//支持哈希查找的关联数组，不允许键重复
		__gnu_cxx::hash_map<const char*, int, __gnu_cxx::hash<const char*>, eqstr> months;
		
		months["january"] = 31;
		months["february"] = 28;
		months["march"] = 31;
		months["april"] = 30;
		months["may"] = 31;
		months["june"] = 30;
		months["july"] = 31;
		months["august"] = 31;
		months["september"] = 30;
		months["october"] = 31;
		months["november"] = 30;
		months["december"] = 31;
		
		std::cout << "september -> " << months["september"] << std::endl;
		std::cout << "april     -> " << months["april"] << std::endl;
		std::cout << "june      -> " << months["june"] << std::endl;
		std::cout << "november  -> " << months["november"] << std::endl;
	}
	
	{
		//hash_multiset<Key, HashFcn, EqualKey, Alloc>
		//支持哈希查找的集合，允许元素重复
		__gnu_cxx::hash_multiset<const char*, __gnu_cxx::hash<const char*>, eqstr> Set;
		Set.insert("mango");
		Set.insert("kiwi");
		Set.insert("apple");
		Set.insert("kiwi");
		Set.insert("mango");
		Set.insert("mango");
		Set.insert("apricot");
		Set.insert("banana");
		Set.insert("mango");

		lookup(Set, "mango");
		lookup(Set, "apple");
		lookup(Set, "durian");
	}
	
	{
		//hash_multimap<Key, Data, HashFcn, EqualKey, Alloc>
		//支持哈希查找的集合，允许键重复
		map_type M;
		M.insert(map_type::value_type("H", 1));
		M.insert(map_type::value_type("H", 2));
		M.insert(map_type::value_type("C", 12));
		M.insert(map_type::value_type("C", 13));
		M.insert(map_type::value_type("O", 16));
		M.insert(map_type::value_type("O", 17));
		M.insert(map_type::value_type("O", 18));
		M.insert(map_type::value_type("I", 127));

		lookup(M, "I");
		lookup(M, "O");
		lookup(M, "Rn");
	}
	
	{
		//stack<T, Sequence>
		//堆栈，后进先出
		
		std::stack<int> S;
		S.push(8);
		S.push(7);
		S.push(4);
		assert(S.size() == 3);
		
		assert(S.top() == 4);
		S.pop();
		
		assert(S.top() == 7);
		S.pop();
		
		assert(S.top() == 8);
		S.pop();
		
		assert(S.empty());
	}
	
	{
		//queue<T, Sequence>
		//队列，先进先出
		std::queue<int> Q;
		Q.push(8);
		Q.push(7);
		Q.push(6);
		Q.push(2);

		assert(Q.size() == 4);
		assert(Q.back() == 2);

		assert(Q.front() == 8);
		Q.pop();

		assert(Q.front() == 7);
		Q.pop();

		assert(Q.front() == 6);
		Q.pop();

		assert(Q.front() == 2);
		Q.pop();

		assert(Q.empty());
	}
	
	{
		//priority_queue<T, Sequence, Compare>
		//优先队列，最先弹出的是队列中的最大值
		std::priority_queue<int> Q;
		Q.push(1);
		Q.push(4);
		Q.push(2);
		Q.push(8);
		Q.push(5);
		Q.push(7);
		
		assert(Q.size() == 6);
		
		assert(Q.top() == 8);
		Q.pop();
		
		assert(Q.top() == 7);
		Q.pop();
		
		assert(Q.top() == 5);
		Q.pop();
		
		assert(Q.top() == 4);
		Q.pop();
		
		assert(Q.top() == 2);
		Q.pop();
		
		assert(Q.top() == 1);
		Q.pop();
		
		assert(Q.empty());
		
		//让队列中最小元素的先出列
		//其中，模板的第二参数表示它的底层实现是std::vector<int>
		std::priority_queue< int, std::vector<int>, std::greater<int> > Q2;
		Q2.push(4);
		Q2.push(2);
		assert(Q2.top() == 2);
		Q2.pop();
		assert(Q2.top() == 4);
	}
	
	//标准C++允许main不返回，但标准C要求必须返回
	return 0;
}

