#include <tejk_it_izi.hpp>
#include <boost_acceptor.hpp>
#include <iostream>
#include <psql_facade.hpp>
#include <thread>
#include <chrono>

int main()
{
  //"user=postgres password=root"
  try
  {
    psql_facade psql_boi("test_user", "aaa", "test_db");
    boost::asio::io_service io_service;
    boost::asio::strand sync_strand(io_service);
    auto ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6112);
    boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(0));
    tejk_it_izi and_worry_not(psql_boi, std::make_unique<boost_acceptor>(io_service, ep, sync_strand), timer, sync_strand, io_service);
    //start worker threads
    std::vector<std::thread> workers;
    for(int i = 0; i < 4; ++i)
      workers.emplace_back([&io_service]
    {
      io_service.run();
    });
    //server tick
    and_worry_not.run();
    io_service.run();
    for (auto& w : workers)
      w.join();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}