//
// Created by YANG QI on 2/3/22.
//
#include "catch.h"
#include "expr.h"
#include "val.h"
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

bool NumExpr::equals(Expr *e) {
    NumExpr *n = dynamic_cast<NumExpr *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

Val * NumExpr::interp() {
    return new NumVal (this->val);
}

bool NumExpr::has_variable() {
    return false;
}

Expr* NumExpr::subst(std::string s, Expr *e) {
    return new NumExpr(val);
}

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
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool AddExpr::equals(Expr *e) {
    AddExpr *n = dynamic_cast<AddExpr *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

Val * AddExpr::interp() {
    return ((this->lhs->interp()) -> add_to(this->rhs->interp()));
}

bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* AddExpr::subst(std::string s, Expr *e) {
    return new AddExpr((this->lhs)->subst(s, e), (this->rhs)->subst(s, e));
}

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
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool MultExpr::equals(Expr *e) {
    MultExpr *n = dynamic_cast<MultExpr *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

Val * MultExpr::interp() {
    return ((this->lhs->interp()) -> mult_to( this->rhs->interp()));
}

bool MultExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* MultExpr::subst(std::string s, Expr *e) {
    return new MultExpr((this->lhs)->subst(s, e), (this->rhs)->subst(s, e));
}

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

bool VarExpr::equals(Expr *e) {
    VarExpr *n = dynamic_cast<VarExpr *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

Val * VarExpr::interp() {
    throw std::runtime_error("no value for variable");
}

bool VarExpr::has_variable() {
    return true;
}

Expr* VarExpr::subst(std::string s, Expr *e) {
    if (this->val == s ){
        return e;
    }else
        return this;
}

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
_letExpr::_letExpr(std::string lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool _letExpr::equals(Expr *e) {
    _letExpr *n = dynamic_cast<_letExpr *>(e);
    if (n == NULL){
        return false;
    }else{
        return (this->lhs == (n->lhs) && this->rhs ->equals(n->rhs)
        && this->body ->equals(n->body));
    }
}

Val * _letExpr::interp() {

        return (this->body->subst(this->lhs,(rhs->interp())->to_expr()))
                ->interp();
        
}

bool _letExpr::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

Expr *_letExpr::subst(std::string s, Expr *e) {
    if(s == this->lhs){
        return new _letExpr(this->lhs, this->rhs->subst(s, e), this->body);
    }
    else{
        return new _letExpr(this->lhs, this->rhs->subst(s, e), this->body->subst(s, e));
    }
}

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

bool BoolExpr::equals(Expr *e) {
    BoolExpr *boolVal = dynamic_cast<BoolExpr *>(e);
    if (boolVal == NULL)
    return false;
    return this -> val == boolVal -> val;
}

Val * BoolExpr::interp() {
    return new BoolVal (this -> val);
}

bool BoolExpr::has_variable() {
    return false;
}

Expr *BoolExpr::subst(std::string s, Expr *e) {
    return new BoolExpr (this -> val);
}

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
EqualExpr::EqualExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(Expr *e) {
    EqualExpr *n = dynamic_cast<EqualExpr *>(e);
    if (n == NULL)
        return false;
    return lhs -> equals(n->lhs) && rhs -> equals(n->rhs);
}

Val *EqualExpr::interp() {
    return new BoolVal (this->lhs->interp()->equals(this->rhs->interp()));
}

bool EqualExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr *EqualExpr::subst(std::string s, Expr *e) {
    return new EqualExpr(lhs->subst(s,e),rhs->subst(s,e));
}

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

IfExpr::IfExpr(Expr *condition, Expr *then_part, Expr *else_part) {
    this -> condition = condition;
    this -> then_part = then_part;
    this -> else_part = else_part;
}

bool IfExpr::equals(Expr *e) {
    IfExpr *n = dynamic_cast<IfExpr*>(e);
    if (n == NULL) return false;
    return  this -> condition -> equals(n -> condition) &&
            this -> then_part -> equals(n -> then_part) &&
            this -> else_part -> equals(n -> else_part);
}

Val *IfExpr::interp() {
    Val *con = this->condition -> interp();
    BoolVal *casted_con = dynamic_cast<BoolVal*>(con);

    if(casted_con->val)
        return then_part -> interp();
    else
        return else_part -> interp();
}

bool IfExpr::has_variable() {
    return  this-> condition ->has_variable() ||
            this-> then_part ->has_variable() ||
            this-> else_part ->has_variable() ;
}

Expr *IfExpr::subst(std::string s, Expr *e) {
    return new IfExpr (this->condition -> subst(s,e),
                       this->then_part->subst(s,e),
                       this->else_part->subst(s,e));
}

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
FunExpr::FunExpr(std::string formal_arg, Expr *body) {
    this -> formal_arg = formal_arg;
    this -> body = body;
}

bool FunExpr::equals(Expr *e) {
    FunExpr *n = dynamic_cast<FunExpr*>(e);
    if (n == NULL) return false;
    return (this -> formal_arg == n -> formal_arg &&
            this -> body -> equals(n -> body));
}

Val *FunExpr::interp() {
    return new FunVal(formal_arg, body);
}

Expr *FunExpr::subst(std::string s, Expr *e) {
//    if (s == formal_arg)
//        return new FunExpr(formal_arg, body);
    if (s != formal_arg)
        return this;
    else
        return new FunExpr(formal_arg, body -> subst(s, e));
}

void FunExpr::print(std::ostream &os) {
    os << "(_fun (" << formal_arg << ") ";
    this -> body -> print(os);
    os << ")";
}

bool FunExpr::has_variable() {

}

void FunExpr::pretty_print(std::ostream &out) {

}

void FunExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {

}

//subclass CallExpr
CallExpr::CallExpr(Expr *to_be_called, Expr *actual_arg) {
    this -> to_be_called = to_be_called;
    this -> actual_arg = actual_arg;
}

bool CallExpr::equals(Expr *e) {
    CallExpr* n = dynamic_cast<CallExpr*>(e);
    if (n == NULL) return false;
    return (this -> to_be_called -> equals(n -> to_be_called) &&
            this -> actual_arg -> equals(n->actual_arg));
}

Val *CallExpr::interp() {
    return to_be_called -> interp() -> call(actual_arg -> interp());
}

Expr *CallExpr::subst(std::string s, Expr *e) {
    return new CallExpr(to_be_called -> subst(s, e),
                        actual_arg -> subst(s, e));
}

void CallExpr::print(std::ostream &os) {
    this -> to_be_called->print(os);
    os << "(";
    this -> actual_arg->print(os);
    os << ")";
}

bool CallExpr::has_variable() {

}

void CallExpr::pretty_print(std::ostream &out) {

}

void CallExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {

}
