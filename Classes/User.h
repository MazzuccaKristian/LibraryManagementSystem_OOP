#include <string>

#include "Person.h"

class User : public Person {
    private:
        int u_id;
        string u_name;
        string u_surname;
        string u_password;

    public:
        User(){};
        User(int id, string name, string surname, string password)
            : Person{ id, name, surname, password }
            {};
};