#include "String.hpp"

#include <stdexcept>
#include <cstring>

//	Todo
// 
// 

String::StrImpl::StrImpl(const char* str, size_t capacity, size_t pos, size_t len) {
	assert((capacity == 0) || (capacity != 0 && pos == 0 && len == npos));
	if (str == nullptr) {
			dat = new char[INIT_LEN];
			memSize = INIT_LEN;
			datLen = 0;
			*dat = '\0';
			return;
	}

	size_t sLen = std::strlen(str);
	if (pos > sLen)	
		throw std::out_of_range{ "String is out of range!" };

	size_t toCopy;
	if (len == npos)
		toCopy = sLen - pos;
	else
		toCopy = (len + pos) >  sLen ? (sLen - pos) : len;
	
	datLen = sLen == 0 ? 0 : toCopy;

	if (capacity <= datLen) 
		memSize = datLen + 1;
	else
		memSize = capacity;
	
	dat = new char[memSize];
	memcpy_s(dat, memSize, str + pos, datLen);
	dat[datLen] = '\0';
}

String::StrImpl::StrImpl(size_t n, char c) {
	if (n >= memSize) {
		memSize = n + 1;
		dat = new char[memSize];
	}

	datLen = n;
	for (size_t i = 0; i < n; i++)
		dat[i] = c;
	dat[n] = '\0';
}

String::~String() {
	assert(pImpl != nullptr);
	pImpl->Release();
	pImpl = nullptr;
}

void String::ReImpl() {
	if (countRef() != 1) {
		StrImpl* old = pImpl;
		pImpl = new StrImpl(data());
		old->Release();
	}
}
											
void String::reserve(size_t n) {
	assert(pImpl != nullptr);
	if ((n == capacity() && pImpl->countRef() == 1) || n < capacity())
		return;

	StrImpl* old = pImpl;
	pImpl = new StrImpl(data(), n);
	old->Release();
}

void String::StrImpl::clear(size_t from) {
	assert(from < memSize);
	datLen = from;
	dat[from] = '\0';
}

void String::clear() {
	assert(pImpl != nullptr);
	if (empty())
		return;
	if (countRef() == 1) {	// Если не копируем..
		pImpl->clear(0);
		return;
	}

	pImpl->Release();
	pImpl = new StrImpl();	// Прописываем дефолтное значение
}

char& String::StrImpl::at(size_t pos) {
	if (pos >= size())
		throw std::out_of_range{ "String is out of range!" };
	return dat[pos];
}

const char& String::StrImpl::at(size_t pos) const {
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };
	return dat[pos];
}

char& String::StrImpl::operator[] (size_t pos) {
	return dat[pos];
}

const char& String::StrImpl::operator[] (size_t pos) const {
	return dat[pos];
}

void String::StrImpl::operator+=(const char* str) {
	memcpy_s(dat + datLen, memSize - datLen, str, memSize - datLen - 1);
	datLen += memSize - datLen - 1;
	dat[datLen] = '\0';
}

String& String::operator+=(const String& str) {
	assert(pImpl != nullptr);
	if (str.pImpl == nullptr || str.empty())
		return *this;

	*this += str.data();

	return *this;
}

String& String::operator+=(const char* str) {
	if (str == nullptr || *str == '\0')
		return *this;

	size_t sLen = std::strlen(str) + 1;
	reserve(sLen + size());
	*pImpl += str;
//	pImpl->size() += (sLen - 1);

	return *this;
}

void String::StrImpl::operator+=(const char c) {
	dat[datLen] = c;
	dat[++datLen] = '\0';
}

String& String::operator+=(char c) {
	reserve(size() + 2);
	*pImpl += c;

	return *this;
}

String& String::operator=(const String& str) {
	assert(pImpl != nullptr && str.pImpl != nullptr);
	if (pImpl == str.pImpl || (empty() && str.empty()))
		return *this;

	StrImpl* old = pImpl;
	pImpl = str.pImpl;
	pImpl->AddRef();
	old->Release();

	return *this;
}

String& String::operator=(const char* str) {
	assert(pImpl != nullptr);
	if (empty() && (str == nullptr || *str == '\0'))
		return *this;

	pImpl->Release();
	pImpl = new StrImpl(str);

	return *this;
}

void String::StrImpl::insert(size_t pos, const char* str, size_t len) {
	memmove_s(dat + pos + len, memSize - pos - len, dat + pos, datLen - pos + 1);
	memcpy_s(dat + pos, memSize - pos, str, len);
	datLen += len;
}

String& String::insert(size_t pos, const String& str) {
	assert(pImpl != nullptr && str.pImpl != nullptr);
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };
	if (str.empty())
		return *this;

	insert(pos, str.data());
	return *this;
}

String& String::insert(size_t pos, const char* str) {
	assert(pImpl != nullptr);
	if (str == nullptr || *str == '\0')
		return *this;
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };

	size_t sLen = std::strlen(str);
	reserve(size() + sLen + 1);

	pImpl->insert(pos, str, sLen);

	return *this;
}

String& String::insert(size_t pos, size_t n, char c) {
	assert(pImpl != nullptr);
	if (c == '\0')
		return *this;
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };
	
	StrImpl newImpl(n, c);
	insert(pos, newImpl.data());

	return *this;
}

void String::StrImpl::erase(size_t pos, size_t len) {
	if (len > datLen - pos)
		clear(pos);
	else {
		memcpy_s(dat + pos, capacity() - pos, dat + len + pos, size() - len - pos + 1);	
		datLen -= len;
	}
}

String& String::erase(size_t pos, size_t len) {
	assert(pImpl != nullptr);
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };

	ReImpl();
	pImpl->erase(pos, len);

	return *this;
}

void String::StrImpl::replace(size_t pos, size_t len, const char* str, size_t sLen) {
	memmove_s(dat + pos + sLen, memSize - pos - sLen, dat + pos + len, datLen - len - pos + 1);
	memcpy_s(dat + pos, memSize - pos, str, sLen);
	datLen += sLen - len;
}

String& String::replace(size_t pos, size_t len, const String& str) {
	assert(pImpl != nullptr && str.pImpl != nullptr);
	if(pos > size())
		throw std::out_of_range{ "String is out of range!" };
	if (str.empty())
		return *this;

	replace(pos, len, str.data());

	return *this;
}

String& String::replace(size_t pos, size_t len, const char* str) {
	assert(pImpl != nullptr);
	if (pos > size())
		throw std::out_of_range{ "String is out of range!" };
	if (str == nullptr || *str == '\0')
		return *this;
	
	if (pos > size()
		|| (len != npos && (pos + len > size()))) 
	{
		throw std::out_of_range{ "String is out of range!" };
	}
	
	ReImpl();

	if (len + pos > size())
		len = size() - pos;

	size_t sLen = std::strlen(str);
	reserve(sLen + size() - len + 1);

	pImpl->replace(pos, len, str, sLen);

	return	*this;
}

String& String::replace(size_t pos, size_t len, size_t n, char c) {
	assert(pImpl != nullptr);
	if( pos > size())
		throw std::out_of_range{ "String is out of range!" };
	ReImpl();
	StrImpl newImpl(n, c);
	replace(pos, len, newImpl.data());

	return	*this;
}

void String::swap(String& str) {
	assert(pImpl != nullptr && str.pImpl != nullptr);

	StrImpl* tmp = pImpl;
	pImpl = str.pImpl;
	str.pImpl = tmp;
}

size_t String::StrImpl::find(const char* str,  size_t pos) { // Алгоритм Бойера-Мура
	size_t sLen = std::strlen(str);
	if (pos + sLen > datLen || sLen == 0)
		return npos;

	size_t* shifts = new size_t[256] {0};	// new тк нужен исззодный массив нулей

	for (size_t i = 0; i < sLen - 1; i++) {
		shifts[(size_t)(str[i])] = sLen - i - 1;		// Считаем сдвиги 
	}
	if (shifts[(size_t)str[sLen - 1]] == 0)
		shifts[(size_t)str[sLen - 1]] = sLen;

	size_t shifter = pos;	// Текущий сдвиг образа относительно строки
	bool flag = true;	// Флаг на найденную подстроку
	 
	while (shifter <= (size() - sLen))	{	// Идем, пока не дойдем до конца
		for (int i = (int)sLen - 1; i >= 0 && flag; i--) {	// Сравниваем подстроку с конца
			if (str[i] != dat[shifter + i]) {	// Если не совпали
				if (shifts[dat[shifter + i]] == 0)
					shifter += sLen;	// Двигаем образ на полную длину
				else
					shifter += shifts[dat[shifter + i]];	// Либо на необходимый сдвиг

				flag = false;	// Переписываем флаг
			}
		}

		if (flag) {
			delete[] shifts;
			return shifter;	// Если надо - возвращаем указатель на найденную подстроку
		}
		flag = true;
	}

	delete[] shifts;
	return npos;
}

size_t String::find(const String& str, size_t pos) const {
	assert(pImpl != nullptr && str.pImpl != nullptr);
	if (empty() || str.empty())
		return npos;
	return pImpl->find(str.data(), pos);
}

size_t String::find(const char* str, size_t pos) const {
	if (empty() || str == nullptr || *str == '\0')
		return npos;

	return pImpl->find(str, pos);
}

size_t String::find(char c, size_t pos) const {
	if (empty() || c == '\0')
		return npos;

	char tofind[2] = { c, '\0' };

	return pImpl->find(tofind, pos);
}

String String::substr(size_t pos, size_t len) const {
	assert(pImpl != nullptr);
	if(pos > pImpl->size())
		throw std::out_of_range{ "String is out of range!" };

	String newStr(*this, pos, len);
	return newStr;
}

int String::StrImpl::compare(const char* toCmp) {
	return strcmp(dat, toCmp);
}	

int String::compare(const String& str) const {
	assert(pImpl != nullptr && str.pImpl != nullptr);

	return pImpl->compare(str.data());
}

int String::compare(const char* str) const {
	assert(pImpl != nullptr);

	return pImpl->compare(str);
}