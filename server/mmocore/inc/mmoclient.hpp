#pragma once

#include <net_session.hpp>
#include <state_transitioner.hpp>
#include <login_validator.hpp>
#include <world.hpp>
#include <logger.hpp>
#include <database_facade.hpp>
#include <task_scheduler.hpp>

class mmoclient
  : public std::enable_shared_from_this<mmoclient>,
    public state_transitioner,
    public net_session
{
    database_facade& db_;
    task_scheduler& scheduler_;

    virtual void on_recv(const net_socket::buffer& data, unsigned size) override;
    virtual void on_error() override;

  public:

    mmoclient(std::shared_ptr<net_socket> sock, database_facade& db, task_scheduler& scheduler);

    void start(world& universe, logger& logger, login_validator& authenticator);
    virtual void force_close() override;
};
