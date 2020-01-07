#ifndef _deal_scope_h
#define _deal_scope_h
#include <iostream>
#include <map>
#include <vector>
#include "Python3BaseVisitor.h"
#include "BigInteger.h"

struct op{
    size_t pos;
    int value;
};

bool Greatersort(op a, op b){return a.pos < b.pos;}

bool search_map(std::vector<std::map<std::string,antlrcpp::Any>> scope , std::string key)
{
    std::vector<std::map<std::string,antlrcpp::Any>>::reverse_iterator out;
    for(out = scope.rbegin();out != scope.rend() ; ++out )
        if((*out).count(key)) return true;
    return false;
}

antlrcpp::Any map_value(std::vector<std::map<std::string,antlrcpp::Any>> scope , std::string key)
{
    std::vector<std::map<std::string,antlrcpp::Any>>::reverse_iterator out;
    for(out = scope.rbegin();out != scope.rend() ; ++out )
        if((*out).count(key)) return (*out)[key];
}

struct reflect
{
    std::string name;
    antlrcpp::Any value;
};




#endif