#include <c2s/send_whisper.hpp>

send_whisper::send_whisper(binary_deserializer& bd, user_environment& my_environment)
  : my_environment_(my_environment)
{
  bd >> recipient_ >> message_;
}

void send_whisper::execute_associated_action()
{
  my_environment_.chat_controller_.send_whisper(recipient_, message_);
}
