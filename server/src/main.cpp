#include <iostream>

#include "test.h"

int main(int argc, char** argv) {

    test(200);

    std::cout << "This is server\n" << argc << ' ' << argv[0] << '\n';

    return 0;
}
