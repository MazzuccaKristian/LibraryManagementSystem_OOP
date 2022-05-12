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

    public:
        bool SearchBook(sql::Connection *connection);
        void RentBook(sql::Connection *connection);
        string TitleInput();
        void ReturnBook(sql::Connection *connection);
        void PostponeReturnDate(sql::Connection *connection);
};

bool User::SearchBook(sql::Connection *connection){
    bool found = false;
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
                found = true;
            }
        }
    }else{
        cout << "Sorry, " << raw << " is not available..." << endl;
    }
    return found;
}

void User::RentBook(sql::Connection *connection){
    // Check if user can rent a book (MAX = 5)
    bool canRent = DB_CanUserRent(connection, this -> getId());
    if(canRent){
        cin.ignore();
        cout << "Enter the title: ";
        string rawTitle;
        getline(cin, rawTitle);
        string title = FormatTitleString(rawTitle);
        DB_RentBook(connection, title, this -> getId());
        cout << "Thank you!" << endl;

    }else{
        cout << "You cannot rent more then five books." << endl;
    }
}

void User::ReturnBook(sql::Connection *connection){
    if(connection -> isValid()){
        if(DB_CountRentedBooks(connection, this -> getId()) < 1){
            cout << "You don't need to return..." << endl;
        }else{
            // book's title -> remove from Rent -> insert into Returning
            cout << "Enter title: ";
            string title, raw;
            cin.ignore();
            getline(cin, raw);
            title = FormatTitleString(raw);
            int rentedBook_ID = DB_CheckRentedBook(connection, this -> getId(), title);
            if(rentedBook_ID > 0){
                // User rented the specified book.
                DB_ReturnBook(connection, this -> getId(), rentedBook_ID);
            }else{
                cout << "You didn't rent " << title << "..." << endl;
            }
        }
    }
}

void User::PostponeReturnDate(sql::Connection *connection){
    string raw, title;
    std::cout << "Enter title: ";
    getline(std::cin, raw);
    title = FormatTitleString(raw);
    DB_PostponeReturnDate(connection, this -> getId(), title);
}