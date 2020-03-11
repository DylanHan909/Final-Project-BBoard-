#ifndef __BBOARD_H__
#define __BBOARD_H__

#include <string>
#include <vector>
using namespace std;

#include "Message.h"
#include "User.h"

class BBoard {
 private:
   string title;
	vector<User> userList;
	const User * currentUser;
	vector<Message *> messageList;
 public:
	BBoard();
	BBoard(const string &);
	~BBoard();
	bool loadUsers(const string &);
	bool loadMessages(const string &);
	bool saveMessages(const string &);
	void login();
	void run();

  private:    
    void addUser(const string& name, const string& pass);
    const User* getUser(const string& name, const string& pw) const;
    void display() const;
    void addTopic();
    void addReply();
};
#endif