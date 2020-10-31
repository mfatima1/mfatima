/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   error.h
 * Author: malihameer
 *
 * Created on October 23, 2020, 3:41 PM
 */

#ifndef ERROR_H
#define ERROR_H


#include<stack>
#include<vector>
using namespace std;

class error {
public:
 //keeps track of the errors. 
    error() {
        errors.push(6);
        errors.push(5);
        errors.push(4);
        errors.push(3);
        errors.push(2);
        errors.push(1);
        errors.push(0);
    }
      void stackdown(){
        errors.pop();
    }
    
    int getNum(){
        return errors.top();
    }
  
private:
    stack<int>errors;
};

#endif /* ERROR_H */

