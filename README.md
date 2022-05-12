# Msdscript

# Table of contents
1. [Description](#introduction)
2. [Geeting Started](#paragraph1)
3. [User Guide](#paragraph2)
4. [API Documentation](#paragraph3)

## Description: <a name="introduction"></a>
### what is Msdscript: 
Msdscript is a language interpreter developed by C++. It just like other programming language which be able to parse and interpret variables, basic arithmetic caculation, logic operation, function call. 
### what can it be used:
Msdscript can be used in two ways: as a excutable program by itself or incorporated into your application as a library.

## Getting Started: <a name="paragraph1"></a>
### How to build as an executable program:
1. **git clone** https://github.com/YangQi007/msdscript.git 
2. **cd** your file directory in your terminal
3. **make** 
4. **./msdscript + comandline** (There are more details about how to run different commandlines in  [User Guide](#paragraph2)) <br> 
For example :<br> **./msdscript --interp   
              <br>./msdscript --step**
              
5. In new line, type some expressions you want to parse or interpret.<br>
For example : <br>
<br> type in terminal : 1 + 2
<br> result : 3 <br>
<br> type in terminal : (_fun (x) (x+2))(1)
<br> result : 3

### How to build as a library incorporated into your application:
1. Download the signal file **"lib.a"** into your program's directory
2. **clang++ -std=c++14 -o filename yourfile.cpp lib.a<br>**
For example :<br>
You can download the test program **"which_day.cpp"** for testing the library<br>
<br><code> About the test file which_day program : Although MSDscript is not a very rich language at the moment—it doesn't even have subtraction—it's not far from the the kinds of languages that are embedded in applications. For example, someone implementing a calendar program might want to have a language for advanced users to calculate dates for repeated meetings (say, more sophisticated than “every Tuesday”), and MSDscript could just about work for that.</code><br>
<br> type in terminal : **clang++ -std=c++14 -o which_day which_day.cpp lib.a**
<br> type in termial : **./which_day 13**
<br> result : 4 (which means Thursday)
                 

## User Guide: <a name="paragraph2"></a>
### There are two modes you can choose in msdscript :
1. Interpret mode : Evaluates the expression and return the proper value. 
<br>The commandline is : **./msdscript --interp** <br>
2. Step mode : The result in this mode are exactly the same as in interp mode. The only difference between step mode and the interp mode is that in step mode the program uses heap memory to interp while in regular mode the program uses stack memory to interp.
<br>The commandline is : **./msdscript --step** <br>
3. You can also type : **./msdscript --help** which can list all the commandlines you can choose. <br>
### Examples for expressions you can input :
#### Variables Expressions :
**./msdscript --interp
<br> <code>3</code>
<br> 3** <br>
#### Add Expressions :
**./msdscript --interp
<br> <code>1 + 2</code>
<br> 3** <br>
#### Multiply Expressions :
**./msdscript --interp
<br> <code>1 * 2 </code>
<br> 2** <br>
#### Let Expressions :
**./msdscript --interp
<br> <code>_let x=5 _in _let x = x+2 _in x + 1</code>
<br> 8** <br>
#### Boolean Expressions :
**./msdscript --interp
<br> <code>_if (3==(1+2)) _then 3 _else 1</code>
<br> 3** <br>
#### Function Expressions :
**./msdscript --interp
<br> The function below will evaluate a factoral 10! :**

<code>**_let factrl = _fun (factrl)
                _fun (x)
                  _if x == 1
                  _then 1
                  _else x * factrl(factrl)(x + -1)
_in  factrl(factrl)(10)**</code>
<br> **3628800** <br>
#### Step Mode Example :
**step mode should get the same result as the interpret mode showed above except in while loop function( which can cause memory leak in interp mode)**
<br>**./msdscript --step
<br> <code>_let countdown = _fun(countdown)
                   _fun(n)
                     _if n == 0
                     _then 0
                     _else countdown(countdown)(n + -1)
_in countdown(countdown)(1000000)</code>
<br> 0** <br>
<br>**./msdscript --interp
<br> <code>_let countdown = _fun(countdown)
                   _fun(n)
                     _if n == 0
                     _then 0
                     _else countdown(countdown)(n + -1)
_in countdown(countdown)(1000000)</code>
<br> segmentation fault** <br>
#### Run print commandline :
**./msdscript --print
<br> <code>_let x=5 _in x+1</code>
<br>(_let x=5 _in (x+1))**<br>
### Msdscript Grammar :

<code>〈expr〉     = 〈comparg〉
                | 〈comparg〉==〈expr〉</code>
            
<code>〈comparg〉  = 〈addend〉
                | 〈addend〉+〈comparg〉</code>
            
<code>〈addend〉   = 〈multicand〉
                | 〈multicand〉*〈addend〉</code>
            
<code>〈multicand〉= 〈inner〉
                | 〈multicand〉(〈expr〉)</code>
            
<code>〈inner〉    = 〈number〉| (〈expr〉) |〈variable〉
                | _let〈variable〉=〈expr〉_in〈expr〉
                | _true | _false
                | _if〈expr〉_then〈expr〉_else〈expr〉
                | _fun (〈variable〉)〈expr〉</code>

## API Documentation: <a name="paragraph3"></a>
### Msdscript provides several API functions:
#### class Expr:
<br> virtual bool equals(PTR(Expr) e) = 0;
<br> virtual PTR(Val) interp(PTR(Env) env) = 0;
<br> virtual bool has_variable() = 0;
<br> virtual void print(std::ostream &out) = 0;
<br> virtual void pretty_print(std::ostream &out) = 0;
<br> virtual void pretty_print_at(std::ostream &out,precedence_t p,long *position) = 0;
<br> virtual void step_interp() = 0;<br>
<br>**subclasses :**
<br> NumExpr
<br> AddExpr
<br> MultExpr
<br> VarExpr
<br> _letExpr
<br> BoolExpr
<br> EqualExpr
<br> IfExpr
<br> FunExpr
<br> CallExpr<br>
#### class Val :
 <br>virtual bool equals(PTR(Val) val) = 0;
 <br>   virtual PTR(Val) add_to(PTR(Val) val) = 0;
 <br>   virtual PTR(Val) mult_to(PTR(Val) val) = 0;
 <br>   virtual std::string to_string() = 0;
 <br>   virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
 <br>  virtual void call_step(PTR(Val) actual_arg, PTR(Cont) rest) = 0;<br>
 <br> **subclasses :**
 <br> NumVal
 <br> BoolVal
 <br> FunVal<br>
 #### class Step :
 <br>static PTR(Val) interp_by_steps(PTR(Expr) e);<br>
 #### class Env :
 <br> virtual PTR(Val) lookup(std::string find_name) = 0;<br>
 <br> **subclasses :**
 <br> EmptyEnv
 <br> ExtendedEnv<br>
 #### class parse :
 <br> PTR(Expr) parse_expr(std::istream &in);<br>
 

 












