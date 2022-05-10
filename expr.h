//
// Created by YANG QI on 2/3/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H
#include <string>
#include <stdio.h>
#include "catch.h"
#include "val.h"
#include <iostream>
#include "pointer.h"

typedef enum {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult,       // = 2
    prec_let,        // = 3
    prec_eq
} precedence_t;

class Expr{
public:
    virtual bool equals(PTR(Expr) e) = 0;
    virtual PTR(Val)  interp() = 0;
    virtual bool has_variable() = 0;
    virtual PTR(Expr) subst(std::string s, PTR(Expr) e) = 0;
    virtual void print(std::ostream &out) = 0;
    virtual void pretty_print(std::ostream &out) = 0;
    virtual void pretty_print_at(std::ostream &out,precedence_t p,long *position) = 0;
    std::string to_string();
    std::string to_string_pretty();

};

class NumExpr : public Expr{
public:
    int val;

    NumExpr(int val);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class AddExpr : public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;


};

class MultExpr : public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class VarExpr : public Expr{
public:
    std::string val;

    VarExpr(std::string val);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class _letExpr : public Expr{
public:
    std::string lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;

    _letExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class BoolExpr : public Expr{
public:
    bool val;

    BoolExpr(bool val);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class EqualExpr : public Expr{
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

    EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class IfExpr : public Expr{
public:
    PTR(Expr) condition;
    PTR(Expr) then_part;
    PTR(Expr) else_part;

    IfExpr(PTR(Expr) condition, PTR(Expr) then_part, PTR(Expr) else_part);
    bool equals(PTR(Expr) e) override;
    PTR(Val)  interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class FunExpr: public Expr {
public:
    std::string formal_arg;
    PTR(Expr) body;

     FunExpr (std::string formal_arg, PTR(Expr) body);
     bool equals(PTR(Expr) e) override;
     PTR(Val) interp() override;
     bool has_variable() override;
     PTR(Expr) subst(std::string s, PTR(Expr) e) override;
     void print(std::ostream& out) override;
     void pretty_print(std::ostream &out) override;
     void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};


class CallExpr: public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

    CallExpr (PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    bool equals(PTR(Expr) e) override;
    PTR(Val) interp() override;
    bool has_variable() override;
    PTR(Expr) subst(std::string s, PTR(Expr) e) override;
    void print(std::ostream& out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};


#endif //MSDSCRIPT_EXPR_H
