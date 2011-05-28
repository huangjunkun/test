

//using namespace test_xl_items;

#include "pch.h"
#include "add_to_trans_download.h"
#include "item001.h"

//#include "test_item_a.cpp"
//#include "test_item_b.cpp"

#ifndef LOG_FOR_TEST
#define LOG_FOR_TEST
#endif

#pragma warning (once: )

struct random_list_node
{
    random_list_node()
    : value(0), next_ptr(NULL), random_ptr(NULL)
    {}
    random_list_node(int v)
    : value(v), next_ptr(NULL), random_ptr(NULL)
    {}
    ~random_list_node() {}

    int value;
    random_list_node* next_ptr;
    random_list_node* random_ptr;
};

typedef random_list_node* random_list;

random_list_node* test_construct_list(unsigned list_size = 7)
{
    // just test ...
    if (0 == list_size)
        return NULL;
    random_list_node* head_ptr = NULL;
    head_ptr = new random_list_node;
    head_ptr->value = 0;
    random_list_node* move_ptr = head_ptr;
    for (unsigned i = 1; i < list_size; ++i)
    {
        random_list_node* new_node = new random_list_node;
        new_node->value = i;
        move_ptr->next_ptr = new_node;
        move_ptr = new_node;
    }
    move_ptr = head_ptr;
    for (unsigned i = 1; i < list_size; i+=2)
    {
        move_ptr->random_ptr = move_ptr->next_ptr->next_ptr;
        move_ptr = move_ptr->next_ptr->next_ptr;
    }
    return head_ptr;
}
///图【1】是需要复制的链表
///图【2】
///如图【2】所示，ABCD是原来的链表，A’B’C’D’是复制的链表，第一遍扫描顺序复制next指针，
///把ABCD的next分别指向A’B’C’D’，将A’的next指针指向B，B’的next指针指向C，依次类推
///复制random指针： A’->random=A->random->next
///恢复:A->next=A’->next;A’->next=A’->next->next;
random_list_node* copy_random_list(random_list_node* head_ptr)
{
    if (NULL == head_ptr) return NULL;

    random_list_node* new_head_ptr = new random_list_node;
    new_head_ptr->value = head_ptr->value;
    new_head_ptr->next_ptr = head_ptr->next_ptr;
    head_ptr->next_ptr = new_head_ptr;
    // copy random_list
    random_list_node* move_ptr = new_head_ptr->next_ptr;
    while (move_ptr)
    {
        random_list_node* new_node = new random_list_node;
        new_node->value = move_ptr->value;
        new_node->next_ptr = move_ptr->next_ptr;
        move_ptr->next_ptr = new_node;
        move_ptr = new_node->next_ptr;
    }

    // scan random_list twice, for random_ptr and next_ptr.
    move_ptr = head_ptr;
    while (move_ptr)
	{
		random_list_node* new_move_ptr = move_ptr->next_ptr;
        if (move_ptr->random_ptr)
            new_move_ptr->random_ptr = move_ptr->random_ptr->next_ptr;

		assert(NULL != new_move_ptr);
        move_ptr = new_move_ptr->next_ptr;
        //new_move_ptr = move_ptr->next_ptr;
    }

    move_ptr = head_ptr;
    random_list_node* new_move_ptr = move_ptr->next_ptr;
    while (new_move_ptr)
    {
        move_ptr->next_ptr = new_move_ptr->next_ptr;
		if (new_move_ptr->next_ptr)
			new_move_ptr->next_ptr = new_move_ptr->next_ptr->next_ptr;
        move_ptr = move_ptr->next_ptr;
        new_move_ptr = new_move_ptr->next_ptr;
    }

    return new_head_ptr;
}

void deallocate_list(random_list_node* head_ptr)
{
#ifdef LOG_FOR_TEST
    unsigned delete_count = 0;
#endif
    while (head_ptr)
    {
        random_list_node* delete_node = head_ptr;
        head_ptr = head_ptr->next_ptr;
        delete delete_node;
        ++delete_count;
    }
#ifdef LOG_FOR_TEST
    std::cout << " deallocate_list -> delete_count:" << delete_count << "\n";
#endif
    return;
}

void print_list(ostream& os, random_list_node* head_ptr)
{
    unsigned index = 0;
    os << "[index](value, random_ptr->value): ";
    while (head_ptr)
    {
        os << "[" << index << "](" << head_ptr->value << ", ";
        if (head_ptr->random_ptr)
            os << head_ptr->random_ptr->value << ") ";
        else
            os << "NULL) ";

        head_ptr = head_ptr->next_ptr;
		++index;
    }

    os << std::endl;
    return;
}

int main()
{
    {
    random_list test_list = test_construct_list(1);
    random_list test_list_bak = copy_random_list(test_list);
    std::cout << ">---   test_list   --->\n";
    print_list(std::cout, test_list);
    std::cout << "<---   test_list   ---<\n";
    std::cout << ">---   test_list_bak   --->\n";
    print_list(std::cout, test_list_bak);
    std::cout << "<---   test_list_bak   ---<\n";
    deallocate_list(test_list);
    deallocate_list(test_list_bak);
    }
/**
    {
    using namespace test_namespace;
    pipe_capacity pipes[6] = { pipe_capacity(15, 115 ,100),
                               pipe_capacity(25, 25 ,150),
                               pipe_capacity(45, 45 ,10),
                               pipe_capacity(35, 135 ,1),
                               pipe_capacity(65, 65 ,1),
                               pipe_capacity(55, 55 ,1),
                                };
    const unsigned PIPES_COUNT = sizeof(pipes) / sizeof(pipe_capacity);
    for (unsigned i =0; i < PIPES_COUNT; ++i)
        std::cout << pipes[i] << "\t";
    std::cout << "\n";
    std::sort(pipes, pipes+PIPES_COUNT, pipe_capacity_greater());

    for (unsigned i =0; i < PIPES_COUNT; ++i)
        std::cout << pipes[i] << "\t";
    std::cout << "\n";
    }


/*
    {
    int nums[6] = {2, 1, 5, 3, 0, 4};
    const unsigned NUMS_LEN = sizeof(nums) / sizeof(int);
    for (unsigned i = 0; i < NUMS_LEN; ++i)
        std::cout << " " << nums[i];
    std::cout << "\n";
    std::partial_sort(nums, nums+6, nums+NUMS_LEN, greater<int>());

    for (unsigned i = 0; i < NUMS_LEN; ++i)
        std::cout << " " << nums[i];
    std::cout << "\n";
    }
/*
    {
    std::map<int, string> test_map;
    test_map[1] = "11111";
    std::map<int, string>::iterator pos = test_map.end();
    std::map<int, string>::iterator pos1 = test_map.begin();
    std::cout << " "<< pos1->first << ", " << pos1->second << "\n";// okokok
    std::cout << " "<< pos->first << ", " << pos->second << "\n";// dangerous


    }
/*
    {
    test_struct a1(1), a2(2), a3(3);
    std::vector<test_struct*> test_vec;
    test_vec.push_back(&a1);
    test_vec.push_back(&a2);
    test_vec.push_back(&a3);
    for (std::vector<test_struct*>::iterator iter = test_vec.begin();
    iter != test_vec.end(); ++iter)
    {
		//std::cout << iter->_a << ", " << iter->_string << "\n";// complier error
		//std::cout << iter->next_ptr << "\n";// complier error
		std::cout << iter->str() << "\n";// complier error
		std::cout << (*iter)->_a << ", " << (*iter)->_string << "\n";
    }

    }
/*
    {
    std::set<std::string> test_set;
    test_set.insert("1111");
    test_set.insert("2222");
    test_set.insert("3333");
    std::set<std::string>::iterator iter = test_set.find("4444");
    if (iter != test_set.end())
    {
        std::cout << " (iter != test_set.end())\n";
        std::cout << "*iter: " << *iter << "\n";
    }
    else
        std::cout << " (iter == test_set.end())\n";

    }
/*
    {
    for (size_t i = 0, double d = 9.99; i < 10; i++)
    {
        std::cout  << " i:" << i << ", d:" << "\n";
    }
    }
/*
//    {
//    const std::string& str = return_string();
//    std::cout << append_string(str) << "\n";
//    }
/*
    {
    using namespace std;
	using namespace test_namespace;

    unsigned args = 5;
 	const unsigned THREAM_SUM = 5;
	/// test 0 ... 【故意的】complie error ...
// 	thread_base *pThread0 = new thread_base(NULL);
// 	delete pThread0;
	/// test 1 ...
// 	test_thread2* pArray = new test_thread2[THREAM_SUM];
// 	for (size_t i=0; i < THREAM_SUM; i++)
// 		::WaitForSingleObject(pArray[i].handle(), INFINITE);
//
// 	delete[] pArray;

	/// test 2 ...
//	auto_ptr<test_thread> pTest1(new test_thread(&args));
//	pTest1->resume();/// on-or-off
//    if (::WaitForSingleObject(pTest1->handle(), 10000) == WAIT_TIMEOUT)
//        std::cout << " The time-out interval 10s elapsed, and the object's state is nonsignaled.\n";

	/// test 3 ...
//     test_thread2 test2;
//     //test2.resume();/// on-or-off
//     if (::WaitForSingleObject(test2.handle(), 10000) == WAIT_TIMEOUT)
//        std::cout << " The time-out interval 10s elapsed, and the object's state is nonsignaled.\n";

	/// test 4 ...
	/// 【注】test_thread, test_thread2 线程类不适宜结合有构造参数的std::vector使用，
	///  可能因为std::vector构造函数内部的复制构造过程导致句柄[HANDLE]失效，详细原因可进步深究！
    std::vector<test_thread2> thread_vec(5, &args);///or test_thread
    std::cout << " " << thread_vec.size() << "\n";
    for (size_t i=0; i < thread_vec.size(); i++)
    {
        ::WaitForSingleObject(thread_vec[i].handle(), INFINITE);// ?...
        std::cout << "WaitForSingleObject Pass.\n";
    }
	/// test 5 【注】可能打印出现错乱，属于IO(std::cout)中断操作导致。
// 	test_thread threads[THREAM_SUM];
// 	for (size_t i=0; i < THREAM_SUM; i++)
// 		threads[i].resume();
//
// 	for (size_t i=0; i < THREAM_SUM; i++)
// 		::WaitForSingleObject(threads[i].handle(), INFINITE);
//
// 	test_thread2 threads2[THREAM_SUM];
// 	for (size_t i=0; i < THREAM_SUM; i++)
// 		::WaitForSingleObject(threads2[i].handle(), INFINITE);

	//////////////////////////////////////////////////////////////////////////
    std::cout << " all threads are finish !\n";
    }
/*
    {

    int a, b;
    // as long as input of a and b is successful
    while (std::cin >> a >> b)
    {
       // output a and b
       std::cout << "a: " << a << " b: " << b << std::endl;
    }
    }
/*
    {
#if 1
	char buf[1024];
	std::strstream stream(buf,sizeof(buf),
		std::ios::out | std::ios::binary);
#else
	std::fstream stream("fas.xx",std::ios::in
		| std::ios::out | std::ios::binary
		| std::ios::trunc);
#endif
	int x = stream.tellg();
	int y = stream.tellp();
	char xxx[512];
	stream.write(xxx,sizeof(xxx));
	x = stream.tellg();
	y = stream.tellp();
	return 0;
    }
    {
    #define TEST_NUM 0xff
	char buf[1024];
	std::strstream sstream(buf,sizeof(buf),
		std::ios::out | std::ios::binary);

    //strstream sstream;

    sstream << (unsigned char) TEST_NUM;
    std::cout << sstream.pcount() << "\n";
    sstream.clear();
    sstream << (int ) TEST_NUM;
    std::cout << sstream.pcount() << "\n";


    std::strstream sstream1;
    sstream1 << (unsigned char) TEST_NUM;
    std::cout << sstream1.pcount() << "\n";

    std::strstream sstream2;
    sstream2 << (int ) TEST_NUM;
    std::cout << sstream2.pcount() << "\n";

    }
    {
    strstream x;
    x << "test1";
    cout << x.rdbuf( )->pcount( ) << endl;
    x << "test2";
    cout << x.rdbuf( )->pcount( ) << endl;
    }
/*
    std::cout << " ." << test() << "\n";
/*
    {
    std::vector<int> vec1, vec2;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);
    vec2 = vec1;//test defalut operator =(...)
    std::cout << vec1.size() << "\n";
    std::cout << vec2.size() << "\n";
    }
/*
    double dNum = 100;
    int iNum = 3;
    ETestData eNum = TEST_DATA1;
    std::cout << " " << iNum << ", " << eNum << "\n";
    setValue((int&)eNum, iNum);
    std::cout << " " << iNum << ", " << eNum << "\n";


    /*
        {
        using namespace test_namespace;
        std::cout << " " << test_t<double>::s_value << "\n";
        test_t<int>::s_value = 5;
        std::cout << " " << test_t<int>::s_value << "\n";
        std::cout << " " << test_t<double>::s_value << "\n";
        }
        {
        DWORD a1 = 101, a2 = 100;
        volatile UINT32 a3;
        a3 = a1 / a2;
        std::cout << " " << a3 << "\n";

        }
    /*
        {
        using namespace std;
        using namespace test_namespace;
        CombineFileParamInfo*   pData = new CombineFileParamInfo;
        pData->strDesFileName = L"111111";
        pData->vSrcFileName.push_back(L"222222");
        pData->vSrcFileName.push_back(L"222222");

        CombineFileParamInfo temp;
        temp = *pData;
        wcout << " " << temp.vSrcFileName.size() << "\n";
        for (size_t i=0 ;i < temp.vSrcFileName.size(); i++)
            wcout << " " << temp.vSrcFileName[i] << "\n";

        wcout << " " << temp.vSrcFileName.size() << "\n";

        delete pData;
        }
    /*
        {
        using namespace std;
        using namespace test_namespace;
    //    /// test ...
    //    TestCEntity();
    //    /// test ...
    //    CTestDeleteVoidPtr* pObj1 = new CTestDeleteVoidPtr;
    //    SAFE_DELETE(pObj1);
    //    void* pObj2 = new CTestDeleteVoidPtr;
    //    SAFE_DELETE(pObj2) ;///
        /// test ...  very surprised !!!
        CTestDeleteVoidPtr obj;
            cout << *((string*)obj.data_ptr()) << "\n";

        const unsigned OBJ_COUNT = 5;
        /// 11111111111
        vector<CTestDeleteVoidPtr> obj_vec(OBJ_COUNT);
    //    obj_vec.push_back(obj);
    //    obj_vec.push_back(obj);
    //    obj_vec.push_back(obj);
    //    obj_vec.push_back(obj);
    //    obj_vec.push_back(obj);
        for (size_t i=0; i < obj_vec.size(); i++)
            cout << *((string*)obj_vec[i].data_ptr()) << "\n";
        /// 22222222222
    //    CTestDeleteVoidPtr array[OBJ_COUNT];
    //    for (size_t i=0; i < OBJ_COUNT; i++)
    //        cout << *((string*)array[i].data_ptr()) << "\n";

        cout << " ... end ...\n";
        void* pNull = NULL;
        delete pNull; /// c++ standard ok
        }
    /*
        {
        using namespace test_namespace;
        /// 0. testing ...
        cout_buffer << 1 << 2 << 3 << 4 << 5;
        unsigned ui1, ui2, ui3, ui4, ui5;
        cout_buffer >> ui1 >> ui2 >> ui3 >> ui4 >> ui5;
        cout << ui1 << ", " << ui2 << ", " << ui3 << ", " << ui4 << ", " << ui5 << "\n";
        /// 1. testing ...
        cout_buffer.reset();
        cout_buffer << "hello";
        cout_buffer << " world! \n";
        string res;
        cout_buffer >> res;
        std::cout << " res: " << res << "\n";
        cout_buffer >> res;
        std::cout << " res: " << res << "\n";

        std::cout << " " << cout_buffer.length() << ", " << cout_buffer.capability() << "\n";
        /// 2. testing ...
        cout_buffer.reset();
        CTestCoutBuffer test_obj;
        cout_buffer << test_obj;
        std::cout << " " << cout_buffer.to_string() << "\n";
        //std::cout << " " << cout_buffer.buffer() << "\n";// runtime error ...
        /// 3. testing ...
        cout_buffer.reset();
        vector<string> str_vec1, str_vec2;
        str_vec1.push_back("1111");
        str_vec1.push_back("2222");
        str_vec1.push_back("3333");
        cout_buffer << str_vec1;

        cout_buffer >> str_vec2;
        for (size_t i=0; i < str_vec2.size(); i++)
            std::cout << " " << str_vec2[i] << "\n";

        /// 4. testing ...
        cout_buffer.reset();
        vector<CTestCoutBuffer> test_vec1, test_vec2;
        test_obj.setValue("aaaaa");
        test_vec1.push_back(test_obj);
        test_obj.setValue("bbbbb");
        test_vec1.push_back(test_obj);
        test_obj.setValue("ccccc");
        test_vec1.push_back(test_obj);
        cout_buffer << test_vec1;

        cout_buffer >> test_vec2;
        for (size_t i=0; i < test_vec2.size(); i++)
            std::cout << " " << test_vec2[i].value() << "\n";

        /// 5. testing ...
        wcout_buffer.reset();
        vector<wstring> wstr_vec1, wstr_vec2;
        wstr_vec1.push_back(L"1111");
        wstr_vec1.push_back(L"2222");
        wstr_vec1.push_back(L"3333");
        wcout_buffer << wstr_vec1;

        wcout_buffer >> wstr_vec2;
        std::wcout << " " << wstr_vec2.size() << "\n";

        for (size_t i=0; i < wstr_vec2.size(); i++)
        {
            std::wcout << L" " << wstr_vec1[i] << L", "<< wstr_vec2[i] << L"\n";
            //std::wcout << L" " << wstr_vec2[i].size() << L"\n";
        }

        std::cout << " " << sizeof(unsigned) << ", " << sizeof(wchar_t) << "\n";
        }
    /*
        {
        using namespace std;
        using namespace TestSpace;
        /// 0. testing ...
        cout_buffer << 1 << 2 << 3 << 4 << 5;
        unsigned ui1, ui2, ui3, ui4, ui5;
        cout_buffer >> ui1 >> ui2 >> ui3 >> ui4 >> ui5;
        cout << ui1 << ", " << ui2 << ", " << ui3 << ", " << ui4 << ", " << ui5 << "\n";
        /// 1. testing ...
        cout_buffer.reset();
        cout_buffer << "hello";
        cout_buffer << " world! \n";
        string res;
        cout_buffer >> res;
        std::cout << " res: " << res << "\n";
        cout_buffer >> res;
        std::cout << " res: " << res << "\n";

        std::cout << " " << cout_buffer.length() << ", " << cout_buffer.capability() << "\n";
        /// 2. testing ...
        cout_buffer.reset();
        CTestCoutBuffer test_obj;
        cout_buffer << test_obj;
        std::cout << " " << cout_buffer.to_string() << "\n";
        //std::cout << " " << cout_buffer.buffer() << "\n";// runtime error ...
        /// 3. testing ...
        cout_buffer.reset();
        vector<string> str_vec1, str_vec2;
        str_vec1.push_back("1111");
        str_vec1.push_back("2222");
        str_vec1.push_back("3333");
        cout_buffer << str_vec1;

        cout_buffer >> str_vec2;
        for (size_t i=0; i < str_vec2.size(); i++)
            std::cout << " " << str_vec2[i] << "\n";

        /// 4. testing ...
        cout_buffer.reset();
        vector<CTestCoutBuffer> test_vec1, test_vec2;
        test_obj.setValue("aaaaa");
        test_vec1.push_back(test_obj);
        test_obj.setValue("bbbbb");
        test_vec1.push_back(test_obj);
        test_obj.setValue("ccccc");
        test_vec1.push_back(test_obj);
        cout_buffer << test_vec1;

        cout_buffer >> test_vec2;
        for (size_t i=0; i < test_vec2.size(); i++)
            std::cout << " " << test_vec2[i].value() << "\n";
        /// 5. testing ...
        CTestCoutBuffer2    test_obj2;
        cout_buffer.reset();
        vector<CTestCoutBuffer2> test_vec3, test_vec4;
        test_obj2.setValue("aaaaa");
        test_vec3.push_back(test_obj2);
        test_obj2.setValue("bbbbb");
        test_vec3.push_back(test_obj2);
        test_obj2.setValue("ccccc");
        test_vec3.push_back(test_obj2);
        cout_buffer << test_vec3;

        cout_buffer >> test_vec4;
        for (size_t i=0; i < test_vec4.size(); i++)
            std::cout << " " << test_vec4[i].value() << "\n";

        //cout <<
        }
    /*
        {
        using namespace test_xl_items;
        uint8  a_uint8  = 0x12;
        uint16 a_uint16 = 0x1234;
        uint32 a_uint32 = 0x12345678;
        uint64 a_uint64 = 0x1234567890abcdef;
        std::cout << " " << TestBigEndian() << "\n";

        std::cout << hex << a_uint8 << "\n";
        std::cout << hex << a_uint16 << "\n";
        std::cout << hex << a_uint32 << "\n";
        std::cout << hex << a_uint64 << "\n";

        std::cout << hex << hton(a_uint8) << "\n";
        std::cout << hex << hton(a_uint16) << "\n";
        std::cout << hex << hton(a_uint32) << "\n";
        std::cout << hex << hton(a_uint64) << "\n";

        std::cout << " " << CLOUD_TRANS_OPER_QUERY << "\n";
        }
    /*
        {
        using namespace TestSpace;
        cout << " test_big_endian:" << test_big_endian() << " ,test_little_endian:" << test_little_endian() << "\n";
        cout << " test_big_endian:" << test_big_endian() << " ,test_little_endian:" << test_little_endian() << "\n";
        cout << " test_big_endian:" << test_big_endian() << " ,test_little_endian:" << test_little_endian() << "\n";


        CWidget widget;
        cout << " " << widget.Length() << "\n";
        }

    /*
        {
        struct test_struct
        {
          const int * int_ptr;
        };
        const int& int_cref = int(999);
        test_struct test;
        test.int_ptr = &int_cref;
        std::cout << " " << *test.int_ptr << "\n";
        //int_cref = 111;
        std::cout << " " << *test.int_ptr << "\n";
        }
    /*
        {
        using namespace TestSpace;
        a_test_struct a_test;
        a_test._value = 1000;
        int value_test;
        paster(9);
        PRINT_TEST_STRING1(value_test);
        PRINT_TEST_STRING2(a_test, VALUE_NAME);
        int  int_value001 = 1111;
        PRINT_TEST_STRING3(int_value, 001);
        }

    /*
        {
        using namespace TestSpace;
        CBase* pBase = new CDerived;
        cout << pBase->Hello() << "\n";
        }

        #define MAX 100 //as a golbal macro definition

        {
        cout << " MAX: " << MAX << "\n";
        #define MAX 10
        cout << " MAX: " << MAX << "\n";
        #define MAX 20
        cout << " MAX: " << MAX << "\n";
        #define MAX 30
        cout << " MAX: " << MAX << "\n";
        #undef MAX

        // E:\codeblocks\Hello\main.cpp|35|error: 'MAX' was not declared in this scope|
    //    cout << " MAX: " << MAX << "\n";
    //    #undef MAX
    //    cout << " MAX: " << MAX << "\n";
    //    #undef MAX
    //    cout << " MAX: " << MAX << "\n";

        }
    /*
        {
        int state_tmp = 0x11;
        cout << boolalpha << !(state_tmp & (state_tmp-1)) << noboolalpha << "\n";

        using namespace TestSpace;
        const vector<int> cref = return_vec_ref();
        cout << " " << cref.size() << "\n";
        vector<int> int_vec(3, 1);
        int_vec = return_vec_ref();
        cout << " " << int_vec.size() << "\n";
        int_vec.clear();
        cout << " " << cref.size() << "\n";
        cout << " " << int_vec.size() << "\n";
        string str("0123456789");
    //    std::copy(str.begin()+5, str.end(), str.begin());
    //    str.resize(str.size()-5);
        str.assign(str.begin()+5, str.end());
        cout << " "<< str << "\n";
        }
    /*
        {
        const char* file_name = "g:\\tmp.txt";
        HANDLE hfile = ::CreateFile(file_name,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
            NULL);

        thunder_assert(hfile != INVALID_HANDLE_VALUE);
        int64_t range_pos;
        bool get_ok = ::GetFileSizeEx(hfile, range_pos);
        thunder_assert(get_ok);
        cout << " " << range_pos << "\n";

        CloseHandle(hfile);
        }
    /*
        {
        string str;
        std::find(str.begin(), str.end(), '\n');
        using namespace TestSpace;
        test_to_string(str);
        cout << "" << str << "\n";
    //    char chars[50];
    //    sprintf(chars, "%i, %s", 1, "234");
    //    cout << (chars) << "\n";
    //    cout << strlen(chars) << "\n";
        }
    /*
        {
        using namespace test_namespace;
        trans_format_header header;
        /// 1
        header.request_cmd_get("/data/media/", 0, "Chrome 7.2");
        std::cout << " " << header.requestheader() << "\n";
        const std::string SAMPLE1 = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nAccept-Ranges: bytes\r\n\r\n111";
        cout << " SAMPLE1.size(): " << SAMPLE1.size() << "\n";
        cout << " " << header.accpet_response_cmd(SAMPLE1) << "\n";
        std::cout << " " << header.responseheader() << "\n";
    //    cout << " " << header.accpet_response_cmd(SAMPLE1) << "\n";//assert fail ...
        ///
        header.reset();
        /// 2
        const std::string SAMPLE2 = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nAccept-Ranges: bytes\r\n\r";
        const std::string SAMPLE3 = "\n111";
        cout << " SAMPLE3.size(): " << SAMPLE3.size() << "\n";
    //    cout << " " << header.accpet_response_cmd(SAMPLE2) << "\n";//assert fail ...
    //    std::cout << " " << header.responseheader() << "\n";
    //    cout << " SAMPLE3.size(): " << SAMPLE3.size() << "\n";
    //    cout << " " << header.accpet_response_cmd(SAMPLE3) << "\n";//assert fail ...
    //    std::cout << " " << header.responseheader() << "\n";
        header.request_cmd_get("/data/media/", 0, "Chrome 7.2");
        cout << " " << header.accpet_response_cmd(SAMPLE2) << "\n";//ok
        std::cout << " header_is_full: " << header.header_is_full() << "\n";
        std::cout << " " << header.responseheader() << "\n";
        cout << " " << header.accpet_response_cmd(SAMPLE3) << "\n";//ok
        std::cout << " header_is_full: " << header.header_is_full() << "\n";
        std::cout << " " << header.responseheader() << "\n";
        std::cout << " " << header.has_M_Range_field() << "\n";
        /// 3
        const string SAMPLE4 = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nM-Range: bytes=16-4; 58-16; 100-99\r\n\r\n";
        cout << " SAMPLE4.size(): " << SAMPLE4.size() << "\n";
        header.reset();
        header.request_cmd_get("/data/media/", 500, "Chrome 7.2");
        cout << " " << header.accpet_response_cmd(SAMPLE4) << "\n";//ok
        std::cout << " " << header.responseheader() << "\n";
        if (header.has_M_Range_field())
        {
            cout << " header has _M_Range field ... M-Range:\n";
            const std::vector<range>& m_ranges = header.m_range_vec();
            std::vector<range>::const_iterator citer = m_ranges.begin();
            for (; citer != m_ranges.end(); citer++)
            {
                cout << (*citer) << "\n";
            }
        }
        cout << " " << header.get_response_line(0) << "\n";
        cout << " " << header.get_response_line(1) << "\n";
        cout << " " << header.get_response_line(2) << "\n";
        cout << " " << header.get_response_line(3) << "\n";
        cout << " " << header.get_response_line(4) << "\n";
        cout << " " << header.get_response_line(5) << "\n";


        }
    /*
        {
        using namespace TestSpace;
        int state = 0;
        state |= S_INIT;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state |= TestSpace::S_CONNECTED;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state |= TestSpace::S_READING;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state |= TestSpace::S_SENDING;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state &= ~TestSpace::S_READING;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state &= ~TestSpace::S_CONNECTED;
        std::cout << " " << handle_io_state2str(state) << "\n";
        state &= ~TestSpace::S_INIT;
        string str_state = handle_io_state2str(state);
        if (str_state.empty()) std::cout << " str_state.empty()\n";
        std::cout << " " << str_state << "\n";
        }
    /*
        {
        size_t len1 = 10, len2 = 12;
        cout << " " << len1 - len2 << "\n";
        int len = len1 - len2;
        cout << " " << len << "\n";
        }

    /*
        {
        using namespace TestSpace;
        TSingleFile file("write.txt");
        char str[1024*10] = {'0'};
        memset(str, str[0], sizeof(str));
        file.write(str, sizeof(str));
        file.close();

    //    file.open("write.txt", "rb+");
        char to_str[1024*10] = {'1'};
        memset(str, to_str[0], sizeof(str));
        std::cout << " " << to_str[0] << "\n";
        file.read(to_str, sizeof(to_str));
        std::cout << " " << to_str[0] << "\n";

        }
    /*
        {
        ifstream fin("g:\tmp.txt");
        ofstream fout("g:\tmp_to");
    //    string str;
    //    str.getline()
        string tmp_str;
        while(fin >> tmp_str )
        {
            string to_str(tmp_str.rbegin(), tmp_str.rend());
            fout << to_str << "\n";
            std::cout << to_str << "\n";
        }
        fout << " hello world" << "\n";

        }

    /*
        {
        map<int, std::string>    map_i;
        map_i[1] = "one";
        map_i[2] = "two";
        map_i[3] = "three";
        map_i[4] = "four";
        map_i[5] = "five";
        map_i[6] = "six";
        std::string& str = map_i[7];
        str = "seven";
        typedef map<int, std::string> map_int_string;
        typedef map_int_string::iterator map_int_string_it;
        map_int_string_it it = map_i.find(7);
        if(it != map_i.end())
            std::cout << " " << it->second << "\n";
        else
            std::cout << " it == map_i.end() \n";

        typedef pair<map_int_string_it, bool> pair_bool_map_it;
        pair_bool_map_it    p_it = map_i.insert(std::make_pair(2, "two"));
        if (p_it.second)
        {
            std::cout << " insert ok ...\n";
        }
        else
        {
            std::cout << " insert fail ...\n";
        }
        it = p_it.first;
        std::cout << " " << it->first << ", "<< it->second << "\n";
        std::cout << " " << map_i.size() << "\n";

        it = (map_i.find(2));//return void;
        map_i.erase(it);
    //    if (it != map_i.end())
    //        std::cout << " " << it->first << ", " << it->second << "\n";
        std::cout << " " << map_i.size() << "\n";

        }
    /*
        {
        vector<int> vec_int;//(3, 1);
        cout << " " << vec_int.size() << "\n";
        vector<int>::iterator it = vec_int.begin();
        vec_int.erase(it);
        cout << " " << vec_int.size() << "\n";
        }
    /*
        {
        const int int_max = 0xffffffff;
        cout << " " << int_max << ", " << static_cast<unsigned>(int_max) << "\n";
        vector<int> vec_int;
        typedef vector<int>::value_type value_type;
        cout << " " << sizeof(value_type) << "\n";
        }
    /*
        {
        using namespace TestSpace;
        piece_info  p1;
        std::cout << sizeof(p1) << " : sizeof(p1)\n";
        std::cout << " 5 times : p1._down_pipes.push_back(1); ... \n";
        int temp;
        p1._down_pipes.push_back(&temp);
        p1._down_pipes.push_back(&temp);
        p1._down_pipes.push_back(&temp);
        p1._down_pipes.push_back(&temp);
        p1._down_pipes.push_back(&temp);
        std::cout << sizeof(p1) << " : sizeof(p1)\n";
        std::cout << p1._down_pipes.size() << " : p1._down_pipes.size()\n";

        piece_info  p2 = p1;
        std::cout << sizeof(p2) << " : sizeof(p2)\n";
        std::cout << p2._down_pipes.size() << " : p2._down_pipes.size()\n";

        std::cout << &p1._down_pipes[0] << " : &p1._down_pipes\n";
        std::cout << &p2._down_pipes[0] << " : &p2._down_pipes\n";
        }
    /*
        {
        typedef map<int,string> MapIntStr;
        MapIntStr _i2str;
        _i2str[1] = "one";
        _i2str[2] = "two";
        _i2str[3] = "three";
        _i2str[4] = "four";
        MapIntStr::const_iterator   cit = _i2str.begin();
        std::cout << (*cit).second << "\n";
        std::cout << &(*cit) << "\n";
        cit--;
        std::cout << &(*cit) << "\n";
    //    std::cout << (*cit).second << "\n";// error
        }

    /*
        {
        using namespace TestSpace;
        TestSpace::TestFunc(auto_ptr<CWidget>(new CWidget).get()) ;
        TestSpace::TestFunc(new CWidget) ;
        }

    ///*
        {
        unsigned unum = UINT_MAX;//0xffffffff
        int num = unum;
        std::cout << unum << "(unum), " << num << "(num) \n";
        std::cout << sizeof(Ecolor) << ", " << sizeof(black) << "\n";
        Ecolor color_flags = (Ecolor)0xf; //black|white|red|green;
        std::cout << color_flags << "\n";
        }
    /**
        {
    //    func1(100);
    //    func2(100);
        }

        {
    //    int i = 0;
        int a[10];
        for(int i = 0; i <= 10; i++)
        {// Debug -- error, Release -- OK
            a[i] = 0;
            std::cout << i << "\n";
        }
        }
        /** =========================*/
    cout << "Hello world!" << endl;
    return 0;
}
