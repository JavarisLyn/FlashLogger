/*
 * @Descripttion: 
 * @version: 
 * @Author: justin
 * @Date: 2022-08-11 14:16:32
 * @LastEditTime: 2022-08-11 15:46:58
 * @copyright: Copyright (c) 2022
 */
#include <iostream>
#include <typeinfo>
#include <string.h>
#include <memory>
using namespace std;
int main(){
    const char a[6] = {'1','2','3','4','5','\0'};

    const char (&d)[6] = a;
    cout<<d<<endl;
    cout<<a<<endl;

    
    const char * c = a;
    cout<<strlen(c)<<endl;
    cout<<sizeof(a)<<endl;
    const char * b = __FILE__;
    cout<<sizeof(b)<<endl;
    cout<<sizeof(__FILE__)<<endl;
    cout<<typeid(__FILE__).name()<<endl;
    cout<<__FILE__<<endl;

    
    shared_ptr

    // cout<<typeid(__FILE__).name()<<endl;
    // cout<<sizeof(__FILE__);
}