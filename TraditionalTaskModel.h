#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>

#include "TaskStruct.h"

class TraditionalTaskModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TraditionalTaskModel(QVector<Task> tasks, QObject* parent = nullptr)
        : m_tasks{tasks}, QAbstractListModel(parent)
    {
    }

    enum Roles {
        TitleRole = Qt::UserRole,
        DoneRole
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        if (parent.isValid())
            return 0;

        return m_tasks.size();
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!index.isValid())
            return {};

        const Task& task = m_tasks.at(index.row());

        switch (role) {
        case TitleRole:
            return task.Title;

        case DoneRole:
            return task.Done;
        }

        return {};
    }

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role) override
    {
        if (!index.isValid())
            return false;

        Task& task = m_tasks[index.row()];

        switch (role) {
        case TitleRole:
            task.Title = value.toString();
            break;

        case DoneRole:
            task.Done = value.toBool();
            break;

        default:
            return false;
        }

        emit dataChanged(index, index, {role});
        return true;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override
    {
        if (!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    QHash<int, QByteArray> roleNames() const override
    {
        return {
            {TitleRole, "Title"},
            {DoneRole,  "Done"}
        };
    }

    Q_INVOKABLE void addTask(const QString& title)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_tasks.append({title, false});
        endInsertRows();
    }

    Q_INVOKABLE void removeTask(int row)
    {
        if (row < 0 || row >= m_tasks.size())
            return;

        beginRemoveRows(QModelIndex(), row, row);
        m_tasks.removeAt(row);
        endRemoveRows();
    }

    Q_INVOKABLE QStringList getRoleNames() const
    {
        QStringList names;

        auto roles = roleNames();

        for (auto it = roles.begin(); it != roles.end(); ++it)
            names << it.value();

        return names;
    }

private:
    QVector<Task> m_tasks;
};
