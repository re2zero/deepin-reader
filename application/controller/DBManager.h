/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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
#ifndef DBMANAGER_H
#define DBMANAGER_H

// BookMarkTable
///////////////////////////////////////////////////////
//FilePath           | FileName | PageNumber        | Time  //
//TEXT primari key   | TEXT     | TEXT       | TEXT  //
///////////////////////////////////////////////////////


#include <QSqlDatabase>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

class DBManager : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(DBManager)

public:
    explicit DBManager(QObject *parent = nullptr);
    virtual ~DBManager();

protected:
    const QSqlDatabase getDatabase() const;
    void checkDatabase();

public:
    void setStrFilePath(const QString &strFilePath);

    void getBookMarks();     //  获取给文件 所有标签的页码
    void saveBookMark();

    QList<int> getBookMarkList() const;

    void setBookMarkList(const QList<int> &pBookMarkList);

    bool saveasBookMark(const QString &oldpath, const QString &newpath);

private:
    void insertBookMark(const QString &, const QString &strFilePath = "", const QString &strFileName = "");
    void updateBookMark(const QString &);
    void deleteBookMark();

    void clearInvalidRecord();

protected:
    bool hasFilePathDB(const QString &);

private:
    QList<int>      m_pBookMarkList;
    int             m_nDbType = -1;   //  原来是否有数据

    QString m_connectionName = "";

protected:
    QString m_strFilePath = "";
    QString m_strFileName = "";
    mutable QMutex m_mutex;
};

#endif // DBMANAGER_H
