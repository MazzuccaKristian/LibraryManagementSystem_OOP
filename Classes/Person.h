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
        string getName(){
            return p_name;
        }
        string getSurname(){
            return p_name;
        }
        string getPassword(){
            return p_password;
        }

        string FormatTitleString(string rawTitle);

};

string Person::FormatTitleString(string rawTitle){
    string title = rawTitle;
    int cursor = title.find_first_of(' ');
    int end = title.size();
    while(cursor < end && cursor != title.npos){
        title = title.replace(cursor, 1, "_");
        cursor = title.find_first_of(' ');
    }
    return title;
}