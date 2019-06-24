//
//  timeline widget.hpp
//  Pixel 2
//
//  Created by Indi Kernick on 10/3/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef timeline_widget_hpp
#define timeline_widget_hpp

#include "animation.hpp"
#include <QtWidgets/qwidget.h>

class LayerCellsWidget;
class ControlsWidget;
class LayersWidget;
class FramesWidget;
class CellsWidget;
class LayerScrollWidget;
class FrameScrollWidget;
class CellScrollWidget;

class TimelineWidget final : public QWidget {
  Q_OBJECT
  
  friend LayerCellsWidget;

public:
  explicit TimelineWidget(QWidget *);
  
  void createInitialCell();
  
public Q_SLOTS:
  void initialize(QSize, Format);
  void save(const QString &) const;
  void load(const QString &);
  void paletteChanged(Palette *);

  /*
  create layer above current
  remove current layer
  move current layer up
  move current layer down
  create frame after current
  remove current frame
  toggle playing animation
  
  set position to newly created layers and frames
  */

Q_SIGNALS:
  // emitted when the current layer/frame has changed
  // or the current cell has changed
  void posChange(Cell *, LayerIdx, FrameIdx);
  // @TODO frame changes when visibility changes
  void frameChanged(const Frame &);

private:
  ControlsWidget *controls = nullptr;
  LayersWidget *layers = nullptr;
  FramesWidget *frames = nullptr;
  CellsWidget *cells = nullptr;
  Palette *palette = nullptr;
  QSize size;
  Format format;
};

#endif
