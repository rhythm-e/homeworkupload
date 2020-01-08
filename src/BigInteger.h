#ifndef _BigInteger_h
#define _BigInteger_h
#include <iostream>
#include <cstring>
#include <vector>
#include <deque>
#include <cstdlib>
using namespace std;

class BigInteger {
private:
    vector<int> num;
    bool sign;
    int len;
public:
    BigInteger() {
        len = 0;
        sign = true;
        num.push_back(0);
    }

    BigInteger(int obj);
    BigInteger(const string &obj);
    BigInteger(const char *obj);
    BigInteger(const double obj);
    BigInteger(const BigInteger &obj);
    BigInteger &operator=(const BigInteger &right);
    BigInteger abs() const;

    friend BigInteger operator-(const BigInteger &obj);
    friend ostream& operator<<(ostream& os, const BigInteger& obj);
    friend istream& operator>>(istream& is,BigInteger& obj);
    friend bool operator>(const BigInteger& r1, const BigInteger& r2);
    friend bool operator>=(const BigInteger& r1, const BigInteger& r2);
    friend bool operator==(const BigInteger& r1, const BigInteger& r2);
    friend bool operator!=(const BigInteger& r1, const BigInteger& r2);
    friend bool operator<(const BigInteger& r1, const BigInteger& r2);
    friend bool operator<=(const BigInteger& r1, const BigInteger& r2);
    friend BigInteger operator+(const BigInteger& r1, const BigInteger& r2);
    friend BigInteger operator-(const BigInteger& r1, const BigInteger& r2);
    friend BigInteger operator*(const BigInteger& r1, const BigInteger& r2);
    friend BigInteger operator/(const BigInteger& r1, const BigInteger& r2);
    friend BigInteger operator%(const BigInteger& r1, const BigInteger& r2);
    explicit operator bool()const;
    explicit operator string()const;
    explicit operator double()const;
};

BigInteger::BigInteger(int obj)
{
    {
        num.clear();
        if(obj >= 0) sign = true;
        else {sign = false ; obj = - obj;}
        if(obj == 0)num.push_back(0);
        else
        {
            while(obj != 0)
            {
                num.push_back(obj % 10);
                obj /= 10;
            }
        }
        len = num.size();
    }
}
BigInteger::BigInteger(const string &obj)
{
    num.clear();
    len = obj.size();
    if(len == 2 && *obj.begin()=='"') 
    {
        sign = true;
        len = 1;
        num.push_back(0);
    }
    else
    {
        string s = obj;
        if(*s.begin() == '"')
        {   
            s.erase(s.begin());
            s.erase(s.end() - 1);
        }
        len = s.size();
        sign = s[0] != '-';
        if(sign)
        {
            for (int i = 0; i < len; i++)
                num.push_back(s[len - i - 1] - '0');
        }
        else
        {
            for(int i = 1; i < len; i++)
                num.push_back(s[len - i -1] - '0');
            len--;
        } 
    }
}
BigInteger::BigInteger(const char* obj)
{
    len = strlen(obj);
    sign = obj[0] != '-';
    if(sign)
    {
        for (int i = 0; i <len; i++)
            num.push_back(obj[len - i - 1] - '0');
    }
    else
    {
        for(int i = 0; i < len - 1; i++)
            num.push_back(obj[len - i -1] - '0');
        len--;
    }
}
BigInteger::BigInteger(const double obj)
{
    num.clear();
    int pos = 0;   
    string tmp = to_string(obj);
    if(tmp[0] == '-' ) {sign = false; pos++;}
    else {sign = true;}
    while(tmp[pos] != '.')
        num.push_back(tmp[pos++] - '0');
    len = num.size();
}
BigInteger::BigInteger(const BigInteger& obj)
{
    len = obj.len;
    sign = obj.sign;
    for(int i = 0; i < len; i++)
        num.push_back(obj.num[i]);
}

BigInteger &BigInteger::operator=(const BigInteger& right)
{
    if( this == &right ) return *this;
    num.clear();
    len = right.len;
    sign = right.sign;
    for(int i = 0; i < len; i++)
        num.push_back(right.num[i]);
    return *this;
}

BigInteger BigInteger::abs()const
{
    if(sign) return *this;
    else
    {
        BigInteger tmp = *this;
        tmp.sign = true;
        return tmp;
    }
}

BigInteger operator-(const BigInteger &obj)
{
    BigInteger tmp = obj;
    tmp.sign = !tmp.sign;
    return tmp;
}

ostream& operator<<(ostream& os, const BigInteger& obj)
{
    if(!obj.sign)
        os<<'-';
    for(int i = obj.len - 1 ; i >= 0; --i)
        os << obj.num[i];
    return os;
}

istream& operator>>(istream& is, BigInteger& obj)
{
    string s;
    is >> s;
    for(unsigned int i = s.size()-1; i >= 0; --i)
        obj.num.push_back(s[i]);
    obj.len = obj.num.size();
    return is;
}

bool operator>(const BigInteger& r1, const BigInteger& r2)
{
    if(r1.sign && !r2.sign) return true;
    if(!r1.sign && r2.sign) return false;
    if(r1.sign && r2.sign)
    {
        if(r1.len > r2.len) return true;
        if(r1.len < r2.len) return false;
        else
        {
            int ln = r1.len - 1;
            while(r1.num[ln] == r2.num[ln] && ln >= 0) ln--;
            return ln >= 0 && r1.num[ln] > r2.num[ln];
        }
    }
    if(!r1.sign && !r2.sign)
    {
        if(r1.len < r2.len) return true;
        if(r1.len > r2.len) return false;
        else
        {
            int ln = r1.len - 1;
            while(r1.num[ln] == r2.num[ln] && ln >= 0) ln--;
            return ln >= 0 && r1.num[ln] < r2.num[ln];
        }
    }
}

bool operator==(const BigInteger& r1, const BigInteger& r2)
{
    if(r1.len != r2.len) return false;
    else
    {
        int ln = r1.len - 1;
        while(r1.num[ln] == r2.num[ln] && ln >= 0) ln--;
        return ln < 0;
    }
}

bool operator!=(const BigInteger& r1, const BigInteger& r2)
{ return !(r1 == r2);}
bool operator<(const BigInteger& r1, const BigInteger& r2)
{
    if(r1 == r2) return false;
    else return !(r1 > r2);
}

bool operator>=(const BigInteger& r1, const BigInteger& r2)
{
    return (r1 > r2 || r1 == r2) ;
}

bool operator<=(const BigInteger& r1, const BigInteger& r2)
{
    return (r1 < r2 || r1 == r2);
}
BigInteger operator+(const BigInteger& r1, const BigInteger& r2)
{
    BigInteger r3;
    if(r1.len == 0 && r2.len != 0) r3 = r2;
    else if(r2.len == 0 && r1.len != 0) r3 = r1;
    else if(r1.len == 0 && r2.len == 0) r3.len = 1;
    else {
        r3.num.clear();
        if ((r1.sign && r2.sign) || (!r1.sign && !r2.sign)) {
            int surplus = 0;
            int big_size = r1.len > r2.len ? r1.len : r2.len;
            int short_size = r1.len < r2.len ? r1.len : r2.len;
            for (int i = 0; i < short_size; i++) {
                r3.num.push_back((r1.num[i] + r2.num[i] + surplus) % 10);
                surplus = (r1.num[i] + r2.num[i] + surplus) / 10;
            }
            for (int i = short_size; i < big_size; ++i) {
                if (r1.len > r2.len) {
                    r3.num.push_back((r1.num[i] + surplus) % 10);
                    surplus = (r1.num[i] + surplus) / 10;
                } else {
                    r3.num.push_back((r2.num[i] + surplus) % 10);
                    surplus = (r2.num[i] + surplus) / 10;
                }
            }
            if (surplus)
                r3.num.push_back(surplus);
            r3.len = r3.num.size();
        }
        if(r1.sign && !r2.sign) {
            BigInteger tmp = r2;
            tmp.sign = true;
            r3 = r1 - tmp;
        }
        if(!r1.sign && r2.sign)
        {
            BigInteger tmp = r1;
            tmp.sign = true;
            r3 = r2 - tmp;
        }
    }
    if(!r1.sign && !r2.sign) r3.sign = false;
    return r3;
}
BigInteger operator-(const BigInteger& r1, const BigInteger& r2)
{
    BigInteger r3;
    if(r1.len == 0 && r2.len != 0) r3 = r2;
    else if(r2.len == 0 && r1.len != 0) r3 = r1;
    else if(r1.len == 0 && r2.len == 0) r3.len = 1;
    else {
        r3.num.clear();
        if(r1.sign && !r2.sign)
        {
            BigInteger tmp = r2;
            tmp.sign = true;
            r3 = r1 + tmp;
        }
        if(!r1.sign && r2.sign)
        {
            BigInteger tmp = r2;
            tmp.sign = false;
            r3 = r1 + tmp;
        }
        if ((r1.sign && r2.sign) || (!r1.sign && !r2.sign)) {
            int surplus = 0 , k = 0;
            BigInteger small,big;
            bool need_reverse = false;
            if(r1 == r2)return BigInteger(0);
            if(r1.sign)
            {
                if (r1 > r2) {big = r1; small = r2;}
                else         {big = r2; small = r1; need_reverse = true;}
            }
            else
            {
                if(r1 > r2) {big = r2; small = r1;}
                else         {big = r1; small = r2; need_reverse = true;}
            }
            for(int i = 0 ; i < small.len ; ++i)
            {
                surplus = big.num[i] - small.num[i] + k;
                if(surplus >= 0) { r3.num.push_back(surplus); k = 0;}
                else{ r3.num.push_back(10 + surplus) ; k = -1;}
            }
            for(int i = small.len ;i < big.len ; ++i)
            {
                surplus = big.num[i] + k;
                if(surplus >= 0) { r3.num.push_back(surplus); k = 0;}
                else{ r3.num.push_back(10 + surplus); k = -1;}
            }
            vector <int>::iterator Iter;
            Iter = r3.num.end()-1;
            while(*Iter == 0){r3.num.erase(Iter); Iter = r3.num.end()-1;}
            if(need_reverse) r3.sign = false;
        }
    }
    r3.len = r3.num.size();
    return r3;
}

BigInteger operator*(const BigInteger& r1, const BigInteger& r2)
{
    int surplus = 0;
    BigInteger r3;
    if(r1.len == 0 || r2.len == 0) r3.len = 1;
    else if((r1.len == 1 && r1.num[0] == 0) ||(r2.len == 1 && r2.num[0] == 0))
        r3.len = r3.num.size();
    else {
        vector<int> tmp;
        r3.num.clear();
        for (int i = 0; i < r1.len; ++i)
            for (int j = 0; j < r2.len; ++j)
            {
                if (i + j < tmp.size()) {
                    int temp = tmp[i + j] + r1.num[i] * r2.num[j];
                    tmp[i + j] = temp % 10;
                    if (temp / 10 > 0) {
                        if (i + j + 1 == tmp.size())
                            tmp.push_back(temp / 10);
                        else
                            tmp[i + j + 1] = temp / 10 + tmp[i + j +1];
                    }
                }

                else
                    tmp.push_back(r1.num[i] * r2.num[j]);
            }
        for (int i : tmp)
        {
            r3.num.push_back((i + surplus) % 10);
            surplus = (i + surplus) / 10;
        }
        if (surplus)
            r3.num.push_back(surplus);
        if ((!r1.sign && r2.sign) || (r1.sign && !r2.sign))
            r3.sign = false;
        r3.len = r3.num.size();
    }
    return r3;
}

BigInteger operator/(const BigInteger& r1, const BigInteger& r2)
{
    BigInteger r3;
    BigInteger tmp(0);
    if(r2.num[0] == 0) exit(0);
    BigInteger t1 = r1.abs(), t2 = r2.abs();
    if(t1 < t2) {r3.len++;}
    else
    {
        r3.num.clear();
        deque<int>temp_put;
        auto Iter = t1.num.rbegin();
        while(Iter != t1.num.rend())
        {
            tmp = tmp * BigInteger(10) + BigInteger(*Iter);
            int cnt = 0 ;
            while(tmp >= t2)
            {
                tmp = tmp - t2;
                cnt++;
            }
            temp_put.push_front(cnt);
            Iter++;
        }
        r3.num.insert(r3.num.end(), temp_put.begin(), temp_put.end());
        vector <int>::iterator Iter2;
        Iter2 = r3.num.end()-1;
        while(*Iter2 == 0){r3.num.erase(Iter2); Iter2 = r3.num.end()-1;}
    }
    r3.len = r3.num.size();
    if((r1.sign && !r2.sign) || (!r1.sign && r2.sign))
    {
        if(r1.len == 1 && r1.num[0] == 0)
            r3.len = 1;
        else
        {
            r3.sign = false;
            if(r1 != r2 * r3)
                r3 = r3 - BigInteger(1);
        }
    }
    return r3;
}

BigInteger operator%(const BigInteger& r1, const BigInteger& r2)
{return r1 - (r1 / r2)* r2;}

BigInteger::operator bool()const
{return num[0] != 0;}

BigInteger::operator string()const
{
    string x;
    x.push_back('"');
    for(int i = len - 1 ; i >= 0; --i)
        x.push_back('0' + num[i]);
    x.push_back('"');
    return x;
}
BigInteger::operator double()const
{
    string x;
    for(int i = len - 1 ; i >= 0; --i)
        x.push_back('0' + num[i]);
    return stod(x);
}
#endif
