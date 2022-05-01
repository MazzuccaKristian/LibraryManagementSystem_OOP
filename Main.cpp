#include "Main.h"

#include <iostream>

using namespace std;


int main(int argc, char **argv){
    bool isStillWorking = true;
    bool isUserLoggedin = false;

    Admin admin;
    User user;

    cout << "Connecting...";
    sql::Connection *con = HandlerSetup();
    if(con->isValid()){
        cout << "OK" << endl;
    }else{
        cout << "ERROR" << endl;
    }

    do{
        if(!isUserLoggedin){
            // Login phase
            LoginMenu();
            int userChoice = RetrieveLoginChoice();
            switch(userChoice){
                case 1:
                {
                    // User/Admin login
                    sql::ResultSet *loginResult = Login(con);
                    if(loginResult -> next()){
                        int id = loginResult -> getInt("PersonID");
                        bool isAdmin = loginResult -> getInt("isAdmin");
                        string name = loginResult -> getString("Name");
                        string surname = loginResult -> getString("Surname");
                        string password = loginResult -> getString("Password");
                        string role = loginResult -> getString("Role");
                        switch(isAdmin){
                            case 0:
                            // Standard user
                            {
                                user = User(id, name, surname, password);
                                cout << "Welcome back!" << endl;
                                isUserLoggedin = true;
                                break;
                            }

                            case 1:
                            // Admin
                            {
                                admin = Admin(id, name, surname, password, role);
                                cout << "Welcome back!" << endl;
                                isUserLoggedin = true;
                                break;
                            }
                        }
                    }
                    break;
                }

                case 2:
                // Registration
                {
                    
                }

                case 3:
                {
                    cout << "Program ended. Please, wait..." << endl;
                    isStillWorking = false;
                    break;
                }
            }
        }
    }while(isStillWorking);


    exit(EXIT_SUCCESS);
}