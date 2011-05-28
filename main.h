#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <exception>
#include <process.h>

using namespace std;
//#include <F:/download_lib_xl7/dl_common/common/range.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) do { if (ptr) delete ptr; ptr = NULL; } while(0)
#endif



namespace test_namespace
{
    static int Hash(const char *z, int n)
    {
        int h = 0;
        if( n<=0 )
        {
            n = strlen(z);
        }
        while( n-- )
        {
            h = h ^ (h<<5) ^ *z++;
        }
        if( h<0 ) h = -h;
        return h;
    }


    template <typename T>
    class test_t
    {
    public:
        static T s_value;
    };

    template <typename T>
    T test_t<T>::s_value = 0;
}

///
namespace test_namespace
{

#pragma warning( push)
#pragma warning(disable : 4290)

unsigned __stdcall lanuch_thread_proc(void* args);

template <class T>
class thread_launcher_t
{
public:
    thread_launcher_t()
        : _pObj(0), _pArgs(0)
    {}
    thread_launcher_t(T* pObj)
        : _pObj(pObj), _pArgs(0)
    {}
    thread_launcher_t(void* pArgs)
        : _pObj(0), _pArgs(pArgs)
    {}
    thread_launcher_t(T* pObj, void* pArgs)
        : _pObj(pObj), _pArgs(pArgs)
    {}
    void set_launcher(T* pObj, void* pArgs)
    {
        _pObj = pObj;
        _pArgs = pArgs;
    }
    void set_args(void* pArgs)
    {
        _pArgs = pArgs;
    }
    void set_obj(T* pObj)
    {
        _pObj = pObj;
    }

    unsigned launch()
    {
        assert(_pObj);// && _pArgs
        return _pObj->thread_proc(_pArgs);
    }
private:
    T*      _pObj;
    void*   _pArgs;
};

class thread_base;
typedef thread_launcher_t<thread_base> thread_launcher;

class thread_base
{
public:
    typedef unsigned ( *start_address )( void * );

    static void* operator new(std::size_t size) throw(std::bad_alloc)
    {
        std::cout << " thread_base::operator new(std::size_t size) throw(std::bad_alloc) ...\n";
        return ::operator new(size);
    }
    static void operator delete(void * mem) throw()
    {
        std::cout << " thread_base::operator delete(void * mem) throw() ...\n";
        ::operator delete(mem);
    }

    static void* operator new(std::size_t size, const std::nothrow_t& nt) throw()
    {
        std::cout << " thread_base::operator new(std::size_t size, const std::nothrow_t& nt) throw() ...\n";
        return ::operator new(size, nt);
    }
    static void operator delete(void * mem, const std::nothrow_t& nt) throw()
    {
        std::cout << " thread_base::operator delete(void * mem, const std::nothrow_t& nt) throw()...\n";
        return ::operator delete (mem);
    }
protected:
    // 不允许静态创建，可以 new ???
    thread_base(void* args, bool is_create_suspended=false)
        : _handle_thread(0), _thread_id(0), _launcher(args) //, _launcher(this, args)
    {
        _do_constor(is_create_suspended);
    }
    virtual ~thread_base()
    {
        if (_handle_thread)
        {
            //_endthreadex(0);
            CloseHandle(_handle_thread);
        }
    }
public:
    HANDLE handle() const
    {
        return _handle_thread;
    }
    unsigned thread_id() const
    {
        return _thread_id;
    }

    int resume()
    {
        assert(_handle_thread);
        return ::ResumeThread(_handle_thread);
    }
    int suspend()
    {
        assert(_handle_thread);
        ::SuspendThread(_handle_thread);
    }
    //////////////////////////////////////////////////////////////////////////
    virtual unsigned thread_proc(void* args) = 0;
private:
    void _do_constor(bool is_create_suspended)
    {
        unsigned init_flag;
        if (is_create_suspended)
            init_flag = CREATE_SUSPENDED;
        else
            init_flag = 0;
        _launcher.set_obj(this);
        _handle_thread = (HANDLE)_beginthreadex(NULL, 0, &thread_base::_lanuch_thread_proc, (void*)&_launcher, init_flag, &_thread_id);//__stdcall
        assert(_handle_thread);
    }

    static unsigned __stdcall _lanuch_thread_proc(void* args);

protected:
    HANDLE		_handle_thread;
    unsigned	_thread_id;
    thread_launcher    _launcher;
};
// static member function ...
unsigned  __stdcall thread_base::_lanuch_thread_proc(void* args)
{
    thread_launcher* pObj = reinterpret_cast<thread_launcher*> (args);
    return pObj->launch();
    _endthreadex(0);
}

unsigned thread_base::thread_proc(void* args)
{
    std::cout << " unsigned thread_base::thread_proc(void* args) ...\n";
    return 0;
}

// global function ...
unsigned __stdcall lanuch_thread_proc(void* args)
{
    thread_launcher* pObj = reinterpret_cast<thread_launcher*> (args);
    return pObj->launch();
    _endthreadex(0);
}
//////////////////////////////////////////////////////////////////////////

template <bool IsCreateSuspended>
class test_thread_t : public thread_base
{
public:
    static void* operator new(std::size_t size) throw(std::bad_alloc)
    {
        std::cout << " test_thread_t::operator new(std::size_t size) throw(std::bad_alloc)...\n";
        return ::operator new(size);
    }
    static void operator delete(void * mem) throw()
    {
        std::cout << " test_thread_t<IsCreateSuspended>::operator delete(void* mem, size_t size) throw()...\n";
        ::operator delete (mem);
    }

    static void* operator new(std::size_t size, const std::nothrow_t& nt) throw()
    {
        std::cout << " operator test_thread_t::new(std::size_t size, const std::nothrow_t& nt) throw()...\n";
        return ::operator new(size, nt);
    }
    static void operator delete(void * mem, const std::nothrow_t& nt) throw()
    {
        std::cout << " operator test_thread_t::delete(void * mem, const std::nothrow_t& nt) throw()...\n";
        return ::operator delete (mem);
    }

    static void* operator new[](std::size_t size) throw(std::bad_alloc);
    static void operator delete[](void * mem);

    static void* operator new(std::size_t size, void *ptr) throw();
    static void operator delete(void * mem, void *ptr) throw();
    // 	static void* operator new[](std::size_t size, void *ptr) throw();
    // 	static void operator delete[](void * mem, void *ptr);
public:
    test_thread_t() : thread_base(NULL, IsCreateSuspended)
    { std::cout << "test_thread_t()" << IsCreateSuspended << "\n"; }
    test_thread_t(void* args)
        : thread_base(args, IsCreateSuspended)
    { std::cout << "test_thread_t(void* args)" << IsCreateSuspended << "\n"; }
//    test_thread_t(void* args, bool is_create_suspended)
//        : thread_base(args, is_create_suspended)
//    { std::cout << "test_thread_t(void* args, bool is_create_suspended)" << is_create_suspended << "\n"; }
    virtual ~test_thread_t() { std::cout << "~test_thread_t()" << IsCreateSuspended << "\n"; }

    virtual unsigned thread_proc(void* args)
    {
        unsigned count_sum = 5;
        if (0 != args)
            count_sum = *static_cast<unsigned*>(args);

        unsigned tick_count = 0;
        while (tick_count < count_sum)
        {
            std::cout << _thread_id << "(thread_id), ... tick count(1 tick/s), tick_count -> " << ++tick_count << "\n";
            Sleep(1000);
        }
        return 0;
    }
};

typedef test_thread_t<true> test_thread;
typedef test_thread_t<false> test_thread2;


template <bool IsCreateSuspended>
void* test_thread_t<IsCreateSuspended>::operator new[](std::size_t size) throw(std::bad_alloc)
{
    std::cout << " test_thread_t<IsCreateSuspended>::operator new(std::size_t size) throw(std::bad_alloc)...\n";
    std::cout << " size: " << size << "\n";
    return ::operator new(size) ;
}

template <bool IsCreateSuspended>
void test_thread_t<IsCreateSuspended>::operator delete[](void* mem)
{
    std::cout << " test_thread_t<IsCreateSuspended>::operator delete[](void* mem, size_t size)...\n";
    ::operator delete[] (mem);
}

template <bool IsCreateSuspended>
void* test_thread_t<IsCreateSuspended>::operator new(std::size_t size, void *ptr) throw()
{
    std::cout << " test_thread_t<IsCreateSuspended>::operator new(std::size_t size, void *ptr)...\n";
    return ::new test_thread_t<IsCreateSuspended>(ptr);
}

template <bool IsCreateSuspended>
void test_thread_t<IsCreateSuspended>::operator delete( void* mem, void *ptr) throw()
{
    return ::operator delete(mem, ptr);
}

// template <bool IsCreateSuspended>
// void* test_thread_t<IsCreateSuspended>::operator new[](std::size_t size, void *ptr) throw()
// {
// 	std::cout << " test_thread_t<IsCreateSuspended>::operator new[](std::size_t size, void *ptr)...\n";
// 	return ::new test_thread_t[size](ptr);
// }
// template <bool IsCreateSuspended>
// void test_thread_t<IsCreateSuspended>::operator delete[]( void* mem, void *ptr )
// {
// 	return ::operator delete(mem, ptr);
// }

//delete void *
//这一操作对于一些简单的系统内建类型不会有问题，但如果void*所指向的对象在析构函数里要释放，
//进行此造作会丢失内存，因为它不执行析构函数，例如：

#pragma warning( pop)
}/// namespace test_namespace

namespace test_namespace
{

class CEntity
{
public:
    CEntity(char flag);
    ~CEntity();
private:
    void* m_data;
    char   m_flag;
};

CEntity::CEntity(char flag)
{
    m_flag = flag;
    cout<<"constructing entity "<<m_flag<<endl;
    m_data = new char[100];
}

CEntity::~CEntity()
{
    cout<<"destructing entity " << m_flag << endl;
    delete[] m_data;
}

//以上这个类是没什么问题的，但是看以下的调用：
void TestCEntity()
{
    CEntity * a = new CEntity('a');
    delete a;

    void * b = new CEntity('b');
    delete b;

    return ;
}

}/// namespace test_namespace
namespace test_xl_items
{
enum ECloudTransOperType
{
    CLOUD_TRANS_OPER_QUERY = 0,
    CLOUD_TRANS_OPER_REQUEST = 1,
};

#define release_assert assert
#define thunder_assert assert
class range
{
public:
    range() : pos(0), len(0) {}
    range(unsigned p, unsigned l) : pos(p), len(l) {}
    unsigned length() const
    {
        return len;
    }
private:
    unsigned pos,len;
public:
    friend std::ostream& operator << (std::ostream& os, const range& r)
    {
        os << " range[" << r.pos << ", " << (r.pos + r.len) << "]";
        return os;
    }
};


bool _TestBigEndian()
{
    union UnionForTest
    {
        short _short;
        char  _char;
    };
    UnionForTest test;
    test._short = 0x0100;
    return 1 == (int)test._char;
}
bool TestBigEndian()
{
    static bool s_is_big_endian = _TestBigEndian();
    return s_is_big_endian;
}

bool _TestLittleEndian()
{
    union UnionForTest
    {
        short _short;
        char  _char;
    };
    UnionForTest test;
    test._short = 0x0001;
    return 1 == (int)test._char;
}
bool TestLittleEndian()
{
    static bool s_is_little_endian = _TestLittleEndian();
    return s_is_little_endian;
}

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;

uint16 htons(uint16 n)
{
    if (TestBigEndian())
        return n;

    return	((n & 0xff) << 8) |
            ((n & 0xff00) >> 8);
}
uint16 ntohs(uint16 n)
{
    return htons(n);
}

uint32 htonl(uint32 n)
{
    if (TestBigEndian())
        return n;

    return ((n & 0xff) << 24) |
           ((n & 0xff00) << 8) |
           ((n & 0xff0000) >> 8) |
           ((n & 0xff000000) >> 24);
}
uint32 ntohl(uint32 n)
{
    return htonl(n);
}

uint64 htonll(uint64 n)
{
    if (TestBigEndian())
        return n;

    release_assert(TestLittleEndian());

    return ((n & 0xff) << 56) |
           ((n & 0xff00) << 40) |
           ((n & 0xff0000) << 24) |
           ((n & 0xff000000) << 8) |
           ((n & 0xff00000000) >> 8) |
           ((n & 0xff0000000000) >> 24) |
           ((n & 0xff000000000000) >> 40) |
           ((n & 0xff00000000000000) >> 56);
}
uint64 ntohll(uint64 n)
{
    return htonll(n);
}

uint8  hton(uint8 n)
{
    return n;
}
uint16 hton(uint16 n)
{
    return htons(n);
}
uint32 hton(uint32 n)
{
    return htonl(n);
}
uint64 hton(uint64 n)
{
    return htonll(n);
}

uint8  ntoh(uint8 n)
{
    return n;
}
uint16 ntoh(uint16 n)
{
    return ntohs(n);
}
uint32 ntoh(uint32 n)
{
    return ntohl(n);
}
uint64 ntoh(uint64 n)
{
    return ntohll(n);
}
}

namespace TestSpace
{
bool TestBigEndian()
{
    std::cout << " bool TestBigEndian()\n";
    union UnionForTest
    {
        short _short;
        char  _char;
    };
    UnionForTest test;
    test._short = 0x0100;
    return 1 == (int)test._char;
}
bool TestLittleEndian()
{
    std::cout << " bool TestBigEndian()\n";
    union UnionForTest
    {
        short _short;
        char  _char;
    };
    UnionForTest test;
    test._short = 0x0001;
    return 1 == (int)test._char;
}
struct test_big_endian
{
//        test_big_endian()
//        {
//            _big_endian_flag = TestBigEndian();
//        }
//        operator bool () { return _big_endian_flag; }
//    private:
//        bool _big_endian_flag;
    operator bool ()
    {
        static bool s_big_endian_flag = TestBigEndian();
        return s_big_endian_flag;
    }
};

struct test_little_endian
{
//        test_little_endian()
//        {
//            _little_endian_flag = TestLittleEndian();
//        }
//        operator bool () { return _little_endian_flag; }
//    private:
//        bool _little_endian_flag;
    operator bool ()
    {
        static bool s_little_endian_flag = TestLittleEndian();
        return s_little_endian_flag;
    }
};

#define VALUE_NAME _value
struct a_test_struct
{
    int VALUE_NAME;
};
#define paster( n ) printf( "token" #n " = %d \n", token##n )
int token9 = 9;
#define PRINT_TEST_STRING1(str) std::cout << "a_test_struct::" # str << "\n";
#define PRINT_TEST_STRING2(obj, mem_var) std::cout << obj.mem_var << "\n";
#define PRINT_TEST_STRING3(obj, num) std::cout << obj##num << "\n";


class CCoutBuffer
{
public:
    CCoutBuffer()
        : _buffer(0), _buffer_size(0), _write_pos(0), _read_pos(0)
    {}
    ~CCoutBuffer()
    {
        _safe_delete_buffer();
    }

    void reset()
    {
        //memset(_buffer, '\0', _buffer_size);
        _read_pos = _write_pos = 0;
    }
    unsigned length() const
    {
        assert(_write_pos >= _read_pos);
        return _write_pos-_read_pos;
    }
    unsigned write_pos() const
    {
        return _write_pos;
    }
    unsigned read_pos() const
    {
        return _read_pos;
    }
    unsigned capability() const
    {
        return _buffer_size;
    }
    const char* buffer() const
    {
        return _buffer;
    }
    char* buffer()
    {
        return const_cast<char*>(buffer());
    }
    std::string to_string() const
    {
        return std::string(_buffer+_read_pos, _buffer+_write_pos);
    }

    CCoutBuffer& operator >> (int& obj)
    {
        unsigned length = sizeof(obj);
        if (_write_pos >= length)
        {
            memcpy(&obj,_buffer+_read_pos , length);
            _read_pos += length;
        }
        //else { throw std::bad_exception(); }//or throw as error ...

        return *this;
    }
    CCoutBuffer& operator << (int obj)
    {
        unsigned length = sizeof(obj);
        bool apply_res = _try_apply_buffer(length);

        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        memcpy(_buffer+_write_pos, &obj, length);
        _write_pos += length;
        return *this;
    }

    CCoutBuffer& operator >> (unsigned& obj)
    {
        return operator >>( (int&)obj );
    }
    CCoutBuffer& operator << (unsigned obj)
    {
        return operator <<( (int)obj );
    }

    CCoutBuffer& operator >> (std::string& obj)
    {
        obj.clear();
        unsigned size;
        operator >> (size);/// step 1
        assert(size <= length());
        if (size > length())
            throw std::bad_exception();

        char* read_pos = _buffer+_read_pos;
        std::copy(read_pos, read_pos+size, back_inserter(obj));/// step 2
        assert(size == obj.size());
        _read_pos += obj.size();

        return *this;
    }
    CCoutBuffer& operator << (const std::string& obj)
    {
        unsigned length = sizeof(obj.size()) + obj.size();
        bool apply_res = _try_apply_buffer(length);

        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        operator << (obj.size());/// step 1
        memcpy(_buffer+_write_pos, obj.c_str(), obj.size());/// step 2
        _write_pos += obj.size();

        return *this;
    }

    template <typename T>
    CCoutBuffer& operator >> (std::vector<T>& obj)
    {
        unsigned size;
        *this >> size;
        for (size_t i=0; i < size; i++)
        {
            T   temp;
            *this >> temp;
            obj.push_back(temp);
        }

        return *this;
    }

    template <typename T>
    CCoutBuffer& operator << (const std::vector<T>& obj)
    {
        *this << obj.size();
        for (size_t i=0; i < obj.size(); i++)
            *this << obj[i];

        return *this;
    }
    char* reserve(unsigned size)
    {
        if (_buffer_size < size)
        {
            char* buffer = new char[size];
            if (!buffer)
                return NULL;//new error, or throw...

            memcpy(buffer, _buffer, _write_pos);
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = size;

        }
        return _buffer;
    }
protected:
    void _safe_delete_buffer()
    {
        if (_buffer)
        {
            delete _buffer;
            _buffer = NULL;
        }
    }
    bool _try_apply_buffer(unsigned length)
    {
        if (_write_pos+length > _buffer_size)
        {
            char* buffer = new char[2*(_write_pos+length)+1];
            if (!buffer)
                return false;//new error ...

            memcpy(buffer, _buffer, _write_pos);
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = 2*(_write_pos+length);
            //memset(_buffer, '\0', _buffer_size);
        }
        return true;
    }
    void _abjust_buffer()
    {
        if (_read_pos == _write_pos)
            reset();
        else if (_read_pos < _write_pos)
        {
            if (_read_pos > 0)
            {
                memcpy(_buffer, _buffer+_read_pos, length());
            }
        }
    }
private:
    char*       _buffer;
    unsigned    _buffer_size;
    unsigned    _write_pos;
    unsigned    _read_pos;
};

static CCoutBuffer cout_buffer;

class CTestCoutBuffer
{
public:
    CTestCoutBuffer() : _value("CTestCoutBuffer::_value \n") {}
    const std::string& value() const
    {
        return _value;
    }
    void setValue(const std::string& value)
    {
        _value = value;
    }

    friend CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj);
    friend CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj);
private:
//    public:
    std::string _value;
};

CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj)
{
    outs << obj._value;
    return outs;
}
CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj)
{
    ins >> obj._value;
    return ins;
}



class CBase
{
public:
    virtual std::string Hello()
    {
        return "CBase::Hello() ";
    }
};
class CDerived : public CBase
{
public:
    virtual std::string Hello()
    {
        return CBase::Hello() + "CDerived::Hello() ";
    }
};

const std::vector<int>& return_vec_ref ()
{
    const int NUM[] = {1 ,2, 3, 4, 5};
    static vector<int> s_int_vec(NUM, NUM+sizeof(NUM)/sizeof(int));
    return s_int_vec;
}

void test_to_string( std::string & result )
{
    char buf[64];
    result = "";
    for ( unsigned int i=0; i<3; i++ )
    {
        sprintf(buf, "(%lu)[%u, %u, %u]%s", i+1, 111111, 111111, 222222,"\r\n");
        //      i<=2 ? "\r\n" : "" );
//            std::cout << " strlen(buf)" << strlen(buf) << "\n";
        result += buf;
    }
}
unsigned _max_global_conn_count;
void func1(unsigned max_count)
{
    // equal to:
    //
    // if (max_count != 0)
    // {
    //     _max_global_conn_count = max_count;
    // }
    //
    // but cpu pipeline will not be interrupted.
    _max_global_conn_count = (-(0 == max_count) & _max_global_conn_count) | max_count;
}

void func2(unsigned max_count)
{
    // equal to:

    if (max_count != 0)
    {
        _max_global_conn_count = max_count;
    }

    // but cpu pipeline will not be interrupted.
    //	_max_global_conn_count = (-(0 == max_count) & _max_global_conn_count) | max_count;
}
enum  Ecolor { black = 0x0001, white = 0x0002, red = 0x0004, green = 0x0008 };

class CWidget
{
public:
    CWidget()
    {
        std::cout << "CWidget::CWidget() ...\n";
    }
    ~CWidget()
    {
        std::cout << "CWidget::~CWidget() ...\n";
    }
    void print()
    {
        std::cout << "void CWidget::print() ...\n";
    }
    unsigned Length() const
    {
        return sizeof(Length()) + sizeof(_value);
    }
private:
    int _value;
};
void TestFunc(CWidget* w)
{
    w->print();
}


struct piece_info
{
    typedef int data_pipe_wrapper;// for test ...
    bool  _is_complete;
    std::vector<data_pipe_wrapper *> _down_pipes;
    unsigned __int64 _score;
};
/// ...     ...     ...
enum //e_state
{
    S_INIT			= 0x1,
    S_CONNECTING	= 0x2,
    S_CONNECTED		= 0x4,
    S_WRITTING		= 0x8,
    S_READING		= 0x10,
    S_SENDING		= 0x20,
    S_RECVING		= 0x40,
    S_SOCK_CLOSE	= 0x80,
    S_FILE_CLOSE	= 0x100,
};

std::string handle_io_state2str(int _handle_io_state)
{
    const std::string STATE_STRS[] = { "S_INIT", "S_CONNECTING", "S_CONNECTED", "S_WRITTING", "S_READING",
                                       "S_SENDING", "S_RECVING", "S_SOCK_CLOSE", "S_FILE_CLOSE", "", ""
                                     };
    std::string state_str;
    int state_tmp = _handle_io_state;
    size_t states_count = 0;
    for (size_t i=0; STATE_STRS[i] != ""; i++)
        ++states_count;

    for (size_t i=0; i < states_count; i++)
    {
        if (state_tmp&1)
            state_str += STATE_STRS[i] + " " ;
        state_tmp = state_tmp >> 1;
    }
    return state_str;
}

class TSingleFile
{
public:
//		using namespace std;
public:
    TSingleFile() : _file(0) {}
    TSingleFile(const std::string& filename, const std::string& open_type="wb+")
        : _file(0)
    {
        if ( !open(filename, open_type))
            return;//error
    }
    ~TSingleFile()
    {
        close();
    }
    bool open(const std::string& filename, const std::string& open_type="rb+")
    {
        _file = fopen(filename.c_str(), open_type.c_str());
        return (NULL != _file) ;
    }
    size_t write(void* pData,unsigned len)
    {
        assert(_file);
        return fwrite(pData, 1, len, _file);
    }
    size_t read(void* pData,unsigned len)
    {
        assert(_file);
        return fread(pData, 1, len, _file);
    }
    bool close()
    {
        if (_file)
        {
            int res = fclose(_file);
            _file = NULL;
            return (0==res);
        }
        return false;
    }
private:
    FILE* _file;
};
}


namespace test_namespace
{

template <typename TChar>
class CCoutBufferT
{
public:
    CCoutBufferT()
        : _buffer(0), _buffer_size(0), _write_pos(0), _read_pos(0)
    {}
    ~CCoutBufferT()
    {
        _safe_delete_buffer();
    }

    void reset()
    {
        //memset(_buffer, '\0', _buffer_size);
        _read_pos = _write_pos = 0;
    }
    unsigned length() const
    {
        assert(_write_pos >= _read_pos);
        return (_write_pos-_read_pos);
    }
    unsigned write_pos() const
    {
        return _write_pos;
    }
    unsigned read_pos() const
    {
        return _read_pos;
    }
    unsigned capability() const
    {
        return _buffer_size;
    }
    const TChar* buffer() const
    {
        return _buffer;
    }
    TChar* buffer()
    {
        return const_cast<TChar*>(buffer());
    }
    std::string to_string() const
    {
        return std::string(_buffer+_read_pos, _buffer+_write_pos);
    }

    CCoutBufferT& operator >> (int& obj)
    {
        unsigned length = sizeof(obj)/sizeof(TChar);
        if (this->length() >= length)
        {
            memcpy(&obj,_buffer+_read_pos , sizeof(obj));
            _read_pos += length;
        }
        //else { throw std::bad_exception(); }//or throw as error ...

        return *this;
    }
    CCoutBufferT& operator << (int obj) throw (std::bad_exception)
    {
        unsigned length = sizeof(obj)/sizeof(TChar);
        bool apply_res = _try_apply_buffer(length);

        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        memcpy(_buffer+_write_pos, &obj, sizeof(obj));
        _write_pos += length;
        return *this;
    }

    CCoutBufferT& operator >> (unsigned& obj)
    {
        return operator >>( (int&)obj );
    }
    CCoutBufferT& operator << (unsigned obj)
    {
        return operator <<( (int)obj );
    }

    CCoutBufferT& operator >> (std::basic_string<TChar>& obj) throw (std::bad_exception)
    {
        obj.clear();
        unsigned size;
        operator >> (size);/// step 1
        assert(size <= length());
        if (size > length())
            throw std::bad_exception();

        TChar* read_pos = _buffer+_read_pos;
        std::copy(read_pos, read_pos+size, back_inserter(obj));/// step 2
        assert(size == obj.size());
        _read_pos += obj.size();

        return *this;
    }
    CCoutBufferT& operator << (const std::basic_string<TChar>& obj) throw (std::bad_exception)
    {
        unsigned length = sizeof(obj.size())/sizeof(TChar) + obj.size();
        bool apply_res = _try_apply_buffer(length);
        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        operator << (obj.size());/// step 1
//            unsigned copy_size = obj.size()*sizeof(TChar);
//            memcpy(_buffer+_write_pos, obj.c_str(), copy_size);/// step 2
        std::copy(obj.begin(), obj.end(), _buffer+_write_pos);
        _write_pos += obj.size();

        return *this;
    }

    template <typename T>
    CCoutBufferT& operator >> (std::vector<T>& obj)
    {
        unsigned size;
        *this >> size;
        for (size_t i=0; i < size; i++)
        {
            T   temp;
            *this >> temp;
            obj.push_back(temp);
        }

        return *this;
    }

    template <typename T>
    CCoutBufferT& operator << (const std::vector<T>& obj)
    {
        *this << obj.size();
        for (size_t i=0; i < obj.size(); i++)
            *this << obj[i];

        return *this;
    }
    TChar* reserve(unsigned size)
    {
        if (_buffer_size < size)
        {
            TChar* buffer = new TChar[size];
            if (!buffer)
                return NULL;//new error, or throw...

            memcpy(buffer, _buffer, _write_pos*sizeof(TChar));
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = size;

        }
        return _buffer;
    }
protected:
    void _safe_delete_buffer()
    {
        if (_buffer)
        {
            delete _buffer;
            _buffer = NULL;
        }
    }
    bool _try_apply_buffer(unsigned length)
    {
        if (_write_pos+length > _buffer_size)
        {
            TChar* buffer = new TChar[2*(_write_pos+length)+1];
            if (!buffer)
                return false;//new error ...

            memcpy(buffer, _buffer, _write_pos*sizeof(TChar));
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = 2*(_write_pos+length);
            //memset(_buffer, '\0', _buffer_size);
        }
        return true;
    }
    void _abjust_buffer() throw (std::bad_exception)
    {
        if (_read_pos == _write_pos)
            reset();
        else if (_read_pos < _write_pos)
        {
            if (_read_pos > 0)
            {
                memcpy(_buffer, _buffer+_read_pos, length()*sizeof(TChar));
            }
        }
        else // _read_pos > _write_pos
        {
            assert(false);
            throw std::bad_exception();
        }
    }
private:
    TChar*      _buffer;
    unsigned    _buffer_size;
    unsigned    _write_pos;
    unsigned    _read_pos;
};

typedef CCoutBufferT<char> CCoutBuffer;
typedef CCoutBufferT<wchar_t> CCoutBufferW;

static CCoutBuffer cout_buffer;
static CCoutBufferW wcout_buffer;
////////////////////////////////////////////////////////////////

class CTestCoutBuffer
{
public:
    CTestCoutBuffer() : _value("CTestCoutBuffer::_value \n") {}
    const std::string& value() const
    {
        return _value;
    }
    void setValue(const std::string& value)
    {
        _value = value;
    }

    friend CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj);
    friend CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj);
private:
//    public:
    std::string _value;
};

CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj)
{
    outs << obj._value;
    return outs;
}
CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj)
{
    ins >> obj._value;
    return ins;
}

}/// namespace test_namespace

namespace test_namespace2
{

template <typename TChar>
class CCoutBufferT
{
public:
    CCoutBufferT()
        : _buffer(0), _buffer_size(0), _write_pos(0), _read_pos(0)
    {}
    ~CCoutBufferT()
    {
        _safe_delete_buffer();
    }

    void reset()
    {
        //memset(_buffer, '\0', _buffer_size);
        _read_pos = _write_pos = 0;
    }
    unsigned length() const
    {
        assert(_write_pos >= _read_pos);
        return (_write_pos-_read_pos);
    }
    unsigned write_pos() const
    {
        return _write_pos;
    }
    unsigned read_pos() const
    {
        return _read_pos;
    }
    unsigned capability() const
    {
        return _buffer_size;
    }
    const TChar* buffer() const
    {
        return _buffer;
    }
    TChar* buffer()
    {
        return const_cast<TChar*>(buffer());
    }
    std::string to_string() const
    {
        return std::string(_buffer+_read_pos, _buffer+_write_pos);
    }

    CCoutBufferT& operator >> (int& obj)
    {
        unsigned length = sizeof(obj)/sizeof(TChar);
        if (this->length() >= length)
        {
            memcpy(&obj,_buffer+_read_pos , sizeof(obj));
            _read_pos += length;
        }
        //else { throw std::bad_exception(); }//or throw as error ...

        return *this;
    }
    CCoutBufferT& operator << (int obj) throw (std::bad_exception)
    {
        unsigned length = sizeof(obj)/sizeof(TChar);
        bool apply_res = _try_apply_buffer(length);

        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        memcpy(_buffer+_write_pos, &obj, sizeof(obj));
        _write_pos += length;
        return *this;
    }

    CCoutBufferT& operator >> (unsigned& obj)
    {
        return operator >>( (int&)obj );
    }
    CCoutBufferT& operator << (unsigned obj)
    {
        return operator <<( (int)obj );
    }

    CCoutBufferT& operator >> (std::basic_string<TChar>& obj) throw (std::bad_exception)
    {
        obj.clear();
        unsigned size;
        operator >> (size);/// step 1
        assert(size <= length());
        if (size > length())
            throw std::bad_exception();

        TChar* read_pos = _buffer+_read_pos;
        std::copy(read_pos, read_pos+size, back_inserter(obj));/// step 2
        assert(size == obj.size());
        _read_pos += obj.size();

        return *this;
    }
    CCoutBufferT& operator << (const std::basic_string<TChar>& obj) throw (std::bad_exception)
    {
        unsigned length = sizeof(obj.size())/sizeof(TChar) + obj.size();
        bool apply_res = _try_apply_buffer(length);
        assert(apply_res);//or throw as error ...
        if (!apply_res)
            throw std::bad_exception();

        operator << (obj.size());/// step 1
//            unsigned copy_size = obj.size()*sizeof(TChar);
//            memcpy(_buffer+_write_pos, obj.c_str(), copy_size);/// step 2
        std::copy(obj.begin(), obj.end(), _buffer+_write_pos);
        _write_pos += obj.size();

        return *this;
    }

    template <typename T>
    CCoutBufferT& operator >> (std::vector<T>& obj)
    {
        unsigned size;
        *this >> size;
        for (size_t i=0; i < size; i++)
        {
            T   temp;
            *this >> temp;
            obj.push_back(temp);
        }

        return *this;
    }

    template <typename T>
    CCoutBufferT& operator << (const std::vector<T>& obj)
    {
        *this << obj.size();
        for (size_t i=0; i < obj.size(); i++)
            *this << obj[i];

        return *this;
    }
    TChar* reserve(unsigned size)
    {
        if (_buffer_size < size)
        {
            TChar* buffer = new TChar[size];
            if (!buffer)
                return NULL;//new error, or throw...

            memcpy(buffer, _buffer, _write_pos*sizeof(TChar));
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = size;

        }
        return _buffer;
    }
protected:
    void _safe_delete_buffer()
    {
        if (_buffer)
        {
            delete _buffer;
            _buffer = NULL;
        }
    }
    bool _try_apply_buffer(unsigned length)
    {
        if (_write_pos+length > _buffer_size)
        {
            TChar* buffer = new TChar[2*(_write_pos+length)+1];
            if (!buffer)
                return false;//new error ...

            memcpy(buffer, _buffer, _write_pos*sizeof(TChar));
            _safe_delete_buffer();

            _buffer = buffer;
            _buffer_size = 2*(_write_pos+length);
            //memset(_buffer, '\0', _buffer_size);
        }
        return true;
    }
    void _abjust_buffer() throw (std::bad_exception)
    {
        if (_read_pos == _write_pos)
            reset();
        else if (_read_pos < _write_pos)
        {
            if (_read_pos > 0)
            {
                memcpy(_buffer, _buffer+_read_pos, length()*sizeof(TChar));
            }
        }
        else // _read_pos > _write_pos
        {
            assert(false);
            throw std::bad_exception();
        }
    }
private:
    TChar*      _buffer;
    unsigned    _buffer_size;
    unsigned    _write_pos;
    unsigned    _read_pos;
};

typedef CCoutBufferT<char> CCoutBuffer;
typedef CCoutBufferT<wchar_t> CCoutBufferW;

static CCoutBuffer cout_buffer;
static CCoutBufferW wcout_buffer;
////////////////////////////////////////////////////////////////

class CTestCoutBuffer
{
public:
    CTestCoutBuffer() : _value("CTestCoutBuffer::_value \n") {}
    const std::string& value() const
    {
        return _value;
    }
    void setValue(const std::string& value)
    {
        _value = value;
    }

    friend CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj);
    friend CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj);
private:
//    public:
    std::string _value;
};

CCoutBuffer& operator << (CCoutBuffer& outs, const CTestCoutBuffer& obj)
{
    outs << obj._value;
    return outs;
}
CCoutBuffer& operator >> (CCoutBuffer& ins, CTestCoutBuffer& obj)
{
    ins >> obj._value;
    return ins;
}

}/// namespace test_namespace2
#endif // MAIN_H_INCLUDED
