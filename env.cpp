//
// Created by YANG QI on 5/10/22.
//

#include "env.h"

PTR(Env) Env::empty = NEW(EmptyEnv)();

EmptyEnv::EmptyEnv() {

}

PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}

ExtendedEnv::ExtendedEnv(std::string name, std::shared_ptr<Val> val, std::shared_ptr<Env> rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;
}

PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == name){
        return val;
    } else {
        return rest->lookup(find_name);
    }
}
