//
// Created by YANG QI on 2/3/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H
#include <string>
#include <stdio.h>
#include "catch.h"
#include <iostream>

typedef enum {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult       // = 2
} precedence_t;

class Expr{
public:
    virtual bool equals(Expr *e) = 0;
    virtual int interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string s, Expr *e) = 0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out) = 0;
    virtual void pretty_print_at(std::ostream &out,precedence_t p) = 0;
    std::string to_string();
    std::string to_string_pretty();


};

class Num : public Expr{
public:
    int val;

    Num(int val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p) override;

};

class Add : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p) override;


};

class Mult : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p) override;

};

class Variables : public Expr{
public:
    std::string val;

    Variables(std::string val);
    bool equals(Expr *e) override;
    int interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p) override;

};
#endif //MSDSCRIPT_EXPR_H
