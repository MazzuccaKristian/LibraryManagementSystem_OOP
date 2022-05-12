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
const string DecrementCopiesQuery = "UPDATE Book SET Copies = Copies - 1 WHERE BookID = ?";
const string RentQuery = "INSERT INTO Rent(BookID, PersonID, RentDate, ReturnDate) VALUES (?, ?, CURDATE(), CURDATE() + 14)";
const string RentedBooks = "SELECT DISTINCT R.BookID, Title FROM Rent AS R JOIN Book AS B ON R.BookID = B.BookID WHERE PersonID = ?";
const string ReturnBook_DeleteRow = "DELETE FROM Rent WHERE PersonID = ? AND BookID = ?";
const string ReturnBook_InsertRow = "INSERT INTO Returning(PersonID, BookID, ReturnDate) VALUES (?, ?, CURDATE())";
const string RegistrationQuery_Admin = "INSERT INTO User(Name, Surname, Password, isAdmin, Role) VALUES (?, ?, ?, ?, ?)";
const string PostponeReturnDateQuery = "UPDATE Rent SET ReturnDate = ReturnDate + 7 WHERE PersonID = ? AND BookID = ?";

sql::Connection *HandlerSetup();
sql::ResultSet *DB_LoginUser(sql::Connection *connection, string *record);
sql::ResultSet *DB_LoginAdmin(sql::Connection *connection, string *record);
void DB_RegisterUser(sql::Connection *connection, string *record);
sql::ResultSet *DB_SearchBook(sql::Connection *connection, string book);
bool DB_CanUserRent(sql::Connection *connection, int userID);
void DB_RentBook(sql::Connection *connection, string title, int userID);
int DB_SearchBookForRent(sql::Connection *connection, string title);
void DB_DecrementCopies(sql::Connection *connection, int bookID);
void DB_Rent(sql::Connection *connection, int userID, int bookID);
int DB_CountRentedBooks(sql::Connection *connection, int userID);
int DB_CheckRentedBook(sql::Connection *connection, int userID, string title);
void DB_ReturnBook(sql::Connection *connection, int userID, int bookID);
bool DB_RegisterAdmin(sql::Connection *connection, string *adminData);
void DB_PostponeReturnDate(sql::Connection *connection, int userID, string title);

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
        std::perror(exception.what());
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
        // Search book -> decrement copies -> insert row for Rent
        int bookID = DB_SearchBookForRent(connection, title);
        if(bookID > 0){
            DB_DecrementCopies(connection, bookID);
            DB_Rent(connection, userID, bookID);
        }
    }
}

int DB_SearchBookForRent(sql::Connection *connection, string title){
    int bookID;
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *searchResult;
        try{
            p_stmt = connection -> prepareStatement(SearchBookQuery);
            p_stmt -> setString(1, title);
            searchResult = p_stmt -> executeQuery();
            if(searchResult -> next()){
                bookID = searchResult -> getInt("BookID");
            }else{
                bookID = 0;
            }
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
        delete searchResult;
    }
    return bookID;
}

void DB_DecrementCopies(sql::Connection *connection, int bookID){
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        try{
            p_stmt = connection -> prepareStatement(DecrementCopiesQuery);
            p_stmt -> setInt(1, bookID);
            p_stmt -> execute();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
    }
}

void DB_Rent( sql::Connection *connection, int userID, int bookID){
    if(connection -> isValid()){
        string todayDate, returnDate;
        sql::PreparedStatement *p_stmt;
        try{
            p_stmt = connection -> prepareStatement(RentQuery);
            p_stmt -> setInt(1, bookID);
            p_stmt -> setInt(2, userID);
            p_stmt -> execute();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
    }
}

int DB_CountRentedBooks(sql::Connection *connection, int userID){
    int counter = 0;
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *countResult;
        try{
            p_stmt = connection -> prepareStatement(CountRentedBooks);
            p_stmt -> setInt(1, userID);
            countResult = p_stmt -> executeQuery();
            if(countResult -> next()){
                counter = countResult -> getInt("Counter");
            }
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
        delete countResult;
    }
    return counter;
}

int DB_CheckRentedBook(sql::Connection *connection, int userID, string title){
    int bookID = 0;
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        sql::ResultSet *queryResult;
        try{
            p_stmt = connection -> prepareStatement(RentedBooks);
            p_stmt -> setInt(1, userID);
            queryResult = p_stmt -> executeQuery();
            while(queryResult -> next()){
                string retrievedTitle = queryResult -> getString("Title");
                if(retrievedTitle.compare(title) == 0){
                    bookID = queryResult -> getInt("BookID");
                }
            }
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        delete p_stmt;
        delete queryResult;
    }
    return bookID;
}

void DB_ReturnBook(sql::Connection *connection, int userID, int bookID){
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        try{
            p_stmt = connection -> prepareStatement(ReturnBook_DeleteRow);
            p_stmt -> setInt(1, userID);
            p_stmt -> setInt(2, bookID);
            p_stmt -> execute();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        try{
            p_stmt = connection -> prepareStatement(ReturnBook_InsertRow);
            p_stmt -> setInt(1, userID);
            p_stmt -> setInt(2, bookID);
            p_stmt -> execute();
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
}

bool DB_RegisterAdmin(sql::Connection *connection, string *adminData){
    bool registrationSuccess = false;
    if(connection -> isValid()){
        sql::PreparedStatement *p_stmt;
        try{
            p_stmt = connection -> prepareStatement(RegistrationQuery_Admin);
            p_stmt -> setString(1, adminData[0]);
            p_stmt -> setString(2, adminData[1]);
            p_stmt -> setString(3, adminData[2]);
            p_stmt -> setInt(4, 1);
            p_stmt -> setString(5, adminData[3]);
            
            if(p_stmt -> execute()){
                registrationSuccess = true;
            }
        }catch(sql::SQLException *exception){
            std::perror(exception->what());
        }
        delete p_stmt;
    }
    return registrationSuccess;
}

void DB_PostponeReturnDate(sql::Connection *connection, int userID, string title){
    if(connection -> isValid()){
        int bookID;
        try{
            sql::PreparedStatement *pstmt_getBookID;
            pstmt_getBookID = connection -> prepareStatement(SearchBookQuery);
            pstmt_getBookID -> setString(1, title);
            sql::ResultSet *result = pstmt_getBookID -> executeQuery();
            if(result -> next()){
                bookID = result -> getInt("BookID");
            }
            delete result;
            delete pstmt_getBookID;
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
        try{
            sql::PreparedStatement *p_stmt;
            p_stmt = connection -> prepareStatement(PostponeReturnDateQuery);
            p_stmt -> setInt(1, userID);
            p_stmt -> setInt(2, bookID);
            p_stmt -> execute();
            delete p_stmt;
        }catch(sql::SQLException *exception){
            std::perror(exception -> what());
        }
    }
}
