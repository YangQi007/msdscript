//
// Created by YANG QI on 2/1/22.
//

#include "cmdline.h"
#include <iostream>

void use_arguments(int argc, char * argv[]){
    if (argc == 1){
        return;
    }
    if (argc > 1){
        int count = 0;
        for (int i = 1; i < argc; i++){
            std::string argus = argv[i];
            if (argus == "--help"){
                std::cout<<"'--test' or '--help' is a valid argument,otherwise is invalid."<<"\n";
                exit(0);
            }
            else if (argus == "--test" && count == 0){
                std::cout<<"Tests passed"<<"\n";
            }
            else if (argus == "--test" && count >0){
                std::cerr<<"Error! Please enter valid arguments" <<"\n";
                exit(1);
            }
            else {
                std::cerr<<"Error! Please enter valid arguments" <<"\n";
                exit(1);
            }
             count++;
        }
    }
}
