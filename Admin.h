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
};