#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <rapidjson/document.h>
#include <mosquittopp.h>

#include "Cfg_reader.h"

class Parser
{
    public:
        Parser(std::string const& k2);
        virtual ~Parser();
    protected:
    private:
        rapidjson::Document json1, json2, json3;
        std::vector<std::string> tokenizacja(std::string const& line);
};

#endif // PARSER_H
