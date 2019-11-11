#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QColor>
#include <QPoint>
#include <QRect>

/**
 * @brief The DataManager class
 * @brief   固定数据管理区， 采用 单例
 */

//  文档显示状态
enum File_Show_Enum {
    FILE_NORMAL,        //  正常显示
    FILE_FULLSCREEN,    //  全屏
    FILE_SLIDE,         //  幻灯片
};

// 窗口类型
enum Widget_Type{
    WIDGET_THUMBNAIL,   //  缩略图
    WIDGET_BOOKMARK,    //  书签
    WIDGET_NOTE,        //  注释
    WIDGET_SEARCH,      //  搜索
};

class DataManager : public QObject
{
    Q_OBJECT
private:
    DataManager(QObject *parent = nullptr);

public:
    static DataManager *instance()
    {
        static DataManager manager;
        return &manager;
    }

    QString strOnlyFilePath() const;
    void setStrOnlyFilePath(const QString &strOnlyFilePath);

    int CurShowState() const;
    void setCurShowState(int nCurShowState);

    bool bIsUpdate() const;
    void setBIsUpdate(bool bIsUpdate);

    QList<QColor>   getLightColorList();

    inline void mousePressLocal(bool &highLight, QPoint &point)
    {
        highLight = m_bIsHighLight;
        point = m_point;
    }
    void setMousePressLocal(const bool &highLight, const QPoint &point);

    inline void setScreenRect(const QRect &rect)
    {
        m_screenRect = rect;
    }

    inline void setSmallNoteWidgetSize(const QSize &size)
    {
        m_smallNoteSize = size;
    }

    QString gettrCurrentTheme() const;
    void settrCurrentTheme(const QString &strCurrentTheme);

    inline QColor selectColor()
    {
        return m_selectColor;
    }
    inline void setSelectColor(const QColor&color)
    {
        m_selectColor = color;
    }

    inline int currentWidget()
    {
        return m_nCurrentWidget;
    }
    inline void setCurrentWidget(const int&index)
    {
        m_nCurrentWidget = index;
    }

private:
    QString m_strCurrentTheme = "";     //  当前主题
    QString m_strOnlyFilePath = "";     //  只显示一个pdf 文件
    int     m_nCurShowState = -1;       //  文档当前显示状态
    bool    m_bIsUpdate = false;        //  文档是否有修改
    QList<QColor> m_listColor;          //  color list
    bool m_bIsHighLight = false;        // 鼠标左键点击位置有没有高亮
    QPoint m_point;                     // 鼠标左键点击位置
    QRect m_screenRect;                 // 屏幕的分辨率
    QSize m_smallNoteSize;              // 注释小窗体的大小
    QColor m_selectColor;               // 高亮颜色
    int m_nCurrentWidget = WIDGET_THUMBNAIL; // 当前焦点处于那个窗体上（左侧缩略图）
};

#endif // DATAMANAGER_H
