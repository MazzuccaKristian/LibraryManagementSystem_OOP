#include <iostream>
#include <string>

#include "DBHandler.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;

int LoginPhase(sql::Connection *connection);
void ShowLoginMenu();
int RetrieveLoginChoice();
int Login(sql::Connection *connection);
// int ValidateChoice(string input);

/**
 * @brief
 * Login phase:
 * 1: Login or registration?
 * - 1.1: Login.
 *
 * @return int
 */
int LoginPhase(sql::Connection *connection){
    bool isChoiceValid = false;
    int userID;
    do{
        ShowLoginMenu();
        int choice = RetrieveLoginChoice();
        switch(choice){
            case 1:
                // Login process
                userID = Login(connection);
                break;

            case 3:
                exit(EXIT_SUCCESS);
        }
    } while (!isChoiceValid);
    return userID;
}

/**
 * @brief Login Menu
 * 
 */
void ShowLoginMenu()
{
    cout << "-- LOGIN MENU --" << endl;
    cout << "1. Login;" << endl;
    cout << "2. Registration;" << endl;
    cout << "3. Exit." << endl;
}

/**
 * @brief Retrieve input from user and return a valid option.
 * 
 * @return int 
 */
int RetrieveLoginChoice()
{
    string rawInput;
    bool isValid = false;
        int choice;
    do
    {
        cout << "Enter your option: ";
        getline(cin, rawInput);
        try{
            choice = std::stoi(rawInput);
        }catch (std::invalid_argument exception){
            perror(exception.what());
        }
        if(choice >= 1 && choice <= 3){
            isValid = true;
        }else{
            isValid = false;
            cout << "Option not allowed." << endl;
        }
    }while (!isValid);
    return choice;
}

int Login(sql::Connection *connection){
    string name, surname, password;
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your surname: ";
    getline(cin, surname);
    cout << "Enter your password: ";
    getline(cin, password);
    sql::ResultSet *loginquery_res = DB_Login(connection, name, surname, password);
    if(loginquery_res -> next()){
        cout << "RES_ " << loginquery_res -> getInt("PersonID") << endl;
    }
    return 1;
}