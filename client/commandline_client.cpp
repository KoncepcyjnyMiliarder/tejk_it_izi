#include <boost_socket.hpp>
#include <preauth_state_protocol.hpp>
#include <lobby_state_protocol.hpp>
#include <ingame_state_protocol.hpp>
#include <iostream>
#include <thread>
#include <binary_serializer.hpp>
#include <binary_deserializer.hpp>
#include <s2c/character_creation_fail.hpp>

/*
The most naive and simple implementation of client
*/

enum current_state
{
  preauth,
  lobby,
  ingame
};

current_state cs = preauth;

class my_recv_completion_handler
  : public net_socket::recv_completion_handler
{
  public:

    virtual void on_recv(const net_socket::buffer& data, unsigned size) override
    {
      binary_deserializer bd(data.data(), size);
      switch(cs)
      {
        case preauth:
          {
            preauth_state_protocol::to_client_packet_opcodes opcode;
            bd >> opcode;
            switch (opcode)
            {
              case preauth_state_protocol::to_client_packet_opcodes::fuck_you_bad_keys:
                {
                  printf("fuck_you_bad_keys\n");
                  exit(0);
                }
                break;
              case preauth_state_protocol::to_client_packet_opcodes::good_boy_good_keys:
                {
                  printf("good_boy_good_keys :3\n");
                  cs = lobby;
                }
                break;
              default:
                printf("HUUH unknown opcode for preauth %d\n", opcode);
                break;
            }
          }
          break;
        case lobby:
          {
            lobby_state_protocol::to_client_packet_opcodes opcode;
            bd >> opcode;
            switch (opcode)
            {
              case lobby_state_protocol::to_client_packet_opcodes::character_list:
                {
                  std::size_t count;
                  bd >> count;
                  printf("you got %u chars!\n", count);
                  while (count--)
                  {
                    unsigned uid;
                    std::string nick;
                    bd >> uid >> nick;
                    printf("\tuid %u nick %s\n", uid, nick.c_str());
                  }
                }
                break;
              case lobby_state_protocol::to_client_packet_opcodes::character_selected:
                {
                  std::string nick;
                  bd >> nick;
                  printf("Selected character nick %s\n", nick.c_str());
                  cs = ingame;
                }
                break;
              case lobby_state_protocol::to_client_packet_opcodes::character_created:
                {
                  unsigned uid;
                  std::string nick;
                  bd >> uid >> nick;
                  printf("Created char uid %u nick %s\n", uid, nick.c_str());
                }
                break;
              case lobby_state_protocol::to_client_packet_opcodes::character_creation_fail:
                {
                  character_creation_fail_reason fail_reason;
                  bd >> fail_reason;
                  switch (fail_reason)
                  {
                    case character_creation_fail_reason::name_too_long:
                      printf("name_too_long\n");
                      break;
                    case character_creation_fail_reason::name_contains_incorrect_digits:
                      printf("name_contains_incorrect_digits\n");
                      break;
                    case character_creation_fail_reason::name_already_used:
                      printf("name_already_used\n");
                      break;
                    case character_creation_fail_reason::character_limit_exceeded:
                      printf("character_limit_exceeded\n");
                      break;
                    default:
                      printf("HUUH unknown fail_reason for character_creation_fail %d\n", fail_reason);
                  }
                }
                break;
              default:
                printf("HUUH unknown opcode for lobby %d\n", opcode);
                break;
            }

          }
          break;
        case ingame:
          {
            ingame_state_protocol::to_client_packet_opcodes opcode;
            bd >> opcode;
            switch (opcode)
            {
              case ingame_state_protocol::to_client_packet_opcodes::moved_to_lobby:
                {
                  printf("Moved to lobby\n");
                  cs = lobby;
                }
                break;
              case ingame_state_protocol::to_client_packet_opcodes::user_joined_chat:
                {
                  std::string chatroom, user;
                  bd >> chatroom >> user;
                  printf("User %s join chat %s\n", user.c_str(), chatroom.c_str());
                }
                break;
              case ingame_state_protocol::to_client_packet_opcodes::user_left_chat:
                {
                  std::string chatroom, user;
                  bd >> chatroom >> user;
                  printf("User %s left chat %s\n", user.c_str(), chatroom.c_str());
                }
                break;
              case ingame_state_protocol::to_client_packet_opcodes::chat_msg:
                {
                  std::string chatroom, user, msg;
                  bd >> chatroom >> user >> msg;
                  printf("[%s]\t%s:\t%s\n", chatroom.c_str(), user.c_str(), msg.c_str());
                }
                break;
              default:
                printf("HUUH unknown opcode for ingame %d\n", opcode);
                break;
            }
          }
          break;
      }
    }

    virtual void on_error() override
    {
      printf("socket error RIP\n");
      exit(0);
    }
};

void handle_ingame_command(char command, std::shared_ptr<boost_socket> socket)
{
  net_socket::buffer sendbuf;
  switch (command)
  {
    case 'j': //join chat
      {
        std::cout << "Enter chatroom to join:\n";
        std::string name;
        std::cin >> name;
        binary_serializer bs(sendbuf);
        bs << ingame_state_protocol::to_server_packet_opcodes::join_chat << name;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    case 'l': //leave chat
      {
        std::cout << "Enter chatroom to leave:\n";
        std::string name;
        std::cin >> name;
        binary_serializer bs(sendbuf);
        bs << ingame_state_protocol::to_server_packet_opcodes::leave_chat << name;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    case 's': //send message
      {
        std::cout << "Enter [chatroom_name] [message]:\n";
        std::string chatroom_name, msg;
        std::cin >> chatroom_name;
        std::getline(std::cin, msg);
        binary_serializer bs(sendbuf);
        bs << ingame_state_protocol::to_server_packet_opcodes::send_chat_msg << chatroom_name << msg;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    case 'q': //quit to lobby
      {
        binary_serializer bs(sendbuf);
        std::string redundant_data = "asdadsadadadasdasd";
        bs << ingame_state_protocol::to_server_packet_opcodes::log_off_to_lobby << redundant_data;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    default:
      std::cout << "Unknown command: " << command << std::endl;
      break;
  }
}

void handle_lobby_command(char command, std::shared_ptr<boost_socket> socket)
{
  net_socket::buffer sendbuf;
  switch (command)
  {
    case 'c': //create char
      {
        std::cout << "Enter character name to create:\n";
        std::string name;
        std::cin >> name;
        binary_serializer bs(sendbuf);
        bs << lobby_state_protocol::to_server_packet_opcodes::create_character << name << 1337;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    case 'd': //delete char
      {
        std::cout << "Enter character name to delete:\n";
        std::string name;
        std::cin >> name;
        binary_serializer bs(sendbuf);
        bs << lobby_state_protocol::to_server_packet_opcodes::delete_character << name;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    case 's': //select char
      {
        std::cout << "Enter character name to select:\n";
        std::string name;
        std::cin >> name;
        binary_serializer bs(sendbuf);
        bs << lobby_state_protocol::to_server_packet_opcodes::select_character << name;
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
    case 'q': //quit
      {
        binary_serializer bs(sendbuf);
        bs << lobby_state_protocol::to_server_packet_opcodes::log_off << 321564; // btw redundant data should be OK
        socket->send_to_client(sendbuf, bs.get_current_size());
      }
      break;
    default:
      std::cout << "Unknown command: " << command << std::endl;
      break;
  }
}

int main()
{
  boost::asio::io_service io_service;
  boost::asio::ip::tcp::socket mysock(io_service);
  boost::system::error_code ec;
  char answer{};
  while (answer != 'l' && answer != 'r')
  {
    printf("LOCAL SERVER: \'l\' REMOTE SERVER: \'r\'");
    std::cin >> answer;
  }
  std::string ip = "127.0.0.1";
  if (answer == 'r')
  {
    std::cout << "Enter server IP: ";
    std::cin >> ip;
  }
  auto ep = boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip, ec), 6112);
  if (ec)
  {
    printf("from_string, error msg: %s\n", ec.message().c_str());
    return 1;
  }
  puts("Attempting connection...");
  mysock.connect(ep, ec);
  if (ec)
  {
    printf("Unable to connect, RIP in peppers, error msg: %s\n", ec.message().c_str());
    return 1;
  }
  printf("connected \\o/\n");
  std::shared_ptr<boost_socket> socket(std::make_unique<boost_socket>(std::move(mysock)));
  std::thread([&socket]
  {
    net_socket::buffer sendbuf;
    std::cout << "Enter username:\n";
    std::string login;
    std::cin >> login;
    binary_serializer bs(sendbuf);
    std::string loginkeys(64, 'x');
    bs << preauth_state_protocol::to_server_packet_opcodes::loginserver_keys << loginkeys << login;
    socket->send_to_client(sendbuf, bs.get_current_size());
    while (1)
    {
      char command;
      std::cin >> command;
      switch (cs)
      {
        case preauth:
          printf("Still awaiting authentication\n");
          break;
        case lobby:
          handle_lobby_command(command, socket);
          break;
        case ingame:
          handle_ingame_command(command, socket);
          break;
      }
    }
  }).detach();
  my_recv_completion_handler tralala;
  socket->async_recv(tralala);
  while (1)
    io_service.poll();
}