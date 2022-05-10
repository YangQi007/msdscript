#include "cmdline.h"
#include <iostream>
#include "expr.h"
#include "parse.h"
#include "val.h"
#include "pointer.h"

int main(int argc, char **argv) {
    use_arguments(argc, argv);
//    std::string temp = argv[1];
//    if(temp == "--test"){
//        Catch::Session().run(1,argv);
//    }
    return 0;
}

TEST_CASE("NumExpr"){
    PTR(NumExpr) n1 = NEW (NumExpr)(1);
    PTR(NumExpr) n2 = NEW (NumExpr)(2);
    PTR(NumExpr) n3 = NEW (NumExpr)(2);

    SECTION("equals"){
        CHECK(n2 ->equals(n3));
        CHECK(n2 ->equals(NULL) == false);
        CHECK(!n1 ->equals(n3));
    }

    SECTION("interp"){
        CHECK(n1 ->interp() ->equals (NEW (NumVal)(1)));
        CHECK(n2 ->interp() ->equals(NEW (NumVal)(2)));
    }

    SECTION("has_variable"){
        CHECK(n1 ->has_variable() == false);
        CHECK(!n2 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK(n1 ->subst("x",NEW (NumExpr)(4)) ->equals(NEW (NumExpr)(1)));
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

TEST_CASE("(AddExpr)") {
    PTR(NumExpr) n1 = NEW (NumExpr)(1);
    PTR(NumExpr) n2 = NEW (NumExpr)(2);
    PTR(NumExpr) n3 = NEW (NumExpr)(3);
    PTR(AddExpr) add1= NEW (AddExpr)(n1, n2);
    PTR(AddExpr) add2 = NEW (AddExpr)(n1, n3);
    PTR(AddExpr) add3 = NEW (AddExpr)(n1, n2);
    PTR(MultExpr) m1 = NEW (MultExpr)(n1, n2);
    PTR(MultExpr) m2 = NEW (MultExpr)(n1, n3);
    PTR(MultExpr) m3 = NEW (MultExpr)(n1, n2);

    SECTION("equals") {
        CHECK(add1->equals(add3) == true);
        CHECK(add1->equals(add2) == false);
        CHECK(add1->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK( add1->interp() ->equals (NEW (NumVal)(3)));
        CHECK(add2 ->interp()->equals (NEW (NumVal)(4)));
    }

    SECTION("has_variable"){
        CHECK( add1->has_variable() == false);
        CHECK(!add2 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK( (NEW ((AddExpr))(NEW ((VarExpr))("x"), NEW (NumExpr)(7)))
                       ->subst("x", NEW (VarExpr)("y"))
                       ->equals(NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(7))) );
    }

    SECTION("print"){
        CHECK((NEW (AddExpr)(add1, n1)) ->to_string() == "((1+2)+1)");
        CHECK((NEW (AddExpr)(n2, add2)) ->to_string() == "(2+(1+3))");
    }

    SECTION("pretty_print"){
        CHECK((NEW (AddExpr)(add1, n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((NEW (AddExpr)(m1, n1))  ->to_string_pretty() == "(1 * 2) + 1");
        CHECK((NEW (AddExpr)(n2, add2)) ->to_string_pretty() == "2 + 1 + 3");
    }

    SECTION("pretty_print_at"){
        CHECK((NEW (AddExpr)(add1, n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((NEW (AddExpr)(n1, add1)) ->to_string_pretty() == "1 + 1 + 2");
    }

}

TEST_CASE("(MultExpr)"){
    PTR(NumExpr) n1 = NEW (NumExpr)(1);
    PTR(NumExpr) n2 = NEW (NumExpr)(2);
    PTR(NumExpr) n3 = NEW (NumExpr)(3);
    PTR(AddExpr) add1 = NEW (AddExpr)(n1, n2);
    PTR(MultExpr) m1 = NEW (MultExpr)(n1, n2);
    PTR(MultExpr) m2 = NEW (MultExpr)(n1, n3);
    PTR(MultExpr) m3 = NEW (MultExpr)(n1, n2);
    SECTION("equals") {

        CHECK(m1->equals(m3) == true);
        CHECK(m1->equals(m2) == false);
        CHECK(m1->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK(m1 ->interp()->equals (NEW (NumVal)(2)));
        CHECK(m2 ->interp() ->equals (NEW (NumVal)(3)));
    }

    SECTION("has_variable"){
        CHECK(m1 ->has_variable() == false);
        CHECK(!m2 ->has_variable() == true);
    }

    SECTION("subst"){
        CHECK( (NEW (MultExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(7)))
                       ->subst("x", NEW (VarExpr)("y"))
                       ->equals(NEW (MultExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(7))) );
    }

    SECTION("print"){
        CHECK((NEW (MultExpr)(m1, n1)) ->to_string() == "((1*2)*1)");
        CHECK((NEW (MultExpr)(n2, m2)) ->to_string() == "(2*(1*3))");
    }

    SECTION("pretty_print"){
        CHECK((NEW (MultExpr)(m1, n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((NEW (MultExpr)(n2, m2)) ->to_string_pretty() == "2 * 1 * 3");
    }

    SECTION("pretty_print_at"){
        CHECK((NEW (MultExpr)(m1, n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((NEW (MultExpr)(n2, m2)) ->to_string_pretty() == "2 * 1 * 3");
        CHECK((NEW (MultExpr)(n2, add1)) ->to_string_pretty() == "2 * 1 + 2");
        CHECK((NEW (MultExpr)(add1, n2)) ->to_string_pretty() == "(1 + 2) * 2");
    }

}

TEST_CASE("Variable") {
    PTR(VarExpr) v1 = NEW (VarExpr)("roger");
    PTR(NumExpr) n1 = NEW (NumExpr)(1);
    SECTION("equals") {
        CHECK((NEW (VarExpr)("1"))->equals(NEW (VarExpr)("1")) == true);
        CHECK((NEW (VarExpr)("1"))->equals(NEW (VarExpr)("2")) == false);
        CHECK((NEW (VarExpr)("thomas"))->equals(NEW (VarExpr)("thomas")) == true);
        CHECK((NEW (VarExpr)("anna"))->equals(NEW (VarExpr)("anne")) == false);
        CHECK((NEW (VarExpr)("1"))->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK_THROWS_WITH((NEW (VarExpr)("x"))->interp(), "no value for variable" );
    }

    SECTION("has_variable"){
        CHECK(v1 ->has_variable() == true);
        CHECK(n1 ->has_variable() == false);
    }

    SECTION("subst"){
        CHECK((NEW (VarExpr)("x"))
                      ->subst("x", NEW (VarExpr)("s"))
                      ->equals(NEW (VarExpr)("s")));

        CHECK((NEW (VarExpr)("y"))
                      ->subst("x", NEW (VarExpr)("s"))
                      ->equals(NEW (VarExpr)("y")));
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

TEST_CASE("(_letExpr)"){
    std::string lhs = "x";
    std::string lhs1 = "y";

    PTR(_letExpr) let1 = NEW (_letExpr) (lhs, NEW (NumExpr)(5), NEW (AddExpr) (NEW (VarExpr)("x"), NEW (NumExpr) (1)));
    PTR(_letExpr) let2 = NEW (_letExpr) (lhs, NEW (NumExpr)(5), NEW (AddExpr) (NEW (VarExpr)("x"), NEW (NumExpr) (1)));
    PTR(_letExpr) let3 = NEW (_letExpr) (lhs, NEW (NumExpr)(5), NEW (AddExpr) (NEW (VarExpr)("y"), NEW (NumExpr) (1)));
    PTR(_letExpr) let4 = NEW (_letExpr) (lhs, NEW (AddExpr)(NEW (NumExpr)(5), NEW (NumExpr)(3)), NEW (AddExpr) (NEW (VarExpr)("x"), NEW (NumExpr) (1)));
    PTR(_letExpr) let5 = NEW (_letExpr) (lhs, NEW (NumExpr)(5), NEW (_letExpr) (lhs, NEW (AddExpr)(NEW (VarExpr)("x"),
                                                                                       NEW (NumExpr)(2)), NEW (AddExpr)(NEW (NumExpr) (1), NEW (VarExpr)("x"))));
    PTR(_letExpr) let6 = NEW (_letExpr) (lhs, NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)), NEW (NumExpr)(1));
    PTR(_letExpr) let7 = NEW (_letExpr) (lhs, NEW (NumExpr)(5), NEW (NumExpr)(6));
    PTR(_letExpr) let8 = NEW (_letExpr) (lhs, NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)));
    //((_letExpr) x=5 _in (((_letExpr) y=3 _in (y+2))+x))
    PTR(_letExpr) let9 = NEW (_letExpr)(lhs, NEW (NumExpr)(5),
                                  NEW (AddExpr)(NEW (_letExpr)("y", NEW (NumExpr)(3), NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(2))), NEW (VarExpr)("x")));

    SECTION("equals") {
       CHECK(let1 ->equals (let2) == true);
       CHECK(let1 ->equals (let3) == false);
       CHECK(let1 ->equals (NULL) == false);
    }

    SECTION("interp"){
        CHECK(let1 ->interp() ->equals (NEW (NumVal)(6)));
        CHECK(let4 ->interp() ->equals (NEW (NumVal)(9)));
        CHECK(let5 ->interp() ->equals (NEW (NumVal)(8)));
    }

    SECTION("has_variable"){
        CHECK(let6 ->has_variable() == true);
        CHECK(let6 ->has_variable() == true);
        CHECK(let7 ->has_variable() == false);

    }

    SECTION("subst"){
        CHECK(let7 ->subst("x",NEW (NumExpr)(5)) ->equals(let7));
        CHECK(let8 ->subst("x",NEW (NumExpr)(5)) ->equals(NEW (_letExpr)(lhs,
                                                                     NEW (AddExpr)(NEW (NumExpr)(5), NEW (NumExpr)(1)), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)))));

        CHECK(let7 ->subst("y",NEW (NumExpr)(5)) ->equals(let7));
    }

    SECTION("print"){
        CHECK(let1 ->to_string() == "(_let x=5 _in (x+1))");
        CHECK(let5 ->to_string() == "(_let x=5 _in (_let x=(x+2) _in (1+x)))");
        //((_letExpr) x=5 _in (((_letExpr) y=3 _in (y+2))+x))
        CHECK(let9 ->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");


    }

    SECTION("pretty_print"){
        CHECK(let1 ->to_string_pretty() == " _let x = 5\n_in x + 1");

        CHECK(let5 ->to_string_pretty() == " _let x = 5\n"
                                           "_in  _let x = x + 2\n"
                                           "    _in 1 + x");

    }

    SECTION("pretty_print_at"){
        CHECK(let1 ->to_string_pretty() == " _let x = 5\n_in x + 1");

        CHECK(let5 ->to_string_pretty() == " _let x = 5\n"
                                           "_in  _let x = x + 2\n"
                                           "    _in 1 + x");

    }

}

TEST_CASE("parse"){
    //std::string lhs = "x";

    SECTION("parse_num"){

        CHECK(parse_str("123") -> equals(NEW (NumExpr)(123)));
        CHECK(parse_str("-123") -> equals(NEW (NumExpr)(-123)));
        CHECK(parse_str(" 123") -> equals(NEW (NumExpr)(123)));
        CHECK(parse_str("( -123 )") -> equals(NEW (NumExpr)(-123)));
        CHECK_THROWS(parse_str("( -123" ));
    }

    SECTION("parse_val"){

        CHECK(parse_str("Roger") -> equals(NEW (VarExpr)("Roger")));
        CHECK(parse_str("thomas") -> equals(NEW (VarExpr)("thomas")));
        CHECK(parse_str(" anna") -> equals(NEW (VarExpr)("anna")));

    }

    SECTION("parse_let"){
        PTR(_letExpr) let9 = NEW (_letExpr)("x", NEW (NumExpr)(5),
                                      NEW (AddExpr)(NEW (_letExpr)("y", NEW (NumExpr)(3), NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(2))), NEW (VarExpr)("x")));

        CHECK(parse_str("_let x = 5 _in x+1") -> equals
                (NEW (_letExpr) ("x", NEW (NumExpr)(5),
                               NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)))));
        //((_letExpr) x=5 _in (((_letExpr) y=3 _in (y+2))+x))
        CHECK((parse_str("_let x=5 _in ((_let y=3 _in (y+2))+x)") -> equals(let9)) == true);

    }

    SECTION("parse_if"){

        CHECK(parse_str("_if 3 == 1+2 _then 3 _else 0") -> equals
                (NEW (IfExpr) (NEW (EqualExpr)(NEW (NumExpr)(3),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2)))
                             ,NEW (NumExpr)(3),NEW (NumExpr)(0))));

    }

    SECTION("Parse_function"){
        CHECK(parse_str("_fun (x) x+2") -> equals(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))));

        CHECK(parse_str("(_fun (x) (x+2))(1)")->equals(NEW (CallExpr)(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2))),
                      NEW (NumExpr)(1))));
    }

    SECTION("parse_expr,parse_comparg,parse_addend,parse_multicand"){
        CHECK(parse_str("1+2") -> equals(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("  1+2") -> equals(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("(1+2)") -> equals(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("-1+2") -> equals(NEW (AddExpr)(NEW (NumExpr)(-1), NEW (NumExpr)(2))));
        CHECK(parse_str("3==3") -> equals(NEW (EqualExpr)(NEW (NumExpr)(3), NEW (NumExpr)(3))));
        CHECK(parse_str("(3==3)") -> equals(NEW (EqualExpr)(NEW (NumExpr)(3), NEW (NumExpr)(3))));
        CHECK(parse_str(" 3==3") -> equals(NEW (EqualExpr)(NEW (NumExpr)(3), NEW (NumExpr)(3))));

        CHECK(parse_str("1*2") -> equals(NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("  1*2") -> equals(NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("(1*2)") -> equals(NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2))));
        CHECK(parse_str("-1*2") -> equals(NEW (MultExpr)(NEW (NumExpr)(-1), NEW (NumExpr)(2))));
        CHECK(parse_str("x*y") -> equals(NEW (MultExpr)(NEW (VarExpr)("x"), NEW (VarExpr)("y"))));


        CHECK(parse_str("1*2 + 1*2") -> equals(NEW (AddExpr)(NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)), NEW (MultExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)))));
        CHECK(parse_str("(1+2)*2") -> equals(NEW (MultExpr)(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)), NEW (NumExpr)(2))));
        CHECK(parse_str("(1+2)+2") -> equals(NEW (AddExpr)(NEW (AddExpr)(NEW (NumExpr)(1), NEW (NumExpr)(2)), NEW (NumExpr)(2))));

        CHECK((parse_str("_let x=5 _in _let x = x+2 _in x + 1")->equals
        (NEW (_letExpr)("x", NEW (NumExpr)(5), NEW (_letExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)), NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(1)))))));

        CHECK(parse_str("_true") -> equals(NEW (BoolExpr)(true)));
        CHECK(parse_str("_false") -> equals(NEW (BoolExpr)(false)));

    }

}

TEST_CASE("Value refactor"){
    Val *n1 = NEW (NumVal)(1);
    Val *n2 = NEW (NumVal)(2);
    Val *n3 = NEW (NumVal)(2);
    Val *n4 = NEW (NumVal)(3);

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
        CHECK((n2 -> to_expr()) -> equals(NEW (NumExpr)(2)) == true);
        CHECK((n1 -> to_expr()) -> equals(NEW (NumExpr)(2)) == false);
    }

    SECTION("to_string"){
        CHECK(n1 -> to_string() == "1");
        CHECK(n2 -> to_string() == "2");
        CHECK((n2 -> to_string() == "1" ) == false);
    }

}

TEST_CASE("subclass boolValue"){
    Val *n1 = NEW (BoolVal)(true);
    Val *n2 = NEW (BoolVal)(false);
    Val *n3 = NEW (BoolVal)(true);
    Val *n4 = NEW (BoolVal)(false);
    Val *n5 = NEW (NumVal) (1);

    SECTION("equals"){
        CHECK(n1 -> equals(n2) == false);
        CHECK(n2 -> equals(n4) == true);
        CHECK(n1 -> equals(n5) == false);
    }

    SECTION("add_to"){
        CHECK_THROWS_WITH(n1 -> add_to(n2),"Cannot add BooVal type.");
        CHECK_THROWS_WITH(n1 -> add_to(n5),"Cannot add BooVal type.");
    }

    SECTION("mult_to"){
        CHECK_THROWS_WITH(n1 -> mult_to(n2),"Cannot multiply BooVal type.");
        CHECK_THROWS_WITH(n1 -> mult_to(n5),"Cannot multiply BooVal type.");
    }

    SECTION("to_expr"){
        CHECK((n2 -> to_expr()) -> equals(NEW (BoolExpr)(false)) == true);
        CHECK((n1 -> to_expr()) -> equals(NEW (BoolExpr)(false)) == false);
    }

    SECTION("to_string"){
        CHECK(((n1 -> to_string()) == "_true") == true);
        CHECK(((n2 -> to_string()) == "_false") == true);
        CHECK(((n1 -> to_string()) == "_false") == false);
    }
}

TEST_CASE("BoolExpr"){
    PTR(BoolExpr) n1 = NEW (BoolExpr)(true);
    PTR(BoolExpr) n2 = NEW (BoolExpr)(false);
    PTR(BoolExpr) n3 = NEW (BoolExpr)(true);
    PTR(NumExpr) n4 = NEW (NumExpr)(1);

    SECTION("equals"){
        CHECK(n1 ->equals(n3));
        CHECK(n2 ->equals(NULL) == false);
        CHECK(n2 ->equals(n4) == false);
        CHECK(!n1 ->equals(n2));
    }

    SECTION("interp"){
        CHECK(n1 ->interp() ->equals (NEW (BoolVal)(true)));
        CHECK(n2 ->interp() ->equals(NEW (BoolVal)(false)));
    }

    SECTION("has_variable"){
        CHECK(n1 ->has_variable() == false);
        CHECK(!n2 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK(n1 ->subst("x",NEW (NumExpr)(4)) ->equals(NEW (BoolExpr)(true)));
        CHECK(n2 ->subst("x",NEW (NumExpr)(4)) ->equals(NEW (BoolExpr)(false)));
    }

    SECTION("print"){
        CHECK(n1 ->to_string() == "_true");
        CHECK(n2 ->to_string() == "_false");
    }

    SECTION("pretty_print"){
        CHECK(n1 ->to_string_pretty() == "_true");
        CHECK(n2 ->to_string_pretty() == "_false");
    }

    SECTION("pretty_print_at"){
        CHECK(n1 ->to_string_pretty() == "_true");
        CHECK(n2 ->to_string_pretty() == "_false");
    }
}

TEST_CASE("EqualExpr") {
    PTR(NumExpr) n1 = NEW (NumExpr)(1);
    PTR(VarExpr) v1 = NEW (VarExpr)("1");
    PTR(NumExpr) n2 = NEW (NumExpr)(2);
    PTR(NumExpr) n3 = NEW (NumExpr)(3);
    PTR(AddExpr) n4 = NEW (AddExpr)(n1,n2);
    EqualExpr *n5 = NEW (EqualExpr)(n3, n4);
    EqualExpr *n6 = NEW (EqualExpr)(n3, n4);
    EqualExpr *n7 = NEW (EqualExpr)(n1, n2);
    EqualExpr *n8 = NEW (EqualExpr)(n1, v1);

//    (MultExpr) *m1 = NEW (MultExpr)(n1, n2);
//    (MultExpr) *m2 = NEW (MultExpr)(n1, n3);
//    (MultExpr) *m3 = NEW (MultExpr)(n1, n2);

    SECTION("equals") {
        CHECK(n5->equals(n6) == true);
        CHECK(n5->equals(n7) == false);
        CHECK(n5->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK(( n5->interp() ->equals (NEW (BoolVal)(true))) == true);
        CHECK((n5 ->interp()->equals (NEW (BoolVal)(false))) == false);
    }

    SECTION("has_variable"){
        CHECK( n5->has_variable() == false);
        CHECK( n8 ->has_variable() == true);

    }

    SECTION("subst"){
        CHECK( (NEW (EqualExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(7)))
                       ->subst("x", NEW (VarExpr)("y"))
                       ->equals(NEW (EqualExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(7))) );
    }

    SECTION("print"){
        CHECK((NEW (EqualExpr)(n4, n3)) ->to_string() == "((1+2)==3)");
        CHECK((NEW (EqualExpr)(NEW (NumExpr)(3), n3)) ->to_string() == "(3==3)");
    }

    SECTION("pretty_print"){
        CHECK((NEW (EqualExpr)(n4, n3))  ->to_string_pretty() == "1 + 2 == 3");
        CHECK((NEW (EqualExpr)(NEW (NumExpr)(3), n3)) ->to_string_pretty() == "3 == 3");
        CHECK((NEW (EqualExpr)(NEW (MultExpr)(NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2)),NEW (NumExpr)(1)), n3))  ->to_string_pretty() == "(1 + 2) * 1 == 3");

    }

    SECTION("pretty_print_at"){
        CHECK((NEW (EqualExpr)(n4, n3))  ->to_string_pretty() == "1 + 2 == 3");
        CHECK((NEW (EqualExpr)(NEW (NumExpr)(3), n3)) ->to_string_pretty() == "3 == 3");
        CHECK((NEW (EqualExpr)(NEW (MultExpr)(NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2)),NEW (NumExpr)(1)), n3))  ->to_string_pretty() == "(1 + 2) * 1 == 3");
    }

}


TEST_CASE("IfExpr"){
    // _if 3 == 1 + 2
    // _then 3
    // _else 1
    PTR(IfExpr) n1 = NEW (IfExpr)(NEW (EqualExpr)(NEW (NumExpr)(3),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))),NEW (NumExpr)(3),NEW (NumExpr)(1));

    // _if 2 == 1 + 2
    // _then 3
    // _else 1
    PTR(IfExpr) n2 = NEW (IfExpr)(NEW (EqualExpr)(NEW (NumExpr)(2),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))),NEW (NumExpr)(3),NEW (NumExpr)(1));

    // _if 3 == 1 + 2
    // _then x
    // _else y
    PTR(IfExpr) n3 = NEW (IfExpr)(NEW (EqualExpr)(NEW (NumExpr)(3),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))),NEW (VarExpr)("x"),NEW (VarExpr)("y"));

    SECTION("equals"){
        CHECK(n1 ->equals(NULL) == false);
        CHECK(n1 ->equals(NEW (IfExpr)(NEW (EqualExpr)(NEW (NumExpr)(3),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))),NEW (NumExpr)(3),NEW (NumExpr)(1))) == true);
    }

    SECTION("interp"){
        CHECK((n1 -> interp()) -> equals(NEW (NumVal)(3)) == true);
        CHECK((n1 -> interp()) -> equals(NEW (NumVal)(1)) == false);
        CHECK((n2 -> interp()) -> equals(NEW (NumVal)(3)) == false);
        CHECK((n2 -> interp()) -> equals(NEW (NumVal)(1)) == true);
    }

    SECTION("has_variable"){
        CHECK(n1 ->has_variable() == false);
        CHECK(n3 ->has_variable() == true);

    }

    SECTION("subst"){
        // _if 3 == 1 + 2
        // _then y
        // _else x
        PTR(IfExpr) n4 = NEW (IfExpr)(NEW (EqualExpr)(NEW (NumExpr)(3),NEW (AddExpr)(NEW (NumExpr)(1),NEW (NumExpr)(2))),NEW (VarExpr)("y"),NEW (VarExpr)("y"));

        CHECK(n3 ->subst("x",NEW (VarExpr)("y")) ->equals(n4) == true);
    }

    SECTION("print"){
        CHECK(n1 ->to_string() ==  "(_if (3==(1+2)) _then 3 _else 1)");
        CHECK(n3 ->to_string() ==  "(_if (3==(1+2)) _then x _else y)");
    }

    SECTION("pretty_print"){
        CHECK(n1 ->to_string_pretty() == " _if 3 == 1 + 2\n"
                                         "_then 3\n"
                                         "_else 1");
        CHECK(n3 ->to_string_pretty() ==   " _if 3 == 1 + 2\n"
                                           "_then x\n"
                                           "_else y");
    }

    SECTION("pretty_print_at"){
        CHECK(n1 ->to_string_pretty() == " _if 3 == 1 + 2\n"
                                         "_then 3\n"
                                         "_else 1");
        CHECK(n3 ->to_string_pretty() ==   " _if 3 == 1 + 2\n"
                                           "_then x\n"
                                           "_else y");
    }
}

TEST_CASE("FunExpr") {
    Expr *n1 = NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)));

    SECTION("equals") {

        CHECK(n1 -> equals(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))));
        CHECK(n1 -> equals(NEW (FunExpr)("x", NEW (AddExpr)(NEW (NumExpr)(2), NEW (VarExpr)("x")))) == false);
    }

    SECTION("interp"){
        Expr *n1 = NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)));
        CHECK((n1 -> interp()) -> equals(NEW (FunVal)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))));
        CHECK((n1 -> interp()) -> equals(NEW (FunVal)("y", NEW (AddExpr)(NEW (VarExpr)("y"), NEW (NumExpr)(2)))) == false);
    }

    SECTION("subst"){

        CHECK((n1 -> subst("x", NEW (NumExpr)(3))) -> equals (NEW (FunExpr)("x",
                                                                          NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)))));
        CHECK((n1 -> subst("y", NEW (NumExpr)(3))) -> equals (n1));
    }

    SECTION("print"){

        CHECK(n1 -> to_string() == "(_fun (x) (x+2))");
    }

}


TEST_CASE("CallExpr") {
    Expr *n1 = NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2)));
    Expr *n2 = NEW (CallExpr)(n1, NEW (NumExpr)(1));
    Expr *n3 = NEW (CallExpr)(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2))),
                            NEW (NumExpr)(1));

    SECTION("equals") {

        CHECK(n3->equals(NEW (CallExpr)(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2))),
                                        NEW (NumExpr)(1))));
        CHECK(n3->equals(NEW (CallExpr)(NEW (FunExpr)("x", NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2))),
                                        NEW (NumExpr)(-1)))
              == false);
    }

    SECTION("interp") {

        CHECK((n2 -> interp()) -> equals((n1 -> interp()) -> call((NEW (NumExpr)(1)) -> interp())));
    }

    SECTION("subst") {
        CHECK(n2->subst("x", NEW (NumExpr)(3))->equals
                (NEW (CallExpr)(NEW (FunExpr)("x",NEW (AddExpr)(NEW (VarExpr)("x"), NEW (NumExpr)(2))),
                              NEW (NumExpr)(1))));
        CHECK(n1->subst("y", NEW (NumExpr)(3))->equals(n1));
    }

    SECTION("print") {
        CHECK(n2->to_string() == "(_fun (x) (x+2))(1)");
    }
}
