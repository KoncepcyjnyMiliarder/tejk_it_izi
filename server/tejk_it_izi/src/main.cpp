#include <tejk_it_izi.hpp>
#include <boost_acceptor.hpp>
#include <iostream>
#include <psql_facade.hpp>

int main()
{
  //"user=postgres password=root"
  try
  {
    psql_facade psql_boi("test_user", "aaa", "test_db");
    boost::asio::io_service io_service;
    auto ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6112);
    tejk_it_izi and_worry_not(psql_boi, std::make_unique<boost_acceptor>(io_service, ep));
    for (;;)
    {
      and_worry_not.tick();
      io_service.poll();
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}