#include <string>

#include "Person.h"

class Admin : public Person {
    private:
        int a_id;
        string a_name;
        string a_surname;
        string a_password;
        string a_role;

    public:
        Admin(){};
        Admin(int id, string name, string surname, string password, string role)
            : Person{ id, name, surname, password }
            {
                setRole(role);
            };
        void setRole(string role){
            a_role = role;
        }
        // Getters
        string getRole(){
            return a_role;
        }
        void AddNewAdmin(sql::Connection *connection);
};

void Admin::AddNewAdmin(sql::Connection *connection){
    string adminRecord[4];
    std::cin.ignore();
    std::cout << "Enter name: ";
    getline(std::cin, adminRecord[0]);
    std::cout << "Enter surname: ";
    getline(std::cin, adminRecord[1]);
    std::cout << "Enter password: ";
    getline(std::cin, adminRecord[2]);
    std::cout << "Enter role: ";
    getline(std::cin, adminRecord[3]);
    DB_RegisterAdmin(connection, adminRecord);
    std::cout << "Operation completed!" << std::endl; 
}