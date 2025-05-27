#include "../Include/database.h"

PGconn* db_connect(const char *conninfo) {
    PGconn* connection = PQconnectdb(conninfo);
    if (PQstatus(connection) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(connection));
        PQfinish(connection);
        return NULL; // Return NULL on failure
    }
    return connection;
}

void db_disconnect(PGconn *connection) {
    if (connection != NULL) {
        PQfinish(connection);
    }
}

bool db_insert_transaction(PGconn *connection, const char *pan, const char *amount, const char *datetime) {
    if (connection == NULL) {
        fprintf(stderr, "Database connection is not established.\n");
        return false;
    }

    PGresult *res;
    const char *paramValues[3] = {pan, amount, datetime};
    res = PQexecParams(
        connection,
        "INSERT INTO transactions (pan, amount, datetime) VALUES ($1, $2, $3)",
        3,       // number of parameters
        NULL,    // parameter types (NULL means text)
        paramValues,
        NULL,    // result format (NULL means text)
        NULL,    // result format (0 means text)
        0
    );      

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Insert transaction failed: %s", PQerrorMessage(connection));
        PQclear(res);
        return false; // Return false on failure
    }

    PQclear(res);
    return true; // Return true on success
}

bool db_authorize_transaction(const char *pan, const char *amount) {
    /*Acá tendríamos que autorizar la transacción, es decir, mandar 
    una query a la base de datos para chequear el balance de la cuenta,
    entre otras cosas*/

    return true; // Assume authorization is successful
}