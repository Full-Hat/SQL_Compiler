# SQL Compiler

SQL Compiler is a C++ database library.

# Features

- Supports several SQL languages as SQLite.
- Provides a simple and intuitive interface for querying databases and retrieving results.
- Supports prepared statements for secure and efficient database queries.
- Perform SQL queries compilation which make it faster.
- Lightweight and easy to integrate into existing projects.
# Getting started

Prerequisites
- C++ compiler with C++20 support
- CMake 3.3 or higher
# Building
Clone the MyDatabaseLibrary repository:
``` bash
git clone https://github.com/Full-Hat/SQL_Compiler.git
```
Run init script:

**Windows**:
```
bash init.bat
```
**UNIX** (MacOS & Linux):
```
bash init.sh
```
Create a build directory and navigate into it:
```
mkdir build && cd build
```
Run CMake to generate the build files:
```
cmake ../
```
Build the library:
```
make
```
# Usage
To use SQL Compiler in your project, simply link against the library and include the appropriate headers. Here's an example of how to connect to a MySQL database and execute a simple query:

For more information on using MyDatabaseLibrary, please refer to the documentation.

# Contributing

Contributions to SQL Compiler are always welcome! To contribute, simply fork the repository, make your changes, and submit a pull request.

# License

SQL Compiler is licensed under the MIT License. See LICENSE for details.