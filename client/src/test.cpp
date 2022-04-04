#include <iostream>

#include "test.h"

void test(int a) {
    for (int i=0; i < a; ++i) {
        std::cout << "This is client-test\n";
    }
}
