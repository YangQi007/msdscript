#include "cmdline.h"
#include <iostream>
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"

int main(int argc, char **argv) {
   // use_arguments(argc, argv);
    std::string temp = argv[1];
    if(temp == "--test"){
        Catch::Session().run(1,argv);
    }
    return 0;
}

TEST_CASE("Num"){
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(2);

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
        CHECK(n1 ->subst("x",new Num(4)) ->equals(new Num(1)));
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

TEST_CASE("Add") {
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Add *add1 = new Add(n1, n2);
    Add *add2 = new Add(n1, n3);
    Add *add3 = new Add(n1, n2);
    Mult *m1 = new Mult(n1, n2);
    Mult *m2 = new Mult(n1, n3);
    Mult *m3 = new Mult(n1, n2);

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
        CHECK( (new Add(new Variables("x"), new Num(7)))
                       ->subst("x", new Variables("y"))
                       ->equals(new Add(new Variables("y"), new Num(7))) );
    }

    SECTION("print"){
        CHECK((new Add(add1,n1)) ->to_string() == "((1+2)+1)");
        CHECK((new Add(n2,add2)) ->to_string() == "(2+(1+3))");
    }

    SECTION("pretty_print"){
        CHECK((new Add(add1,n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((new Add(m1,n1))  ->to_string_pretty() == "(1 * 2) + 1");
        CHECK((new Add(n2,add2)) ->to_string_pretty() == "2 + 1 + 3");
    }

    SECTION("pretty_print_at"){
        CHECK((new Add(add1,n1))  ->to_string_pretty() == "(1 + 2) + 1");
        CHECK((new Add(n1,add1)) ->to_string_pretty() == "1 + 1 + 2");
    }

}

TEST_CASE("Mult"){
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Add *add1 = new Add(n1, n2);
    Mult *m1 = new Mult(n1, n2);
    Mult *m2 = new Mult(n1, n3);
    Mult *m3 = new Mult(n1, n2);
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
        CHECK( (new Mult(new Variables("x"), new Num(7)))
                       ->subst("x", new Variables("y"))
                       ->equals(new Mult(new Variables("y"), new Num(7))) );
    }

    SECTION("print"){
        CHECK((new Mult(m1,n1)) ->to_string() == "((1*2)*1)");
        CHECK((new Mult(n2,m2)) ->to_string() == "(2*(1*3))");
    }

    SECTION("pretty_print"){
        CHECK((new Mult(m1,n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((new Mult(n2,m2)) ->to_string_pretty() == "2 * 1 * 3");
    }

    SECTION("pretty_print_at"){
        CHECK((new Mult(m1,n1))  ->to_string_pretty() == "(1 * 2) * 1");
        CHECK((new Mult(n2,m2)) ->to_string_pretty() == "2 * 1 * 3");
        CHECK((new Mult(n2,add1)) ->to_string_pretty() == "2 * 1 + 2");
        CHECK((new Mult(add1,n2)) ->to_string_pretty() == "(1 + 2) * 2");
    }

}

TEST_CASE("Variable") {
    Variables *v1 = new Variables("roger");
    Num *n1 = new Num(1);
    SECTION("equals") {
        CHECK((new Variables("1"))->equals(new Variables("1")) == true);
        CHECK((new Variables("1"))->equals(new Variables("2")) == false);
        CHECK((new Variables("thomas"))->equals(new Variables("thomas")) == true);
        CHECK((new Variables("anna"))->equals(new Variables("anne")) == false);
        CHECK((new Variables("1"))->equals(NULL) == false);
    }

    SECTION("interp"){
        CHECK_THROWS_WITH( (new Variables("x"))->interp(), "no value for variable" );
    }

    SECTION("has_variable"){
        CHECK(v1 ->has_variable() == true);
        CHECK(n1 ->has_variable() == false);
    }

    SECTION("subst"){
        CHECK((new Variables("x"))
                      ->subst("x", new Variables("s"))
                      ->equals(new Variables("s")));

        CHECK((new Variables("y"))
                      ->subst("x", new Variables("s"))
                      ->equals(new Variables("x")));
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

TEST_CASE("_let"){
    std::string lhs = "x";
    std::string lhs1 = "y";

    _let *let1 = new _let (lhs, new Num(5), new Add (new Variables("x"), new Num (1)));
    _let *let2 = new _let (lhs, new Num(5), new Add (new Variables("x"), new Num (1)));
    _let *let3 = new _let (lhs, new Num(5), new Add (new Variables("y"), new Num (1)));
    _let *let4 = new _let (lhs, new Add(new Num(5),new Num(3)), new Add (new Variables("x"), new Num (1)));
    _let *let5 = new _let (lhs, new Num(5), new _let (lhs,new Add(new Variables("x"),
                                                                  new Num(2)), new Add(new Num (1),new Variables("x"))));
   // _let *let6 = new _let (lhs, new Num(5), new Add(new Variables("x"),new _let (lhs1,new Num(3),
                                                              //new Add(new Variables("y"),new Num (1)))));

    SECTION("equals") {
       CHECK(let1 ->equals (let2) == true);
       CHECK(let1 ->equals (let3) == false);
       CHECK(let1 ->equals (NULL) == false);
    }

    SECTION("interp"){
        CHECK(let1 ->interp() == 6);
        CHECK(let4 ->interp() == 9);
        CHECK(let5 ->interp() == 8);
        //CHECK(let6 ->interp() == 9);
    }

    SECTION("has_variable"){

    }

    SECTION("subst"){

    }

    SECTION("print"){

    }

    SECTION("pretty_print"){

    }

    SECTION("pretty_print_at"){

    }


}