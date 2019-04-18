//
//  color picker widget.hpp
//  Pixel 2
//
//  Created by Indi Kernick on 7/4/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef color_picker_widget_hpp
#define color_picker_widget_hpp

#include "tool.hpp"
#include <QtWidgets/qwidget.h>

class SVGraph;
class HueSlider;
class AlphaSlider;

class ColorPickerWidget final : public QWidget {
  Q_OBJECT

public:
  explicit ColorPickerWidget(QWidget *);

Q_SIGNALS:
  void colorsModified(ToolColors);

private:
  SVGraph *svGraph;
  HueSlider *hueSlider;
  AlphaSlider *alphaSlider;
  
  void setupLayout();
  void connectSignals();
};

#endif
