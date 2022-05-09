//
// Created by YANG QI on 4/2/22.
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
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        }else
            break;
    }
    if (negative)
        n = -n;
    return new NumExpr(n);
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
    return new VarExpr(val);
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

//parse _letExpr
Expr *parse_let(std::istream &in){
    skip_whitespace(in);
    std::string lhs = parse_val(in) -> to_string();
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
    return new _letExpr(lhs, rhs, body);
}

//parse _IfExpr
Expr *parse_if(std::istream &in){
    skip_whitespace(in);
    Expr *condition = parse_expr(in);
    skip_whitespace(in);
    if(parse_keyword(in) != "_then"){
        throw std::runtime_error("then is required");
}
    skip_whitespace(in);
    Expr *then_part = parse_expr(in);
    skip_whitespace(in);
    if(parse_keyword(in) != "_else"){
        throw std::runtime_error("else is required");
}
    Expr *else_part = parse_expr(in);
    return new IfExpr(condition, then_part, else_part);
}

Expr *parse_function(std::istream &in){
    skip_whitespace(in);
    std::string formal_arg;
    if (in.peek() == '('){
        consume(in, '(');
        skip_whitespace(in);
        formal_arg = parse_val(in)->to_string();
        skip_whitespace(in);
        int c = in.get();
        if (c!= ')'){
            throw std::runtime_error("missing close parenthesis");
        }
    }
    skip_whitespace(in);
    Expr *body;
    body = parse_expr(in);
    return new FunExpr(formal_arg, body);
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



/* <expr> = <comparg>
          | <comparg> == <expr>

**/

Expr *parse_expr(std::istream &in){
    Expr *e;

    e = parse_comparg(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '='){
        consume(in, '=');
        c=in.peek();
        if (c == '='){
            consume(in, '=');
            Expr *rhs = parse_expr(in);
            return new EqualExpr(e,rhs);
        }
    } else
        return e;
}

/* <expr> = <addend>
          | <addend> + <comparg>

**/

Expr *parse_comparg(std::istream &in){
    Expr *e;

    e = parse_addend(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+'){
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
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
        return new MultExpr(e, rhs);
    }else
        return e;
}

/*

<multicand> = <inner>
            | <multicand> ( <expr> )

 **/
Expr *parse_multicand(std::istream &in){
    skip_whitespace(in);
    Expr *e;
    e = parse_inner(in);

    while (in.peek() == '('){
        consume(in, '(');
        skip_whitespace(in);
        Expr *actual_arg = parse_expr(in);
        skip_whitespace(in);
        consume(in,')');
        e = new CallExpr(e, actual_arg);
    }
    return e;
}

/*

<inner> = <number>
            | ( <expr> )
            | <variable>
            | _letExpr <variable> = <expr> _in <expr>
            | _true
            | _false
            | _if <expr> _then <expr> _else <expr>
            | _fun ( <variable> ) <expr>
 **/

Expr *parse_inner(std::istream &in) {
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
        //grammar _letExpr _ifExpr _true _false _fun
    else if (c == '_') {
        std::string keyword = parse_keyword(in);
        if (keyword == "_letExpr") {
            return parse_let(in);
        }
        else if(keyword == "_true"){
            return new BoolExpr(true);
        }
        else if(keyword == "_false"){
            return new BoolExpr(false);
        }
        else if(keyword == "_if"){
            return parse_if(in);
        }
        else if(keyword == "_fun"){
            return parse_function(in);
        }
    }

    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}









