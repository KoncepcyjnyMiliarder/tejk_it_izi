#include <console_logger.hpp>
#include <iostream>
#include <thread>

void
console_logger::log_diagnostic(const std::string& msg)
{
  std::cout << '[' << std::this_thread::get_id() << "]\t" << msg << std::endl;
}
