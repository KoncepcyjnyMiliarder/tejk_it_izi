DROP TABLE IF EXISTS accounts CASCADE;
DROP TABLE IF EXISTS access_level CASCADE;
DROP TABLE IF EXISTS characters CASCADE;
DROP TABLE IF EXISTS account_access CASCADE;

CREATE TABLE accounts(
id serial PRIMARY KEY,
login varchar(20) UNIQUE NOT NULL,
pass_hash varchar(20) NOT NULL,
pass_salt varchar(20) NOT NULL);

CREATE TABLE access_level(
account_id int references accounts(id),
privilege_level smallint);

CREATE TABLE characters(
id serial PRIMARY KEY,
owner_account_id int references accounts(id),
nickname varchar(20) UNIQUE NOT NULL);

CREATE TABLE account_access(
account_id int references accounts(id),
ip varchar(15) NOT NULL,
access_time timestamp NOT NULL);

CREATE TABLE character_friends(
character_id int references characters(id), --and what if someone deletes character?
his_friend_id int references characters(id));