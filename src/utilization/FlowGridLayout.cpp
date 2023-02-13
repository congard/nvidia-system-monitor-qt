// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QWidget>

#include "FlowGridLayout.h"

FlowGridLayout::FlowGridLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowGridLayout::FlowGridLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowGridLayout::~FlowGridLayout() {
    QLayoutItem *item;

    while ((item = takeAt(0))) {
        delete item;
    }
}

void FlowGridLayout::addItem(QLayoutItem *item) {
    itemList.append(item);
}

void FlowGridLayout::addLayout(QLayout *layout) {
    addItem(layout);
    addChildLayout(layout);
}

int FlowGridLayout::horizontalSpacing() const {
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowGridLayout::verticalSpacing() const {
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int FlowGridLayout::count() const {
    return itemList.size();
}

QLayoutItem *FlowGridLayout::itemAt(int index) const {
    return itemList.value(index);
}

QLayoutItem *FlowGridLayout::takeAt(int index) {
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowGridLayout::expandingDirections() const {
    return {};
}

bool FlowGridLayout::hasHeightForWidth() const {
    return true;
}

int FlowGridLayout::heightForWidth(int width) const {
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void FlowGridLayout::setGeometry(const QRect &rect) {
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowGridLayout::sizeHint() const {
    return minimumSize();
}

QSize FlowGridLayout::minimumSize() const {
    QSize size;

    for (const QLayoutItem *item : itemList)
        size = size.expandedTo(item->minimumSize());

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());

    auto layoutSize = this->contentsRect();
    size.setHeight(heightForWidth(layoutSize.width()));

    return size;
}

int FlowGridLayout::doLayout(const QRect &rect, bool testOnly) const {
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);

    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    QSize cellSize(-1, -1);

    for (QLayoutItem *item : itemList)
        cellSize = cellSize.expandedTo(item->sizeHint());

    for (QLayoutItem *item : itemList) {
        const QWidget *wid = item->widget();

        int spaceX = horizontalSpacing();
        int spaceY = verticalSpacing();

        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);

        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);

        int nextX = x + cellSize.width() + spaceX;

        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + cellSize.width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        lineHeight = qMax(lineHeight, cellSize.height());
    }

    return y + lineHeight - rect.y() + bottom;
}

int FlowGridLayout::smartSpacing(QStyle::PixelMetric pm) const {
    QObject *parent = this->parent();

    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        auto pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    } else {
        return static_cast<QLayout*>(parent)->spacing();
    }
}
