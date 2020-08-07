#include "Worker.h"

#include <iostream>

Worker::~Worker() {
    std::cout << "Worker " << this << " deleted\n";
}
