#pragma once

#include <lobby_character.hpp>
#include <net_session.hpp>

class tii_entity_representative
{
public:
  using pointer = tii_entity_representative*;
  using const_pointer = const tii_entity_representative*;
  using reference = tii_entity_representative&;
  using const_reference = const tii_entity_representative&;

private:
  const unsigned id_;
  const std::string name_;
  std::shared_ptr<net_session> my_session_;

public:
  struct less
  {
    bool operator()(const tii_entity_representative& lhs,
                    const tii_entity_representative& rhs) const
    {
      return lhs.uid() < rhs.uid();
    }
  };

  tii_entity_representative(const lobby_character& selected_char,
                            std::shared_ptr<net_session> my_session);

  unsigned uid() const;
  const std::string& name() const;
  std::shared_ptr<net_session> my_session() const;
};