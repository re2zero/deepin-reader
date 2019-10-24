#include "NotesItemWidget.h"
#include <QDebug>
#include "translator/PdfControl.h"
#include <DApplication>
#include <QClipboard>

NotesItemWidget::NotesItemWidget(CustomItemWidget *parent) :
    CustomItemWidget(QString("NotesItemWidget"), parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slotShowContextMenu(const QPoint &)));

    initWidget();
}

void NotesItemWidget::setTextEditText(const QString &contant)
{
    if (m_pTextEdit) {
        m_pTextEdit->clear();
        m_pTextEdit->setText(contant);
    }
}

void NotesItemWidget::setSerchResultText(const QString &result)
{
    if (m_pSearchResultNum) {
        m_pSearchResultNum->setText(result);
    }
}

void NotesItemWidget::slotDltNoteContant()
{
//    if(m_pTextEdit){
//        DocummentProxy *dproxy = DocummentProxy::instance();
//        if (nullptr == dproxy) {
//            return;
//        }
//        if(dproxy){
//            dproxy->removeAnnotation(m_strUUid);

//            m_pTextEdit->clear();
//            sendMsg(MSG_NOTE_DLTNOTEITEM, m_strUUid);
//        }
//    }
    sendMsg(MSG_NOTE_DLTNOTEITEM, m_strUUid);
}

void NotesItemWidget::slotCopyContant()
{
    if (m_pTextEdit) {
        QString str = m_pTextEdit->document()->toPlainText();
        if (str != QString("")) {
            QClipboard *clipboard = DApplication::clipboard();   //获取系统剪贴板指针
            clipboard->setText(str);
        }
    }
}

void NotesItemWidget::slotShowContextMenu(const QPoint &)
{
    if(!m_isNote){
        return;
    }
    QMenu *t_menu = new QMenu(this);
    QAction *copyAction = t_menu->addAction(PdfControl::COPY_CONT);
    QAction *dltItemAction = t_menu->addAction(PdfControl::DLT_NOTE);
    connect(dltItemAction, SIGNAL(triggered()), this, SLOT(slotDltNoteContant()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(slotCopyContant()));
    t_menu->exec(QCursor::pos());
}

void NotesItemWidget::initWidget()
{
    auto t_vLayout = new QVBoxLayout;
    t_vLayout->setContentsMargins(0, 0, 0, 0);
    t_vLayout->setSpacing(0);

    auto t_hLayout = new QHBoxLayout;
    t_hLayout->setContentsMargins(0, 0, 0, 0);
    t_hLayout->setSpacing(0);

    m_pPicture = new CustomLabel(this);
    m_pPicture->setFixedSize(QSize(40, 60));
    m_pPicture->setAlignment(Qt::AlignCenter);

    m_pPage = new DLabel;
    m_pSearchResultNum = new DLabel;
    m_pTextEdit = new DTextEdit;
    m_pTextEdit->setEnabled(false);
    m_pTextEdit->setFixedHeight(51);
    m_pTextEdit->setMinimumWidth(102);
    m_pTextEdit->setFrameStyle(QFrame::NoFrame);
//    m_pTextEdit->setTextBackgroundColor(this->palette().highlightedText().color());

    t_hLayout->addWidget(m_pPage);
    t_hLayout->addWidget(m_pSearchResultNum);

    t_vLayout->addItem(t_hLayout);
    t_vLayout->addWidget(m_pTextEdit);

    auto m_pHLayout = new QHBoxLayout;

    m_pHLayout->setSpacing(1);
    m_pHLayout->setContentsMargins(1, 0, 1, 0);
    m_pHLayout->addWidget(m_pPicture);
    m_pHLayout->addItem(t_vLayout);
    m_pHLayout->setSpacing(1);

    this->setLayout(m_pHLayout);
}

int NotesItemWidget::dealWithData(const int &, const QString &)
{
    return 0;
}
