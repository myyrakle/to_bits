#include <string>
#include <array>
#include <cassert>
#include <stdint.h>

template <class T>
class to_bits
{
public:
    constexpr static size_t byte_size = sizeof(T);
private:
    union bit_and_byte_union
    {
        uint8_t byte;
        struct 
        {
            unsigned char _1: 1;
            unsigned char _2: 1;
            unsigned char _3: 1;
            unsigned char _4: 1;
            unsigned char _5: 1;
            unsigned char _6: 1;
            unsigned char _7: 1;
            unsigned char _8: 1;
        };
    };

    std::array<bit_and_byte_union, byte_size> bytes;
private:
    void set_bytes(const T&);
public: // two basic friend
    to_bits() = default;
    virtual ~to_bits() = default;
public: // default members
    to_bits(const to_bits&) = delete;
    to_bits(to_bits&&) = default;
    to_bits& operator=(const to_bits&) = delete;
    to_bits& operator=(to_bits&&) = default;
public:
    to_bits(const T&);
    to_bits(T&&);
    to_bits& operator=(const T&);
    to_bits& operator=(T&&);
public:
    std::string to_bitstring() const;
public:
    class reference
    {};
    int operator[](size_t) const;
};

template <class T>
constexpr size_t get_byte()
{
    return sizeof(T);
}


inline bool _is_big_endian()
{
    static const uint32_t n = 1;
    return reinterpret_cast<const uint8_t*>(&n)[3] == 1;
}
inline bool _is_little_endian()
{
    static const uint32_t n = 1;
    return reinterpret_cast<const uint8_t*>(&n)[0] == 1;
}


template <class T>
void to_bits<T>::set_bytes(const T& v) 
{
    if(_is_big_endian())
        for(int i=0; i<byte_size; ++i)
            bytes[i].byte = reinterpret_cast<const uint8_t*>(&v)[i];
    else if(_is_little_endian())
        for(int i = 0, ri=byte_size-1; ri>=0; ++i, --ri)
            bytes[i].byte = reinterpret_cast<const uint8_t*>(&v)[ri];
    else
        assert(false);
}


template <class T>
to_bits<T>::to_bits(const T& v) 
{
    set_bytes(v);
}

template <class T>
to_bits<T>::to_bits(T&& v) 
{
    set_bytes(v);
}

template <class T>
to_bits<T>& to_bits<T>::operator=(const T& v)
{
    set_bytes(v);
    return *this;
}

template <class T>
to_bits<T>& to_bits<T>::operator=(T&& v)
{
    set_bytes(v);
    return *this;
}


bool _bitfield_is_big()
{
    union bit_and_byte_union
    {
        unsigned char byte;
        struct 
        {
            unsigned char _1: 1;
            unsigned char _2: 1;
            unsigned char _3: 1;
            unsigned char _4: 1;
            unsigned char _5: 1;
            unsigned char _6: 1;
            unsigned char _7: 1;
            unsigned char _8: 1;
        };
    } hmm;
    hmm.byte=1;
    return hmm._1==1;
}

template <class T>
std::string to_bits<T>::to_bitstring() const
{
    constexpr auto to_inc = '0'- (char)0;

    std::string res;
    auto adder = [](std::string& s, bit_and_byte_union byte)
    {
        if(_bitfield_is_big()) //비트필드 순서 처리
        {
            s.push_back(byte._8 + to_inc);
            s.push_back(byte._7 + to_inc);
            s.push_back(byte._6 + to_inc);
            s.push_back(byte._5 + to_inc);
            s.push_back(byte._4 + to_inc);
            s.push_back(byte._3 + to_inc);
            s.push_back(byte._2 + to_inc);
            s.push_back(byte._1 + to_inc);
        }
        else
        {
            s.push_back(byte._1 + to_inc);
            s.push_back(byte._2 + to_inc);
            s.push_back(byte._3 + to_inc);
            s.push_back(byte._4 + to_inc);
            s.push_back(byte._5 + to_inc);
            s.push_back(byte._6 + to_inc);
            s.push_back(byte._7 + to_inc);
            s.push_back(byte._8 + to_inc);
        }
        s.push_back(' ');
    };
    
    for(const auto& e : this->bytes)
        adder(res, e);
    
    res.pop_back();
    return res;
}

template <class T>
to_bits<T> make_to_bits(const T& value)
{
    return to_bits<T>(value);
}