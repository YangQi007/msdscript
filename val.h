//
// Created by YANG QI on 4/14/22.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <stdio.h>
#include <string>
#include "pointer.h"
#include "expr.h"

class Expr;
class Env;

CLASS (Val) {
public:
    virtual ~Val() {};
    virtual bool equals(PTR(Val) val) = 0;
    virtual PTR(Val) add_to(PTR(Val) val) = 0;
    virtual PTR(Val) mult_to(PTR(Val) val) = 0;
    //virtual PTR(Expr) to_expr() = 0;
    virtual std::string to_string() = 0;
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;

};

class NumVal : public Val{
public:
    int val;
    NumVal (int val);

    bool equals(PTR(Val) val) override;
    PTR(Val) add_to(PTR(Val) val) override;
    PTR(Val) mult_to(PTR(Val) val) override;
    //PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};

class BoolVal : public Val{
public:
    bool val;
    BoolVal (bool val);

    bool equals(PTR(Val) val) override;
    PTR(Val) add_to(PTR(Val) val) override;
    PTR(Val) mult_to(PTR(Val) val) override;
    //PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};

class FunVal: public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

    FunVal(std::string formal_arg, PTR(Expr) body,PTR(Env) env);

    bool equals(PTR(Val) val) override;
    PTR(Val) add_to(PTR(Val) val) override;
    PTR(Val) mult_to(PTR(Val) val) override;
    //PTR(Expr) to_expr() override;
    std::string to_string() override;
    PTR(Val) call(PTR(Val) actual_arg) override;
};


#endif //MSDSCRIPT_VAL_H
