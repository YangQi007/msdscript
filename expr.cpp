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
        return new VarExpr(s);
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
    out << "(_letExpr ";
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
    out << " _letExpr " << lhs << " = ";
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

}

bool BoolExpr::equals(Expr *e) {
    return false;
}

Val * BoolExpr::interp() {
    return 0;
}

bool BoolExpr::has_variable() {
    return false;
}

Expr *BoolExpr::subst(std::string s, Expr *e) {
    return nullptr;
}

void BoolExpr::print(std::ostream &out) {

}

void BoolExpr::pretty_print(std::ostream &out) {

}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t p, long *position) {

}
