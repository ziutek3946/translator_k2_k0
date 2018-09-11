#ifndef CFG_READER_H
#define CFG_READER_H

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <vector>
#include <string>
#include <fstream>

class Cfg_reader
{
    public:
        Cfg_reader();
        virtual ~Cfg_reader();
        std::vector<std::string> GetCommand();
        std::vector<std::string> GetDevice(std::string command);
        void GetCfg(rapidjson::Document& Json1, rapidjson::Document& Json2, rapidjson::Document& Json3);
    protected:
    private:
        const std::string names, types, methods;    //sciezki do configow
        rapidjson::Document json1, json2, json3;    //uchwyty do configow
        void ReadFile();                            //tworzenie uchwytow
        std::string GetJsonText(const rapidjson::Document& doc);
};

#endif // CFG_READER_H
