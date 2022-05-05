#include <string>
#include <iostream>

#include "Person.h"

using std::cout;
using std::endl;
using std::cin;

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

    // private:
    //     string FormatTitleString(string rawTitle);

    public:
        void SearchBook(sql::Connection *connection);
};

void User::SearchBook(sql::Connection *connection){
    cout << "Enter the title: ";
    string raw, title;
    cin.ignore();
    getline(cin, raw);
    title = FormatTitleString(raw);
    sql::ResultSet *searchResult = DB_SearchBook(connection, title);
    if(searchResult -> rowsCount() > 0){
        while(searchResult -> next()){
            int copies = searchResult -> getInt("Copies");
            int edition = searchResult -> getInt("Edition");
            if(copies >= 1){
                if(copies == 1){
                    cout << raw << ", edition " << edition << " is available! One copy left." << endl;
                }else{
                    cout << raw << ", edition " << edition << " is available! " << copies << " copies left." << endl;
                }
            }
        }
    }else{
        cout << "Sorry, " << raw << " is not available..." << endl;
    }
}