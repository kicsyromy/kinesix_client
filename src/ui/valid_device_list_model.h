#ifndef VALID_DEVICE_LIST_MODEL_H
#define VALID_DEVICE_LIST_MODEL_H

#include <QAbstractListModel>

#include <vector>

#include "kinesixd_proxy.h"
#include "device.h"

namespace kinesix
{
    constexpr int DEVICE_ROLE = Qt::UserRole + 1;

    class ValidDeviceListModel : public QAbstractListModel
    {
        Q_OBJECT

    public:
        ValidDeviceListModel(KinesixdProxy &dbusProxy, QObject *parent = Q_NULLPTR);

    protected:
        virtual int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
        virtual QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    private:
        KinesixdProxy &m_dbusProxy;
        std::vector<Device> m_deviceList;
    };
}

#endif // VALID_DEVICE_LIST_MODEL_H
