#include <iostream>
#define CURL_STATICLIB
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <curl\curl.h>
#include <string>

static size_t WriteCallback(void *contents, size_t size, size_t npos, void *userp) {
    ((std::string *) userp) -> append((char *) contents, size * npos);
    return size * npos;
}

int main() {
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