//
// Created by YANG QI on 4/3/22.
//

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H


#include "expr.h"
void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_str(std::string s);

Expr *parse_num(std::istream &in);
Expr *parse_val(std::istream &in);
std::string parse_keyword(std::istream &in);
Expr *parse_let(std::istream &in);

Expr *parse_expr(std::istream &in);
Expr *parse_addend(std::istream &in);
Expr *parse_multicand(std::istream &in);


#endif //MSDSCRIPT_PARSE_H
