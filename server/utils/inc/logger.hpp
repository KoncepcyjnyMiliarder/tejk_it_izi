#pragma once

#include <string>

class logger
{
  public:

    virtual void log_diagnostic(const std::string& msg) = 0;
    virtual ~logger() = default;
};