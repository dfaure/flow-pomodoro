#ifndef STORAGEBACKENDMODEL_H
#define STORAGEBACKENDMODEL_H

#include "flow_export.h"
#include "storagebackendinstance.h"

#include <QAbstractListModel>
#include <QList>

class FLOW_EXPORT StorageBackendInstanceModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:

    enum Role {
        BackendInstanceRole = Qt::UserRole + 1,
    };

    explicit StorageBackendInstanceModel(QObject *parent = nullptr);
    ~StorageBackendInstanceModel();
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int count() const;

    void appendInstance(StorageBackendInstance *);
    void removeInstance(StorageBackendInstance *);
    void clear();

    StorageBackendInstance::List instances() const;

Q_SIGNALS:
    void countChanged();

private:
    StorageBackendInstance::List m_instances;
};

#endif
