//
//  editor widget.cpp
//  Pixel 2
//
//  Created by Indi Kernick on 10/3/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#include "editor widget.hpp"

#include "config.hpp"
#include "composite.hpp"
#include <QtGui/qevent.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qscrollbar.h>

class EditorImage final : public QWidget {
  Q_OBJECT
  
public:
  explicit EditorImage(QScrollArea *parent)
    : QWidget{parent}, parent{parent} {
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    initCursor();
    setCursor(cursor);
    connect(parent->horizontalScrollBar(), &QScrollBar::valueChanged, this, &EditorImage::updateMouse);
    connect(parent->verticalScrollBar(), &QScrollBar::valueChanged, this, &EditorImage::updateMouse);
  }
  
  void setImage(const QImage &img) {
    if (editor.size() != img.size()) {
      resize(img.size());
    }
    [[maybe_unused]] bool converted = editor.convertFromImage(img);
    assert(converted);
    updatePixmap();
  }
  void zoomIn() {
    const int oldScale = scale;
    scale = std::min(scale + 1, edit_max_scale);
    updatePixmap();
    adjustScroll(oldScale);
    updateMouse();
  }
  void zoomOut() {
    const int oldScale = scale;
    scale = std::max(scale - 1, edit_min_scale);
    updatePixmap();
    adjustScroll(oldScale);
    updateMouse();
  }
  
Q_SIGNALS:
  void mouseLeave();
  void mouseDown(QPoint, ButtonType, QImage *);
  void mouseMove(QPoint, QImage *);
  void mouseUp(QPoint, ButtonType, QImage *);
  void keyPress(Qt::Key, QImage *);
  
private:
  QScrollArea *parent;
  QCursor cursor;
  QPixmap checkers;
  QImage overlay;
  QPixmap editor;
  QPoint pos;
  int scale = edit_default_scale;
  int keysDown = 0;

  void adjustScroll(const int oldScale) {
    if (scale == oldScale) return;
    QScrollBar *hbar = parent->horizontalScrollBar();
    QScrollBar *vbar = parent->verticalScrollBar();
    if (width() >= parent->width()) {
      const int halfWidth = parent->width() / 2;
      hbar->setValue((hbar->value() + halfWidth) * scale / oldScale - halfWidth);
    }
    if (height() >= parent->height()) {
      const int halfHeight = parent->height() / 2;
      vbar->setValue((vbar->value() + halfHeight) * scale / oldScale - halfHeight);
    }
  }
  
  void updateMouse() {
    pos = getPos();
    Q_EMIT mouseMove(pos, &overlay);
  }

  void initCursor() {
    cursor = QCursor{
      QBitmap{":/Cursors/circle b.pbm"}.scaled(edit_cursor_size),
      QBitmap{":/Cursors/circle m.pbm"}.scaled(edit_cursor_size),
      edit_cursor_offset.x(),
      edit_cursor_offset.y()
    };
  }

  void resize(const QSize newSize) {
    updateCheckers(newSize);
    overlay = QImage{newSize, QImage::Format_ARGB32};
    clearImage(overlay);
  }

  void updateCheckers(QSize newSize) {
    checkers = QPixmap{newSize * 2};
    QPainter painter{&checkers};
    const QRect rect{{}, newSize * 2};
    painter.fillRect(rect, edit_checker_a);
    painter.fillRect(rect, {edit_checker_b, Qt::Dense4Pattern});
  }

  void updatePixmap() {
    setFixedSize(editor.size() * scale);
    repaint();
  }
  
  void paintEvent(QPaintEvent *) override {
    QPainter painter{this};
    const QRect rect{{}, editor.size() * scale};
    painter.drawPixmap(rect, checkers);
    painter.drawPixmap(rect, editor);
    painter.drawImage(rect, overlay);
  }
  
  QPoint getPixelPos(const QPointF localPos) {
    const QPointF posF = localPos / scale;
    return {static_cast<int>(posF.x()), static_cast<int>(posF.y())};
  }
  QPoint getPos(QMouseEvent *event) {
    return getPixelPos(event->localPos());
  }
  QPoint getPos() {
    return getPixelPos(mapFromGlobal(QCursor::pos()));
  }
  ButtonType getButton(QMouseEvent *event) {
    switch (event->button()) {
      case mouse_primary:   return ButtonType::primary;
      case mouse_secondary: return ButtonType::secondary;
      case mouse_tertiary:  return ButtonType::erase;
      default: Q_UNREACHABLE();
    }
  }
  ButtonType getButton(QKeyEvent *event) {
    switch (event->key()) {
      case key_primary:   return ButtonType::primary;
      case key_secondary: return ButtonType::secondary;
      case key_tertiary:  return ButtonType::erase;
      default: return ButtonType::none;
    }
  }

  void mousePressEvent(QMouseEvent *event) override {
    pos = getPos(event);
    Q_EMIT mouseDown(pos, getButton(event), &overlay);
  }
  void mouseReleaseEvent(QMouseEvent *event) override {
    pos = getPos(event);
    Q_EMIT mouseUp(pos, getButton(event), &overlay);
  }
  void mouseMoveEvent(QMouseEvent *event) override {
    pos = getPos(event);
    Q_EMIT mouseMove(pos, &overlay);
  }
  void enterEvent(QEvent *) override {
    setFocus();
  }
  void leaveEvent(QEvent *) override {
    clearImage(overlay);
    repaint();
    Q_EMIT mouseLeave();
  }

public:
  void keyPressEvent(QKeyEvent *event) override {
    if (!event->isAutoRepeat()) {
      ++keysDown;
      if (keysDown == 1) grabMouse(cursor);
    }
    if (event->key() == key_zoom_out) {
      zoomOut();
    } else if (event->key() == key_zoom_in) {
      zoomIn();
    } else if (ButtonType button = getButton(event); button != ButtonType::none) {
      if (!event->isAutoRepeat()) {
        Q_EMIT mouseDown(pos, button, &overlay);
      }
    } else {
      Q_EMIT keyPress(static_cast<Qt::Key>(event->key()), &overlay);
    }
  }
  void keyReleaseEvent(QKeyEvent *event) override {
    --keysDown;
    if (keysDown == 0) releaseMouse();
    const ButtonType button = getButton(event);
    if (button != ButtonType::none) {
      Q_EMIT mouseUp(pos, button, &overlay);
    }
  }
};

EditorWidget::EditorWidget(QWidget *parent, Animation &anim)
  : QScrollArea{parent}, anim{anim}, view{new EditorImage{this}} {
  setWidget(view);
  connect(view, &EditorImage::mouseLeave, this, &EditorWidget::mouseLeave);
  connect(view, &EditorImage::mouseDown,  this, &EditorWidget::mouseDown);
  connect(view, &EditorImage::mouseMove,  this, &EditorWidget::mouseMove);
  connect(view, &EditorImage::mouseUp,    this, &EditorWidget::mouseUp);
  connect(view, &EditorImage::keyPress,   this, &EditorWidget::keyPress);
  setAlignment(Qt::AlignCenter);
  setFocusPolicy(Qt::WheelFocus);
}

void EditorWidget::composite() {
  view->setImage(compositeFrame(anim.getPallete(), anim.getFrame(frame), visibility));
}

void EditorWidget::compositeOverlay() {
  view->repaint();
}

void EditorWidget::compositePos(Cell *, LayerIdx newLayer, FrameIdx newFrame) {
  layer = newLayer;
  frame = newFrame;
  composite();
}

void EditorWidget::compositeVis(const LayerVisible &newVisibility) {
  visibility = newVisibility;
  composite();
}

#include "editor widget.moc"
