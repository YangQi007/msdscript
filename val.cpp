//
// Created by YANG QI on 4/14/22.
//

#include "val.h"
#include "expr.h"
/*
Original
int
Expr
NumExpr, AddExpr, MultExpr
Var, Let
int interp()
Expr* subst(...)
void print(...), ...
 //////////////////////////////////
Refactored
Val
NumVal, BoolVal
Expr* to_expr(), ...
Expr
NumExpr, AddExpr, MultExpr
VarExpr, LetExpr
BoolExpr,  EqExpr, IfExpr
Val* interp()
Expr* subst(...)
void print(...), ...
 * **/
NumVal::NumVal(int val) {
    this -> val = val;

}

bool NumVal::equals(Val *val) {
    NumVal *numVal = dynamic_cast<NumVal *>(val);
    if (numVal == NULL)
    return false;
    return this -> val == numVal -> val;
}

Val *NumVal::add_to(Val *val) {
    NumVal *numVal = dynamic_cast<NumVal *>(val);
    if (numVal == NULL)
        throw std::runtime_error("Should be a number.");
    return new NumVal (this -> val + numVal -> val);
}

Val *NumVal::mult_to(Val *val) {
    NumVal *numVal = dynamic_cast<NumVal *>(val);
    if (numVal == NULL)
        throw std::runtime_error("Should be a number.");
    return new NumVal (this -> val * numVal -> val);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this -> val);
}

std::string NumVal::to_string() {
    return std::to_string(this -> val);
}
