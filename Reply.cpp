#include <iostream>
#include <string>

using namespace std;

#include "Message.h"
#include "Reply.h"
#include "User.h"
#include "BBoard.h"

Reply::Reply()
:Message(){
    
}

Reply::Reply(const string &author, const string &subject, const string &body, unsigned id)
:Message(author, subject, body, id){
    
}

bool Reply::isReply() const{
    return true;
}

string Reply::toFormattedString() const{
   stringstream iss;
   string formatted;
   iss << "<begin_reply>" << endl;
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