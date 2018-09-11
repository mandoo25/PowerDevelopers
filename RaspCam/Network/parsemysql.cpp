#include "parsemysql.h"
#include "parserInfo.h"


using namespace std;


MYSQL *connection, mysql;
MYSQL_RES *result;
MYSQL_ROW row;
int query_state;

static int getWorkitemsFromDB(MYSQL *mysql, const unsigned int step);

int mysql_connect(void)
{
    cout << "connect sql server" <<endl;
    //initialize database connection
    mysql_init(&mysql);

    // the three zeros are: Which port to connect to, which socket to connect to
    // and what client flags to use.  unless you're changing the defaults you only need to put 0 here
    connection = mysql_real_connect(&mysql,HOST,USER,PASSWD,DB,DBPORT,nullptr,0);

    // Report error if failed to connect to database
    if (connection == NULL) {
        cout << mysql_error(&mysql) << endl;
        cout << "connection failure" <<endl;
        return 1;
    }
    else { cout << "Database connection successful" <<endl; }

#if 0
    char *sql_query_text = "SELECT * FROM process WHERE Type='3029C003AA' AND Process='F2'";

    //Send query to database
    query_state = mysql_query(connection, sql_query_text);
    if(query_state != 0)
    {
        cout << " mysql_query failure" <<endl;
        return -1;
    }


    // store result
    result = mysql_store_result(connection);
    if (result != NULL)
    {
        //Get the number of columns
        int num_rows = mysql_num_rows(result); //row
        int num_fields = mysql_num_fields(result); //column
        cout << "field num: "<< num_fields <<endl;

        MYSQL_ROW row;			//An array of strings
        while( (row = mysql_fetch_row(result)) )
        {
            static int idx =0;
            do{
                char *value_int = row[idx];
                char *value_string = row[idx];
                if(value_string == nullptr) break;

                printf( "Got value ::%d, %s\n", *value_int, value_string);
                idx++;
            }while(num_fields > idx);


        }
        mysql_free_result(result);
    }

#endif
    return 0;
}

void mysql_disconnect(void)
{
    mysql_close(&mysql);
    cout << "Disconnected from database."<<endl;
}

static int getWorkitemsFromDB(MYSQL *mysql, const unsigned int step)
{
    unsigned int ret = -1;

    char *sql_query_text = "SELECT ActionType FROM workitems WHERE ID=%d";
    char *query_string = (char *)malloc(strlen(sql_query_text)+5);
    sprintf(query_string, sql_query_text, step);
    cout <<"query string: <<"<<query_string <<">>"<<endl;

    int qstate = mysql_query(mysql, query_string);
    if(qstate != 0)
    {
        cout << " mysql_query failure" <<endl;
        return -1;
    }
    // store result
    MYSQL_RES *qresult = mysql_store_result(mysql);
    if (qresult != NULL)
    {
        //Get the number of columns
        int num_rows = mysql_num_rows(qresult); //row
        int num_fields = mysql_num_fields(qresult); //column
        //cout << "workitem num: "<< num_fields <<endl;

        MYSQL_ROW row;			//An array of strings
        if(row = mysql_fetch_row(qresult))
        {
            char *value_string = row[0];
            if(value_string != nullptr){
                ret = (unsigned int)atoi(value_string);
                printf( "got workitems :%d\n", ret);
            }
        }
        mysql_free_result(qresult);
    }

    free(query_string);
    return ret;
}

int getProcessSeqFromDB(const char *ordernum, const char *process)
{
    //connect to mysql server
    mysql_connect();

    char *sql_query_text = "SELECT * FROM process WHERE Type='%s' AND Process='%s'";
    char *query_string = (char *)malloc(strlen(sql_query_text)+20);
    sprintf(query_string, sql_query_text, ordernum, process);
    cout << "process Seq query: <<" << query_string << ">>" <<endl;

    //Send query to database
    query_state = mysql_query(connection, query_string);
    if(query_state != 0)
    {
        cout << " mysql_query failure" <<endl;
        return -1;
    }

    // store result
    result = mysql_store_result(connection);
    if (result != NULL)
    {
        //Get the number of columns
        int num_rows = mysql_num_rows(result); //row
        int num_fields = mysql_num_fields(result); //column
        //cout << "field num: "<< num_fields <<endl;

        MYSQL_ROW row;			//An array of strings
        while( (row = mysql_fetch_row(result)) )
        {
            static int idx =2;
            do{
                char *value_string = row[idx];
                if(value_string == nullptr) break;
                //printf( "process Seq value ::%s\n", value_string);

                unsigned int procnum = (unsigned int)atoi(value_string);
                getWorkitemsFromDB(connection, procnum);

                idx++;
            }while(num_fields > idx);


        }
        mysql_free_result(result);
    }

    free(query_string);

    mysql_disconnect();

    return 1;

}
