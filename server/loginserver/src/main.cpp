#include <iostream>
#include <loginserver.hpp>

int
main()
{
  try
  {
    boost::asio::io_context ioc;
    auto login_ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 6789);
    auto gs_ls_ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 32132);
    boost::asio::ip::tcp::acceptor login_acceptor(ioc, login_ep);
    boost::asio::ip::tcp::acceptor gs_ls_acceptor(ioc, gs_ls_ep);
    loginserver ls(login_acceptor, gs_ls_acceptor);
    ls.run();
    ioc.run();
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
