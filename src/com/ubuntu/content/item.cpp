/*
 * Copyright © 2013 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Thomas Voß <thomas.voss@canonical.com>
 */

#include <com/ubuntu/content/item.h>

namespace cuc = com::ubuntu::content;

struct cuc::Item::Private
{
    QUrl url;

    bool operator==(const Private& rhs) const
    {
        return url == rhs.url;
    }
};

cuc::Item::Item(const QUrl& url, QObject* parent) : QObject(parent), d{new cuc::Item::Private{url}}
{
}

cuc::Item::Item(const cuc::Item& rhs) : QObject(rhs.parent()), d(rhs.d)
{
}

cuc::Item& cuc::Item::operator=(const cuc::Item& rhs)
{
    d = rhs.d;
    return *this;
}

bool cuc::Item::operator==(const cuc::Item& rhs) const
{
    if (d == rhs.d)
        return true;

    return *d == *rhs.d;
}

cuc::Item::~Item()
{
}

const QUrl& cuc::Item::url() const
{
    return d->url;
}
