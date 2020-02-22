#include "shader.h"
#include "buffers.h"
#include "window.h"

#include <exception>
#include <stdio.h>


void init() {
    window::init(700, 400, "MandelbrotGL");
    shader::init();
    buffers::init();
}

int main(void)
{
    try {
        init();
    } catch (std::exception e) {
        printf("Exception thrown during initialisation:\n %s", e.what());
        return 1;
    }

    window::display();
    
    return 0;
}
