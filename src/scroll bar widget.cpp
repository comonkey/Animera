//
//  scroll bar widget.cpp
//  Pixel 2
//
//  Created by Indi Kernick on 21/6/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#include "scroll bar widget.hpp"

#include "config.hpp"
#include <QtGui/qpainter.h>
#include <QtWidgets/qabstractscrollarea.h>

namespace {

int documentLength(const QScrollBar *bar) {
  return bar->maximum() - bar->minimum() + bar->pageStep();
}

int scrollRange(const QScrollBar *bar) {
  return bar->maximum() - bar->minimum();
}

int pagePixels(const QScrollBar *bar, const int pixelLength) {
  return (pixelLength * bar->pageStep()) / documentLength(bar);
}

int scrollRangePixels(const QScrollBar *bar, const int pixelLength) {
  return pixelLength - pagePixels(bar, pixelLength);
}

int valuePixels(const QScrollBar *bar, const int pixelLength) {
  if (bar->minimum() == bar->maximum()) {
    return 0;
  } else {
    return (scrollRangePixels(bar, pixelLength) * bar->value()) / scrollRange(bar);
  }
}

}

ScrollBarWidget::ScrollBarWidget(Qt::Orientation orient, QWidget *parent)
  : QScrollBar{orient, parent} {
  const QString sizeStr = QString::number(glob_scroll_width);
  setStyleSheet("width:" + sizeStr + ";height:" + sizeStr);
}
  
void ScrollBarWidget::paintEvent(QPaintEvent *) {
  QPainter painter{this};
  painter.fillRect(rect(), glob_scroll_back);
  if (orientation() == Qt::Horizontal) {
    painter.fillRect(QRect{
      valuePixels(this, width()),
      0,
      pagePixels(this, width()),
      height()
    }, glob_scroll_handle);
  } else if (orientation() == Qt::Vertical) {
    painter.fillRect(QRect{
      0,
      valuePixels(this, height()),
      width(),
      pagePixels(this, height())
    }, glob_scroll_handle);
  } else {
    Q_UNREACHABLE();
  }
}

ScrollCornerWidget::ScrollCornerWidget(QWidget *parent)
  : QWidget{parent} {}
  
void ScrollCornerWidget::paintEvent(QPaintEvent *) {
  QPainter painter{this};
  painter.fillRect(rect(), glob_scroll_corner);
}

ScrollAreaWidget::ScrollAreaWidget(QWidget *parent)
  : QScrollArea{parent} {
  setVerticalScrollBar(new ScrollBarWidget{Qt::Vertical, this});
  setHorizontalScrollBar(new ScrollBarWidget{Qt::Horizontal, this});
  setCornerWidget(new ScrollCornerWidget{this});
}

void ScrollAreaWidget::adjustMargins() {
  QWidget *view = widget();
  if (view->width() < width() && view->height() < height()) {
    setViewportMargins(0, 0, 0, 0);
  } else {
    const QMargins margins = viewportMargins();
    const int right = height() < view->height() + margins.bottom() ? glob_scroll_width : 0;
    const int bottom = width() < view->width() + margins.right() ? glob_scroll_width : 0;
    setViewportMargins(0, 0, right, bottom);
  }
}
