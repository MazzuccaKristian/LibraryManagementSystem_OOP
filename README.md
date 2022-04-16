# Library Management System
  ![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white) ![MySQL](https://img.shields.io/badge/mysql-%2300f.svg?style=for-the-badge&logo=mysql&logoColor=white)
  ![Github Version](https://img.shields.io/badge/version-0.0.1-yellow) ![Generic badge](https://img.shields.io/badge/status-first_prototype-yellow)
  
 ## Contents
 - **[Overview](#overview)**
 - **[Tech](#tech)**
 - **[Configuration](#configuration)**
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
-- Table 'Book':
CREATE TABLE IF NOT EXISTS Book(
  BookID INT AUTO_INCREMENT PRIMARY KEY,
  Title VARCHAR(50) NOT NULL,
  Edition INT NOT NULL DEFAULT 1,
  Author VARCHAR(30) NOT NULL,
  PublishingYear YEAR NOT NULL,
  Copies INT NOT NULL DEFAULT 0
);
-- Table 'Person' (will be used for users and admins):
CREATE TABLE IF NOT EXISTS Person(
  PersonID INT AUTO_INCREMENT PRIMARY KEY,
	Name VARCHAR(25) NOT NULL,
	Surname VARCHAR(25) NOT NULL,
	DateOfBirth DATE NOT NULL,
	IsAdmin TINYINT(1) NOT NULL DEFAULT 0,
	Role VARCHAR(30)
);
```
6. Create the user: ...
7. Grant privileges: ...

## License

