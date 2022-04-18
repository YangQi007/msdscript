//
// Created by YANG QI on 4/14/22.
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <stdio.h>
#include <string>
class Expr;

class Val {
public:
    virtual bool equals(Val *val) = 0;
    virtual Val *add_to(Val *val) = 0;
    virtual Val *mult_to(Val *val) = 0;
    virtual Expr *to_expr() = 0;
    virtual std::string to_string() = 0;

};

class NumVal : public Val{
public:
    int val;
    NumVal (int val);

    bool equals(Val *val) override;
    Val *add_to(Val *val) override;
    Val *mult_to(Val *val) override;
    Expr *to_expr() override;
    std::string to_string() override;
};

class BoolVal : public Val{
public:
    bool val;
    BoolVal (bool val);

    bool equals(Val *val) override;
    Val *add_to(Val *val) override;
    Val *mult_to(Val *val) override;
    Expr *to_expr() override;
    std::string to_string() override;

};


#endif //MSDSCRIPT_VAL_H
