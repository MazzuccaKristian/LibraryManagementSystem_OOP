# Library Management System
  ![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![MySQL](https://img.shields.io/badge/mysql-%2300f.svg?style=for-the-badge&logo=mysql&logoColor=white)
  ![Github Version](https://img.shields.io/badge/version-0.1.1-yellow) ![Generic badge](https://img.shields.io/badge/status-first_prototype-yellow)
  
 ## Contents
 - **[Overview](#overview)**
 - **[Tech](#tech)**
 - **[Configuration](#configuration)**
 - **[Installation](#installation)**
 - **[License](#license)**
  
## Overview
"University Management System" is a practice-purpose project for OOP with C++ and database's design and usage. The project requires a relational database to store operations' output and to handle the login phase (local MySQL server, at the moment).

## Tech
| Tech | Documentation |
| ------ | ------ |
| C++ | [Documentation](https://en.cppreference.com/) |
| MySQL | [Documentation](https://dev.mysql.com/doc/) |
| MySQL Connector | [Documentation](https://dev.mysql.com/doc/connector-cpp/8.0/en/) |

## Configuration
1. Clone this repo.
2. Start your server: `sudo service mysql start`
3. Log in with a root account (or equivalent): `mysql -u root -p`
4. Create the DB: `CREATE DATABASE IF NOT EXISTS LibraryDB;`
5. Create tables:
   ```sql
   --- Table 'User'
   CREATE TABLE IF NOT EXISTS User (
        PersonID INT AUTO_INCREMENT PRIMARY KEY,
        Name VARCHAR(50) NOT NULL,
        Surname VARCHAR(50) NOT NULL,
        isAdmin TINYINT(1) DEFAULT 0,
        Role VARCHAR(30) NOT NULL );
    --- Table 'Book'
    CREATE TABLE IF NOT EXISTS Book (
        BookID INT AUTO_INCREMENT PRIMARY KEY,
        Title VARCHAR(30) NOT NULL,
        Author VARCHAR(30) NOT NULL,
        PublishingYear INT NOT NULL,
        Edition INT DEFAULT 1,
        Editor VARCHAR(30) NOT NULL );
   ```
6. Create the user:
    ```sql
    CREATE USER 'libraryuser'@'localhost' IDENTIFIED WITH mysql_native_password BY 'Password1!';
    ```
7. Grant privileges:
    ```sql
    GRANT PRIVILEGES SELECT ON LibraryDB.* TO 'libraryuser'@'localhost'; 
    ```

## License

GNU GPL 3.0
