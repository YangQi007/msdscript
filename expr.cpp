//
// Created by YANG QI on 2/3/22.
//
#include "catch.h"
#include "expr.h"
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

//subclass Num
Num::Num(int val) {
    this->val = val;

}

bool Num::equals(Expr *e) {
    Num *n = dynamic_cast<Num *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

int Num::interp() {
    return this->val;
}

bool Num::has_variable() {
    return false;
}

Expr* Num::subst(std::string s, Expr *e) {
    return new Num(val);
}

void Num::print(std::ostream &out) {
    out << this->val;
}

void Num::pretty_print(std::ostream &out) {
    pretty_print_at(out,prec_none,0);
}

void Num::pretty_print_at(std::ostream &out, precedence_t p,long *positon) {
    out << this->val;
}

/////////////////////////////////////////////////////////////////////
//subclass Add
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool Add::equals(Expr *e) {
    Add *n = dynamic_cast<Add *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

int Add::interp() {
    return (this->lhs->interp() + this->rhs->interp());
}

bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Add::subst(std::string s, Expr *e) {
    return new Add((this->lhs)->subst(s,e),(this->rhs)->subst(s,e));
}

void Add::print(std::ostream &out) {
    out<<"(";
    lhs->print(out);
    out<<"+";
    rhs->print(out);
    out<<")";
}

void Add::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr);

}

void Add::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
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
//subclass Mult
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;

}

bool Mult::equals(Expr *e) {
    Mult *n = dynamic_cast<Mult *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->lhs->equals(n->lhs) && this->rhs->equals(n->rhs);
    }
}

int Mult::interp() {
    return (this->lhs->interp() * this->rhs->interp());
}

bool Mult::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}

Expr* Mult::subst(std::string s, Expr *e) {
    return new Mult((this->lhs)->subst(s,e),(this->rhs)->subst(s,e));
}

void Mult::print(std::ostream &out) {
    out<<"(";
    lhs->print(out);
    out<<"*";
    rhs->print(out);
    out<<")";

}

void Mult::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr );

}

void Mult::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
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
Variables::Variables(std::string val) {
    this->val = val;
}

bool Variables::equals(Expr *e) {
    Variables *n = dynamic_cast<Variables *>(e);
    if (n == NULL){
        return false;
    }else{
        return this->val == n->val;
    }
}

int Variables::interp() {
    throw std::runtime_error("no value for variable");
}

bool Variables::has_variable() {
    return true;
}

Expr* Variables::subst(std::string s, Expr *e) {
    if (this->val == s ){
        return e;
    }else
        return new Variables(s);
}

void Variables::print(std::ostream &out) {
    out << this->val;

}

void Variables::pretty_print(std::ostream &out) {
    pretty_print_at(out,prec_none,0);

}

void Variables::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
    out << this->val;

}

////////////////////////////////////////////////////////////////////
//_let expression subclass
_let::_let(std::string lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}

bool _let::equals(Expr *e) {
    _let *n = dynamic_cast<_let *>(e);
    if (n == NULL){
        return false;
    }else{
        return (this->lhs == (n->lhs) && this->rhs ->equals(n->rhs)
        && this->body ->equals(n->body));
    }
}

int _let::interp() {
//    if (rhs->has_variable()){
//        return (this->body->subst(this->lhs,this->rhs))
//        ->interp();
//    }
//    else{
        return (this->body->subst(this->lhs,new Num(rhs->interp())))
                ->interp();
        
}

bool _let::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

Expr *_let::subst(std::string s, Expr *e) {
    if(s == this->lhs){
        return new _let(this->lhs,this->rhs->subst(s,e),this->body);
    }
    else{
        return new _let(this->lhs,this->rhs->subst(s,e),this->body->subst(s,e));
    }
}

void _let::print(std::ostream &out) {
    out << "(_let ";
    out << this->lhs;
    out << "=";
    this->rhs->print(out);
    out << " _in ";
    this->body->print(out);
    out << ")";

}

void _let::pretty_print(std::ostream &out) {
    long position = 0;
    long *posi_ptr = &position;
    pretty_print_at(out,prec_none,posi_ptr);

}

void _let::pretty_print_at(std::ostream &out, precedence_t p, long *position) {
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
