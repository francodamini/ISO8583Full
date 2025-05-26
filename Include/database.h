#ifndef DATABASE_H
#define DATABASE_H

#include <stdbool.h>

bool db_connect(const char *conninfo);
void db_disconnect();

// Insert transaction — returns true if success
bool db_insert_transaction(const char *pan, const char *amount, const char *datetime);

// Check balance, authorize transaction, etc.
bool db_authorize_transaction(const char *pan, const char *amount);

#endif // DATABASE_H
