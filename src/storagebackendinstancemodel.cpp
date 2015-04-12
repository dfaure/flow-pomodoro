#include "storagebackendinstancemodel.h"

StorageBackendInstanceModel::StorageBackendInstanceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    connect(this, &StorageBackendInstanceModel::rowsInserted, this, &StorageBackendInstanceModel::countChanged);
    connect(this, &StorageBackendInstanceModel::rowsRemoved, this, &StorageBackendInstanceModel::countChanged);
    connect(this, &StorageBackendInstanceModel::modelReset, this, &StorageBackendInstanceModel::countChanged);
    connect(this, &StorageBackendInstanceModel::layoutChanged, this, &StorageBackendInstanceModel::countChanged);
}

StorageBackendInstanceModel::~StorageBackendInstanceModel()
{
}

int StorageBackendInstanceModel::rowCount(const QModelIndex &/*parent*/) const
{
    return m_instances.count();
}

QVariant StorageBackendInstanceModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_instances.count())
        return QVariant();

    StorageBackendInstance *instance = m_instances.at(index.row());
    Q_UNUSED(instance);

    switch (role) {
    case BackendInstanceRole:
        return QVariant::fromValue(instance);
    }

    return QVariant();
}

QHash<int, QByteArray> StorageBackendInstanceModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles.insert(BackendInstanceRole, "backendInstance");

    return roles;
}

int StorageBackendInstanceModel::count() const
{
    return m_instances.count();
}

void StorageBackendInstanceModel::appendInstance(StorageBackendInstance *instance)
{
    Q_ASSERT(instance != nullptr);
    Q_ASSERT(!m_instances.contains(instance));
    const int count = this->count();
    beginInsertRows(QModelIndex(), count, count);
    m_instances << instance;
    endInsertRows();
}

void StorageBackendInstanceModel::removeInstance(StorageBackendInstance *instance)
{
    const int index = m_instances.indexOf(instance);
    Q_ASSERT(index != -1);
    beginRemoveRows(QModelIndex(), index, index);
    m_instances.removeAt(index);
    endRemoveRows();
}

void StorageBackendInstanceModel::clear()
{
    beginResetModel();
    m_instances.clear();
    endResetModel();
}

StorageBackendInstance::List StorageBackendInstanceModel::instances() const
{
    return m_instances;
}
