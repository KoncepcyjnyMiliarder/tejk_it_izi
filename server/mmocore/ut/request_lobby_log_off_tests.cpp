#include <c2s/request_lobby_log_off.hpp>
#include <fake_net_session.hpp>
#include <fake_net_socket.hpp>
#include <gtest/gtest.h>

TEST(request_lobby_log_off, construction)
{
  auto socket = std::make_shared<fake_net_socket>();
  EXPECT_NO_THROW(request_lobby_log_off(std::make_shared<fake_net_session>(socket)));
}

TEST(request_lobby_log_off, close_socket_on_execution)
{
  auto socket = std::make_shared<fake_net_socket>();
  request_lobby_log_off(std::make_shared<fake_net_session>(socket)).execute_associated_action();
  EXPECT_FALSE(socket->is_open());
}