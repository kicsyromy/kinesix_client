#include "valid_device_list_model.h"

using namespace kinesix;

ValidDeviceListModel::ValidDeviceListModel(KinesixdProxy &dbusProxy, QObject *parent) :
    QAbstractListModel(parent),
    m_dbusProxy(dbusProxy),
    m_deviceList(m_dbusProxy.GetValidDeviceList())
{
}

int ValidDeviceListModel::rowCount(const QModelIndex &parent) const
{
    return static_cast<int>(m_deviceList.size());
}

QVariant ValidDeviceListModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if (role == Qt::DisplayRole)
        value = QVariant::fromValue(m_deviceList.at(index.row()).name);
    else if (role == DEVICE_ROLE)
        value = QVariant::fromValue(m_deviceList.at(index.row()));

    return value;
}
