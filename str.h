#pragma once
#include <iostream>
#include <cstring>
#include <stdio.h>
class str
{
public:
	char* data;
	int size;

	// constructors
	str();
	str(const char* str);
	str(const str& other);

	// destructor
	~str();

	// assignment operators
	str& operator=(const char* str);
	str& operator=(const str& other);

	// concatenation operators
	str operator+(const char* string) const;
	str operator+(const str& other) const;
	str& operator+=(const char* str);
	str& operator+=(const str& other);
	str& operator+=(char c);

	// accessors
	char& operator[](int index);
	const char& operator[](int index) const;

	// comparison operators
	bool operator==(const char* str) const;
	bool operator==(const str& other) const;
	bool operator!=(const char* str) const;
	bool operator!=(const str& other) const;

	void copy(const char* str);
	void copy(const str& other);

	friend ostream& operator<<(ostream& os, const str& str);
};