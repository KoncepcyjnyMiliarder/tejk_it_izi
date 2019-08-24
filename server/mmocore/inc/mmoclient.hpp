#pragma once

#include <asynchronous_database_adapter.hpp>
#include <logger.hpp>
#include <login_validator.hpp>
#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <task_scheduler.hpp>
#include <world.hpp>

class mmoclient : public std::enable_shared_from_this<mmoclient>,
                  public state_transitioner,
                  public net_session
{
  database_facade& db_;
  task_scheduler& scheduler_;
  logger& logger_;
  asynchronous_database_adapter& async_db_;

  virtual void on_recv(const net_socket::buffer& data, unsigned size) override;
  virtual void on_error() override;

public:
  mmoclient(std::shared_ptr<net_socket> sock,
            database_facade& db,
            task_scheduler& scheduler,
            logger& logger,
            asynchronous_database_adapter& async_db);

  void start(world& universe, login_validator& authenticator);
  virtual void force_close() override;

  virtual void schedule_transition(std::unique_ptr<client_state> new_state) override;
};
