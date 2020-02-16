#include <iostream>
#include <vector>
#include "MyAllocator.h"

using namespace std;	

int main() {
    // const size_t M = 1024 * 1024;
    // MyAllocator alloc(1024 * M);
    // void* p1 = alloc.Allocate(100 * M);
    // void* p2 = alloc.Allocate(M);
    // alloc.Deallocate(p1);
    // p1 = alloc.Allocate(200 * M);
    //
    // void* pointers[10];
    // for (int i = 0; i < 10; ++i)
    // 	pointers[i] = alloc.Allocate(30 * M);
    //
    // alloc.Deallocate(p2);
    // alloc.Deallocate(p1);
    //
    // for (int i = 0; i < 10; ++i)
    // 	alloc.Deallocate(pointers[i]);

    MyAllocator my(1024 * 1024);
    std::vector<void*> ps;
    for (int i = 0; i < 10000; i++) {
        if (rand() % 100 < 40) {
            if (ps.size() == 0)continue;
            auto index = rand() % ps.size();
            my.Deallocate(ps[index]);
            std::cout << "Deallocated " << ps[index] << std::endl;
            ps[index] = ps.back();
            ps.pop_back();

        }
        else {
            size_t sz = rand() % 512 + 512;
            auto p = my.Allocate(sz);
            if (p) {
                ps.push_back(p);
                std::cout << "allocated " << ps.back() << std::endl;
            }
            else {
                std::cout << "can't allocate" << std::endl;
            }
        }
    }

    while (!ps.empty()) {
        auto p = ps.back();
        my.Deallocate(p);
        ps.pop_back();
        std::cout << "Deallocated " << p << std::endl;
    }
	system("pause");
	return 0;
}
