/*
 * Copyright (C) 2016 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Arthur Mello <arthur.mello@canonical.com>
 */

#include "clipboardapplication.h"

#include <QGuiApplication>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>

#include <qpa/qplatformnativeinterface.h>
#include <mir_toolkit/mir_surface.h>

#include "debug.h"
#include "paste-data-model.h"
#include "paste-data-filter-model.h"

ClipboardApplication::ClipboardApplication(int &argc, char **argv)
    : QGuiApplication(argc, argv),
    m_surfaceId(),
    m_view(new QQuickView())
{
    connect(this, SIGNAL(applicationStateChanged(Qt::ApplicationState)), SLOT(onApplicationStateChanged(Qt::ApplicationState)));

    bool fullScreen = false;
    QStringList args = arguments();
    if (args.contains("--fullscreen")) {
        args.removeAll("--fullscreen");
        fullScreen = true;
    }

    const char* uri = "clipboardapp.private";
    qmlRegisterType<PasteDataModel>(uri, 0, 1, "PasteDataModel");
    qmlRegisterType<PasteDataFilterModel>(uri, 0, 1, "PasteDataFilterModel");

    QObject::connect(m_view->engine(), SIGNAL(quit()), SLOT(quit()));
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->rootContext()->setContextProperty("application", this);
    m_view->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    if (fullScreen) {
        m_view->showFullScreen();
    } else {
        m_view->show();
    }
}

ClipboardApplication::~ClipboardApplication()
{
    delete m_view;
}

int ClipboardApplication::exec()
{
    return QGuiApplication::exec();
}

const QString& ClipboardApplication::surfaceId() const
{
    return m_surfaceId;
}

void ClipboardApplication::onApplicationStateChanged(Qt::ApplicationState state)
{
    if (m_surfaceId.isEmpty() && state == Qt::ApplicationActive) {
        m_surfaceId = requestSurfaceId();
        if (m_surfaceId.isEmpty()) {
            TRACE() << Q_FUNC_INFO << "Unable to request MIR surfaceId. Clipboard will not be able to get any data from content-hub";
        } else {
            Q_EMIT(surfaceIdChanged());
        }
    }
}

QString ClipboardApplication::requestSurfaceId()
{
    QWindow *focusWindow = QGuiApplication::focusWindow();
    if (!focusWindow)
        return QString();

    auto mirSurface = static_cast<MirSurface*>(platformNativeInterface()->nativeResourceForWindow("mirsurface", focusWindow));
    if (!mirSurface)
        return QString();

    MirPersistentId* mirPermaId = mir_surface_request_persistent_id_sync(mirSurface);
    QString surfaceId(mir_persistent_id_as_string(mirPermaId));
    mir_persistent_id_release(mirPermaId);

    return surfaceId;
}
