#include <iostream>
#include <string>

using namespace std;

#include "Message.h"
#include "User.h"
#include "BBoard.h"
#include "Reply.h"
#include "Topic.h"
Message::Message(){
    author = "";
    subject= "";
    body = "";
    id = 0;
}

Message::Message(const string &athr, const string &sbjct, const string &body, unsigned id){
    author = athr;
    subject = sbjct;
    this->body = body;
    this->id = id;
}

Message::~Message() {
    for (unsigned i = 0; i < childList.size(); i++){
        childList.at(i) = nullptr;
    }
}

void Message::print(unsigned indent) const{
   string space = "";
   string full = body;

   for (unsigned i = 0; i < indent; i++){
      space += "  ";
   }
   if (isReply()){
      cout << endl;
   }
   cout << space << "Message #" << getID() << ": " << getSubject() << endl;
   cout << space << "from " << author << ": ";
   for (unsigned i = 0; i < full.size()-1; i++){
      if (full.at(i) == '\n'){
         full.insert(i + 1, space);
      }
   }
   cout << full;
   if (childList.size() != 0){
      for (unsigned i = 0; i < childList.size(); i++){
         childList.at(i)->print(indent + 1);
      }
   }

}

const string& Message::getSubject() const{
    return subject;
}

unsigned Message::getID() const{
    return id;
} 

void Message::addChild(Message *child){
    childList.push_back(child);
}

