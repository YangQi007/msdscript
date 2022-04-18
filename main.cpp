#include "cmdline.h"
#include <iostream>
#include "expr.h"
#include "parse.h"
#include "val.h"

int main(int argc, char **argv) {
    use_arguments(argc, argv);
//    std::string temp = argv[1];
//    if(temp == "--test"){
//        Catch::Session().run(1,argv);
//    }
    return 0;
}

TEST_CASE("NumExpr"){
    NumExpr *n1 = new NumExpr(1);
    NumExpr *n2 = new NumExpr(2);
    NumExpr *n3 = new NumExpr(2);

    SECTION("equals"){
        CHECK(n2 ->equals(n3));
        CHECK(n2 ->equals(NULL) == false);
        CHECK(!n1 ->equals(n3));
    }

    SECTION("interp"){
        CHECK(n1 ->interp() == 1);
        CHECK(n2 ->interp() == 2);
    }

    SECTION("has_variable"){
        CHECK(n1 ->has_variable() == false);
        CHECK(!n2 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK(n1 ->subst("x",new NumExpr(4)) ->equals(new NumExpr(1)));
    }

    SECTION("print"){
        CHECK(n1 ->to_string() == "1");
        CHECK(n2 ->to_string() == "2");
    }

    SECTION("pretty_print"){
        CHECK(n1 ->to_string_pretty() == "1");
        CHECK(n2 ->to_string_pretty() == "2");
    }

    SECTION("pretty_print_at"){
        CHECK(n1 ->to_string_pretty() == "1");
        CHECK(n2 ->to_string_pretty() == "2");
    }
}

TEST_CASE("AddExpr") {
    NumExpr *n1 = new NumExpr(1);
    NumExpr *n2 = new NumExpr(2);
    NumExpr *n3 = new NumExpr(3);
    AddExpr *add1 = new AddExpr(n1, n2);
    AddExpr *add2 = new AddExpr(n1, n3);
    AddExpr *add3 = new AddExpr(n1, n2);
    MultExpr *m1 = new MultExpr(n1, n2);
    MultExpr *m2 = new MultExpr(n1, n3);
    MultExpr *m3 = new MultExpr(n1, n2);

    SECTION("equals") {
        CHECK(add1->equals(add3) == true);
        CHECK(add1->equals(add2) == false);
        CHECK(add1->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK(add1 ->interp() == 3);
        CHECK(add2 ->interp() == 4);
    }

    SECTION("has_variable"){
        CHECK(add1 ->has_variable() == false);
        CHECK(!add2 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
                       ->subst("x", new VarExpr("y"))
                       ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    }

    SECTION("print"){
        CHECK((new AddExpr(add1, n1)) ->to_string() == "((1+2)+1)");
        CHECK((new AddExpr(n2, add2)) ->to_string() == "(2+(1+3))");
    }

    SECTION("pretty_print"){
        CHECK((new AddExpr(add1, n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((new AddExpr(m1, n1))  ->to_string_pretty() == "(1 * 2) + 1");
        CHECK((new AddExpr(n2, add2)) ->to_string_pretty() == "2 + 1 + 3");
    }

    SECTION("pretty_print_at"){
        CHECK((new AddExpr(add1, n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((new AddExpr(n1, add1)) ->to_string_pretty() == "1 + 1 + 2");
    }

}

TEST_CASE("MultExpr"){
    NumExpr *n1 = new NumExpr(1);
    NumExpr *n2 = new NumExpr(2);
    NumExpr *n3 = new NumExpr(3);
    AddExpr *add1 = new AddExpr(n1, n2);
    MultExpr *m1 = new MultExpr(n1, n2);
    MultExpr *m2 = new MultExpr(n1, n3);
    MultExpr *m3 = new MultExpr(n1, n2);
    SECTION("equals") {

        CHECK(m1->equals(m3) == true);
        CHECK(m1->equals(m2) == false);
        CHECK(m1->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK(m1 ->interp() == 2);
        CHECK(m2 ->interp() == 3);
    }

    SECTION("has_variable"){
        CHECK(m1 ->has_variable() == false);
        CHECK(!m2 ->has_variable() == true);
    }

    SECTION("subst"){
        CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
                       ->subst("x", new VarExpr("y"))
                       ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) );
    }

    SECTION("print"){
        CHECK((new MultExpr(m1, n1)) ->to_string() == "((1*2)*1)");
        CHECK((new MultExpr(n2, m2)) ->to_string() == "(2*(1*3))");
    }

    SECTION("pretty_print"){
        CHECK((new MultExpr(m1, n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((new MultExpr(n2, m2)) ->to_string_pretty() == "2 * 1 * 3");
    }

    SECTION("pretty_print_at"){
        CHECK((new MultExpr(m1, n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((new MultExpr(n2, m2)) ->to_string_pretty() == "2 * 1 * 3");
        CHECK((new MultExpr(n2, add1)) ->to_string_pretty() == "2 * 1 + 2");
        CHECK((new MultExpr(add1, n2)) ->to_string_pretty() == "(1 + 2) * 2");
    }

}

TEST_CASE("Variable") {
    VarExpr *v1 = new VarExpr("roger");
    NumExpr *n1 = new NumExpr(1);
    SECTION("equals") {
        CHECK((new VarExpr("1"))->equals(new VarExpr("1")) == true);
        CHECK((new VarExpr("1"))->equals(new VarExpr("2")) == false);
        CHECK((new VarExpr("thomas"))->equals(new VarExpr("thomas")) == true);
        CHECK((new VarExpr("anna"))->equals(new VarExpr("anne")) == false);
        CHECK((new VarExpr("1"))->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK_THROWS_WITH((new VarExpr("x"))->interp(), "no value for variable" );
    }

    SECTION("has_variable"){
        CHECK(v1 ->has_variable() == true);
        CHECK(n1 ->has_variable() == false);
    }

    SECTION("subst"){
        CHECK((new VarExpr("x"))
                      ->subst("x", new VarExpr("s"))
                      ->equals(new VarExpr("s")));

        CHECK((new VarExpr("y"))
                      ->subst("x", new VarExpr("s"))
                      ->equals(new VarExpr("x")));
    }

    SECTION("print"){
        CHECK(n1 ->to_string() == "1");
        CHECK(v1 ->to_string() == "roger");
    }

    SECTION("pretty_print"){
        CHECK(n1 ->to_string_pretty() == "1");
        CHECK(v1 ->to_string_pretty() == "roger");
    }

    SECTION("pretty_print_at"){
        CHECK(n1 ->to_string_pretty() == "1");
        CHECK(v1 ->to_string_pretty() == "roger");
    }

}

TEST_CASE("_letExpr"){
    std::string lhs = "x";
    std::string lhs1 = "y";

    _letExpr *let1 = new _letExpr (lhs, new NumExpr(5), new AddExpr (new VarExpr("x"), new NumExpr (1)));
    _letExpr *let2 = new _letExpr (lhs, new NumExpr(5), new AddExpr (new VarExpr("x"), new NumExpr (1)));
    _letExpr *let3 = new _letExpr (lhs, new NumExpr(5), new AddExpr (new VarExpr("y"), new NumExpr (1)));
    _letExpr *let4 = new _letExpr (lhs, new AddExpr(new NumExpr(5), new NumExpr(3)), new AddExpr (new VarExpr("x"), new NumExpr (1)));
    _letExpr *let5 = new _letExpr (lhs, new NumExpr(5), new _letExpr (lhs, new AddExpr(new VarExpr("x"),
                                                                                       new NumExpr(2)), new AddExpr(new NumExpr (1), new VarExpr("x"))));
    _letExpr *let6 = new _letExpr (lhs, new AddExpr(new VarExpr("x"), new NumExpr(1)), new NumExpr(1));
    _letExpr *let7 = new _letExpr (lhs, new NumExpr(5), new NumExpr(6));
    _letExpr *let8 = new _letExpr (lhs, new AddExpr(new VarExpr("x"), new NumExpr(1)), new AddExpr(new VarExpr("x"), new NumExpr(1)));
    //(_letExpr x=5 _in ((_letExpr y=3 _in (y+2))+x))
    _letExpr *let9 = new _letExpr(lhs, new NumExpr(5),
                                  new AddExpr(new _letExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")));

    SECTION("equals") {
       CHECK(let1 ->equals (let2) == true);
       CHECK(let1 ->equals (let3) == false);
       CHECK(let1 ->equals (NULL) == false);
    }

    SECTION("interp"){
        CHECK(let1 ->interp() == 6);
        CHECK(let4 ->interp() == 9);
        CHECK(let5 ->interp() == 8);
    }

    SECTION("has_variable"){
        CHECK(let6 ->has_variable() == true);
        CHECK(let6 ->has_variable() == true);
        CHECK(let7 ->has_variable() == false);

    }

    SECTION("subst"){
        CHECK(let7 ->subst("x",new NumExpr(5)) ->equals(let7));
        CHECK(let8 ->subst("x",new NumExpr(5)) ->equals(new _letExpr(lhs,
                                                                     new AddExpr(new NumExpr(5), new NumExpr(1)), new AddExpr(new VarExpr("x"), new NumExpr(1)))));

        CHECK(let7 ->subst("y",new NumExpr(5)) ->equals(let7));
    }

    SECTION("print"){
        CHECK(let1 ->to_string() == "(_letExpr x=5 _in (x+1))");
        CHECK(let5 ->to_string() == "(_letExpr x=5 _in (_letExpr x=(x+2) _in (1+x)))");
        //(_letExpr x=5 _in ((_letExpr y=3 _in (y+2))+x))
        CHECK(let9 ->to_string() == "(_letExpr x=5 _in ((_letExpr y=3 _in (y+2))+x))");


    }

    SECTION("pretty_print"){
        CHECK(let1 ->to_string_pretty() == " _letExpr x = 5\n_in x + 1");

        CHECK(let5 ->to_string_pretty() == " _letExpr x = 5\n"
                                           "_in  _letExpr x = x + 2\n"
                                           "    _in 1 + x");

    }

    SECTION("pretty_print_at"){
        CHECK(let1 ->to_string_pretty() == " _letExpr x = 5\n_in x + 1");

        CHECK(let5 ->to_string_pretty() == " _letExpr x = 5\n"
                                           "_in  _letExpr x = x + 2\n"
                                           "    _in 1 + x");

    }

}

TEST_CASE("parse"){
    //std::string lhs = "x";

    SECTION("parse_num"){

        CHECK(parse_str("123") -> equals(new NumExpr(123)));
        CHECK(parse_str("-123") -> equals(new NumExpr(-123)));
        CHECK(parse_str(" 123") -> equals(new NumExpr(123)));
        CHECK(parse_str("( -123 )") -> equals(new NumExpr(-123)));
        CHECK_THROWS(parse_str("( -123" ));
    }

    SECTION("parse_val"){

        CHECK(parse_str("Roger") -> equals(new VarExpr("Roger")));
        CHECK(parse_str("thomas") -> equals(new VarExpr("thomas")));
        CHECK(parse_str(" anna") -> equals(new VarExpr("anna")));

    }

    SECTION("parse_let"){
        _letExpr *let9 = new _letExpr("x", new NumExpr(5),
                                      new AddExpr(new _letExpr("y", new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")));

        CHECK(parse_str("_letExpr x = 5 _in x+1") -> equals
                (new _letExpr ("x", new NumExpr(5),
                               new AddExpr(new VarExpr("x"), new NumExpr(1)))));
        //(_letExpr x=5 _in ((_letExpr y=3 _in (y+2))+x))
        CHECK((parse_str("_letExpr x=5 _in ((_letExpr y=3 _in (y+2))+x)") -> equals(let9)) == true);


    }

    SECTION("parse_expr,parse_addend,parse_multicand"){
        CHECK(parse_str("1+2") -> equals(new AddExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("  1+2") -> equals(new AddExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("(1+2)") -> equals(new AddExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("-1+2") -> equals(new AddExpr(new NumExpr(-1), new NumExpr(2))));

        CHECK(parse_str("1*2") -> equals(new MultExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("  1*2") -> equals(new MultExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("(1*2)") -> equals(new MultExpr(new NumExpr(1), new NumExpr(2))));
        CHECK(parse_str("-1*2") -> equals(new MultExpr(new NumExpr(-1), new NumExpr(2))));
        CHECK(parse_str("x*y") -> equals(new MultExpr(new VarExpr("x"), new VarExpr("y"))));


        CHECK(parse_str("1*2 + 1*2") -> equals(new AddExpr(new MultExpr(new NumExpr(1), new NumExpr(2)), new MultExpr(new NumExpr(1), new NumExpr(2)))));
        CHECK(parse_str("(1+2)*2") -> equals(new MultExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(2))));
        CHECK(parse_str("(1+2)+2") -> equals(new AddExpr(new AddExpr(new NumExpr(1), new NumExpr(2)), new NumExpr(2))));

        CHECK((parse_str("_letExpr x=5 _in _letExpr x = x+2 _in x + 1")->equals
        (new _letExpr("x", new NumExpr(5), new _letExpr("x", new AddExpr(new VarExpr("x"), new NumExpr(2)), new AddExpr(new VarExpr("x"), new NumExpr(1)))))));


    }

}

TEST_CASE("Value refactor"){
    Val *n1 = new NumVal(1);
    Val *n2 = new NumVal(2);
    Val *n3 = new NumVal(2);
    Val *n4 = new NumVal(3);

    SECTION("equals"){
        CHECK(n1 -> equals(n2) == false);
        CHECK(n2 -> equals(n3) == true);
    }

    SECTION("add_to"){
        CHECK(n4 -> equals(n1 -> add_to(n2)) == true);
        CHECK(n3 -> equals(n1 -> add_to(n2)) == false);
    }

    SECTION("mult_to"){
        CHECK(n3 -> equals(n1 -> mult_to(n2)) == true);
        CHECK(n4 -> equals(n1 -> mult_to(n2)) == false);
    }

    SECTION("to_expr"){
        CHECK((n2 -> to_expr()) -> equals(new NumExpr(2)) == true);
        CHECK((n1 -> to_expr()) -> equals(new NumExpr(2)) == false);
    }

    SECTION("to_string"){
        CHECK(n1 -> to_string() == "1");
        CHECK(n2 -> to_string() == "2");
        CHECK((n2 -> to_string() == "1" ) == false);
    }


}