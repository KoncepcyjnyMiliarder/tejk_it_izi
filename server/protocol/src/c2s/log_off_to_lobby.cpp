#include <c2s/log_off_to_lobby.hpp>
#include <lobby_state.hpp>

log_off_to_lobby::log_off_to_lobby(std::shared_ptr<net_session> my_session, logger& logger, state_transitioner& transitioner,
                                   world& universe, database_facade& db, account_data& acc_data)
  : my_session_(my_session),
    logger_(logger),
    transitioner_(transitioner),
    world_(universe),
    db_(db),
    acc_data_(acc_data)
{
}

void log_off_to_lobby::execute_associated_action()
{
  transitioner_.transition(std::make_unique<lobby_state>(transitioner_, my_session_, world_, logger_, db_, acc_data_)); //C A R E F U L
}
