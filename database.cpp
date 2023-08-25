#include <QMessageBox>
#include <QSqlQuery>
#include <QVariant>
#include <QtDebug>
#include <QSqlError>
#include "database.h"
#include "login.h"

#include <QObject>

DataDB* DataDB::db = NULL;
DataDB::DataDB() {
    sqldb = QSqlDatabase::addDatabase("QSQLITE");
    sqldb.setDatabaseName("trial.db");

    if (!sqldb.open()) {
        qDebug() << "Error opening database" ;
    }else{
        qDebug() << "connected" ;
    }

    QSqlQuery qry;
    qry.exec("CREATE TABLE users ("
                   "user_id BIGINT PRIMARY KEY NOT NULL UNIQUE,"
                   "username VARCHAR(255) NOT NULL UNIQUE,"
                   "password VARCHAR(255) NOT NULL,"
                   "nickname VARCHAR(255) DEFAULT 'default',"
                   "gender INT DEFAULT -1,"
                   "avatar VARCHAR(255) DEFAULT 'null'"
               ")");
    qry.exec("CREATE TABLE friends ("
                   "user_id BIGINT NOT NULL,"
                   "friend_id BIGINT NOT NULL,"
                   "PRIMARY KEY (user_id, friend_id),"
                   "FOREIGN KEY (user_id) REFERENCES users(user_id),"
                   "FOREIGN KEY (friend_id) REFERENCES users(user_id)"
               ")");
    qry.exec("CREATE TABLE message ("
                   "message_id BIGINT PRIMARY KEY NOT NULL,"
                   "sender_id BIGINT NOT NULL,"
                   "receiver_id BIGINT NOT NULL,"
                   "content TEXT NOT NULL,"
                   "time DATETIME(6) NOT NULL,"
                   "FOREIGN KEY (sender_id) REFERENCES users(user_id),"
                   "FOREIGN KEY (receiver_id) REFERENCES users(user_id)"
               ")");
    qry.exec("CREATE TABLE groups ("
                   "group_id BIGINT PRIMARY KEY NOT NULL,"
                   "group_name VARCHAR(255) NOT NULL,"
                   "create_time DATETIME(6) NOT NULL,"
                   "creator_id BIGINT NOT NULL"
               ")");
    qry.exec("CREATE TABLE group_members ("
                   "group_id BIGINT NOT NULL,"
                   "user_id BIGINT NOT NULL,"
                   "PRIMARY KEY (group_id, user_id),"
                   "FOREIGN KEY (group_id) REFERENCES groups(group_id),"
                   "FOREIGN KEY (user_id) REFERENCES users(user_id)"
               ")");
    qry.exec("CREATE TABLE groupMessage ("
                   "message_id BIGINT PRIMARY KEY NOT NULL,"
                   "group_id BIGINT NOT NULL,"
                   "user_id BIGINT NOT NULL,"
                   "content TEXT NOT NULL,"
                   "time DATETIME(6) NOT NULL,"
                   "FOREIGN KEY (group_id) REFERENCES groups(group_id),"
                   "FOREIGN KEY (user_id) REFERENCES users(user_id)"
               ")");
}

DataDB::~DataDB() {
    if (db != NULL) {
        delete db;
    }
}
DataDB* DataDB::GetInstance() {
    if (db == NULL) {
        db = new DataDB;
    }
    return db;
}

bool DataDB::registerUser(const quint32 user_id, const QString& username, const QString& password) {
    QSqlQuery query(db->sqldb);
    query.prepare("INSERT INTO users (user_id, username, password) VALUES (:user_id, :username, :password)");
    query.bindValue(":user_id", user_id);
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()) {
        qDebug() << "Error registering user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DataDB::loginUser(const quint32 user_id, const QString& username, const QString& password) {
    QSqlQuery query(db->sqldb);
    query.prepare("SELECT * FROM users WHERE user_id = :user_id AND password = :password AND username = :username");
    query.bindValue(":user_id", user_id);
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()) {
        qDebug() << "Error logging in";
        return false;
    }

    if (query.next()) {
        qDebug() << "Login successful. Welcome, " << username;
        login *configWindow = new login;
        configWindow->show();
        return true;
    } else {
        qDebug() << "Login failed. Invalid username or password.";
        return false;
    }
}
