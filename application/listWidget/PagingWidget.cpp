#include "PagingWidget.h"
#include <QDebug>

PagingWidget::PagingWidget(CustomWidget *parent) :
    CustomWidget(parent)
{
    resize(250, 20);
    initWidget();

    connect(m_pPrePageBtn, SIGNAL(clicked()), this, SLOT(slotPrePage()));
    connect(m_pNextPageBtn, SIGNAL(clicked()), this, SLOT(slotNextPage()));
}

void PagingWidget::initWidget()
{
    m_pTotalPagesLab = new DLabel(this);
    m_pTotalPagesLab->setText(QString("/xxx页"));
    m_pTotalPagesLab->setMinimumWidth(80);

    m_pPrePageBtn = new DPushButton(this);
    m_pPrePageBtn->setText(QString("<"));
    m_pPrePageBtn->setFixedSize(QSize(40, 40));

    m_pNextPageBtn = new DPushButton(this);
    m_pNextPageBtn->setText(QString(">"));
    m_pNextPageBtn->setFixedSize(QSize(40, 40));

    m_pJumpPageSpinBox = new DSpinBox(this);
    m_pJumpPageSpinBox->setRange(1, 100);
    m_pJumpPageSpinBox->setValue(1);
    m_pJumpPageSpinBox->setMinimumWidth(50);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_pJumpPageSpinBox);
    hLayout->addWidget(m_pTotalPagesLab);
    hLayout->addWidget(m_pPrePageBtn);
    hLayout->addWidget(m_pNextPageBtn);
    this->setLayout(hLayout);

    setTotalPages(30);
}

void PagingWidget::setCurrentPage(const int &index)
{
    m_pJumpPageSpinBox->setValue(index);

    qDebug() << tr("page: %1").arg(index);
}

void PagingWidget::setTotalPages(int pages)
{
    m_totalPage = pages;
    m_currntPage = FIRSTPAGES;
    m_pTotalPagesLab->setText(QString("/%1页").arg(pages));

    m_pJumpPageSpinBox->setRange(1, (pages < 1) ? 1 : pages);
}

int PagingWidget::dealWithData(const int &msgType, const QString &msgContant)
{
    return 0;
}

void PagingWidget::slotPrePage()
{
    int t_page = --m_currntPage;

    m_pNextPageBtn->setEnabled(true);
    if (t_page <= FIRSTPAGES) {
        m_pPrePageBtn->setEnabled(false);
    }
    if (t_page < FIRSTPAGES) {
        m_currntPage = FIRSTPAGES;
        return;
    }
    m_currntPage = t_page;

    setCurrentPage(m_currntPage);
    sendMsg(MSG_THUMBNAIL_JUMPTOPAGE, QString::number(m_currntPage - FIRSTPAGES));
}

void PagingWidget::slotNextPage()
{
    int t_page = ++m_currntPage;

    m_pPrePageBtn->setEnabled(true);
    if (t_page >= m_totalPage) {
        m_pNextPageBtn->setEnabled(false);
    }
    if (t_page > m_totalPage) {
        m_currntPage = m_totalPage;
        return;
    }
    m_currntPage = t_page;

    setCurrentPage(m_currntPage);
    sendMsg(MSG_THUMBNAIL_JUMPTOPAGE, QString::number(m_currntPage - FIRSTPAGES));
}

void PagingWidget::setCurrentPageValue(const int &index)
{
    m_currntPage = index + FIRSTPAGES;

    if (m_currntPage <= FIRSTPAGES) {
        m_pPrePageBtn->setEnabled(false);
        m_pNextPageBtn->setEnabled(true);
    } else if (m_currntPage >= m_totalPage && m_totalPage != FIRSTPAGES) {
        m_pPrePageBtn->setEnabled(true);
        m_pNextPageBtn->setEnabled(false);
    } else {
        m_pPrePageBtn->setEnabled(true);
        m_pNextPageBtn->setEnabled(true);
    }

    if (m_totalPage == FIRSTPAGES) {
        m_pPrePageBtn->setEnabled(false);
        m_pNextPageBtn->setEnabled(false);
    }

    setCurrentPage(m_currntPage);
}

//void PagingWidget::keyPressEvent(QKeyEvent *event)
//{
//    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
//    {
//        qDebug() << "emit enter event";
//    }
//}
