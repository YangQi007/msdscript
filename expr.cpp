//
// Created by YANG QI on 2/3/22.
//
#include "catch.h"
#include "expr.h"


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

TEST_CASE("Num"){
    CHECK((new Num(1)) ->equals(new Num(1)) == true);
    CHECK((new Num(1)) ->equals(new Num(2)) == false);
}

TEST_CASE("Add"){
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Add *add1 = new Add(n1,n2);
    Add *add2= new Add(n1,n3);
    Add *add3 = new Add(n1,n2);
    CHECK(add1->equals(add3) == true);
    CHECK(add1->equals(add2) == false);
}

TEST_CASE("Mult"){
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Mult *m1 = new Mult(n1,n2);
    Mult *m2= new Mult(n1,n3);
    Mult *m3 = new Mult(n1,n2);
    CHECK(m1->equals(m3) == true);
    CHECK(m1->equals(m2) == false);
}

TEST_CASE("Variable"){
    CHECK((new Variables("1")) ->equals(new Variables("1")) == true);
    CHECK((new Variables("1")) ->equals(new Variables("2")) == false);

}



