//
// Created by YANG QI on 2/1/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "cmdline.h"
#include <iostream>

void use_arguments(int argc, char * argv[]){
    if (argc == 1){
        return;
    }
    if (argc > 1){
        bool test = false;
        for (int i = 1; i < argc; i++){
            std::string argus = argv[i];
            if (argus == "--help"){
                std::cout<<"Valid command lines: '--test' '--interp' '--print' '--pretty-print'."<<"\n";
                exit(0);
            }
            else if (argus == "--test"){
                if (test == false){
                    if (Catch::Session().run(1,argv) != 0){
                        std::cerr << "Tests failed!" << "\n";
                        exit(1);
                    }
                    test = true;
                }
                else{
                        std::cout << "Tested already!" <<"\n";
                        exit(1);
                    }
                }
            else if (argus == "--interp"){
                Expr *input = parse_expr(std::cin);
                Val *value = input->interp();
                std::cout << value << "\n";
            }
            else if (argus == "--print"){
                Expr *input = parse_expr(std::cin);
                input->print(std::cout);
            }
            else if (argus == "--pretty-print"){
                Expr *input = parse_expr(std::cin);
                std::cout << input->to_string_pretty();
            }
            else {
                std::cerr << "Invalid Argument\n";
                exit(1);
            }
            std::cout << "\n";
        }
    }
}
