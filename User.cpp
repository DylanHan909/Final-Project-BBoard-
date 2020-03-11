

#include <iostream>
#include <string>

using namespace std;

#include "User.h"
#include "Message.h"
#include "BBoard.h"

User::User(){
    username = "";
    password = "";
}

User::User(const string& uname, const string& pass){
    username = uname;
    password = pass;
}

string User::getUsername() const{
    return username;
}

bool User::check(const string &uname, const string &pass) const{
    if (username.empty()){
        return false;
    }
    
    else if (uname != username || pass != password){
        return false;
    }
    
    else{ 
        return true;
    }
}

bool User::setPassword(const string &oldpass, const string &newpass){
    if (username.empty()){
        return false;
    }
    else if (oldpass == password){
        password = newpass;
        return true;
    }
    else{
        return false;
    }
}
