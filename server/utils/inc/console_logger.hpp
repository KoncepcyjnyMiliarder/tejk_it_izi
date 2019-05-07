#pragma once

#include <logger.hpp>

class console_logger
  : public logger
{
  public:

    virtual void log(const std::string& msg) override;
};