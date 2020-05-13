/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     leiyu
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
#include "BookMarkWidget.h"
#include "DocSheet.h"
#include "WidgetHeader.h"
#include "pdfControl/docview/docummentproxy.h"
#include "lpreviewControl/ImageListview.h"
#include "lpreviewControl/ImageViewModel.h"
#include "bookmarkdelegate.h"

#include <DHorizontalLine>

const int LEFTMINHEIGHT = 80;
BookMarkWidget::BookMarkWidget(DocSheet *sheet, DWidget *parent)
    : CustomWidget(parent), m_sheet(sheet)
{
    initWidget();
    onUpdateTheme();
}

BookMarkWidget::~BookMarkWidget()
{

}

void BookMarkWidget::initWidget()
{
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &BookMarkWidget::onUpdateTheme);

    m_pImageListView = new ImageListView(m_sheet, this);
    m_pImageListView->setListType(DR_SPACE::E_BOOKMARK_WIDGET);
    BookMarkDelegate *imageDelegate = new BookMarkDelegate(m_pImageListView);
    m_pImageListView->setItemDelegate(imageDelegate);

    m_pAddBookMarkBtn = new DPushButton(this);
    m_pAddBookMarkBtn->setFixedHeight(36);
    m_pAddBookMarkBtn->setMinimumWidth(170);
    m_pAddBookMarkBtn->setText(tr("Add bookmark"));
    DFontSizeManager::instance()->bind(m_pAddBookMarkBtn, DFontSizeManager::T6);
    connect(m_pAddBookMarkBtn, SIGNAL(clicked()), this, SLOT(onAddBookMarkClicked()));

    QHBoxLayout *pHBoxLayout = new QHBoxLayout;
    pHBoxLayout->setContentsMargins(10, 6, 10, 6);
    pHBoxLayout->addWidget(m_pAddBookMarkBtn);

    QVBoxLayout *pVBoxLayout = new QVBoxLayout;
    pVBoxLayout->setContentsMargins(0, 10, 0, 0);
    pVBoxLayout->setSpacing(0);

    pVBoxLayout->addWidget(m_pImageListView);
    pVBoxLayout->addWidget(new DHorizontalLine(this));
    pVBoxLayout->addItem(pHBoxLayout);
    this->setLayout(pVBoxLayout);

    connect(m_pImageListView, SIGNAL(sigListMenuClick(const int &)), SLOT(onListMenuClick(const int &)));
    m_pImageListView->setItemSize(QSize(LEFTMINWIDTH * 1.0, LEFTMINHEIGHT * 1.0));
}


void BookMarkWidget::prevPage()
{
    if (m_sheet.isNull())
        return;
    int curPage = m_pImageListView->currentIndex().row() - 1;
    if (curPage < 0)
        return;
    m_sheet->jumpToIndex(m_pImageListView->getPageIndexForModelIndex(curPage));
}

void BookMarkWidget::nextPage()
{
    int curPage = m_pImageListView->currentIndex().row() + 1;
    if (curPage >= m_pImageListView->model()->rowCount())
        return;
    m_sheet->jumpToIndex(m_pImageListView->getPageIndexForModelIndex(curPage));
}

void BookMarkWidget::handleOpenSuccess()
{
    const QList<int> &pageList = dApp->m_pDBService->getBookMarkList(m_sheet->filePath());
    int nCurPage = m_sheet->currentIndex();
    for (int iPage : pageList) {
        m_sheet->setBookMarkState(iPage, true);
        if (iPage == nCurPage) m_pAddBookMarkBtn->setEnabled(false);
    }
    m_pImageListView->handleOpenSuccess();
}

void BookMarkWidget::handlePage(int page)
{
    bool result = m_pImageListView->scrollToIndex(page);
    m_pAddBookMarkBtn->setDisabled(result);
}

void BookMarkWidget::handleBookMark(int page, int state)
{
    if (state) {
        //addPageIndex
        const QString &sPath = m_sheet->filePath();
        QList<int> pageList = dApp->m_pDBService->getBookMarkList(sPath);
        if (pageList.contains(page)) {
            return;
        }

        pageList.append(page);
        dApp->m_pDBService->setBookMarkList(sPath, pageList);
        m_sheet->setFileChanged(true);

        int nCurPage = m_sheet->currentIndex();
        if (nCurPage == page) {
            m_sheet->setBookMarkState(page, true);
            m_pAddBookMarkBtn->setEnabled(false);
        }
        emit sigSetBookMarkState(true, page);
        m_pImageListView->getImageModel()->insertPageIndex(page);
    } else {
        //deletePageIndex
        QList<int> pageList = dApp->m_pDBService->getBookMarkList(m_sheet->filePath());
        if (!pageList.contains(page)) {
            return;
        }

        pageList.removeAll(page);
        dApp->m_pDBService->setBookMarkList(m_sheet->filePath(), pageList);
        m_sheet->setFileChanged(true);

        m_sheet->showTips(tr("The bookmark has been removed"));
        m_sheet->setBookMarkState(page, false);
        int nCurPage = m_sheet->currentIndex();
        if (nCurPage == page) {
            m_pAddBookMarkBtn->setEnabled(true);
        }
        emit sigSetBookMarkState(false, page);
        m_pImageListView->getImageModel()->removePageIndex(page);
    }
    scrollToCurrentPage();
}

void BookMarkWidget::handleRotate(int)
{
    //Not toDO
}

void BookMarkWidget::DeleteItemByKey()
{
    int curPage = m_pImageListView->getPageIndexForModelIndex(m_pImageListView->currentIndex().row());
    if (curPage >= 0)
        handleBookMark(curPage, false);
}

void BookMarkWidget::onAddBookMarkClicked()
{
    if (m_sheet.isNull()) return;
    int nPage = m_sheet->currentIndex();
    handleBookMark(nPage, true);
}

void BookMarkWidget::adaptWindowSize(const double &scale)
{
    m_pImageListView->setProperty("adaptScale", scale);
    m_pImageListView->setItemSize(QSize(LEFTMINWIDTH * scale, LEFTMINHEIGHT));
    m_pImageListView->reset();
    scrollToCurrentPage();
}

void BookMarkWidget::updateThumbnail(const int &page)
{
    m_pImageListView->getImageModel()->updatePageIndex(page);
}

void BookMarkWidget::onUpdateTheme()
{
    DPalette plt = Dtk::Gui::DGuiApplicationHelper::instance()->applicationPalette();
    plt.setColor(Dtk::Gui::DPalette::Background, plt.color(Dtk::Gui::DPalette::Base));
    setPalette(plt);
    m_pAddBookMarkBtn->setPalette(plt);
}

void BookMarkWidget::scrollToCurrentPage()
{
    m_pImageListView->scrollToIndex(m_sheet->currentIndex(), false);
}

void BookMarkWidget::onListMenuClick(const int &iType)
{
    if (iType == E_BOOKMARK_DELETE) {
        DeleteItemByKey();
    }
}
