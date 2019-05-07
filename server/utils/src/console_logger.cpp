#include <console_logger.hpp>
#include <iostream>

void console_logger::log(const std::string& msg)
{
  std::cout << msg << std::endl;
}
