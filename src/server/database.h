/********************************************************************************
 * Files         : database.h
 * Description   : the head file 
 * Author        : zpy
 * Last Modified : 2018.9.4
 ********************************************************************************/

/********************************************************************************
 * Description : Declaration of all global variables 
 * Author      : zpy
 * Date        : 2018.9.4
 ********************************************************************************/

char server[] = "0";       //server ID (you can search it in mysql)
char user[] = "root";      //USER name (nevermind)
char password[] = "123";//password (sunjinda's birthday)(not anymore)
char database[] = "realRosetta"; //database name (this is NOT a default database)

/********************************************************************************
Description : Declarations of all functions 
Author      : zpy
Date        : 2018.9.4
********************************************************************************/
int check_signup(const char* line);
int check_login(const char* line);
int if_user_exist(const char* line);
void insert_friend_into_database(const char* name1,const char* name2);
