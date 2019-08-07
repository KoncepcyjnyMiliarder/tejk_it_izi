#pragma once

#include <database_facade.hpp>
#include <task_scheduler.hpp>

class asynchronous_database_adapter
{

    database_facade& synchronous_db_;
    task_scheduler& game_loop_scheduler_; //hmmmm do i really need to synchronize completion handlers with the game loop? they sleep during db operation so...
    boost::asio::strand database_task_strand_;
    task_scheduler database_task_scheduler_;

  public:

    asynchronous_database_adapter(database_facade& synchronous_db, task_scheduler& game_loop_scheduler, boost::asio::io_service& io_service)
      : synchronous_db_(synchronous_db),
        game_loop_scheduler_(game_loop_scheduler),
        database_task_strand_(io_service),
        database_task_scheduler_(database_task_strand_) {}

    template<typename CharacterListCallback>
    void get_lobby_chars(unsigned account_id, CharacterListCallback&& cb)
    {
      database_task_scheduler_.schedule_task([this, account_id, cb]
      {
        game_loop_scheduler_.schedule_task(std::bind(cb, synchronous_db_.get_lobby_chars(account_id)));
      });
    }
    /*
    virtual void erase_character(const lobby_character& character) = 0;
    virtual bool is_nickname_already_taken(const std::string& nickname) = 0;
    virtual unsigned char_name_to_uid(const std::string& nickname) = 0;
    virtual lobby_character create_character(const std::string& nickname, unsigned owner_acc_id) = 0;
    virtual account_data get_account_data(const std::string& login) = 0;
    virtual std::unordered_set<unsigned> get_friends_of(unsigned char_id) = 0;
    virtual void make_friends(unsigned one, unsigned other) = 0;
    virtual void remove_friends(unsigned one, unsigned other) = 0;
    */
};