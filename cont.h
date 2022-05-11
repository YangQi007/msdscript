//
// Created by YANG QI on 5/11/22.
//

#ifndef MSDSCRIPT_CONT_H
#define MSDSCRIPT_CONT_H


#include "expr.h"
#include "val.h"
#include "env.h"
#include "pointer.h"
#include "step.h"

class Expr;
class Cont;
class Val;
class Env;

CLASS (Cont){
public:
    virtual void step_continue() = 0;
    static PTR(Cont) done;
};

class Done : public Cont{
public:
    Done();
    void step_continue();
};

class RightThenAddCont : public Cont{
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
};

class AddCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

    void step_continue();
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest);
};

class RightThenMultCont : public Cont{
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
};

class MultCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

    void step_continue();
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest);
};

class IfBranchCont : public Cont{
public:
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
};

class LetBodyCont : public Cont{
public:
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
};

class ArgThenCallCont : public Cont{
public:
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);
};

class CallCont : public Cont{
public:
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;

    void step_continue();
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);
};

class RightThenEqCont : public Cont{
public:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    void step_continue();
    RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
};

class EqCont : public Cont{
public:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

    void step_continue();
    EqCont(PTR(Val) lhs_val, PTR(Cont) rest);
};


#endif //MSDSCRIPT_CONT_H
