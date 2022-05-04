#include <iostream>
#include <string>

#include "DBHandler.h"
#include "Classes/Admin.h"
#include "Classes/User.h"

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
void RegisterUser(sql::Connection *connection, string *record);
void UserMenu();
int UserMenu_Choice();
void SearchBook(sql::Connection *connection, string book);

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
    delete loginResult;
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
    delete loginResult;
    return admin;
}

void RegisterUser(sql::Connection *connection, string *record){
    DB_RegisterUser(connection, record);
    cout << "Registration completed!" << endl;
}

void UserMenu(){
    cout << "--- MENU ---" << endl;
    cout << "1. Search for a book;" << endl;
    cout << "2. Rent a book;" << endl;
    cout << "3. Return a book;" << endl;
    cout << "4. Postpone book's return date;" << endl;
    cout << "5. Exit." << endl;
}

int UserMenu_Choice(){
    int choice;
    bool isChoiceValid = false;
    do{
        cout << "Enter your option: ";
        cin >> choice;
        if(choice > 0 && choice < 6){
            isChoiceValid = true;
        }else{
            cout << "Option not allowed." << endl;
        }
    }while(!isChoiceValid);
    return choice;
}

void SearchBook(sql::Connection *connection, string book){
    bool found = false;
    sql::ResultSet *result = DB_SearchBook(connection, book);
    if(result -> next()){
        if((result -> getInt("Copies")) > 0){
            cout << book << " is available!" << endl;
        }else{
            cout << "Sorry, " << book << " is not available at the moment..." << endl;
        }
    }else{
        cout << "Sorry, " << book << " is not avaiable..." << endl;
    }
}