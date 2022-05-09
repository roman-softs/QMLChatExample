#include "sql_conversation_model.h"

#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>

namespace  {
    const char* cChatHistoryTableName = "ChatHistory";
    const char* cAuthorKey = "author";
    const char* cTimestampKey = "timestamp";
    const char* cMessageKey = "message";
}

void createTable()
{
    if (QSqlDatabase::database().tables().contains(cChatHistoryTableName)) {
        return;
    }

    QSqlQuery query;
    if (!query.exec(QString("CREATE TABLE IF NOT EXISTS '%1' ("
                            "'%2' TEXT NOT NULL,"
                            "'%3' TEXT NOT NULL,"
                            "'%4' TEXT NOT NULL,"
                            "FOREIGN KEY('%2') REFERENCES Contacts ( name )"
                            ")").arg(cChatHistoryTableName, cAuthorKey, cTimestampKey, cMessageKey)
        )) {
        qFatal("Failed to query database: %s", qPrintable(query.lastError().text()));
    }
}

SqlConversationModel::SqlConversationModel(QObject *parent) :
    QSqlTableModel(parent)
{
    createTable();
    setTable(cChatHistoryTableName);
    setSort(1, Qt::DescendingOrder);
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    select();
}

QVariant SqlConversationModel::data(const QModelIndex &index, int role) const
{
    if (role < Qt::UserRole)
        return QSqlTableModel::data(index, role);

    const QSqlRecord sqlRecord = record(index.row());
    return sqlRecord.value(role - Qt::UserRole);
}

QHash<int, QByteArray> SqlConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = cAuthorKey;
    names[Qt::UserRole + 1] = cTimestampKey;
    names[Qt::UserRole + 2] = cMessageKey;
    return names;
}

void SqlConversationModel::sendMessage(const QDateTime& dateTime, const QString& userName, const QString text)
{
    QSqlRecord newRecord = record();
    newRecord.setValue(cAuthorKey, userName);
    newRecord.setValue(cTimestampKey, dateTime.toString(Qt::ISODate));
    newRecord.setValue(cMessageKey, text);
    if (!insertRecord(rowCount(), newRecord)) {
        qWarning() << "Failed to send message:" << lastError().text();
        return;
    }

    submitAll();
}


