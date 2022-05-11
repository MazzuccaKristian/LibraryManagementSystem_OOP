#include "Main.h"

#include <iostream>

int main(){
    bool isStillWorking = true;
    bool isUserLoggedin = false;
    bool isAdminLoggedin = false;

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
        LoginMenu();
        int loginChoice = LoginMenu_Choice();
        switch(loginChoice){
            case 1:
            {
                // Login
                do{
                    int loginType = SelectType();
                    switch(loginType){
                        case 1:
                        {
                            // Login -> User
                            string userData[3];
                            CollectUserData(userData);
                            user = LoginUser(con, userData);
                            isUserLoggedin = true;
                            cout << "Welcome back, " << user.getName()  << "!" << endl;
                            break;
                        }

                        case 2:
                        {
                            // Login -> Admin
                            string adminData[3];
                            CollectUserData(adminData);
                            admin = LoginAdmin(con, adminData);
                            isAdminLoggedin = true;
                            cout << "Welcome back, " << admin.getName() << "!" << endl;
                            break;
                        }
                    }
                }while(!isUserLoggedin && !isAdminLoggedin);
                break;
            }

            case 2:
            {
                // Registration (standard user only)
                bool isPasswordValid = false;
                do{
                    string userData[3];
                    CollectUserData(userData);
                    cout << "Confirm your password: ";
                    string passwordConfirmation;
                    getline(cin, passwordConfirmation);
                    if(passwordConfirmation.compare(userData[2]) == 0){
                        RegisterUser(con, userData);
                    }
                }while(!isPasswordValid);
            }

            case 3:
            {
                cout << "Program ended. Please, wait..." << endl;
                delete con;
                isStillWorking = false;
            }
        }

        if(isUserLoggedin){
        // USER application
            UserMenu();
            int userChoice = UserMenu_Choice();
            switch(userChoice){
                case 1:
                    user.SearchBook(con);
                    break;

                case 2:
                    user.RentBook(con);
                    break;

                case 3:
                    user.ReturnBook(con);
                    break;
            }
        }else if(isAdminLoggedin){
        // ADMIN application
            AdminMenu();
            int adminChoice = AdminMenu_Choice();
            switch(adminChoice){
                case 1:
                    admin.AddNewAdmin(con);
            }
        }
             
    }while(isStillWorking);

    exit(EXIT_SUCCESS);
}