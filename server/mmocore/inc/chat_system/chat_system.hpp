#pragma once

#include <chat_system/chat_backend.hpp>
#include <unordered_map>
#include <task_scheduler.hpp>
#include <tii_entity_representative.hpp>
#include <chat_system/chatroom.hpp>

class chat_system
{
    //no to co?
    //gowno!!
    //spierdalaj kurwa, mysl

    struct entity_data
    {
      std::unique_ptr<chat_backend> backend;
      std::unordered_set<chatroom*> chatrooms_im_in;
    };

    std::unordered_map<tii_entity_representative::const_pointer, entity_data> entities_;
    std::unordered_map<std::string, chatroom> active_chatrooms_;

    task_scheduler& scheduler_;
    std::unique_ptr<task_scheduler::postponed_task> periodic_broadcast_task_;

    void unregister_from_chatroom(tii_entity_representative::const_pointer my_representative, chatroom* room);
    void periodic_broadcast(); // maybe per chatroom? ; O

  public:

    chat_system(task_scheduler& scheduler)
      : scheduler_(scheduler)
    {
      periodic_broadcast_task_ = std::move(scheduler_.postpone_task(std::bind(&chat_system::periodic_broadcast, this), boost::posix_time::seconds(6)));
    }

    void register_me(tii_entity_representative::const_reference my_representative, std::unique_ptr<chat_backend> my_backend);
    void unregister_me(tii_entity_representative::const_reference my_representative);
    void join_chatroom(tii_entity_representative::const_reference my_representative, const std::string& room_name);
    void leave_chatroom(tii_entity_representative::const_reference my_representative, const std::string& room_name);
    void broadcast_message(tii_entity_representative::const_reference my_representative, const std::string& room_name, const std::string& msg);
    void whisper(tii_entity_representative::const_reference my_representative, const std::string& recipient, const std::string& msg);
};
