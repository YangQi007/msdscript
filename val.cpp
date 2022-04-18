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

//subclass numValue
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


//subclass boolValue
BoolVal::BoolVal(bool val) {
    this -> val = val;
}

bool BoolVal::equals(Val *val) {
    BoolVal *boolVal = dynamic_cast<BoolVal *>(val);
    if (boolVal == NULL)
    return false;
    return this ->val == boolVal -> val;
}

Val *BoolVal::add_to(Val *val) {
    throw std::runtime_error("Cannot add BooVal type.");
}

Val *BoolVal::mult_to(Val *val) {
    throw std::runtime_error("Cannot multiply BooVal type.");
}

Expr *BoolVal::to_expr() {
    return NULL;
}

std::string BoolVal::to_string() {
    if (this -> val)
        return "_true";
    else
        return "_false";
}
