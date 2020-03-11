#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#include "Message.h"
#include "User.h"
#include "BBoard.h"
#include "Topic.h"
#include "Reply.h"

BBoard::BBoard(){
    title = " ";
    currentUser = nullptr;
}

BBoard::BBoard(const string &t){
    title = t;
    currentUser = nullptr;
}

BBoard::~BBoard() {
    for (unsigned i = 0; i < messageList.size(); i++) {
         messageList.at(i) = nullptr;
    }
}

bool BBoard::loadUsers(const string &input){
    string user;
    string pass;
    ifstream inFS;
    inFS.open(input);
    
    if (!inFS.is_open()) {
        inFS.close();
        return false;
    }
    
    if(inFS.fail()) {
        inFS.close();
        return false;
    }

    while (user != "end"){
        inFS >> user;
        if(user != "end"){
            inFS >> pass;
            addUser(user, pass);  
        }
    }
    
    inFS.close();
    return true;
}

bool BBoard::loadMessages(const string &data){
    string author, subject, body, childIdString, type, input;
    unsigned id, numMessages; 
    ifstream inFS;
    inFS.open(data);    
    if (inFS.fail()) {
        return false;   
    }
    
    inFS >> numMessages;    
    vector<vector<int>> holdChildren(numMessages);
    while (numMessages != 0) {    
        input = "";
        id = 0;
        subject = "";
        author = "";
        body = "";
        childIdString = "";
        inFS >> type;  
        while (input != "<end>") {  
            inFS >> input;    
            if (input == ":id:") {  
                inFS >> id;
            } 
            else if (input == ":subject:") {  
                getline(inFS, subject);
                subject = subject.substr(1);
            } 
            else if (input == ":from:") { 
                inFS >> author;
            } 
            else if (input == ":children:") { 
                getline(inFS, childIdString);
                int childID;
                stringstream iss(childIdString);   
                while (iss >> childID) {           
                    holdChildren.at(id-1).push_back(childID);
                }
            } else if (input == ":body:") { 
                while (input != "<end>") {  
                    getline(inFS, input); 
                    if (input == "<end>") {
                        break;  
                    }
                    body = body + input + '\n'; 
                }
               body = body.substr(1);
            }
        }
        if (type == "<begin_topic>") {
            messageList.push_back(new Topic(author, subject, body, id));
        } else if ( type == "<begin_reply>") {
            messageList.push_back( new Reply(author, subject, body, id));
        }
        
        numMessages--;
    }
    
    inFS.close();
    
    for (unsigned i = 0; i < messageList.size(); i++) {
        for (unsigned j = 0; j < holdChildren.at(i).size(); j++) {
            messageList.at(i) -> addChild(messageList.at(holdChildren.at(i).at(j) - 1));
        }
    }
    
    return true;  
}



bool BBoard::saveMessages(const string &data){
    ofstream outFS;
    outFS.open(data);
    if (!outFS.is_open()){
        outFS.close();
        return false;
    }
    if (outFS.fail()){
        outFS.close();
        return false;
    }
    outFS << messageList.size() << endl;
    for (unsigned i = 0; i < messageList.size(); ++i){
        outFS << messageList.at(i)->toFormattedString();
    }
    outFS.close();
    return true;
}

void BBoard::login(){
    cout << "Welcome to " << title << endl;
    string user;
    string pass;
    bool invalid = true;
    while (invalid){
        cout << "Enter your username ('Q' or 'q' to quit): ";
        cin >> user;
        if ((user == "q") || (user== "Q")){
            cout << "Bye!" << endl;
            exit(0);
        }
        else{
             cout << "Enter your password: ";
             cin >> pass;
             if (getUser(user,pass) == nullptr) {  
                 cout << "Invalid Username or Password!" << endl << endl;
             } 
             else {
                cout << endl;
                cout << "Welcome back " << user << "!" << endl << endl;
                currentUser = getUser(user,pass);
                invalid = false;
             }
        }
    }
}

void BBoard::run(){
   if (currentUser == nullptr) {
        return;
   }
   char in = ' ';
   bool loggedIn = true;
   while (loggedIn){
       cout << "Menu" << endl;
       cout << "- Display Messages ('D' or 'd')" << endl;
       cout << "- Add New Topic ('N' or 'n')" << endl;
       cout << "- Add Reply to a Topic ('R' or 'r')" << endl;
       cout << "- Quit ('Q' or 'q')" << endl;
       cout << "Choose an action: ";
       cin >> in;
       if ((in == 'D') || (in == 'd')){
           cout << endl;
           display();
       }
       if ((in == 'N') || (in == 'n')){
           addTopic();
       }
       if( in == 'R' || in == 'r' ) {
           addReply();
       }
       if ((in == 'Q') || (in == 'q')){
           cout << "Bye!" << endl;
           loggedIn = false;
       }
   }
}

void BBoard::addUser(const string &name, const string &pass){
    userList.push_back(User(name,pass));
}

const User * BBoard::getUser(const string &name, const string &pw) const{
    for (unsigned i = 0; i < userList.size(); ++i){
        if (userList.at(i).check(name,pw) == true){
            return &userList.at(i);
        }
    }
    return nullptr;
}

void BBoard::addTopic(){
    bool bodyCreate = true;
    string temp = " ";
    string subject, body, full = " ";
    unsigned id;
    id = messageList.size() + 1;
    cout << "Enter Subject: ";
    cin.ignore();
    getline(cin, subject);
    cout << "Enter Body: ";
    while (bodyCreate){
       getline (cin, body);
       if (body.empty()){
           bodyCreate = false;
           break;
       }
        full += (body + "\n");
    }
    full = full.substr(1);
    Topic *newTopic = new Topic(currentUser->getUsername(), subject, full, id);
    messageList.push_back(newTopic);
    cout << endl;
}
    
void BBoard::addReply(){
    bool bodyCreate = true;
    string temp = " ";
    string subject, body, full = " ";
    int id;
    int size = messageList.size();
    bool valid = false;
    
    while (!valid){
    cout << "Enter Message ID (-1 for Menu): "; 
    cin >> id;
        if ((id == 0) || (id > size)){
            cout << "Invalid Message ID!!" << endl << endl;
        }
        else if (id == -1){
            cout << endl;
            return;
        }
        else{
            valid = true;
            break;
        }
    }
    
    cin.ignore();
    cout << "Body: ";
    
    while (bodyCreate){
       getline (cin, body);
       if (body.empty()){
           bodyCreate = false;
           break;
       }
        full += (body + "\n");
  }
  full = full.substr(1);
  subject = "Re: " + messageList.at(id - 1)->getSubject();
  Reply* newReply = new Reply(currentUser->getUsername(), subject, full, messageList.size()+1);
  messageList.at(id - 1)->addChild(newReply);
  messageList.push_back(newReply);
  cout << endl;
}
   
   
void BBoard::display() const{
    if (messageList.size() == 0){
        cout << "Nothing to Display." << endl << endl;
    }
    else{
        for (unsigned int i = 0; i < messageList.size(); i++){
            if (messageList.at(i)->isReply() == false){
                cout << "---------------------------------------------------------" << endl;
                messageList.at(i)->print(0);
            }
        }
        cout << "---------------------------------------------------------" << endl;
        cout << endl;
    }
}
    


