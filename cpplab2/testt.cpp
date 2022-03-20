#include <iostream>
#include <sqlite3.h>
#define CURL_STATICLIB
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <curl/curl.h>
#include <string>
#define ERRCHECK {if (err!=NULL) {printf("%s\n",err); sqlite3_free(err);  return 0;}}

static size_t curl_callback(void *contents, size_t size, size_t npos, void *userp) {
    ((std::string *) userp) -> append((char *) contents, size * npos);
    return size * npos;
}
int callback(void *NotUsed, int argc, char **argv, char **azColName){
    // int argc: holds the number of results
    // (array) azColName: holds each column returned
    // (array) argv: holds each value
    for(int i = 0; i < argc; i++) {std::cout << azColName[i] << ": " << argv[i] << std::endl;}
    std::cout << std::endl;
    return 0;
}
int main() {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    std::string sql_script;
    char *error_msg = 0;
    if (sqlite3_open("C:/Users/Georgy/Desktop/CLABS/cpp_lab/cpp_lab2/currency_db.db", &db)) {throw std::invalid_argument("db_error: database not connected");}
    CURL *curl = curl_easy_init();
    CURLcode curl_status;
    std::string not_parsed_str;
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.cbr-xml-daily.ru/daily_jsonp.js");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &not_parsed_str);
        //performing process
        curl_status = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        not_parsed_str.erase(0, 90);
        not_parsed_str.erase(not_parsed_str.length() - 2, 2);
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
        std::string name_buffer;
        double value_buffer;
        //insertion
        for (rapidjson::Value::ConstMemberIterator itr = doc_valute.MemberBegin(); itr != doc_valute.MemberEnd(); ++itr) {
            name_buffer = itr->name.GetString();
            const rapidjson::Value& current_value = itr->value;
            value_buffer = current_value["Value"].GetDouble();
        }

//        sql_script = "INSERT INTO 'currency_table' (id, curr_name, median, mean_span, value_1) VALUES (1, 'eWtttttR', 32, 23, 110);";
//        int rc = sqlite3_exec(db, sql_script.c_str(), callback, NULL, reinterpret_cast<char **>(error_msg));
        sql_script = "SELECT * FROM 'currency_table';";
        if(int select = sqlite3_exec(db, sql_script.c_str(), callback, 0, &error_msg)) {throw std::invalid_argument("db_error: data not selected");}
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        doc_json.Accept(writer);
        sqlite3_close(db);
        //output with changes
        //std::cout << buffer.GetString() << std::endl;
        /////////////////////////////////////////////////////////
        /// to buffer_file to sqlite3
    }
    return 0;
}