#ifndef THUMBNAILWIDGET_H
#define THUMBNAILWIDGET_H

#include <DPushButton>

#include <QPalette>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include "CustomControl/CustomWidget.h"
#include "CustomListWidget.h"
#include "PagingWidget.h"
#include "ThumbnailItemWidget.h"

class ThreadRotateImage : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(ThreadRotateImage)

public:
    explicit ThreadRotateImage(QObject *parent = nullptr);
    ~ThreadRotateImage() Q_DECL_OVERRIDE { stopThreadRun(); }

public:
    void stopThreadRun();
    inline void setPages(const int &page) { m_nPages = page; }

signals:
    void sigRotateImage(const int &index);

public:
    inline void setLoadOver() { m_bLoading = true; }

protected:
    void run() Q_DECL_OVERRIDE;

private:
    int m_nFirstIndex = 0;
    int m_nEndIndex = 19;
    int m_nPages = 0;         // 总页数
    bool m_bLoading = false;  // 大缩略是否还在加载
};

/**
 * @brief The ThreadLoadImage class
 * @brief   加载缩略图线程
 */

class ThumbnailWidget;

class ThreadLoadImage : public QThread
{
    Q_OBJECT
    Q_DISABLE_COPY(ThreadLoadImage)

public:
    explicit ThreadLoadImage(QObject *parent = nullptr);
    ~ThreadLoadImage() Q_DECL_OVERRIDE { stopThreadRun(); }

public:
    void stopThreadRun();

    inline void setPages(const int pages) { m_pages = pages; }

    inline int endPage() { return m_nEndPage; }

    inline void setIsLoaded(const bool &load) { m_isLoaded = load; }

    inline bool isLoaded() { return m_isLoaded; }

    inline void setThumbnail(ThumbnailWidget *thumbnail) { m_pThumbnailWidget = thumbnail; }

    inline void setStartAndEndIndex()
    {
        m_nStartPage = 0;  // 加载图片起始页码
        m_nEndPage = 19;   // 加载图片结束页码
    }

signals:
    void signal_loadImage(const int &, const QImage &);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    int m_pages = 0;          // 文件总页数
    bool m_isLoaded = false;  // 是都加载完毕
    ThumbnailWidget *m_pThumbnailWidget = nullptr;
    int m_nStartPage = 0;  // 加载图片起始页码
    int m_nEndPage = 19;   // 加载图片结束页码
};

/*
 *缩略图列表页面
 */
class ThumbnailWidget : public CustomWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(ThumbnailWidget)

public:
    explicit ThumbnailWidget(CustomWidget *parent = nullptr);
    ~ThumbnailWidget() Q_DECL_OVERRIDE;

signals:
    void sigFilePageChanged(const QString &);
    void sigOpenFileOk();
    void sigCloseFile();
    void sigJumpToPrevPage();
    void sigJumpToNextPage();
    void sigSetRotate(int);

public:
    // IObserver interface
    int dealWithData(const int &, const QString &) Q_DECL_OVERRIDE;
    void fillContantToList();

    inline bool isLoading() { return m_isLoading; }
    void showItemBookMark(int ipage = -1);

protected:
    void initWidget() Q_DECL_OVERRIDE;

private:
    void setSelectItemBackColor(QListWidgetItem *);
    void addThumbnailItem(const int &);
    void initConnection();
    void jumpToSpecifiedPage(const int &);

private slots:
    void slotOpenFileOk();
    void slotDocFilePageChanged(const QString &);
    void slotCloseFile();
    void slotUpdateTheme();
    void slotJumpToPrevPage();
    void slotJumpToNextPage();
    void slotSetRotate(int);
    void slotRotateThumbnail(int);

private:
    CustomListWidget *m_pThumbnailListWidget = nullptr;  // list widget item子界面
    PagingWidget *m_pPageWidget = nullptr;               // 跳页界面
    int m_totalPages = -1;                               // 总页码数
    ThreadLoadImage m_ThreadLoadImage;                   // 加载缩略图线程
    bool m_isLoading = false;                            // 缩略图list是否初始化完毕
    ThreadRotateImage m_threadRotateImage;               // 旋转缩略图
    int m_nRotate = 0;                                   // 旋转度数
    int m_nCurrentPage = 0;                              // 当前页码index(从0开始)
};

#endif  // THUMBNAILWIDGET_H
