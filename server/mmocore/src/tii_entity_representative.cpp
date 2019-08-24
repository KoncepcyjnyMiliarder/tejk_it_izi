#include <tii_entity_representative.hpp>

tii_entity_representative::tii_entity_representative(const lobby_character& selected_char,
                                                     std::shared_ptr<net_session> my_session)
  : id_(selected_char.uid)
  , name_(selected_char.name)
  , my_session_(my_session)
{
}

unsigned
tii_entity_representative::uid() const
{
  return id_;
}

const std::string&
tii_entity_representative::name() const
{
  return name_;
}

std::shared_ptr<net_session>
tii_entity_representative::my_session() const
{
  return my_session_;
}
