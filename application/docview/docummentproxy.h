#ifndef DOCUMMENTPROXY_H
#define DOCUMMENTPROXY_H
#include "docummentbase.h"
#include "commonstruct.h"
#include <QObject>
#include <DWidget>

enum DocType_EM {
    DocType_NULL = 0,
    DocType_PDF,
    DocType_DJVU,
    DocType_TIFF,
    DocType_PS,
    DocType_XPS

};

class DocummentProxy: public QObject
{
    Q_OBJECT
public:
    static DocummentProxy *instance(QObject *parent = nullptr);
    bool openFile(DocType_EM type, QString filepath);
    bool closeFile();
    QPoint global2RelativePoint(QPoint globalpoint);
    bool setSelectTextStyle(QColor paintercolor = QColor(72, 118, 255, 100), QColor pencolor = QColor(72, 118, 255, 0), int penwidth = 0);
    bool mouseSelectText(QPoint start, QPoint stop);
    void mouseSelectTextClear();
    bool mouseBeOverText(QPoint point);
    void scaleRotateAndShow(double scale, RotateType_EM rotate);
    bool getImage(int pagenum, QImage &image, double width, double height);
    int  getPageSNum();
    bool setViewModeAndShow(ViewMode_EM viewmode);
    bool showMagnifier(QPoint point);
    bool closeMagnifier();
    bool setMagnifierStyle(QColor magnifiercolor = Qt::white, int magnifierradius = 100, int magnifierringwidth = 10, double magnifierscale = 2);
    QString addAnnotation(const QPoint &startpos, const QPoint &endpos, QColor color = Qt::yellow);
    bool save(const QString &filepath, bool withChanges);
    bool saveas(const QString &filepath, bool withChanges);
    void search(const QString &strtext, QMap<int, stSearchRes> &resmap, const QColor &color);
    void clearsearch();
    int currentPageNo();
    bool pageJump(int pagenum);
    void docBasicInfo(stFileInfo &info);
    QString removeAnnotation(const QPoint &startpos);
    void removeAnnotation(const QString &struuid, int ipage = -1);
    bool pageMove(double mvx, double mvy);
    void title(QString &title);
    Page::Link *mouseBeOverLink(QPoint point);
    bool getSelectTextString(QString &st);
    bool showSlideModel();
    bool exitSlideModel();
    void findNext();
    void findPrev();
    void closeFileAndWaitThreadClearEnd();
    void setAnnotationText(int ipage, const QString &struuid, const QString &strtext);
    void getAnnotationText(const QString &struuid, QString &strtext, int ipage);
    double adaptWidthAndShow(double width);
    double adaptHeightAndShow(double height);
    bool annotationClicked(const QPoint &pos, QString &strtext, QString &struuid);
    void getAllAnnotation(QList<stHighlightContent> &listres);
    int  pointInWhichPage(QPoint pos);
    void jumpToHighLight(const QString &uuid, int ipage);
    bool setBookMarkState(int page, bool state);
    void changeAnnotationColor(int ipage, const QString uuid, const QColor &color);
    void selectAllText();
    void setAutoPlaySlide(bool autoplay, int timemsec = 3000);
    bool getAutoPlaySlideStatu();
signals:
    void signal_pageChange(int);
    bool signal_pageJump(int);
    void signal_searchRes(stSearchRes);
    void signal_searchover();
    bool signal_mouseSelectText(QPoint start, QPoint stop);
    void signal_scaleAndShow(double scale, RotateType_EM rotate);
    bool signal_setViewModeAndShow(ViewMode_EM viewmode);
    void signal_bookMarkStateChange(int page, bool state);
    void signal_openResult(bool);
private slots:
    void slot_pageChange(int);
//    bool startOpenFile();

private:
    DocummentProxy(QObject *parent = nullptr);
    ~DocummentProxy()
    {
        qDebug() << "----~DocummentProxy-------";
        closeFileAndWaitThreadClearEnd();
//        if (threadwaitloadwordsend.isRunning()) {
//            threadwaitloadwordsend.wait();
//        }
    }
    DWidget *qwfather;
    DocType_EM m_type;
    QString m_path;
    DocummentBase *m_documment;
//    ThreadWaitLoadWordsEnd threadwaitloadwordsend;
//    bool bclosefile;
    bool bcloseing;
    static  DocummentProxy *s_pDocProxy;
};

#endif // DOCUMMENTPROXY_H
