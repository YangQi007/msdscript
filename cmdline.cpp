//
// Created by YANG QI on 2/1/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "cmdline.h"
#include <iostream>
#include "env.h"

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
                PTR(Expr) input = parse_expr(std::cin);
                PTR(Val) value = input->interp(Env::empty);
                std::cout << value->to_string() << "\n";
            }
            else if (argus == "--print"){
                PTR(Expr) input = parse_expr(std::cin);
                input->print(std::cout);
            }
            else if (argus == "--pretty-print"){
                PTR(Expr) input = parse_expr(std::cin);
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
