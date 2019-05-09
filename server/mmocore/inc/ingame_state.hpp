#pragma once

#include <state_transitioner.hpp>
#include <ingame_state_recv_packet_factory.hpp>
#include <s2c/moved_to_lobby.hpp>

class ingame_state :
  public client_state
{
    //now this little class is interesting, a consequence of the client_state design
    //it's only purpose is to be destroyed last, and in turn send the final log-off packet
    //maybe move it to other place soon
    struct finalizer
    {
      std::shared_ptr<net_session> my_session_;
      finalizer(std::shared_ptr<net_session> my_session)
        : my_session_(my_session) {}
      ~finalizer()
      {
        net_socket::buffer buffer;
        auto len = ingame_packet_constructors::moved_to_lobby(buffer);
        my_session_->send_to_client(buffer, len);
      }
    };

    state_transitioner& transitioner_;
    finalizer fin_;
    user_environment my_environment_;
    ingame_state_recv_packet_factory recv_factory_;

  public:

    ingame_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe,
                 logger& logger, std::string selected_character_name, database_facade& db, account_data acc_data);
    ~ingame_state();

    virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) override;
    virtual void start() override;

};