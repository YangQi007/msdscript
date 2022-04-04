//
// Created by YANG QI on 4/3/22.
//

#include <istream>
#include <sstream>
#include "parse.h"

//helper function consume
void consume(std::istream &in, int expect){
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}

//helper function check space
void skip_whitespace(std::istream &in){
    while (1){
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

//helper function for testing
Expr *parse_str(std::string s){
    std::stringstream in(s);
    return parse_expr(in);
}


//parse number
Expr *parse_num(std::istream &in){
    int n = 0;
    bool negative = false;

    if (in.peek() == '-'){
        negative = true;
        consume(in, '-');
    }

    while (1){
        int c = in.get();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        }else
            break;
    }
    if (negative)
        n = -n;
    return new Num(n);
}

// parse variable
Expr *parse_val(std::istream &in){
    std::string val;
    while(1){
        char c = in.peek();
        if (isalpha(c)){
            consume(in, c);
            val = val + c;
        }else
            break;
    }
    return new Variables(val);
}

//parse keyword
std::string parse_keyword(std::istream &in){
    int c = in.peek();
    std::string keyword = "_";
    if(c == '_'){
        consume(in,c);
        c = in.peek();
        while(isalpha(c)){
            keyword += c;
            consume (in, c);
            c = in.peek();
        }
    }
    skip_whitespace(in);
    return keyword;
}

//parse _let
Expr *parse_let(std::istream &in){
    skip_whitespace(in);
    std::string lhs = parse_val(in) -> to_string_pretty();
    skip_whitespace(in);
    int c = in.peek();
    if (c == '='){
        consume(in, '=');
    }else{
        throw std::runtime_error("variable '=' is required");
    }
    skip_whitespace(in);
    Expr *rhs = parse_expr(in);
    skip_whitespace(in);
    if (parse_keyword(in) != "_in"){
        throw std::runtime_error("variable _in is required");
    }
    skip_whitespace(in);
    Expr *body = parse_expr(in);
    return new _let(lhs,rhs,body);
}

/*
<addend> = <number>
         | ( <expr> )

**/
//
//Expr *parse_expr(std::istream &in){
//    skip_whitespace(in);
//
//    int c = in.peek();
//    if ((c == '-') || isdigit(c))
//        return parse_num(in);
//
//    else if (c == '('){
//        consume(in, '(');
//        Expr *e = parse_expr(in);// recursive call
//        skip_whitespace(in);
//        c = in.get();
//        if (c != ')')
//            throw std::runtime_error("missing close parenthesis");
//        return e;
//    }
//    else{
//        consume(in, c);
//        throw std::runtime_error("invalid input");
//    }
//}



/* <expr> = <addend>
          | <addend> + <expr>

**/

Expr *parse_expr(std::istream &in){
    Expr *e;

    e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+'){
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new Add(e,rhs);
    }else
        return e;
}

/*
 <addend>    = <multicand>
             | <multicand> * <addend>
 **/
Expr *parse_addend(std::istream &in){
    Expr *e;

    e = parse_multicand(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '*'){
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new Mult(e,rhs);
    }else
        return e;
}

/*

<multicand> = <number>
            | ( <expr> )
            | <variable>
            | _let <variable> = <expr> _in <expr>
 **/

Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();
    // grammar <number>
    if ((c == '-') || isdigit(c)) {
        return parse_num(in);
    }
        //grammar ( <expr> )
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);// parse parenthesized
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    }
        //grammar <variable>
    else if (isalpha(c)) {
        return parse_val(in);
    }
        //grammar _let
    else if (c == '_') {
        std::string keyword = parse_keyword(in);
        if (keyword == "_let") {
            return parse_let(in);
        }
    }

    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}









