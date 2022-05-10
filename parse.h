//
// Created by YANG QI on 4/3/22.
//

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H


#include "expr.h"
#include "pointer.h"

void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
PTR(Expr) parse_str(std::string s);

PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_val(std::istream &in);
std::string parse_keyword(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_function(std::istream &in);

PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);


#endif //MSDSCRIPT_PARSE_H
