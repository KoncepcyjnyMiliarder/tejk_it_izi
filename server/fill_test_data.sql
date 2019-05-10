--3 accounts
INSERT INTO accounts(login, pass_hash, pass_salt) VALUES
('krzysztof', 'dupadupa', '53a666af08d5389b88ab'),
('mirek', 'krojcburg', 'b099c55d001e4e14775c'),
('foo', 'bar', '6f054bb62c30c23f5ee8');

--krzysztof got full access, mirek medium, foo default (not present)
INSERT INTO access_level(account_id, privilege_level)
SELECT id, 9999 FROM accounts WHERE login = 'krzysztof';

INSERT INTO access_level(account_id, privilege_level)
SELECT id, 99 FROM accounts WHERE login = 'mirek';

--krzysztof got 2 characters
INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'Krzysztoforek' FROM accounts WHERE login = 'krzysztof';

INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'KoncepcyjnyMiliarder' FROM accounts WHERE login = 'krzysztof';

--mirek got 3 characters
INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'Mirraseq' FROM accounts WHERE login = 'mirek';

INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'Tag' FROM accounts WHERE login = 'mirek';

INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'JohnFreeman' FROM accounts WHERE login = 'mirek';

--foo got 3 character
INSERT INTO characters(owner_account_id, nickname)
SELECT id, 'foobar' FROM accounts WHERE login = 'foo';

--krzysztof logged once from localhost
INSERT INTO account_access(account_id, ip, access_time)
SELECT id, '127.0.0.1', now() FROM accounts WHERE login = 'krzysztof';

--foo logged at april fool's day "Monday, April 1, 2019 1:23:46 PM"
INSERT INTO account_access(account_id, ip, access_time)
SELECT id, '192.168.12.34', TO_TIMESTAMP(1554125026) FROM accounts WHERE login = 'foo';

--first is ALWAYS smaller
INSERT INTO character_friends(character_id, his_friend_id) VALUES
(1, 3),
(1, 4),
(1, 6),
(2, 4);