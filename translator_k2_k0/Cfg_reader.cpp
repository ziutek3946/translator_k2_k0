#include "Cfg_reader.h"

Cfg_reader::Cfg_reader() : names("names.json"), types("types.json"), methods("methods.json")
{
    ReadFile();
}

Cfg_reader::~Cfg_reader()
{
    //dtor
}

std::vector<std::string> Cfg_reader::GetCommand()
{
    std::vector<std::string> commands;

    //sprawdzanie czy pobrany obiekt to tablica
    assert(json3.IsArray());

    //wylistowanie wszystkich dostepnych rozkazow
    for (auto& it : json3.GetArray())
    {
        assert(it["rozkazy"].IsArray());
        for (auto& it2 : it["rozkazy"].GetArray())
        {
            assert(it2.IsObject());
            commands.push_back(it2.GetObject()["nazwa"].GetString());
        }
    }

    //zwracamy wektor stringow, ktory przyda sie nam do autosugestii w konsoli
    return commands;
}

std::vector<std::string> Cfg_reader::GetDevice(std::string command)
{
    //wektory potrzebne tymczasowo do poprawnego dzialania funkcji
    std::vector<std::string> devices, mac;
    std::vector<int> types;

    //walidacja pobranej typu jako tablicy
    assert(json3.IsArray());

    //iteracja po elementach tablicy pliku konfiguracyjnego methods.json
    for (auto& it : json3.GetArray())
    {
        assert(it["rozkazy"].IsArray());
        //iteracja po pliku methods.json celem okreslenia jakiego typu urzadzenia posiadaja wybrane rozkazy
        for (auto& it2 : it["rozkazy"].GetArray())
        {
            if(it2["nazwa"].GetString() == command)
            {
                types.push_back(it["typ"].GetInt());
            }
        }

    }

    //============ wypisanie wszystkich adresow mac urzadzen o danym typie

    for (auto& it : types)
    {
        assert(json2.IsArray());
        for (auto& it2 : json2.GetArray())
        {
            assert(it2.IsObject());
            if(it2.GetObject()["typ"].GetInt() == it)
            {
                mac.push_back(it2.GetObject()["mac"].GetString());
            }
        }
    }

    //============ wypisanie wszystkich portow urzadzen o danym typie

    for (auto& it : mac)
    {
        assert(json1.IsArray());
        for (auto& it2 : json1.GetArray())
        {
            assert(it2.IsObject());
            if(it2.GetObject()["mac"].GetString() == it)
            {
                assert(it2.GetObject()["porty"].IsArray());
                for (auto& it3 : it2.GetObject()["porty"].GetArray())
                {
                    assert(it3.IsObject());
                    devices.push_back(it3.GetObject()["nazwa"].GetString());
                }
            }
        }
    }

    return devices;
}

void Cfg_reader::ReadFile()
{
    std::fstream plik;
    std::string json, temp;

    //otwieranie pliku w trybie odczytu
    plik.open(names.c_str(), std::ios::in);
    //dopoki nie dojedziemy do konca pliku pobieramy kolejne znaki ignorujac przy tym znaki biale
    while(!plik.eof())
    {
        plik >> temp;
        json += temp;
        temp = "";
    }
    //pobrany string parsujemy i tworzymy z niego uchwyt do dokumentu JSON-a na nim mozemy operowac
    json1.Parse(json.c_str());
    json = "";
    plik.close();

    //============= ponizsze przyklady analogicznie tyle ze dla pliku types.json i methods.json

    plik.open(types.c_str(), std::ios::in);
    while(!plik.eof())
    {
        plik >> temp;
        json += temp;
        temp = "";
    }
    json2.Parse(json.c_str());
    json = "";
    plik.close();

    //==============

    plik.open(methods.c_str(), std::ios::in);
    while(!plik.eof())
    {
        plik >> temp;
        json += temp;
        temp = "";
    }
    json3.Parse(json.c_str());
    json = "";
    plik.close();
}

std::string Cfg_reader::GetJsonText(const rapidjson::Document& doc)
{
    //przerobienie uchwytu JSONowego na zwyklego stringa
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return buffer.GetString();
}

void Cfg_reader::GetCfg(rapidjson::Document& Json1, rapidjson::Document& Json2, rapidjson::Document& Json3)
{
    //kopiowanie uchwytow JSONowych
    Json1.CopyFrom(json1, Json1.GetAllocator());
    Json2.CopyFrom(json2, Json2.GetAllocator());
    Json3.CopyFrom(json3, Json3.GetAllocator());
}
