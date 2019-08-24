#pragma once

#include <asynchronous_database_adapter.hpp>
#include <client_acceptor.hpp>
#include <client_state.hpp>
#include <console_logger.hpp>
#include <database_facade.hpp>
#include <login_validator.hpp>
#include <memory>
#include <net_session.hpp>
#include <set>
#include <task_scheduler.hpp>
#include <world.hpp>

// VERY TEMP DUMMY VALIDATOR
class dummy_validator : public login_validator
{
public:
  virtual bool are_loginkey_and_username_valid(const std::string& loginkey,
                                               const std::string& login) override
  {
    return login == "krzysztof" || login == "mirek" || login == "foo";
  }
};
class tejk_it_izi : private client_acceptor::accept_completion_handler
{

  database_facade& db_;
  std::unique_ptr<client_acceptor> acceptor_;

  boost::asio::deadline_timer& timer_;
  boost::asio::strand& sync_strand_;
  task_scheduler scheduler_;
  std::chrono::time_point<std::chrono::steady_clock> start_;

  world universe_;
  console_logger logger_;
  dummy_validator authenticator_;

  asynchronous_database_adapter async_db_;
  virtual void on_accept(std::shared_ptr<net_socket> sock) override;

  void tick();

public:
  tejk_it_izi(database_facade& db,
              std::unique_ptr<client_acceptor> acceptor,
              boost::asio::deadline_timer& timer,
              boost::asio::strand& sync_strand,
              boost::asio::io_service& io_service);

  void run();
};