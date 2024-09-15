#include <stdio.h>   
#include <sql.h>
   #include <sqlext.h>
 
   int main() {
       SQLHENV hEnv = NULL;
       SQLHDBC hDbc = NULL;
       SQLRETURN ret;
 
       // Allocate an environment handle
       SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
       SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
 
       // Allocate a connection handle
       SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
 
       // Connect to the data source
       ret = SQLConnect(hDbc, (SQLCHAR*)"myodbc", SQL_NTS, (SQLCHAR*)"root", SQL_NTS, (SQLCHAR*)"123456", SQL_NTS);
 
       if (SQL_SUCCEEDED(ret)) {
           printf("Connected to the database successfully.\n");
           // Perform database operations here
       } else {
           printf("Failed to connect to the database.\n");
       }
 
       // Clean up
       SQLDisconnect(hDbc);
       SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
       SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
 
       return 0;
   }
