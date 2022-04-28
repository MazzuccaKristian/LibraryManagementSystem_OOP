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
const string LoginQuery = "SELECT * FROM User WHERE Name = ? AND Surname = ? AND Password = ?";

sql::Connection *HandlerSetup();
sql::ResultSet *DB_Login(sql::Connection *con, string name, string surname, string password);

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

sql::ResultSet *DB_Login(sql::Connection *con, string name, string surname, string password){
    sql::ResultSet *result;
    if(con -> isValid()){
        sql::PreparedStatement *p_stmt;
        try{
            p_stmt = con -> prepareStatement(LoginQuery);
            p_stmt -> setString(1, name);
            p_stmt -> setString(2, surname);
            p_stmt -> setString(3, password);
            result = p_stmt -> executeQuery();
        }catch(sql::SQLException e){
            std::perror(e.what());
        }
        delete p_stmt;
    }
    return result;
}