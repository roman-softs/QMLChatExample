#pragma once

#include <QSqlTableModel>

class SqlConversationModel : public QSqlTableModel
{
    Q_OBJECT

public:
    SqlConversationModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void sendMessage(const QDateTime& dateTime, const QString& userName, const QString text);

};
