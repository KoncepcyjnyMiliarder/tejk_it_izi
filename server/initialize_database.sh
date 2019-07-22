psql -h localhost -d postgres -U postgres -f user_init_script.sql
PGPASSWORD=aaa psql -h localhost -d test_db -U test_user -f init_db.sql
PGPASSWORD=aaa psql -h localhost -d test_db -U test_user -f fill_test_data.sql
