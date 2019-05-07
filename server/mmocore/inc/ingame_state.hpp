#pragma once

#include <state_transitioner.hpp>
#include <world.hpp>
#include <logger.hpp>
#include <net_session.hpp>
#include <database_facade.hpp>
#include <ingame_state_recv_packet_factory.hpp>
#include <s2c/moved_to_lobby.hpp>

class ingame_state :
  public client_state
{
    struct finalizer
    {
      ingame_state& this_;
      finalizer(ingame_state& t)
        : this_(t) {}
      ~finalizer()
      {
        net_socket::buffer buffer;
        auto len = ingame_packet_constructors::moved_to_lobby(buffer);
        this_.my_session_->send_to_client(buffer, len);
      }
    };

    state_transitioner& transitioner_;
    std::shared_ptr<net_session> my_session_;
    finalizer fin_;
    world& world_;
    logger& logger_;
    database_facade& db_;
    world::player_pawn my_pawn_;
    account_data acc_data_;
    std::list<std::unique_ptr<world::chat_system::chat_pressence_token>> chatrooms_im_in_;

    ingame_state_recv_packet_factory recv_factory_;

  public:

    ingame_state(state_transitioner& transitioner, std::shared_ptr<net_session> my_session, world& universe,
                 logger& logger, std::string selected_character_name, database_facade& db, account_data acc_data);
    ~ingame_state();

    virtual void handle_network_packet(const std::array<char, 2048>& data, unsigned len) override;
    virtual void start() override;

};