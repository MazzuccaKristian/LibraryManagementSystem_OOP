#include <string>

#include "Person.h"

class Admin : private Person {
    private:
        int a_id;
        string a_name;
        string a_surname;
        string a_password;
        string a_role;

    public:
        Admin(int a_id, string a_name, string a_surname, string a_password, string role) : Person(a_id, a_name, a_surname, a_password)
        {
            setRole(role);
        };
        void setRole(string role){
            a_role = role;
        }
};