#include "Parser.h"

Parser::Parser(std::string const& k2)
{
    std::string mac, port, rozkaz;
    int type = 0;

    Cfg_reader cfg;
    //skopiowanie uchwytow JSONa
    cfg.GetCfg(json1, json2, json3);

    // Zainicjalizuj działanie biblioteki MQTT:
    mosqpp::lib_init();
    // Utwórz obiekt Mosquitto:
    mosqpp::mosquittopp mos;
	// Połącz się z brokerem:
	mos.connect("m.k.pl");

    //wypisana w konsoli linie tokenizujemy
    std::vector<std::string> token = tokenizacja(k2);
    std::string k0;

    //pozyskanie numeru portu oraz maca na podstawie nazwy portu (konkretnego urzadzenia np. lampka_pokoj)
    assert(json1.IsArray());
    for (auto& it : json1.GetArray())
    {
        assert(it.GetObject()["porty"].IsArray());
        for (auto& it2 : it.GetObject()["porty"].GetArray())
        {
            assert(it2.IsObject());
            if(it2.GetObject()["nazwa"].GetString() == token[1])
            {
                port = it2.GetObject()["numer"].GetString();
                mac = it.GetObject()["mac"].GetString();
                break;
            }
        }

    }

    //na podstawie maca uzyskanego powyzej ustalany jest typ urzadzenia
    assert(json2.IsArray());
    for (auto& it : json2.GetArray())
    {
        assert(it.IsObject());
        if(it.GetObject()["mac"].GetString() == mac) { type = it.GetObject()["typ"].GetInt(); }
    }

    //na podstawie wszystkich powyzszych ustalany jest kod rozkazu
    assert(json3.IsArray());
    for (auto& it : json3.GetArray())
    {
        assert(it.IsObject());
        if(it.GetObject()["typ"].GetInt() == type)
        {
            assert(it.GetObject()["rozkazy"].IsArray());
            for (auto& it2 : it.GetObject()["rozkazy"].GetArray())
            {
                assert(it2.IsObject());
                if(it2.GetObject()["nazwa"].GetString() == token[0]) { rozkaz = it2.GetObject()["kod"].GetString(); }
            }
        }
    }

    //stworzenie wiadomosci w jezyku k0
    k0 += "/d/k/";
    k0 += mac;
    k0 += "/";
    k0 += rozkaz;
    k0 += "/";
    k0 += port;
    k0 += "/00000000/00000000/000000/000000/";

    //opublikowanie gotowej ramki na kanale k0
    mos.publish(NULL, "k0", k0.length(), k0.c_str());
}

Parser::~Parser()
{
    //dtor
}

//przerobienie stringa ze spacjami na vector stringow gdzie stringi oddzielone spacjami sa kolejnymi wartosciami tablicy
std::vector<std::string> Parser::tokenizacja(std::string const& line)
{
    std::vector<std::string> tokeny;
    std::string temp = "";
    for(const char & c : line)
    {
        if(c == ' ')
        {
            tokeny.push_back(temp);
            temp = "";
        }
        else
        {
            temp += c;
        }
    }
    tokeny.push_back(temp);
    return tokeny;
}
