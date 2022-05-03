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
                        cout << "Welcome back, " << admin.getName() << "!" << endl;
                        break;
                    }
                }
            }
        }      
    }while(isStillWorking);


    exit(EXIT_SUCCESS);
}