#ifndef deal_string_h
#define deal_string_h
#include <iostream>
#include "Python3BaseVisitor.h"
#include "BigInteger.h"

antlrcpp::Any multiply(antlrcpp::Any left , antlrcpp::Any right )
{
    if(left.is<BigInteger>() && right.is<std::string>())
    {
        if(left.as<BigInteger>() <= BigInteger(0))
            return std::string("");
        else
        {
            std::string s = right.as<std::string>();
            std::string answer = s ;
            for(BigInteger i = left.as<BigInteger>() ; i > BigInteger(1) ; i = i - BigInteger(1))
                answer += s;
            return answer;
        }
    }
    if(left.is<std::string>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mul"<<std::endl;
        if(right.as<BigInteger>() <= BigInteger(0))
            return std::string("");
        else
        {
            std::string s = left.as<std::string>();
            std::string answer = s ;
            for(BigInteger i = right.as<BigInteger>() ; i > BigInteger(1) ; i = i - BigInteger(1))
                answer += s;
            //std::cout<<answer<<std::endl;
            return answer;
        }
        
    }
    if(left.is<bool>() && right.is<std::string>())
    {
        if(left.as<bool>() == false)
            return std::string("");
    }
    if(left.is<std::string>() && right.is<bool>())
    {
        if(right.as<bool>() == false)
            return std::string("");
    }
    if(left.is<BigInteger>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply1"<<std::endl;
        BigInteger left1,right1;
        left1 = left.as<BigInteger>();
        right1 = right.as<BigInteger>();
        return left1 * right1;
    }
    if(left.is<double>() && right.is<double>())
    {
        //std::cout<<"in_mutiply2"<<std::endl;
        double left1,right1;
        left1 = left.as<double>();
        right1 = right.as<double>();
        return left1 * right1;
    }
    if(left.is<double>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply3"<<std::endl;
        double right1,left1;
        right1 = double(right.as<BigInteger>());
        left1 = left.as<double>();
        return right1 * left1;
    }
    if(right.is<double>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply4"<<std::endl;
        double right1,left1;
        right1 = right.as<double>();
        left1 = double(left.as<BigInteger>());
        return right1 * left1;
    }
    if(right.is<BigInteger>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply5"<<std::endl;
        BigInteger left1,right1;
        right1 = right.as<BigInteger>();
        left1 = BigInteger(int(left.as<bool>()));
        return right1 * left1;
    }
    if(right.is<bool>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply6"<<std::endl;
        BigInteger left1,right1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = left.as<BigInteger>();
        return right1 * left1;
    }
    if(right.is<double>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply7"<<std::endl;
        double left1,right1;
        right1 = right.as<double>();
        left1 = double(left.as<bool>());
        return right1 * left1;
    }
    if(right.is<bool>() && left.is<double>())
    {
        //std::cout<<"in_mutiply8"<<std::endl;
        double left1,right1;
        right1 = double(right.as<bool>());
        left1 = left.as<double>();
        return right1 * left1;
    }
    if(right.is<bool>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply9"<<std::endl;
        BigInteger right1,left1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = BigInteger(int(left.as<bool>()));
        return right1 * left1;
    }
}

antlrcpp::Any divide(antlrcpp::Any left , antlrcpp::Any right)
{
    double right1 , left1;
    if(right.is<BigInteger>()) right1 = double(right.as<BigInteger>());
    else if(right.is<double>()) right1 = right.as<double>();
    else if(right.is<bool>())  right1 = double(right.as<bool>());
    if(left.is<BigInteger>()) left1 = double(left.as<BigInteger>());
    else if(left.is<double>()) left1 = left.as<double>();
    else if(left.is<bool>())  left1 = double(left.as<bool>());
    return left1/right1;
}

BigInteger Idivide(antlrcpp::Any left , antlrcpp::Any right)
{
    BigInteger right1,left1;
    if(right.is<BigInteger>()) right1 = right.as<BigInteger>();
    else if(right.is<bool>()) right1 = BigInteger(int(right.as<bool>()));
    if(left.is<BigInteger>()) left1 = left.as<BigInteger>();
    else if(left.as<BigInteger>()) left1 = BigInteger(int(left.as<bool>()));
    return BigInteger(left1 / right1);
}

BigInteger Mode(antlrcpp::Any left ,antlrcpp::Any right)
{
    BigInteger right1,left1;
    if(right.is<BigInteger>()) right1 = right.as<BigInteger>();
    else if(right.is<bool>()) right1 = BigInteger(int(right.as<bool>()));
    if(left.is<BigInteger>()) left1 = left.as<BigInteger>();
    else if(left.as<BigInteger>()) left1 = BigInteger(int(left.as<bool>()));
    return BigInteger(left1 % right1);
}

antlrcpp::Any add(const antlrcpp::Any left , const antlrcpp::Any right)
{
    if(left.is<BigInteger>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply1"<<std::endl;
        BigInteger left1,right1;
        left1 = left.as<BigInteger>();
        right1 = right.as<BigInteger>();
        return left1 + right1;
    }
    if(left.is<double>() && right.is<double>())
    {
        //std::cout<<"in_mutiply2"<<std::endl;
        double left1,right1;
        left1 = left.as<double>();
        right1 = right.as<double>();
        return left1 + right1;
    }
    if(left.is<double>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply3"<<std::endl;
        double right1,left1;
        right1 = double(right.as<BigInteger>());
        left1 = left.as<double>();
        return right1 + left1;
    }
    if(right.is<double>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply4"<<std::endl;
        double right1,left1;
        right1 = right.as<double>();
        left1 = double(left.as<BigInteger>());
        return right1 + left1;
    }
    if(right.is<BigInteger>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply5"<<std::endl;
        BigInteger left1,right1;
        right1 = right.as<BigInteger>();
        left1 = BigInteger(int(left.as<bool>()));
        return right1 + left1;
    }
    if(right.is<bool>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply6"<<std::endl;
        BigInteger left1,right1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = left.as<BigInteger>();
        return right1 + left1;
    }
    if(right.is<double>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply7"<<std::endl;
        double left1,right1;
        right1 = right.as<double>();
        left1 = double(left.as<bool>());
        return right1 + left1;
    }
    if(right.is<bool>() && left.is<double>())
    {
        //std::cout<<"in_mutiply8"<<std::endl;
        double left1,right1;
        right1 = double(right.as<bool>());
        left1 = left.as<double>();
        return right1 + left1;
    }
    if(right.is<bool>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply9"<<std::endl;
        BigInteger right1,left1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = BigInteger(int(left.as<bool>()));
        return right1 + left1;
    }
    if(right.is<std::string>() && left.is<std::string>())
    {
        std::string s1 = right.as<std::string>();
        std::string s2 = left.as<std::string>();
        s2.erase(s2.end()-1);
        s1.erase(s1.begin());
        return s2+s1;
    }
}
antlrcpp::Any mis(const antlrcpp::Any left , const antlrcpp::Any right)
{
    if(left.is<BigInteger>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply1"<<std::endl;
        BigInteger left1,right1;
        left1 = left.as<BigInteger>();
        right1 = right.as<BigInteger>();
        return left1 - right1;
    }
    if(left.is<double>() && right.is<double>())
    {
        //std::cout<<"in_mutiply2"<<std::endl;
        double left1,right1;
        left1 = left.as<double>();
        right1 = right.as<double>();
        return left1 - right1;
    }
    if(left.is<double>() && right.is<BigInteger>())
    {
        //std::cout<<"in_mutiply3"<<std::endl;
        double right1,left1;
        right1 = double(right.as<BigInteger>());
        left1 = left.as<double>();
        return right1 - left1;
    }
    if(right.is<double>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply4"<<std::endl;
        double right1,left1;
        right1 = right.as<double>();
        left1 = double(left.as<BigInteger>());
        return right1 - left1;
    }
    if(right.is<BigInteger>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply5"<<std::endl;
        BigInteger left1,right1;
        right1 = right.as<BigInteger>();
        left1 = BigInteger(int(left.as<bool>()));
        return right1 - left1;
    }
    if(right.is<bool>() && left.is<BigInteger>())
    {
        //std::cout<<"in_mutiply6"<<std::endl;
        BigInteger left1,right1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = left.as<BigInteger>();
        return right1 - left1;
    }
    if(right.is<double>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply7"<<std::endl;
        double left1,right1;
        right1 = right.as<double>();
        left1 = double(left.as<bool>());
        return right1 - left1;
    }
    if(right.is<bool>() && left.is<double>())
    {
        //std::cout<<"in_mutiply8"<<std::endl;
        double left1,right1;
        right1 = double(right.as<bool>());
        left1 = left.as<double>();
        return right1 - left1;
    }
    if(right.is<bool>() && left.is<bool>())
    {
        //std::cout<<"in_mutiply9"<<std::endl;
        BigInteger right1,left1;
        right1 = BigInteger(int(right.as<bool>()));
        left1 = BigInteger(int(left.as<bool>()));
        return right1 - left1;
    }
}

bool less_than(const antlrcpp::Any left ,const antlrcpp::Any right)
{
    if(left.is<BigInteger>() && right.is<BigInteger>())
        return left.as<BigInteger>() < right.as<BigInteger>();
    if(left.is<BigInteger>() && right.is<double>())
        return double(left.as<BigInteger>()) < right.as<double>();
    if(left.is<double>() && right.is<BigInteger>())
        return left.as<double>() < double(right.as<BigInteger>());
    if(left.is<BigInteger>() && right.is<bool>())
        return left.as<BigInteger>() < BigInteger(int(right.as<bool>()));
    if(left.is<bool>() && right.is<BigInteger>())
        return BigInteger(int(left.as<bool>())) < right.as<bool>();
    if(left.is<double>() && right.is<double>())
        return left.as<double>() < right.as<double>();
    if(left.is<bool>() && right.is<double>())
        return double(left.as<bool>()) < right.as<double>();
    if(left.is<double>() && right.is<bool>())
        return left.as<double>() < double(right.as<bool>());
    if(left.is<bool>() && right.is<bool>())
        return left.as<bool>() < right.as<bool>();
    if(left.is<std::string>() && right.is<std::string>())
        return left.as<std::string>() < right.as<std::string>();
}

bool greater_than(const antlrcpp::Any left ,const antlrcpp::Any right)
{
    if(left.is<BigInteger>() && right.is<BigInteger>())
        return left.as<BigInteger>() > right.as<BigInteger>();
    if(left.is<BigInteger>() && right.is<double>())
        return double(left.as<BigInteger>()) > right.as<double>();
    if(left.is<double>() && right.is<BigInteger>())
        return left.as<double>() > double(right.as<BigInteger>());
    if(left.is<BigInteger>() && right.is<bool>())
        return left.as<BigInteger>() > BigInteger(int(right.as<bool>()));
    if(left.is<bool>() && right.is<BigInteger>())
        return BigInteger(int(left.as<bool>())) > right.as<bool>();
    if(left.is<double>() && right.is<double>())
        return left.as<double>() > right.as<double>();
    if(left.is<bool>() && right.is<double>())
        return double(left.as<bool>()) > right.as<double>();
    if(left.is<double>() && right.is<bool>())
        return left.as<double>() > double(right.as<bool>());
    if(left.is<bool>() && right.is<bool>())
        return left.as<bool>() > right.as<bool>();
    if(left.is<std::string>() && right.is<std::string>())
        return left.as<std::string>() > right.as<std::string>();
}

bool equals(const antlrcpp::Any left ,const antlrcpp::Any right)
{
    if(left.is<BigInteger>() && right.is<BigInteger>())
        return left.as<BigInteger>() == right.as<BigInteger>();
    else if(left.is<BigInteger>() && right.is<double>())
        return double(left.as<BigInteger>()) == right.as<double>();
    else if(left.is<double>() && right.is<BigInteger>())
        return left.as<double>() == double(right.as<BigInteger>());
    else if(left.is<BigInteger>() && right.is<bool>())
        return left.as<BigInteger>() == BigInteger(int(right.as<bool>()));
    else if(left.is<bool>() && right.is<BigInteger>())
        return BigInteger(int(left.as<bool>())) == right.as<bool>();
    else if(left.is<double>() && right.is<double>())
        return left.as<double>() == right.as<double>();
    else if(left.is<bool>() && right.is<double>())
        return double(left.as<bool>()) == right.as<double>();
    else if(left.is<double>() && right.is<bool>())
        return left.as<double>() == double(right.as<bool>());
    else if(left.is<bool>() && right.is<bool>())
        return left.as<bool>() == right.as<bool>();
    else if(left.is<std::string>() && right.is<std::string>())
        return left.as<std::string>() == right.as<std::string>();
    else return false;    
}

bool gt_eq(const antlrcpp::Any left , const antlrcpp::Any right)
{ return (greater_than(left , right) | equals(left , right)); }

bool lt_eq(const antlrcpp::Any left , const antlrcpp::Any right)
{ return (less_than(left , right) | equals(left , right)); }

bool not_eq_2(const antlrcpp::Any left , const antlrcpp::Any right)
{ return !(equals(left , right)); }

bool is_variable(const std::string s)
{ return !(s[0] == '"');}

bool to_continue(const antlrcpp::Any whether_continue)
{
    if(whether_continue.is<BigInteger>()) return bool(whether_continue.as<BigInteger>());
    else if(whether_continue.is<double>())return bool(whether_continue.as<double>());
    else if (whether_continue.is<bool>())return whether_continue.as<bool>();
    else if(whether_continue.is<std::string>())return bool(whether_continue.as<std::string>() > "");    
}
#endif