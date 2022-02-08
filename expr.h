//
// Created by YANG QI on 2/3/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H
#include <string>
#include <stdio.h>
#include "catch.h"
#include <iostream>

class Expr{
public:
    virtual bool equals(Expr *e) = 0;
};

class Num : public Expr{
public:
    int val;

    Num(int val);
    bool equals(Expr *e) override;

};

class Add : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;

};

class Mult : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;

};

class Variables : public Expr{
public:
    std::string val;
    Variables(std::string val);
    bool equals(Expr *e) override;
};
#endif //MSDSCRIPT_EXPR_H
