//
// Created by YANG QI on 2/3/22.
//
#include "catch.h"
#include "expr.h"
#include "val.h"
#include "env.h"
#include <stdexcept>
#include <sstream>

//Helper function to_string and to_string_pretty make it easier for testing
std::string Expr::to_string() {
    std::stringstream ss;
    this -> print(ss);
    return ss.str();
}

std::string Expr::to_string_pretty() {
    std::stringstream ss;
    this ->pretty_print(ss);
    return ss.str();
}

//subclass NumExpr
NumExpr::NumExpr(int val) {
    this->val = val;

}

bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr) n = CAST(NumExpr) (e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW (NumVal) (this->val);
}

bool NumExpr::has_variable() {
    return false;
}

//PTR(Expr) NumExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (NumExpr)(this->val);
//}

void NumExpr::print(std::ostream &out) {
    out << this->val;
}

void NumExpr::pretty_print(std::ostream &out) {
    pretty_print_at(out,prec_none,0);
}

void NumExpr::pretty_print_at(std::ostream &out, precedence_t p, long *positon) {
    out << this->val;
}

/////////////////////////////////////////////////////////////////////
//subclass AddExpr
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) n = CAST(AddExpr)(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

PTR(Val)  AddExpr::interp(PTR(Env) env) {
    return ((this->lhs->interp(env)) -> add_to(this->rhs->interp(env)));
}

bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

//PTR(Expr) AddExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (AddExpr)((this->lhs)->subst(s, e), (this->rhs)->subst(s, e));
//}

void AddExpr::print(std::ostream &out) {
    out<<"(";
    lhs->print(out);
    out<<"+";
    rhs->print(out);
    out<<")";
}

void AddExpr::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr);

}

void AddExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    if (p == prec_add || p == prec_mult || p == prec_let){
        out << "(";
    }
    lhs->pretty_print_at(out,prec_add,position);
    out << " + ";
    rhs->pretty_print_at(out,prec_none,position);

    if (p == prec_add || p == prec_mult || p == prec_let){
        out << ")";
    }
}

////////////////////////////////////////////////////////////////////
//subclass MultExpr
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) n = CAST(MultExpr)(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

PTR(Val)  MultExpr::interp(PTR(Env) env) {
    return ((this->lhs->interp(env)) -> mult_to( this->rhs->interp(env)));
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

//PTR(Expr) MultExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (MultExpr)((this->lhs)->subst(s, e), (this->rhs)->subst(s, e));
//}

void MultExpr::print(std::ostream &out) {
    out<<"(";
    lhs->print(out);
    out<<"*";
    rhs->print(out);
    out<<")";

}

void MultExpr::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr );

}

void MultExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    if (p == prec_add || p == prec_mult || p == prec_let){
        out << "(";
    }
    lhs->pretty_print_at(out,prec_mult, position);
    out << " * ";
    rhs->pretty_print_at(out,prec_none, position);
    if (p == prec_add || p == prec_mult || p == prec_let){
        out << ")";
    }

}

/////////////////////////////////////////////////////////////////////
//subclass variables
VarExpr::VarExpr(std::string val) {
    this->val = val;
}

bool VarExpr::equals(PTR(Expr) e) {
    PTR(VarExpr) n = CAST(VarExpr)(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

PTR(Val)  VarExpr::interp(PTR(Env) env) {
    return env -> lookup(val);
}

bool VarExpr::has_variable() {
    return true;
}

//PTR(Expr) VarExpr::subst(std::string s, PTR(Expr) e) {
//    if (this->val == s ){
//        return e;
//    }else
//        return THIS;
//}

void VarExpr::print(std::ostream &out) {
    out << this->val;

}

void VarExpr::pretty_print(std::ostream &out) {
    pretty_print_at(out,prec_none,0);

}

void VarExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    out << this->val;

}

////////////////////////////////////////////////////////////////////
//_letExpr expression subclass
_letExpr::_letExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool _letExpr::equals(PTR(Expr) e) {
    PTR(_letExpr) n = CAST(_letExpr)(e);
    if (n == NULL){
        return false;
    }else{
        return (this->lhs == (n->lhs) && this->rhs ->equals(n->rhs)
        && this->body ->equals(n->body));
    }
}

PTR(Val)  _letExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs, rhs_val, env);
    return body->interp(new_env);
        
}

bool _letExpr::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

//PTR(Expr) _letExpr::subst(std::string s, PTR(Expr) e) {
//    if(s == this->lhs){
//        return NEW (_letExpr)(this->lhs, this->rhs->subst(s, e), this->body);
//    }
//    else{
//        return NEW (_letExpr)(this->lhs, this->rhs->subst(s, e), this->body->subst(s, e));
//    }
//}

void _letExpr::print(std::ostream &out) {
    out << "(_let ";
    out << this->lhs;
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";

}

void _letExpr::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr);

}

void _letExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    if (p == prec_add || p == prec_mult || p == prec_let){
        out << "(";
    }
    long curr_posi = out.tellp();
    long spaces = curr_posi - *position;
    out << " _let " << lhs << " = ";
    rhs->pretty_print_at(out,prec_none, position);
    out << "\n";
    *position = out.tellp();
    int spaces_count = 0;
    while (spaces_count < spaces){
        out << " ";
        spaces_count++;
    }
    out << "_in ";
    body->pretty_print_at(out,prec_none, position);
    if (p == prec_add || p == prec_mult || p == prec_let){
        out << ")";
    }
}

// boolean expression
BoolExpr::BoolExpr(bool val) {
   this -> val = val;

}

bool BoolExpr::equals(PTR(Expr) e) {
    PTR(BoolExpr) boolVal = CAST(BoolExpr)(e);
    if (boolVal == NULL)
    return false;
    return this -> val == boolVal -> val;
}

PTR(Val)  BoolExpr::interp(PTR(Env) env) {
    return NEW (BoolVal) (this -> val);
}

bool BoolExpr::has_variable() {
    return false;
}

//PTR(Expr) BoolExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (BoolExpr) (this -> val);
//}

void BoolExpr::print(std::ostream &out) {
    if (this ->val == true){
        out << "_true";
    }else {
        out << "_false";
    }
}

void BoolExpr::pretty_print(std::ostream &out) {
    pretty_print_at(out,prec_none,0);
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    print(out);
}

// equal expression
EqualExpr::EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(PTR(Expr) e) {
    PTR(EqualExpr) n = CAST(EqualExpr)(e);
    if (n == NULL)
        return false;
    return lhs -> equals(n->lhs) && rhs -> equals(n->rhs);
}

PTR(Val) EqualExpr::interp(PTR(Env) env) {
    return NEW (BoolVal) (this->lhs->interp(env)->equals(this->rhs->interp(env)));
}

bool EqualExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

//PTR(Expr) EqualExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (EqualExpr)(lhs->subst(s,e),rhs->subst(s,e));
//}

void EqualExpr::print(std::ostream &out) {
    out << "(";
    this->lhs -> print(out);
    out << "==";
    this->rhs -> print(out);
    out << ")";
}

void EqualExpr::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    this-> lhs ->pretty_print_at(out,prec_eq,posi_ptr);
    out << " == ";
    this -> rhs ->pretty_print_at(out,prec_none,posi_ptr);
    //pretty_print_at(out,prec_eq,posi_ptr);

}

void EqualExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    if (p == prec_none){
        this->lhs->pretty_print_at(out, prec_eq, position);
        out << " == ";
        this->rhs->pretty_print_at(out, prec_none, position);
    }
    else {
        out << "(";
        this->lhs->pretty_print_at(out, prec_eq, position);
        out << " == ";
        this->rhs->pretty_print_at(out, prec_none, position);
        out << ")";
    }
}

// conditions expressions

IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) then_part, PTR(Expr) else_part) {
    this -> condition = condition;
    this -> then_part = then_part;
    this -> else_part = else_part;
}

bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) n = CAST(IfExpr)(e);
    if (n == NULL) return false;
    return  this -> condition -> equals(n -> condition) &&
            this -> then_part -> equals(n -> then_part) &&
            this -> else_part -> equals(n -> else_part);
}

PTR(Val) IfExpr::interp(PTR(Env) env) {
    PTR(Val) con = this->condition -> interp(env);
    PTR(BoolVal) casted_con = CAST(BoolVal)(con);

    if(casted_con->val)
        return then_part -> interp(env);
    else
        return else_part -> interp(env);
}

bool IfExpr::has_variable() {
    return  this-> condition ->has_variable() ||
            this-> then_part ->has_variable() ||
            this-> else_part ->has_variable() ;
}

//PTR(Expr) IfExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (IfExpr) (this->condition -> subst(s,e),
//                       this->then_part->subst(s,e),
//                       this->else_part->subst(s,e));
//}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    this -> condition ->print(out);
    out << " _then ";
    this -> then_part ->print(out);
    out << " _else ";
    this -> else_part ->print(out);
    out << ")";
}

void IfExpr::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr);
}

void IfExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    if (p == prec_add || p == prec_mult || p == prec_let || p == prec_eq){
        out << "(";
    }
    long curr_posi = out.tellp();
    long spaces = curr_posi - *position;
    out << " _if ";
    this->condition ->pretty_print_at(out,prec_none, position);
    out << "\n";
    *position = out.tellp();
    int spaces_count = 0;
    while (spaces_count < spaces){
        out << " ";
        spaces_count++;
    }
    out << "_then ";
    this->then_part ->pretty_print_at(out,prec_none, position);
    out << "\n";
    *position = out.tellp();
    spaces_count = 0;
    while (spaces_count < spaces){
        out << " ";
        spaces_count++;
    }
    out << "_else ";
    this->else_part ->pretty_print_at(out, prec_none,position);
    if (p == prec_add || p == prec_mult || p == prec_let || p == prec_eq){
        out << ")";
    }
}


//subclass Functions
FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this -> formal_arg = formal_arg;
    this -> body = body;
}

bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) n = CAST(FunExpr)(e);
    if (n == NULL) return false;
    return (this -> formal_arg == n -> formal_arg &&
            this -> body -> equals(n -> body));
}

PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW (FunVal)(this->formal_arg, this->body,env);
}

//PTR(Expr) FunExpr::subst(std::string s, PTR(Expr) e) {
//    if (s == formal_arg)
//        return NEW (FunExpr)(this->formal_arg, body);
//    if (s != formal_arg)
//        return this;
//    else
//        return NEW (FunExpr)(this->formal_arg, body -> subst(s, e));
//}

void FunExpr::print(std::ostream &out) {
    out << "(_fun (" << this->formal_arg << ") ";
    this -> body -> print(out);
    out << ")";
}

bool FunExpr::has_variable() {

}

void FunExpr::pretty_print(std::ostream &out) {

}

void FunExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {

}

//subclass CallExpr
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this -> to_be_called = to_be_called;
    this -> actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) n = CAST(CallExpr)(e);
    if (n == NULL) return false;
    return (this -> to_be_called -> equals(n -> to_be_called) &&
            this -> actual_arg -> equals(n->actual_arg));
}

PTR(Val) CallExpr::interp(PTR(Env) env) {
    return to_be_called -> interp(env) -> call(actual_arg -> interp(env));
}

//PTR(Expr) CallExpr::subst(std::string s, PTR(Expr) e) {
//    return NEW (CallExpr)(to_be_called -> subst(s, e),
//                        actual_arg -> subst(s, e));
//}

void CallExpr::print(std::ostream &out) {
    this -> to_be_called->print(out);
    out << "(";
    this -> actual_arg->print(out);
    out << ")";
}

bool CallExpr::has_variable() {

}

void CallExpr::pretty_print(std::ostream &out) {

}

void CallExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {

}
