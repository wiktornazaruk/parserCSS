#include "str.h"
str::str()
{
	data = new char[1];
	data[0] = '\0';
	size = 0;
}

str::str(const char* str)
{
	copy(str);
}

str::str(const str& other)
{
	copy(other);
}

str::~str()
{
	if (this->data != NULL)
	{
		delete[] data;
	}
}

str& str::operator=(const char* str)
{
	delete[] data;
	copy(str);
	return *this;
}

str& str::operator=(const str& other)
{
	delete[] data;
	copy(other);
	return *this;
}

str str::operator+(const char* string) const
{
	str result(*this);
	result += string;
	return result;
}

str str::operator+(const str& other) const
{
	str result(*this);
	result += other;
	return result;
}

str& str::operator+=(const char* str)
{
	int new_size = static_cast<int>(size + strlen(str));
	char* new_data = new char[new_size + 1];
	strcpy_s(new_data, new_size + 1, data);
	strcat_s(new_data, new_size + 1, str);
	if (data != NULL)
	{
		delete[] data;
	}
	data = new_data;
	size = new_size;
	return *this;
}

str& str::operator+=(const str& other)
{
	return operator+=(other.data);
}

str& str::operator+=(char c)
{
	char* new_data = new char[size + 2];
	strcpy_s(new_data, size + 2, data);
	new_data[size] = c;
	new_data[size + 1] = '\0';
	data = new_data;
	size++;
	return *this;
}

char& str::operator[](int index)
{
	return data[index];
}

const char& str::operator[](int index) const
{
	return data[index];
}

bool str::operator==(const char* str) const
{
	return strcmp(data, str) == 0;
}

bool str::operator==(const str& other) const
{
	return strcmp(data, other.data) == 0;
}

bool str::operator!=(const char* str) const
{
	return !operator==(str);
}

bool str::operator!=(const str& other) const
{
	return !operator==(other);
}

void str::copy(const char* str)
{
	size = strlen(str);
	data = new char[size + 1];
	strcpy_s(data, size + 1, str);
}

void str::copy(const str& other)
{
	size = other.size;
	data = new char[size + 1];
	strcpy_s(data, size + 1, other.data);
}

ostream& operator<<(ostream& os, const str& str)
{
	os << str.data;
	return os;
}

istream& operator>>(istream& is, str& str)
{
	char buffer[100];
	is.getline(buffer, 100);
	str = buffer;
	return is;
}