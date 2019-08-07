#include <mmoclient.hpp>
#include <preauth_state.hpp>
#include <null_socket.hpp>
#include <null_state.hpp>

void mmoclient::on_recv(const net_socket::buffer& data, unsigned size)
{
  current_state_->handle_network_packet(data, size);
}

void mmoclient::on_error()
{
  logger_.log_diagnostic(__func__);
  force_close();
}

mmoclient::mmoclient(std::shared_ptr<net_socket> sock, database_facade& db, task_scheduler& scheduler,
                     logger& logger, asynchronous_database_adapter& async_db)
  : net_session(std::move(sock)),
    db_(db),
    scheduler_(scheduler),
    logger_(logger),
    async_db_(async_db)
{
}

void mmoclient::start(world& universe, login_validator& authenticator)
{
  logger_.log_diagnostic(__func__);
  current_state_ = std::make_unique<preauth_state>(*this, shared_from_this(), universe, logger_, authenticator, db_, async_db_);
  current_state_->start();
}

void mmoclient::force_close()
{
  logger_.log_diagnostic(__func__);
  my_sock_->force_close();
  my_sock_ = std::make_unique<null_socket>();
  auto self = shared_from_this();
  scheduler_.schedule_task([this, self]
  {
    logger_.log_diagnostic("Finalizing session" + std::to_string((unsigned long long)this));
    //destroying current_state by overwriting it with null_state
    current_state_ = std::make_unique<null_state>();
  });
}

void mmoclient::schedule_transition(std::unique_ptr<client_state> new_state)
{
  logger_.log_diagnostic(__func__);
  async_state_ = state_transitioner::async_operation_status::awaiting_state_transition;
  //uhm, u cant std::move rvalue into lambda?...
  state_awaiting_transition_ = std::move(new_state);
  scheduler_.schedule_task([this]
  {
    // delete the old state, and if it didnt trigger any database tasks, just start the new one
    current_state_ = nullptr;
    if (count_of_awaiting_database_tasks_ == 0)
    {
      current_state_ = std::move(state_awaiting_transition_);
      async_state_ = async_operation_status::awaiting_packet_poll;
      current_state_->start();
    }
  });
}
