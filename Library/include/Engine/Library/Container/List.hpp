#pragma once
#include <Engine/Library/Common.h>

#define LIST_DEFAULT_SIZE 32

template<typename T>
class List {
public:

	List() {
		data = new T[LIST_DEFAULT_SIZE];
		size = LIST_DEFAULT_SIZE;
		count = 0;
		begin = &data[0];
		end = &data[size-1];
	}

	~List() {
		delete[] data;
		begin = nullptr;
		end = nullptr;
		data = nullptr;
	}

private:
	T* data;
	T* begin, end;
	u32 count;
	u32 size;
};