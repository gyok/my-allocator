#include <iostream>
#include "MyAllocator.h"

using namespace std;	

int main() {
	const size_t M = 1024 * 1024;
	MyAllocator alloc(1024 * M);
	void* p1 = alloc.Allocate(100 * M);
	void* p2 = alloc.Allocate(M);
	alloc.Deallocate(p1);
	p1 = alloc.Allocate(200 * M);

	void* pointers[10];
	for (int i = 0; i < 10; ++i)
		pointers[i] = alloc.Allocate(30 * M);

	alloc.Deallocate(p2);
	alloc.Deallocate(p1);

	for (int i = 0; i < 10; ++i)
		alloc.Deallocate(pointers[i]);

	system("pause");
	return 0;
}