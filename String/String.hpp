#ifndef _STRING_H
#define _STRING_H
#include <cstdint>
#include <assert.h>

#define INIT_LEN 5

class String {
public:
    String() : pImpl(new StrImpl()) {};
    String(const char* str) : pImpl(new StrImpl(str)) {};
    String(const char* str, size_t n) : pImpl(new StrImpl(str, 0, 0, n)) {};
    String(size_t n, char c) : pImpl(new StrImpl(n, c)) {};
    String(const String& str) : pImpl(str.pImpl) { pImpl->AddRef(); };
    
    String(const String& str, size_t pos, size_t len = npos) {
        if(pos == 0 && len >= str.size()) {
            pImpl = str.pImpl;
            pImpl->AddRef();
        }
        else {
            pImpl = new StrImpl(str.data(), 0, pos, len);
        }
    };

    virtual ~String();

    size_t size() const {
        assert(pImpl != nullptr);
        return pImpl->size();
    }
    size_t capacity() const {
        assert(pImpl != nullptr);
        return pImpl->capacity();
    }

    void reserve(size_t n = 0);
    void clear();
    bool empty() const {
        assert(pImpl != nullptr);
        return pImpl->empty();
    };

    char& at(size_t pos) {
        assert(pImpl != nullptr);
        ReImpl();
        return pImpl->at(pos);
    }
    const char& at(size_t pos) const {
        assert(pImpl != nullptr);
        return ((const StrImpl*)pImpl)->at(pos);
    }

    char& operator[](size_t pos) {
        assert(pImpl != nullptr);
        ReImpl();
        return (*pImpl)[pos];
    }
    const char& operator[](size_t pos) const {
        assert(pImpl != nullptr);
        return (*(const StrImpl*)pImpl)[pos];
    }

    char& back() {
        if (size() == 0)
            return at(0);
        return at(size() - 1);
    }
    const char& back() const {
        if (size() == 0)
            return at(0);
        return at(size() - 1);
    }

    char& front() { return at(0); }
    const char& front() const { return at(0); }

    String& operator+=(const String& str);
    String& operator+=(const char* str);
    String& operator+=(char c);

    String& operator=(const String& str);
    String& operator=(const char* str);

    String& insert(size_t pos, const String& str);
    String& insert(size_t pos, const char* str);
    String& insert(size_t pos, size_t n, char c);

    String& erase(size_t pos = 0, size_t len = npos);

    String& replace(size_t pos, size_t len, const String& str);
    String& replace(size_t pos, size_t len, const char* str);
    String& replace(size_t pos, size_t len, size_t n, char c);

    void swap(String& str);
    const char* data() const {
        assert(pImpl != nullptr);
        return pImpl->data();
    };

    size_t find(const String& str, size_t pos = 0) const;
    size_t find(const char* str, size_t pos = 0) const;
    size_t find(char c, size_t pos = 0) const;

    String substr(size_t pos = 0, size_t len = npos) const;

    int compare(const String& str) const;
    int compare(const char* str) const;

    static const size_t npos = (size_t)-1;

    size_t countRef() const {
        assert(pImpl != nullptr);
        return pImpl->countRef();
    }

private:
    class StrImpl {
    private:
        char* dat = nullptr;
        int64_t refCounter = 1;
        size_t datLen = 0;
        size_t memSize = 0;

    public:
        StrImpl(const char* str = nullptr, size_t capacity = 0, size_t pos = 0, size_t len = npos);
        StrImpl(size_t n, char c);

        ~StrImpl() { delete dat; }

        void AddRef() { ++refCounter; };
        void Release() {
            if (--refCounter <= 0)
                delete this;
        };

        size_t capacity() const { return memSize; }
        const char* data() { return dat; }
        size_t size() const { return datLen; }
        //size_t& size() { return datLen; }

        size_t countRef() { return refCounter; };
        bool empty() { return datLen == 0; }

        void operator+=(const char* str);
        void operator+=(char c);

       // void alloc_copy(size_t len, size_t cap, const char* dat);    // Для упрощения жизни в String::reserve()
        void clear(size_t from = 0);
        char& at(size_t pos);
        const char& at(size_t pos) const;
        char& operator[](size_t pos);
        const char& operator[](size_t pos) const;

        void replace(size_t pos, size_t len, const char* str, size_t sLen);  //  !!
        void insert(size_t pos, const char* str, size_t len);
        void erase(size_t pos, size_t len);

        size_t find(const char* str, size_t pos);
        int compare(const char* toCmp);
    };

    StrImpl* pImpl = nullptr;
    void ReImpl();
};

#endif // !_STRING_H