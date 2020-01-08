#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H
#include <numeric>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <vector>
#include <algorithm>
#include "Python3BaseVisitor.h"
#include "BigInteger.h"
#include "deal_arith.h"
#include "deal_scope.h"
#include "deal_print.h"

std::vector<std::map<std::string,antlrcpp::Any>> scope;
std::map<std::string,antlrcpp::Any> entire;
std::map<std::string,Python3Parser::SuiteContext*> suite_entire;
std::map<std::string,std::vector<reflect>> par_cot;
std::vector<antlrcpp::Any> return_value;
bool has_break = false;
bool has_continue = false;
bool has_return = false;
class EvalVisitor: public Python3BaseVisitor {

    antlrcpp::Any visitFile_input(Python3Parser::File_inputContext *ctx) override {       
        scope.push_back(entire);
        return visitChildren(ctx);
    }

    antlrcpp::Any visitFuncdef(Python3Parser::FuncdefContext *ctx) override {       
        //std::cout<<"in_fundef"<<std::endl;
        if(ctx -> NAME())
        {
            std::string s = ctx -> NAME() -> toString();
            //std::cout<<"in_define_name "<<s<<std::endl;
            if(ctx -> parameters() -> typedargslist())
                par_cot[s] = visit(ctx -> parameters()).as<std::vector<reflect>>();
            //std::cout<<"parlist_size"<<par_cot.size()<<std::endl;
            suite_entire[s] = ctx -> suite();
            //std::cout<<"suit.size "<<suite_entire.size()<<std::endl;
            //std::cout<<"wheher has s"<<suite_entire.count(s)<<std::endl;
        }    
        return 0;      
    }

    antlrcpp::Any visitParameters(Python3Parser::ParametersContext *ctx) override {
        //std::cout<<"In_parameters"<<std::endl;
        if(ctx -> typedargslist()) return visit(ctx -> typedargslist());
        else  return 0;
    }

    antlrcpp::Any visitTypedargslist(Python3Parser::TypedargslistContext *ctx) override {
        //std::cout<<"in_typedarglist"<<std::endl;
        int type_size = ctx -> tfpdef().size();
        //std::cout<<"type_size "<<type_size<<std::endl;
        int test_size = ctx -> test().size();
        //std::cout<<"test_size "<<test_size<<std::endl;
        std::vector<reflect> tmp;
        for(int i = test_size - 1; i >= 0 ; --i)
        {
            string s = visit(ctx -> tfpdef(type_size - 1)).as<std::string>();
            antlrcpp::Any x = visit(ctx -> test(i));
            tmp.push_back({s,x});
            --type_size;          
        }
        for(int i = type_size - 1 ; i >= 0 ; --i)
        {
            //std::cout<<"have_name"<<std::endl;
            string s = visit(ctx -> tfpdef(i)).as<std::string>();
            //std::cout<<"variable_string "<<s<<std::endl;
            tmp.push_back({s,0});
        }
        return tmp;
    }

    antlrcpp::Any visitTfpdef(Python3Parser::TfpdefContext *ctx) override {
        return ctx -> NAME() -> toString();
    }

    antlrcpp::Any visitStmt(Python3Parser::StmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) override {
        //std::cout<<"in_expr_stm"<<std::endl;
        int test_num = ctx -> testlist().size();
        //std::cout<<"test_num "<<test_num<<std::endl;
        if(test_num == 1){
            //std::cout<<"just_print_value/test"<<std::endl;
            return visit(ctx -> testlist(0));
        }
        else
        {
            std::vector<antlrcpp::Any> test_x = visit(ctx -> testlist(0)).as<std::vector<antlrcpp::Any>>();
            //std::cout<<"test_x.size "<<test_x.size()<<std::endl;
            if(test_x.size() == 1)
            {
                int assign_num = ctx -> ASSIGN().size();
                //std::cout<<"= number "<<assign_num<<std::endl;
                antlrcpp::Any tmp = visit(ctx -> testlist(test_num - 1)-> test(0));
                if(assign_num == 0)
                {
                    antlrcpp::Any left = visit(ctx -> testlist(0) -> test(0));
                    antlrcpp::Any answer;
                    switch((visit(ctx -> augassign())).as<int>())
                    {
                        case 1:answer = add(left , tmp); break;
                        case 2:answer = mis(left , tmp); break;
                        case 3:answer = multiply(left , tmp); break;
                        case 4:answer = divide(left , tmp); break;
                        case 5:answer = Idivide(left , tmp); break;
                        case 6:answer = Mode(left , tmp); break;
                    }
                    std::string s = ctx -> testlist(0) -> test(0) -> or_test() -> and_test(0) -> not_test(0) -> comparison() -> arith_expr(0) -> term(0) -> factor(0) -> atom_expr() -> atom() -> NAME() -> toString();
                    std::vector<std::map<std::string,antlrcpp::Any>>::reverse_iterator out;
                    for(out = scope.rbegin();out != scope.rend() ; ++out )
                    if((*out).count(s)){ /*std::cout<<"find"<<std::endl;*/(*out)[s] = answer; break;}
                    if(out == scope.rend()) {/*std::cout<<"not_find"<<std::endl;*/scope.back()[s] = answer;}
                    return answer;
                }
                else 
                {
                    //std::cout<<"in_assign_one"<<endl;
                    for(int i = assign_num - 1 ; i >= 0 ; --i)
                    {
                        std::string s = ctx -> testlist(i) -> test(0) -> or_test() -> and_test(0) -> not_test(0) -> comparison() -> arith_expr(0) -> term(0) -> factor(0) -> atom_expr() -> atom() -> NAME() -> toString();
                        //std::cout<<s<<std::endl;
                        std::vector<std::map<std::string,antlrcpp::Any>>::reverse_iterator out;
                        for(out = scope.rbegin();out != scope.rend() ; ++out )
                        if((*out).count(s)){/* std::cout<<"find"<<endl;*/(*out)[s] = tmp; break;}
                        if(out == scope.rend()) scope.back()[s] = tmp;
                    }
                    return tmp;
                }
            }
            else
            {
                std::vector<antlrcpp::Any> list;
                std::vector<antlrcpp::Any> tmplist = (visit(ctx -> testlist(test_num - 1))).as<std::vector<antlrcpp::Any>>();
                int test_size = tmplist.size();
                antlrcpp::Any tmp;
                for(int i = 0; i < ctx -> testlist(0) -> test().size() ; ++i)
                {
                    if(test_size != 1)
                        tmp = visit(ctx -> testlist(test_num - 1) -> test(i));
                    else 
                        tmp = visit(ctx -> testlist(test_num - 1) -> test(0));
                    list.push_back(tmp);
                }
                for(int i = 0; i < ctx -> testlist(0) -> test().size() ; ++i)
                {
                    for(int j = test_num - 2 ; j >= 0 ; --j )
                    {
                        std::string s =  ctx -> testlist(j) -> test(i) -> or_test() -> and_test(0) -> not_test(0) -> comparison() -> arith_expr(0) -> term(0) -> factor(0) -> atom_expr() -> atom() -> NAME() -> toString();
                        std::vector<std::map<std::string,antlrcpp::Any>>::reverse_iterator out;
                        for(out = scope.rbegin();out != scope.rend() ; ++out )
                        if((*out).count(s)){ (*out)[s] = list[i]; break;}
                        if(out == scope.rend()) scope.back()[s] = list[i];
                    }
                }
                return list;
            }
        }   
    }

    antlrcpp::Any visitAugassign(Python3Parser::AugassignContext *ctx) override {
        if(ctx -> ADD_ASSIGN())return 1;
        if(ctx -> SUB_ASSIGN())return 2;
        if(ctx -> MULT_ASSIGN())return 3;
        if(ctx -> DIV_ASSIGN())return 4;
        if(ctx -> IDIV_ASSIGN())return 5;
        if(ctx -> MOD_ASSIGN())return 6;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) override {
        has_break = true;
        return 0;
    }

    antlrcpp::Any visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) override {
        has_continue = true;
        return 0;;
    }

    antlrcpp::Any visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) override {
        has_return = true;
        if(!ctx -> testlist())
        {
            //std::cout<<"no_return"<<std::endl;
            return_value.push_back(std::string("None"));
        }
        else
        {
            //std::cout<<"has_return"<<std::endl;
            return_value.push_back(visit(ctx -> testlist()).as<std::vector<antlrcpp::Any>>());
        }
        return std::string("has_return");
    }

    antlrcpp::Any visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) override {
        return visitChildren(ctx);
    }

    antlrcpp::Any visitIf_stmt(Python3Parser::If_stmtContext *ctx) override {
        std::map<std::string , antlrcpp::Any> tmp;
        antlrcpp::Any x;
        scope.push_back(tmp);
        antlrcpp::Any whether_continue = visit(ctx -> test(0));
        if(to_continue(whether_continue))
        {
            //std::cout<<"if"<<std::endl;
            x = visit(ctx -> suite(0));
            scope.pop_back();
            return x;
        }
        int else_i_num = ctx -> ELIF().size();
        for(int i = 1 ; i <= else_i_num ; i++)
        {
            whether_continue = visit(ctx -> test(i));
            if(to_continue(whether_continue))
            {
                x = visit(ctx -> suite(i));
                scope.pop_back();
                return x;
            }
        }
        if(ctx -> ELSE()) 
        {
            //std::cout<<"else"<<std::endl;
            x = visit(ctx -> suite(ctx -> suite().size()-1));
            scope.pop_back();
            return x;
        }
    }

    antlrcpp::Any visitWhile_stmt(Python3Parser::While_stmtContext *ctx) override {
        //std::cout<<"in_while"<<std::endl;
        std::map<std::string , antlrcpp::Any> tmp;
        antlrcpp::Any x = 0;
        scope.push_back(tmp);
        antlrcpp::Any whether_continue = visit(ctx -> test());
        int time = 0 ;//delete
        while(to_continue(whether_continue))
        {  
            x = visit(ctx ->suite());
            if(has_break) break;
            if(has_continue) continue;
            whether_continue = visit(ctx -> test()); 
            //std::cout<<"loop_time "<< time++ <<std::endl;
        }
        has_break = false;
        has_continue = false;
        scope.pop_back();
        return x;
    }

    antlrcpp::Any visitSuite(Python3Parser::SuiteContext *ctx) override {
        //std::cout<<"in_suite"<<std::endl;
        antlrcpp::Any return_ctx;
        int stmt_num = ctx -> stmt().size();
        if(ctx -> simple_stmt()){
            //std::cout<<"has_simple_stmt"<<std::endl;
            return_ctx = visit(ctx -> simple_stmt());
        }
        else
        {
            //std::cout<<"stmt_size"<<stmt_num<<std::endl;
            for(int i = 0 ; i < stmt_num ; ++i)
            {    
                //std::cout<<"int statement "<< i <<std::endl;
                return_ctx = visit(ctx -> stmt(i));
                //prints(return_ctx);
                //std::cout<<std::endl;
                if(has_break) break;
                if(has_continue) continue;
                if (has_return)
                {   
                    //std::cout<<"please return "<<std::endl;
                    antlrcpp::Any x = return_value.back();
                    if(x.is<std::vector<antlrcpp::Any>>()) return_ctx = x.as<std::vector<antlrcpp::Any>>();
                    else  if(x.is<std::string>()) return_ctx = x.as<std::string>();
                    return_value.pop_back();
                    break;
                }
            }
            has_return = false;            
        }
        return return_ctx;
    }

    antlrcpp::Any visitTest(Python3Parser::TestContext *ctx) override {
        return visit(ctx -> or_test());
    }

    antlrcpp::Any visitOr_test(Python3Parser::Or_testContext *ctx) override {//completed
        /*std::cout<<"or_test"<<std::endl;*/
        int test_num = ctx -> and_test().size();
        //std::cout<<test_num<<std::endl;
        if(test_num > 1)
        {
            for(int i = 0 ; i < test_num ; i++)
            {
                antlrcpp::Any tmp = visit(ctx -> and_test(i));
                if(tmp.is<BigInteger>())
                    if(tmp.as<BigInteger>() != BigInteger(0))
                        return true;
                if(tmp.is<double>())
                    if(tmp.as<double>() != 0.0)
                        return true;
                if(tmp.is<bool>()) if(tmp.as<bool>()) return true;
                if(tmp.is<std::string>()) 
                {
                    std::string s = tmp.as<std::string>();
                    if(*s.begin() = '"') 
                        if(s.size() != 2)
                            return true;
                    else if(s.size()) return true;
                }
            }
            return false;
        }
        else
            return visit(ctx -> and_test(0));

    }

    antlrcpp::Any visitAnd_test(Python3Parser::And_testContext *ctx) override {
     //  std::cout<<"and_test"<<std::endl;
        int test_num = ctx -> not_test().size();
        if(test_num > 1)
        {
            for(int i = 0 ; i < test_num ; i++)
            {
                antlrcpp::Any tmp = visit(ctx -> not_test(i));
                if(tmp.is<BigInteger>())
                    if(tmp.as<BigInteger>() == BigInteger(0))
                        return false;
                if(tmp.is<double>())
                    if(tmp.as<double>() == 0.0)
                        return false;
                if(tmp.is<bool>()) if(!tmp.as<bool>()) return false;
                if(tmp.is<std::string>()) 
                {
                    std::string s = tmp.as<std::string>();
                    if(*s.begin() = '"') 
                        if(s.size() == 2)
                            return false;
                    else if(!s.size()) return false;
                }
            }
            return true;
        }
        return visit(ctx -> not_test(0));
    }

    antlrcpp::Any visitNot_test(Python3Parser::Not_testContext *ctx) override {
    //    std::cout<<"not_test"<<std::endl;
        if(ctx -> not_test())
        {
            antlrcpp::Any tmp = visit(ctx -> not_test());
            if(tmp.is<BigInteger>())
                return !(tmp.as<BigInteger>() != BigInteger(0));
            if(tmp.is<double>())
                return !(tmp.as<double>() != 0.0);
            if(tmp.is<bool>()) return !tmp.as<bool>();
            if(tmp.is<std::string>()) 
            {
                std::string s = tmp.as<std::string>();
                if(*s.begin() = '"') 
                    return !(s.size() != 2);
                else return !s.size();
            }
            else return !visit(ctx -> not_test());
        }
        else
            return visit (ctx -> comparison());
    }

    antlrcpp::Any visitComparison(Python3Parser::ComparisonContext *ctx) override {
    //    std::cout<<"comp"<<std::endl;
        int con_number = ctx -> arith_expr().size();
        if(con_number == 1)
            return visit(ctx -> arith_expr(0));
        else
        {
            for(int i = 0 ; i < con_number -1 ; i++)
            {
                bool answer;
                antlrcpp::Any left = visit(ctx -> arith_expr(i));
                antlrcpp::Any right = visit(ctx -> arith_expr(i + 1));
                switch((visit(ctx -> comp_op(i))).as<int>())
                {
                    case 1:answer = less_than(left , right); break;
                    case 2:answer = greater_than(left , right); break; 
                    case 3:answer = equals(left , right); break;
                    case 4:answer = gt_eq(left , right); break;
                    case 5:answer = lt_eq(left , right); break;
                    case 6:answer = not_eq_2(left , right); break;
                }
                if(!answer)return false;
            }
            return true;
        }
    }

   antlrcpp::Any visitComp_op(Python3Parser::Comp_opContext *ctx) override {
        if(ctx -> LESS_THAN()) return 1;
        if(ctx -> GREATER_THAN())return 2;
        if(ctx -> EQUALS())return 3;
        if(ctx -> GT_EQ())return 4;
        if(ctx -> LT_EQ())return 5;
        if(ctx -> NOT_EQ_2())return 6;
        return visitChildren(ctx);
    }

    antlrcpp::Any visitArith_expr(Python3Parser::Arith_exprContext *ctx) override {
        int arith_num = ctx -> term().size();
        if(arith_num == 1)
            return visit(ctx -> term(0));
        else
        {
            int add_num = ctx -> ADD().size();
            int min_num = ctx -> MINUS().size();
            antlrcpp::Any answer = visit(ctx -> term(0));
            antlrcpp::Any left , right;
            std::vector<struct op> tmp;
            for(int i = 0 ; i < add_num ; ++i){
                tmp.push_back({ctx -> ADD(i) -> getSymbol() -> getTokenIndex(), 1});
            }
            for(int i = 0 ; i < min_num ; ++i)
                tmp.push_back({ctx -> MINUS(i) -> getSymbol() -> getTokenIndex(), 2});
            sort(tmp.begin() , tmp.end() , Greatersort);
            for(int i = 0 ; i < arith_num -1 ; ++i )
            {
                right = visit(ctx -> term(i + 1));
                left = answer;
                switch(tmp[i].value)
                {
                    case 1:answer = add(left , right); break;
                    case 2:answer = mis(left , right); break;
                }
            } 
            return answer;   
        }
    }

    antlrcpp::Any visitTerm(Python3Parser::TermContext *ctx) override {
        //std::cout<<"in_term"<<std::endl;
        int term_num = ctx -> factor().size();
        if(term_num == 1)
            return visit(ctx -> factor(0));
        else
        {   
            int star_num = ctx -> STAR().size();
            int div_num = ctx -> DIV().size();
            int idiv_num = ctx -> IDIV().size();
            int mod_num = ctx -> MOD().size();
            antlrcpp::Any answer = visit(ctx -> factor(0));
            antlrcpp::Any left , right;
            std::vector<struct op> tmp;
            for(int i = 0 ; i < star_num ; ++i){
                tmp.push_back({ctx -> STAR(i) -> getSymbol() -> getTokenIndex(), 1});
            }
            for(int i = 0 ; i < idiv_num ; ++i)
                tmp.push_back({ctx -> IDIV(i) -> getSymbol() -> getTokenIndex(), 3});
            for(int i = 0 ; i < div_num ; ++i){
                tmp.push_back({ctx -> DIV(i) -> getSymbol() -> getTokenIndex(), 2});
            }
            for(int i = 0 ; i < mod_num ; ++i)
                tmp.push_back({ctx -> MOD(i) -> getSymbol() -> getTokenIndex(), 4});
            sort(tmp.begin() , tmp.end() , Greatersort);
            for(int i = 0 ; i < term_num -1 ; ++i )
            {
                right = visit(ctx -> factor(i + 1));
                left = answer;
                switch(tmp[i].value)
                {
                    case 1:answer = multiply(left , right); break;
                    case 2:answer = divide(left , right); break;
                    case 3:answer = Idivide(left , right); break;
                    case 4:answer = Mode(left , right); break;
                }
            } 
            return answer;   
        }
    }

    antlrcpp::Any visitFactor(Python3Parser::FactorContext *ctx) override {
        //std::cout<<"in_factor"<<std::endl;
        if(ctx -> atom_expr())
            return visit(ctx -> atom_expr());
        else
        {
            antlrcpp::Any tmp = visit(ctx -> factor());
            if(ctx -> MINUS())
            {
                if(tmp.is<BigInteger>())
                {
                    if(tmp.as<BigInteger>() == BigInteger(0)) return BigInteger(0);
                    else return -(tmp.as<BigInteger>());
                }
                if(tmp.is<double>())
                    return -(tmp.as<double>());
                if(tmp.is<bool>())
                    return !tmp.as<bool>();
            }
        }
    }

    antlrcpp::Any visitAtom_expr(Python3Parser::Atom_exprContext *ctx) override {
        //std::cout<<"in_atom_exp"<<std::endl;
        if(ctx -> atom() && !ctx -> trailer())
            return visit(ctx -> atom());
        else
        {
            //std::cout<<"atom_exp has trailor"<<std::endl;
            std::string tmp1 = visit(ctx -> atom());
            //std::cout<<"tmp1= "<<tmp1<<std::endl;
            int tmp_trailor_size;
            std::vector<antlrcpp::Any> v;
            if(ctx -> trailer() -> arglist())
            {
                v = visit(ctx -> trailer()).as<std::vector<antlrcpp::Any>>();
                tmp_trailor_size = v.size();
                //std::cout<<"trailor size= "<<tmp_trailor_size<<std::endl;
            }
            //std::cout<<suite_entire.count(tmp1)<<std::endl;
            if(suite_entire.count(tmp1))
            {
                //std::cout<<"it's a func"<<std::endl;
                std::map<std::string , antlrcpp::Any> tmp;
                antlrcpp::Any x;
                scope.push_back(tmp);
                if(par_cot.count(tmp1) == 0) 
                {
                    //std::cout<<"no pararmeters"<<std::endl;
                    x = visit(suite_entire[tmp1]);//have no parameters
                }
                else //all have 
                {
                    std::vector<reflect> parlist = par_cot[tmp1];
                    int type_num = par_cot[tmp1].size();
                    bool can_change_pos = false;
                    int assign_num = 0;
                    if(tmp_trailor_size == type_num)
                    {
                        for(int i = 0 ; i < type_num ; i++)
                        {
                            if(ctx -> trailer() -> arglist() -> argument(i) -> ASSIGN()) assign_num++;
                            else break;
                        }
                        if(assign_num == type_num) can_change_pos = true;
                    }
                    if(can_change_pos)
                    {
                        //std::cout<<"in_func_can_change_pos"<<std::endl;
                        for(int i = 0 ; i < type_num ; i++)
                            scope.back()[ctx -> trailer() -> arglist() -> argument(i) -> NAME() -> toString()] = v[i]; 
                    }
                    else
                    {
                        //std::cout<<"in_par_assign"<<std::endl;
                        int trailor_size = tmp_trailor_size;
                        //std::cout<<"func_type_num "<<type_num<<std::endl;
                        for(int i = 0 ; i < type_num ; ++i)//from beginning to assign
                        {
                            if(trailor_size >= 1) scope.back()[par_cot[tmp1][type_num - i - 1].name] = v[i]; //surplus trailor_size
                            else scope.back()[par_cot[tmp1][type_num - i - 1].name] = par_cot[tmp1][type_num - i - 1].value;
                            trailor_size--;
                        }
                    }
                    //std::cout<<"func_to_vist_suite"<<std::endl;
                    x = visit(suite_entire[tmp1]);
                    //std::cout<<"yes"<<std::endl;
                }
                scope.pop_back(); 
                return x;
            }
            if(tmp1 == "print")
            {
                if(!ctx -> trailer() -> arglist())
                {
                    std::cout<<std::endl;
                    return 0;
                }    
                for(int i = 0  ; i < tmp_trailor_size ; i++)
                {
                    //std::cout<<"in_print"<<std::endl;
                    antlrcpp::Any tmp3 = v[i];
                    if(!tmp3.is<std::vector<antlrcpp::Any>>())
                    {
                        //std::cout<<"in_print_one"<<std::endl;
                        prints(v[i]);
                    }
                    else
                    {
                        std::vector<antlrcpp::Any> vec = tmp3.as<std::vector<antlrcpp::Any>>();
                        int size = vec.size();
                        if(size == 1) prints(vec[0]);
                        else
                        {
                            std::cout<<"(";
                            for(int i = 0 ; i < size ; ++i)
                            {
                                prints(vec[i]);
                                if(i != size -1)
                                std::cout<<",";
                            }
                            std::cout<<")";
                        }
                    }
                    if(i != tmp_trailor_size - 1)
                        std::cout<<' ';
                    else
                        std::cout<<std::endl;
                }   
            }
            else if(tmp1 == "int" && tmp_trailor_size == 1)
            {
                //std::cout<<"in_int"<<std::endl;
                if(v[0].is<BigInteger>()){
                    //std::cout<<"in_int_integer"<<std::endl;
                    return v[0].as<BigInteger>();}
                else if(v[0].is<double>())
                    return BigInteger(v[0].as<double>());
                else if(v[0].is<bool>())
                    return int(v[0].as<bool>());
                else if(v[0].is<std::string>())
                    return BigInteger(v[0].as<std::string>());
            }
            else if(tmp1 == "float" && tmp_trailor_size == 1)
            {
                if(v[0].is<BigInteger>())
                    return double(v[0].as<BigInteger>());
                else if(v[0].is<bool>())
                    return double(v[0].as<bool>());
                else if(v[0].is<double>())
                    return v[0].as<double>();
            }
            else if(tmp1 == "str" && tmp_trailor_size == 1)
            {
                if(v[0].is<BigInteger>())
                {
                    BigInteger be_string = v[0].as<BigInteger>();
                    return string(be_string);
                }
                else if(v[0].is<double>())
                    return to_string(v[0].as<double>());
                else if(v[0].is<bool>())
                {
                    if(v[0].as<bool>()) return string("True");
                    else return string("False");
                }
                else if(v[0].is<std::string>())
                    return v[0].as<std::string>();
            }
            else if(tmp1 == "bool" && tmp_trailor_size == 1)
            {
                if(v[0].is<BigInteger>())
                {   return (v[0].as<BigInteger>() != BigInteger(0));}
                else if(v[0].is<double>())
                {   return (v[0].as<double>() != 0); }
                else if(v[0].is<string>())
                {
                    string s = v[0].as<std::string>();
                    s.erase(std::remove(s.begin(), s.end(), '"'), s.end());
                    if(s != "None")
                    {return (s.size() != 0);}
                    else return false;
                }
            }
        }   
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTrailer(Python3Parser::TrailerContext *ctx) override {
        return visit(ctx -> arglist());
    }

    antlrcpp::Any visitAtom(Python3Parser::AtomContext *ctx) override {
        //std::cout<<"int_atom"<<std::endl;
        if(ctx -> NUMBER())
        {
            //std::cout<<"number"<<std::endl;
            std::string s = ctx ->NUMBER() -> toString();
            std::string::size_type x = s.find('.');
            if(x != std::string::npos)
                return std::stod(s);
            else
                return BigInteger(s);
        }
        else if(ctx -> STRING().size())
        {
            //std::cout<<"in_String"<<std::endl;
            std::string s ;
            int size_cur =  ctx ->STRING().size();
            if(size_cur > 1)
            {
                for(int i = 0 ; i < size_cur; i++)
                {
                    std::string tmp = ctx -> STRING(i) -> toString();
                    if(i == 0)
                        tmp.erase(tmp.end()-1);
                    else if(i == size_cur - 1)
                        tmp.erase(tmp.begin());
                    else
                        tmp.erase(std::remove(tmp.begin(), tmp.end(), '"'), tmp.end());
                    s = s + tmp;    
                }
            }
            else s = ctx ->STRING(0) -> toString();
            return s;
        }
        else if(ctx -> NAME())
        {
            //std::cout<<"in_NAME()"<<std::endl;
            std::string s = ctx -> NAME() -> toString();
            //std::cout<<"ctx -> NAME() "<<s<<std::endl;
            if(search_map(scope , s))
            {   
                //std::cout<<"has_match_value"<<std::endl;
                return map_value(scope , s);
            }
            else return s;
        }
        else if(ctx -> FALSE())
            return false;
        else if(ctx -> TRUE())
            return true;
        else if (ctx -> NONE())
            return (std::string)"None";
        else if(ctx -> test())
            return visit(ctx -> test());
        return visitChildren(ctx);
    }

    antlrcpp::Any visitTestlist(Python3Parser::TestlistContext *ctx) override {
        std::vector<antlrcpp::Any> list;
        int test_num = ctx -> test().size();
        for(int i = 0 ; i < test_num ; ++i)
        {
            antlrcpp::Any tmp = visit(ctx -> test(i));
            list.push_back(tmp);
        }
        return list;
    }

    antlrcpp::Any visitArglist(Python3Parser::ArglistContext *ctx) override {
        //std::cout<<"in_arglist"<<std::endl;
        std::vector<antlrcpp::Any> tmp ;
        int arglist_num = ctx -> argument().size();
        for(int i = 0 ; i < arglist_num ; i++)
        {
            antlrcpp::Any tmp2 = visit(ctx -> argument(i));
            tmp.push_back(tmp2);
        }
        return tmp;
    }
    antlrcpp::Any visitArgument(Python3Parser::ArgumentContext *ctx) override {
        //std::cout<<"in_argument"<<std::endl;
        return visit(ctx -> test());
    }
};
#endif 
//PYTHON_INTERPRETER_EVALVISITOR_H