#include <iostream>
#include <string>

using namespace std;

#include "Message.h"
#include "Topic.h"
#include "User.h"
#include "BBoard.h"

Topic::Topic()
: Message(){
    
}

Topic::Topic(const string &author, const string &subject, const string &body, unsigned id)
: Message(author, subject, body, id){
    
}

bool Topic::isReply() const{
    return false;
}

string Topic::toFormattedString() const{
   stringstream iss;
   string formatted;
   iss << "<begin_topic>" << endl;
   iss << ":id: " << id << endl;
   iss << ":subject: " << subject << endl;
   iss << ":from: " << author << endl;
   if(childList.size() != 0){
      iss << ":children: ";
      for (unsigned i = 0; i < childList.size(); i++){
         iss << childList.at(i)->getID();
         if (i != childList.size() - 1)
            iss << " ";
      }
      iss << endl;
   }
      
   iss << ":body: " << body;
   if (body.at(body.size()-1) == '\n'){
   }
   else{
       iss << endl;
   }
   iss << "<end>" << endl;
   
   formatted = iss.str();
   
   return formatted;

}