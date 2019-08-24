#include <binary_serializer.hpp>
#include <ingame_state_protocol.hpp>
#include <s2c/whisper_message.hpp>

unsigned
ingame_packet_constructors::whisper_message(net_socket::buffer& buffer_to_fill,
                                            const std::string& sender,
                                            const std::string& msg)
{
  binary_serializer bs(buffer_to_fill);
  bs << ingame_state_protocol::to_client_packet_opcodes::whisper_message << sender << msg;
  return bs.get_current_size();
}
