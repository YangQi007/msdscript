//
// Created by YANG QI on 5/11/22.
//

#ifndef MSDSCRIPT_STEP_H
#define MSDSCRIPT_STEP_H


#include <stdio.h>
#include "expr.h"
#include "env.h"
#include "val.h"
#include "cont.h"
#include "pointer.h"

class Expr;
class Val;
class Env;
class Cont;

CLASS (Step){
public:
        typedef enum{
            interp_mode,
            continue_mode
        } mode_t;
        static mode_t mode;/* chooses mode */

        static PTR(Expr) expr;/* for interp_mode */
        static PTR(Env) env;/* for interp_mode */
        static PTR(Val) val;/* for continue_mode */
        static PTR(Cont) cont;/* all modes */

        static PTR(Val) interp_by_steps(PTR(Expr) e);
};


#endif //MSDSCRIPT_STEP_H
