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
    virtual bool equals(Expr *e) = 0;
    virtual Val * interp() = 0;
    virtual bool has_variable() = 0;
    virtual Expr* subst(std::string s, Expr *e) = 0;
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
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class AddExpr : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    AddExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;


};

class MultExpr : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    MultExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class VarExpr : public Expr{
public:
    std::string val;

    VarExpr(std::string val);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class _letExpr : public Expr{
public:
    std::string lhs;
    Expr *rhs;
    Expr *body;

    _letExpr(std::string lhs, Expr *rhs, Expr *body);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position) override;

};

class BoolExpr : public Expr{
public:
    bool val;

    BoolExpr(bool val);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class EqualExpr : public Expr{
public:
    Expr *lhs;
    Expr *rhs;

    EqualExpr(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class IfExpr : public Expr{
public:
    Expr *condition;
    Expr *then_part;
    Expr *else_part;

    IfExpr(Expr *condition, Expr *then_part, Expr *else_part);
    bool equals(Expr *e) override;
    Val * interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream &out) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};

class FunExpr: public Expr {
public:
    std::string formal_arg;
    Expr* body;

     FunExpr (std::string formal_arg, Expr* body);
     bool equals(Expr* e) override;
     Val* interp() override;
     bool has_variable() override;
     Expr* subst(std::string s, Expr *e) override;
     void print(std::ostream& os) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};


class CallExpr: public Expr {
public:
    Expr* to_be_called;
    Expr* actual_arg;

    CallExpr (Expr* to_be_called, Expr* actual_arg);
    bool equals(Expr* e) override;
    Val* interp() override;
    bool has_variable() override;
    Expr* subst(std::string s, Expr *e) override;
    void print(std::ostream& os) override;
    void pretty_print(std::ostream &out) override;
    void pretty_print_at(std::ostream &out,precedence_t p,long *position)  override;

};


#endif //MSDSCRIPT_EXPR_H
