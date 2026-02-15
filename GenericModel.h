#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include <QVariant>
#include <QDebug>
#include <meta>

#include "TaskStruct.h"

template <class T>
class GenericModel : public QAbstractListModel
{
public:
    explicit GenericModel(QVector<T>* vec, QObject* parent = nullptr)
        : QAbstractListModel(parent), m_data(vec) {}

    int rowCount(const QModelIndex& parent = QModelIndex()) const override
    {
        if (parent.isValid() || !m_data) return 0;
        return static_cast<int>(m_data->size());
    }

    QVariant data(const QModelIndex& index, int role) const override
    {
        if (!m_data || !index.isValid()) return {};

        const int row = index.row();
        if (row < 0 || row >= static_cast<int>(m_data->size())) return {};

        const T& item = (*m_data)[row];

        // We use roles starting at Qt::UserRole
        const int memberIndex = role - Qt::UserRole;
        if (memberIndex < 0) return {};

        int current = 0;

        static constexpr auto members =
            std::define_static_array(
                std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::current()));

        constexpr auto member =  members.at(0);

        template for (constexpr auto member : members)
        {
            if (current == memberIndex)
            {
                return QVariant::fromValue(item.[:member:]);
            }
            ++current;
        }

        return {};
    }

    bool setData(const QModelIndex& index,
                 const QVariant& value,
                 int role) override
    {
        if (!index.isValid()) return false;

        T& item = (*m_data)[index.row()];
        const int memberIndex = role - Qt::UserRole;

        int current = 0;

        static constexpr auto members =
            std::define_static_array(
                std::meta::nonstatic_data_members_of(^^T,
                                                     std::meta::access_context::current()));

        template for (constexpr auto member : members)
        {
            if (current == memberIndex)
            {
                using MemberType = [:type_of(member):];

                if (!value.canConvert<MemberType>())
                    return false;

                item.[:member:] = value.value<MemberType>();
                emit dataChanged(index, index, {role});
                return true;
            }
            ++current;
        }

        return false;
    }

    Qt::ItemFlags flags(const QModelIndex& index) const override
    {
        if (!index.isValid()) return Qt::NoItemFlags;
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }

    QHash<int, QByteArray> roleNames() const override
    {
        static const QHash<int, QByteArray> roles = [] {
            QHash<int, QByteArray> r;

            static constexpr auto members =
                std::define_static_array(
                    std::meta::nonstatic_data_members_of(^^T, std::meta::access_context::current()));

            int roleIdx = 0;
            template for (constexpr auto member : members)
            {
                auto sv = std::meta::identifier_of(member);
                r.insert(Qt::UserRole + roleIdx, QByteArray(sv.data(), sv.size()));
                ++roleIdx;
            }
            return r;
        }();

        return roles;
    }

    Q_INVOKABLE void addTask(const QString& title)
    {
        beginInsertRows(QModelIndex(),
                        rowCount(),
                        rowCount());

        m_data->push_back({title, false});

        endInsertRows();
    }

protected:
     QVector<T>* m_data = nullptr;
};

class TaskModel : public GenericModel<Task>
{
    Q_OBJECT
public:
    using GenericModel<Task>::GenericModel;

    Q_INVOKABLE void addTask(const QString& title)
    {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        m_data->push_back({title, false});
        endInsertRows();
    }

    Q_INVOKABLE QStringList getRoleNames() const
    {
        QStringList names;

        auto roles = roleNames();

        for (auto it = roles.begin(); it != roles.end(); ++it)
            names << it.value();

        return names;
    }
};
