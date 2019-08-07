#include <fake_logger.hpp>

void fake_logger::log_diagnostic(const std::string& msg)
{
  log_msgs_.push(msg);
}

std::queue<std::string>& fake_logger::get_log()
{
  return log_msgs_;
}
