#pragma once
#include <string>

using std::string;

class Person{
    private:
        int p_id;
        string p_name;
        string p_surname;
        string p_password;

    public:
        Person(){};
        Person(int id, string name, string surname, string password){
            setId(id);
            setName(name);
            setSurname(surname);
            setPassword(password);
        }
        // Setters
        void setId(int id){
            p_id = id;
        }
        void setName(string name){
            p_name = name;
        }
        void setSurname(string surname){
            p_surname = surname;
        }
        void setPassword(string password){
            p_password = password;
        }
        // Getters
        int getId(){
            return p_id;
        }
};