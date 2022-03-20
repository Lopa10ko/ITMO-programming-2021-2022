#include <iostream>
#include <sqlite3.h>
#define CURL_STATICLIB
#define MAX_RAP 10
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include <winsock2.h>
#include <windows.h>
#include "rapidjson/stringbuffer.h"
#include <curl/curl.h>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

static size_t curl_callback(void *contents, size_t size, size_t npos, void *json_data) {
    ((std::string *) json_data) -> append((char *) contents, size * npos);
    return size * npos;
}
double get_median(std::vector<double> temp_values) {
    if (temp_values.empty()) {return std::numeric_limits<double>::signaling_NaN();}
    const auto first = temp_values.begin(), last = temp_values.end();
    const auto ptr1 = first + (temp_values.size() - 1) / 2, ptr2 = first + temp_values.size() / 2;
    std::nth_element(first, ptr1, last);
    std::nth_element(ptr1, ptr2, last);
    return 0.5 * (*ptr1 + *ptr2);
}
// (upd, quantity, array of values, typename)
int select_callback(void *upd, int argc, char **argv, char **column) {
    long double summation;
    std::vector<double> values;
    int quantity = 0;
    for(int i = 2; i < argc; i++) {
        std::string s(argv[i]);
        double k = std::stod(s);
        summation += k;
        values.push_back(k);
        quantity++;
    }
    //for (auto i: values) {std::cout << i<< std::endl;}
    std::cout << argv[1] << ": avg("<< summation/quantity << ")"<< " med("<< get_median(values) << ")"<< std::endl;
    return 0;
}
int insert_callback(void *upd, int argc, char **argv, char **column) {
    for(int i = 0; i < argc; i++) {std::cout << column[i] << ": " << argv[i] << std::endl;}
    return 0;
}
std::string create_sql_insert(double value_buffer, std::string name_buffer, int counter) {
    return "INSERT INTO 'currency_table' (id, curr_name, value_1) VALUES (" +
            std::to_string(counter) + ", '" + name_buffer + "', " + std::to_string(value_buffer) + ");";
}
std::string create_sql_update(double value_buffer, std::string name_buffer, int counter, int iteration) {
    return "UPDATE 'currency_table' set value_" + std::to_string(iteration)+ " = " + std::to_string(value_buffer) +
            " where id=" + std::to_string(counter) + ";";
}
int main(int argc, char **argv) {
    for (auto iteration = 1; iteration <= MAX_RAP; iteration++) {
        sqlite3 *db;
        sqlite3_stmt *stmt;
        std::string sql_script;
        char *error_msg = 0;
        if (sqlite3_open("C:/Users/Georgy/Desktop/CLABS/cpp_lab/cpp_lab2/currency_db.db", &db)) {throw std::invalid_argument("db_error: database not connected");}
        CURL *curl = curl_easy_init();
        CURLcode curl_status;
        std::string not_parsed_str;
        if (curl) {
            //performing process
            curl_easy_setopt(curl, CURLOPT_URL, "http://www.cbr-xml-daily.ru/daily_jsonp.js");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &not_parsed_str);
            curl_status = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
            //reformat string_data
            not_parsed_str.erase(0, 90);
            not_parsed_str.erase(not_parsed_str.length() - 2, 2);
            //parse json string into subjson
            rapidjson::Document doc_json;
            doc_json.Parse(not_parsed_str.c_str());
            assert(doc_json.IsObject());
            rapidjson::Value &date_value = doc_json["Date"];
            std::string date = date_value.GetString();
            rapidjson::Value &valute_value = doc_json["Valute"];
            rapidjson::StringBuffer buffer_valute;
            rapidjson::Writer<rapidjson::StringBuffer> writer_valute(buffer_valute);
            valute_value.Accept(writer_valute);
            //std::cout << buffer_valute.GetString() << std::endl;
            rapidjson::Document doc_valute;
            doc_valute.Parse(buffer_valute.GetString());
            std::string name_buffer;
            double value_buffer;
            int cnt = 1;
            //insertion with parsed <string-double> pairs
            for (rapidjson::Value::ConstMemberIterator itr = doc_valute.MemberBegin();
                 itr != doc_valute.MemberEnd(); ++itr) {

                name_buffer = itr->name.GetString();
                const rapidjson::Value &current_value = itr->value;
                value_buffer = current_value["Value"].GetDouble();
                sql_script = create_sql_insert(value_buffer, name_buffer, cnt);
                try {
                    if (int insert = sqlite3_exec(db, sql_script.c_str(), insert_callback, 0,
                                                  reinterpret_cast<char **>(error_msg))) {
                        throw std::invalid_argument("db_error: data not inserted");
                    }
                }
                catch (...) {
                    //iteration [2..MAX_RAP]
                    if (cnt == 1) {
                        sql_script = "ALTER TABLE currency_table ADD COLUMN value_" + std::to_string(iteration) +" double; ";
                        if (int add = sqlite3_exec(db, sql_script.c_str(), 0, 0,
                                                   reinterpret_cast<char **>(error_msg))) {
                            throw std::invalid_argument("db_error: data not updated (column addition)");
                        }
                    }
                    sql_script = create_sql_update(value_buffer, name_buffer, cnt, iteration);
                    if (int update = sqlite3_exec(db, sql_script.c_str(), 0, 0,
                                                  reinterpret_cast<char **>(error_msg))) {
                        throw std::invalid_argument("db_error: data not updated");
                    }
                }
                cnt++;
            }
            //summing up
            if(iteration == MAX_RAP) {
                sql_script = "SELECT * FROM 'currency_table';";
                if (int select = sqlite3_exec(db, sql_script.c_str(), select_callback, 0,
                                              &error_msg)) {
                    throw std::invalid_argument("db_error: data not selected");
                }
            }

            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            doc_json.Accept(writer);
            sqlite3_close(db);
            //output with changes
            //std::cout << buffer.GetString() << std::endl;
        }
        Sleep(10000);
    }
    return 0;
}