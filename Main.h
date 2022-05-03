#include <iostream>
#include <string>

#include "DBHandler.h"
#include "Admin.h"
#include "User.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

void LoginMenu();
int LoginMenu_Choice();
int SelectType();
void CollectUserData(string *record);
User LoginUser(sql::Connection *connection, string *record);
Admin LoginAdmin(sql::Connection *connection, string *record);

void LoginMenu(){
    cout << "-- LOGIN MENU --" << endl;
    cout << "1. Login;" << endl;
    cout << "2. Registration;" << endl;
    cout << "3. Exit." << endl;
}

int LoginMenu_Choice(){
    bool isChoiceValid = false;
    int choice;
    do{
        cout << "Enter your option: ";
        cin >> choice;
        if(choice >= 1 && choice <= 3){
            isChoiceValid = true;
        }else{
            cout << "Option not allowed." << endl;
        }
    }while(!isChoiceValid);
    return choice;
}

int SelectType(){
    bool isTypeValid = false;
    int type;
    cout << "Enter the type of user:" << endl;
    cout << "1. User;" << endl;
    cout << "2. Admin." << endl;
    do{
        cout << "Enter your option: ";
        cin >> type;
        if(type == 1 || type ==2){
            isTypeValid = true;
        }else{
            cout << "Option not allowed." << endl;
        }
    }while(!isTypeValid);
    return type;
}

void CollectUserData(string *record){
    cin.ignore();
    cout << "Enter your name: ";
    string name;
    getline(cin, name);
    cout << "Enter your surname: ";
    string surname;
    getline(cin, surname);
    cout << "Enter your password: ";
    string password;
    getline(cin, password);
    record[0] = name;
    record[1] = surname;
    record[2] = password;
}

User LoginUser(sql::Connection *connection, string *record){
    sql::ResultSet *loginResult = DB_LoginUser(connection, record);
    int id;
    if(loginResult -> next()){
        id = loginResult -> getInt("PersonID");
    }
    User user(id, record[0], record[1], record[2]);
    return user;
}

Admin LoginAdmin(sql::Connection *connection, string *record){
    sql::ResultSet *loginResult = DB_LoginAdmin(connection, record);
    int id;
    string role;
    if(loginResult -> next()){
        id = loginResult -> getInt("PersonID");
        role = loginResult -> getString("Role");
    }
    Admin admin(id, record[0], record[1], record[2], role);
    return admin;
}