//
//  tool colors widget.hpp
//  Pixel 2
//
//  Created by Indi Kernick on 30/4/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef tool_colors_widget_hpp
#define tool_colors_widget_hpp

#include "tool.hpp"
#include <QtWidgets/qwidget.h>

class ColorHandle;
class ActiveColorWidget;

class ToolColorsWidget final : public QWidget {
  Q_OBJECT
  
public:
  explicit ToolColorsWidget(QWidget *);

public Q_SLOTS:
  void initCanvas(Format);
  void setPalette(PaletteCSpan);
  void setIndex(int);
  void changePaletteColors();

Q_SIGNALS:
  void colorsChanged(ToolColors);
  void attachColor(ColorHandle *);
  void attachIndex(int);

private Q_SLOTS:
  void changeColors();

private:
  ToolColors colors;
  ActiveColorWidget *primary = nullptr;
  ActiveColorWidget *secondary = nullptr;
  ActiveColorWidget *erase = nullptr;
  
  void setupLayout();
  template <auto>
  void toggleColor(bool);
  template <auto>
  void toggleIndex(bool);
  void connectSignals(Format);
  void initColors(Format);
};

#endif
