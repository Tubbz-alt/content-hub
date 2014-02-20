/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COM_UBUNTU_CONTENTPEERMODEL_H_
#define COM_UBUNTU_CONTENTPEERMODEL_H_

#include "contentpeer.h"
#include "contenttype.h"
#include <com/ubuntu/content/hub.h>

#include <QObject>
#include <QVariantList>

class ContentPeerModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QVariantList peers READ peers NOTIFY peersChanged)

public:
    ContentPeerModel(QObject *parent = nullptr);

    int type();
    void setType(int type);
    QVariantList peers();

Q_SIGNALS:
    void typeChanged();
    void peersChanged();

private:
    com::ubuntu::content::Hub *m_hub;
    int m_type;
    QVariantList m_peers;
};

#endif // COM_UBUNTU_CONTENTPEERMODEL_H_
