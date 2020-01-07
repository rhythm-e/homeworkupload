#ifndef _deal_print_h
#define _deal_print_h
#include <iostream>
#include "Python3BaseVisitor.h"
#include "BigInteger.h"

void prints(antlrcpp::Any k)
{
    if(k.is<BigInteger>())
        std::cout<<k.as<BigInteger>();
    else if(k.is<double>())
        std::cout << std::fixed<<setprecision(6) << k.as<double>();
    else if(k.is<std::string>())
    {
        std::string tmp_s = k.as<std::string>();
        if(tmp_s == "None")    cout<<"None";                       
        else 
        {
            tmp_s.erase(std::remove(tmp_s.begin(), tmp_s.end(), '"'), tmp_s.end());
            std::cout << tmp_s;
        }                    
    }
    else if(k.is<bool>())
    {
        if(k.as<bool>())    cout<<"True";
        else                cout<<"False";
    }
}

#endif