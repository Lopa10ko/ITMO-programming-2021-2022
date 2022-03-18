#include <iostream>
#include "sqlite3.h"
#define CURL_STATICLIB
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <curl/curl.h>
#include <string>

static size_t WriteCallback(void *contents, size_t size, size_t npos, void *userp) {
    ((std::string *) userp) -> append((char *) contents, size * npos);
    return size * npos;
}

//struct connection_details {
//    const char *host, *user, *passwd, *db, *unix_socket;
//    unsigned long port, flags;
//};
//MYSQL *mysql_connection_setup(struct connection_details info) {
//    MYSQL *connection = mysql_init(NULL);
//    if (!mysql_real_connect(connection, info.host, info.user, info.passwd, info.db, info.port = 0, info.unix_socket = NULL, info.flags = 0)) {
//        std::cout << "Not connected with MySQL! " << mysql_error(connection) << std::endl;
//        exit(1);
//    }
//    return connection;
//}
//MYSQL_RES *mysql_perform_query(MYSQL *connection, const char *sql_query) {
//    if (mysql_query(connection, sql_query)) {throw std::invalid_argument(mysql_error(connection));}
//    return mysql_use_result(connection);
//}

int main() {
//    MYSQL *con;	// the connection
//    MYSQL_RES *res;	// the results
//    MYSQL_ROW row;	// the results rows (array)
//
//    struct connection_details mysqlD;
//    mysqlD.host = "localhost";  // where the mysql database is
//    mysqlD.user = "username"; // user
//    mysqlD.passwd = "password"; // the password for the database
//    mysqlD.db = "mydatabase";	// the databse
//
//    // connect to the mysql database
//    con = mysql_connection_setup(mysqlD);
//
//    // get the results from executing commands
//    res = mysql_perform_query(con, "select * from tblUsers;");
//
//    std::cout << ("Database Output:\n") << std::endl;

//    while ((row = mysql_fetch_row(res)) != NULL){
//        // the below row[] parametes may change depending on the size of the table and your objective
//        std::cout << row[0] << " | " << row[1] << " | " << row[2] << " | " << row[3] << " | " << row[4] << std::endl << std::endl;
//    }
//
//    // clean up the database result
//    mysql_free_result(res);
//
//    // close database connection
//    mysql_close(con);
//    std::cout<<"hrr"<<std::endl;

    CURL *curl = curl_easy_init();
    CURLcode curl_status;
    std::string not_parsed_str;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.cbr-xml-daily.ru/daily_jsonp.js");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &not_parsed_str);
        curl_status = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        not_parsed_str.erase(0, 90);
        not_parsed_str.erase(not_parsed_str.length() - 2, 2);
        //std::cout << not_parsed_str << std::endl;
        rapidjson::Document doc_json;
        doc_json.Parse(not_parsed_str.c_str());
        assert(doc_json.IsObject());
        rapidjson::Value& date_value = doc_json["Date"];
        std::string date = date_value.GetString();
        rapidjson::Value& valute_value = doc_json["Valute"];
        rapidjson::StringBuffer buffer_valute;
        rapidjson::Writer<rapidjson::StringBuffer> writer_valute(buffer_valute);
        valute_value.Accept(writer_valute);
        //std::cout << buffer_valute.GetString() << std::endl;
        rapidjson::Document doc_valute;
        doc_valute.Parse(buffer_valute.GetString());
        for (rapidjson::Value::ConstMemberIterator itr = doc_valute.MemberBegin(); itr != doc_valute.MemberEnd(); ++itr) {
            std::cout << itr->name.GetString() << " -- ";
            const rapidjson::Value& current_value = itr->value;
            std::cout << current_value["Value"].GetDouble() << std::endl;
        }
        std::cout << date << std::endl;

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc_json.Accept(writer);
        //output with changes
        //std::cout << buffer.GetString() << std::endl;
    }
    return 0;
}