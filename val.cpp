//
// Created by YANG QI on 4/14/22.
//

#include "val.h"
#include "expr.h"
#include "env.h"
/*
Original
int
Expr
NumExpr, AddExpr, MultExpr
Var, Let
int interp()
Expr subst(...)
void print(...), ...
 //////////////////////////////////
Refactored
Val
NumVal, BoolVal
Expr to_expr(), ...
Expr
NumExpr, AddExpr, MultExpr
VarExpr, LetExpr
BoolExpr,  EqExpr, IfExpr
Val interp()
Expr subst(...)
void print(...), ...
  **/

//subclass numValue
NumVal::NumVal(int val) {
    this -> val = val;

}

bool NumVal::equals(PTR(Val) val) {
    PTR(NumVal) numVal = CAST(NumVal)(val);
    if (numVal == NULL)
    return false;
    return this -> val == numVal -> val;
}

PTR(Val) NumVal::add_to(PTR(Val) val) {
    PTR(NumVal) numVal = CAST(NumVal)(val);
    if (numVal == NULL)
        throw std::runtime_error("Should be a number.");
    return NEW (NumVal) (this -> val + numVal -> val);
}

PTR(Val) NumVal::mult_to(PTR(Val) val) {
    PTR(NumVal) numVal = CAST(NumVal )(val);
    if (numVal == NULL)
        throw std::runtime_error("Should be a number.");
    return NEW (NumVal) (this -> val * numVal -> val);
}

//PTR(Expr) NumVal::to_expr() {
//    return NEW (NumExpr)(this -> val);
//}

std::string NumVal::to_string() {
    return std::to_string(this -> val);
}

PTR(Val) NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Error: call() is unavailable.");
}


//subclass boolValue
BoolVal::BoolVal(bool val) {
    this -> val = val;
}

bool BoolVal::equals(PTR(Val) val) {
    PTR(BoolVal) boolVal = CAST(BoolVal)(val);
    if (boolVal == NULL)
    return false;
    return this ->val == boolVal -> val;
}

PTR(Val) BoolVal::add_to(PTR(Val) val) {
    throw std::runtime_error("Cannot add BooVal type.");
}

PTR(Val) BoolVal::mult_to(PTR(Val) val) {
    throw std::runtime_error("Cannot multiply BooVal type.");
}

//PTR(Expr) BoolVal::to_expr() {
//    return NEW (BoolExpr)(this ->val);
//}

std::string BoolVal::to_string() {
    if (this -> val)
        return "_true";
    else
        return "_false";
}

PTR(Val) BoolVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("Error: call() is unavailable.");
}


//subclass Funval
FunVal::FunVal(std::string formal_arg, PTR(Expr) body,PTR(Env) env) {
    this -> formal_arg = formal_arg;
    this -> body = body;
    this ->env = env;
}

bool FunVal::equals(PTR(Val) val) {
    PTR(FunVal) n = CAST(FunVal)(val);
    if (n == NULL) return false;
    else {
        return (this -> formal_arg == n -> formal_arg &&
                this -> body -> equals(n -> body));
    }
}

PTR(Val) FunVal::add_to(PTR(Val) val) {
    throw std::runtime_error("Error: Invalid input.");
}

PTR(Val) FunVal::mult_to(PTR(Val) val) {
    throw std::runtime_error("Error: Invalid input");
}

//PTR(Expr) FunVal::to_expr() {
//    return NEW (FunExpr)(this -> formal_arg, this -> body);
//}

std::string FunVal::to_string() {
    return ("_fun (" + this -> formal_arg + ") " +
            this -> body -> to_string());
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return body -> interp(NEW(ExtendedEnv)(formal_arg, actual_arg,env));
}
