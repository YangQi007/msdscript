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

}

TEST_CASE("Add") {
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Add *add1 = new Add(n1, n2);
    Add *add2 = new Add(n1, n3);
    Add *add3 = new Add(n1, n2);

    SECTION("equals") {
        CHECK(add1->equals(add3) == true);
        CHECK(add1->equals(add2) == false);
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
}

TEST_CASE("Mult"){
    Num *n1 = new Num(1);
    Num *n2 = new Num(2);
    Num *n3 = new Num(3);
    Mult *m1 = new Mult(n1, n2);
    Mult *m2 = new Mult(n1, n3);
    Mult *m3 = new Mult(n1, n2);
    SECTION("equals") {

        CHECK(m1->equals(m3) == true);
        CHECK(m1->equals(m2) == false);
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
}

TEST_CASE("Variable") {
    Variables *v1 = new Variables("roger");
    Num *n1 = new Num(1);
    SECTION("equals") {
        CHECK((new Variables("1"))->equals(new Variables("1")) == true);
        CHECK((new Variables("1"))->equals(new Variables("2")) == false);
        CHECK((new Variables("thomas"))->equals(new Variables("thomas")) == true);
        CHECK((new Variables("anna"))->equals(new Variables("anne")) == false);
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
    }
}
