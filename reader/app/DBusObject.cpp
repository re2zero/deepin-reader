/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhangsong<zhangsong@uniontech.com>
*
* Maintainer: zhangsong<zhangsong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "DBusObject.h"
#include "MainWindow.h"
#include "Application.h"

#include <QDBusConnection>
#include <QDebug>

// gesture 触控板手势
#define DBUS_SERVER             "com.deepin.Reader"
#define DBUS_SERVER_PATH        "/com/deepin/Reader"

#define DBUS_GESTURE            "com.deepin.daemon.Gesture"
#define DBUS_GESTURE_PATH       "/com/deepin/daemon/Gesture"
#define DBUS_GESTURE_INTERFACE  "com.deepin.daemon.Gesture"
#define DBUS_GESTURE_SIGNAL     "Event"

#define DBUS_IM                 "com.deepin.im"
#define DBUS_IM_PATH            "/com/deepin/im"
#define DBUS_IM_INTERFACE       "com.deepin.im"
#define DBUS_IM_SIGNAL          "imActiveChanged"

DBusObject *DBusObject::s_instance = nullptr;

DBusObject *DBusObject::instance()
{
    if (nullptr == s_instance) {
        s_instance = new DBusObject;
    }

    return s_instance;
}

void DBusObject::destory()
{
    if (nullptr != s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

bool DBusObject::registerOrNotify(QStringList arguments)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.registerService(DBUS_SERVER)) {
        QDBusInterface notification(DBUS_SERVER, DBUS_SERVER_PATH, DBUS_SERVER, QDBusConnection::sessionBus());
        QList<QVariant> args;
        args.append(arguments);
        QString error = notification.callWithArgumentList(QDBus::Block, "handleFiles", args).errorMessage();
        if (!error.isEmpty())
            qInfo() << error;
        return false;
    }

    dbus.registerObject(DBUS_SERVER_PATH, this, QDBusConnection::ExportScriptableSlots);

    if (nullptr == m_keyboardInterface) {
        m_keyboardInterface = new QDBusInterface(DBUS_IM, DBUS_IM_PATH, DBUS_IM_INTERFACE, dbus);
        if (m_keyboardInterface->isValid()) {
            connect(m_keyboardInterface, SIGNAL(imActiveChanged(bool)), this, SIGNAL(sigImActiveChanged(bool)));
        } else {
            delete m_keyboardInterface;
            m_keyboardInterface = nullptr;
        }
    }

    QDBusConnection::systemBus().connect(DBUS_GESTURE, DBUS_GESTURE_PATH, DBUS_GESTURE_INTERFACE, DBUS_GESTURE_SIGNAL, this, SIGNAL(sigTouchPadEventSignal(QString, QString, int)));

    return true;
}

void DBusObject::unRegister()
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterService(DBUS_SERVER);
}

void DBusObject::blockShutdown()
{
    if (m_isBlockShutdown)
        return;

    if (m_blockShutdownReply.value().isValid()) {
        return;
    }

    if (m_blockShutdownInterface == nullptr)
        m_blockShutdownInterface = new QDBusInterface("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", QDBusConnection::systemBus());

    QList<QVariant> args;
    args << QString("shutdown") // what
         << qApp->applicationDisplayName() // who
         << QObject::tr("Document not saved") // why
         << QString("delay"); // mode

    m_blockShutdownReply = m_blockShutdownInterface->callWithArgumentList(QDBus::Block, "Inhibit", args);
    if (m_blockShutdownReply.isValid()) {
        m_blockShutdownReply.value().fileDescriptor();
        m_isBlockShutdown = true;
    } else {
        qInfo() << m_blockShutdownReply.error();
    }
}

void DBusObject::unBlockShutdown()
{
    if (m_blockShutdownReply.isValid()) {
        m_blockShutdownReply = QDBusReply<QDBusUnixFileDescriptor>();
        m_isBlockShutdown = false;
    }
}

void DBusObject::handleFiles(QStringList filePathList)
{
    if (filePathList.count() <= 0) {
        MainWindow::createWindow()->show();
        return;
    }

    MainWindow *mainwindow = MainWindow::m_list.count() > 0 ? MainWindow::m_list[0] : MainWindow::createWindow();
    mainwindow->setProperty("loading", true);
    foreach (QString filePath, filePathList) {
        if (mainwindow->property("windowClosed").toBool())
            break;

        //如果存在则活跃该窗口
        if (!MainWindow::activateSheetIfExist(filePath)) {
            mainwindow->setWindowState((MainWindow::m_list[0]->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
            mainwindow->addFile(filePath);
        }
    }

    mainwindow->setProperty("loading", false);
}

DBusObject::DBusObject(QObject *parent) : QObject(parent)
{

}

DBusObject::~DBusObject()
{
    m_blockShutdownReply = QDBusReply<QDBusUnixFileDescriptor>();
    m_isBlockShutdown = false;

    if (nullptr != m_blockShutdownInterface)
        delete m_blockShutdownInterface;

    if (nullptr != m_keyboardInterface)
        delete m_keyboardInterface;
}
