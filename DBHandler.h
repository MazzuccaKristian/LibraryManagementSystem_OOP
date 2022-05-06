#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <string>

using std::string;

const string DB_SelectedSchema = "LibraryDB";
const string DB_HostCompleted = "tcp://127.0.0.1:3306";
const string DB_User = "libraryuser";
const string DB_Password = "Password1!";

const string UseSchemaQuery = "USE LibraryDB";
const string LoginQuery_User = "SELECT PersonID FROM User WHERE Name = ? AND Surname = ? AND Password = ? AND isAdmin = 0";
const string LoginQuery_Admin = "SELECT PersonID, Role FROM User WHERE Name = ? AND Surname = ? AND Password = ? AND isAdmin = 1";
const string RegistrationQuery_User = "INSERT INTO User(Name, Surname, Password) VALUES (?, ?, ?)";
const string SearchBookQuery = "SELECT * FROM Book WHERE Title = ?";
const string CountRentedBooks = "SELECT COUNT(DISTINCT BookID) AS Counter FROM Rent WHERE PersonID = ? AND ReturnDate > CURDATE()";

sql::Connection *HandlerSetup();
sql::ResultSet *DB_LoginUser(sql::Connection *connection, string *record);
sql::ResultSet *DB_LoginAdmin(sql::Connection *connection, string *record);
void DB_RegisterUser(sql::Connection *connection, string *record);
sql::ResultSet *DB_SearchBook(sql::Connection *connection, string book);
bool DB_CanUserRent(sql::Connection *connection, int userID);
void DB_RentBook(sql::Connection *connection, string title, int userID);

sql::Connection *HandlerSetup(){
    sql::Driver *driver;
    sql::Connection *connection;
    sql::Statement *stmt;
    try{
        driver = get_driver_instance();
        connection = driver -> connect(DB_HostCompleted, DB_User, DB_Password);
        stmt = connection -> createStatement();
        stmt -> execute(UseSchemaQuery);

    }catch(sql::SQLException exception){
        perror(exception.what());
    }
    delete stmt;
    return connection;
}

sql::ResultSet *DB_LoginUser(sql::Connection *connection, string *record){
    sql::PreparedStatement *p_stmt;
    sql::ResultSet *result;
    if (connection->isValid())
    {
        p_stmt = connection -> prepareStatement(LoginQuery_User);
        try{
            p_stmt -> setString(1, record[0]);
            p_stmt -> setString(2, record[1]);
            p_stmt -> setString(3, record[2]);
            result = p_stmt ->executeQuery();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
    delete p_stmt;
    return result;
}

sql::ResultSet *DB_LoginAdmin(sql::Connection *connection, string *record){
    sql::PreparedStatement *p_stmt;
    sql::ResultSet *result;
    if(connection -> isValid()){
        p_stmt = connection -> prepareStatement(LoginQuery_Admin);
        try{
            p_stmt -> setString(1, record[0]);
            p_stmt -> setString(2, record[1]);
            p_stmt -> setString(3, record[2]);
            result = p_stmt -> executeQuery();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
    delete p_stmt;
    return result;
}

void DB_RegisterUser(sql::Connection *connection, string *record){
    sql::PreparedStatement *p_stmt;
    if(connection -> isValid()){
        p_stmt = connection -> prepareStatement(RegistrationQuery_User);
        try{
            p_stmt -> setString(1, record[0]);
            p_stmt -> setString(2, record[1]);
            p_stmt -> setString(3, record[2]);
            p_stmt -> execute();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
    delete p_stmt;
}

sql::ResultSet *DB_SearchBook(sql::Connection *connection, string book){
    sql::PreparedStatement *p_stmt;
    sql::ResultSet *searchResult;
    if(connection -> isValid()){
        try{
            p_stmt = connection -> prepareStatement(SearchBookQuery);
            p_stmt -> setString(1, book);
            searchResult = p_stmt -> executeQuery();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
    delete p_stmt;
    return searchResult;
}

bool DB_CanUserRent(sql::Connection *connection, int userID){
    bool canUserRent = false;
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *queryResult;
        try{
            p_stmt = connection -> prepareStatement(CountRentedBooks);
            p_stmt -> setInt(1, userID);
            queryResult = p_stmt -> executeQuery();
            if(queryResult -> next()){
                int counter = queryResult -> getInt("Counter");
                if(counter < 5){
                    canUserRent = true;
                }
            }
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
        delete queryResult;
    }
    return canUserRent;
}

void DB_RentBook(sql::Connection *connection, string title, int userID){
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        // Search book -> decrement copies -> insert row for Rent
        
    }
}