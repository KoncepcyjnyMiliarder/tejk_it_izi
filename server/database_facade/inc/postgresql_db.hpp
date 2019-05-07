#pragma once

#include <pqxx/pqxx>
#include <string>

class postgresql_db
{

    pqxx::connection conn_;

  public:

    postgresql_db(const std::string& username, const std::string& pass, const std::string& dbname);
    pqxx::connection& get_conn();
};