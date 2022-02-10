//
// Created by YANG QI on 2/3/22.
//
#include "catch.h"
#include "expr.h"
#include <stdexcept>

//subclass Num
Num::Num(int val) {
    this->val = val;

}

bool Num::equals(Expr *e) {
    Num *n = dynamic_cast<Num *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

int Num::interp() {
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string s, Expr *e) {
    return new Num(val);
}

/////////////////////////////////////////////////////////////////////
//subclass Add
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool Add::equals(Expr *e) {
    Add *n = dynamic_cast<Add *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs == n->lhs && this->rhs == n->rhs;
    }
}

int Add::interp() {
    return (this->lhs->interp() + this->rhs->interp());
}

bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Add::subst(std::string s, Expr *e) {
    return new Add((this->lhs)->subst(s,e),(this->rhs)->subst(s,e));
}

////////////////////////////////////////////////////////////////////
//subclass Mult
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool Mult::equals(Expr *e) {
    Mult *n = dynamic_cast<Mult *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs == n->lhs && this->rhs == n->rhs;
    }
}

int Mult::interp() {
    return (this->lhs->interp() * this->rhs->interp());
}

bool Mult::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Mult::subst(std::string s, Expr *e) {
    return new Mult((this->lhs)->subst(s,e),(this->rhs)->subst(s,e));
}

/////////////////////////////////////////////////////////////////////
//subclass variables
Variables::Variables(std::string val) {
    this->val = val;
}

bool Variables::equals(Expr *e) {
    Variables *n = dynamic_cast<Variables *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

int Variables::interp() {
    throw std::runtime_error("no value for variable");
}

bool Variables::has_variable() {
    return true;
}

Expr* Variables::subst(std::string s, Expr *e) {
    if (this->val == s ){
        return e;
    }else
        return new Variables(s);
}

////////////////////////////////////////////////////////////////////





