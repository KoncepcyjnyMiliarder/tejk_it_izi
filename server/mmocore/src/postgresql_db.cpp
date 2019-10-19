#include <postgresql_db.hpp>

postgresql_db::postgresql_db(const std::string& username,
                             const std::string& pass,
                             const std::string& dbname)
  : conn_("host=localhost user=" + username + " dbname=" + dbname + " password=" + pass)
{
}

pqxx::connection&
postgresql_db::get_conn()
{
  return conn_;
}
