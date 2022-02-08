#include "cmdline.h"
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"

int main(int argc, char **argv) {
   // use_arguments(argc, argv);
    std::string temp = argv[1];
    if(temp == "--test"){
        Catch::Session().run(1,argv);
    }
    return 0;

}
