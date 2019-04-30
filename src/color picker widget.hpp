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
#include "color convert.hpp"
#include <QtWidgets/qwidget.h>

class SVGraphWidget;
class HueSliderWidget;
class AlphaSliderWidget;
class NumberInputWidget;
class HexInputWidget;
class LabelWidget;

class ColorPickerWidget final : public QWidget {
  Q_OBJECT

public:
  explicit ColorPickerWidget(QWidget *);

Q_SIGNALS:
  void colorsModified(ToolColors);

private:
  HSV colorHsv;
  RGB colorRgb;
  int alpha;
  
  SVGraphWidget *svGraph;
  HueSliderWidget *hueSlider;
  AlphaSliderWidget *alphaSlider;
  
  NumberInputWidget *boxR;
  NumberInputWidget *boxG;
  NumberInputWidget *boxB;
  NumberInputWidget *boxH;
  NumberInputWidget *boxS;
  NumberInputWidget *boxV;
  NumberInputWidget *boxA;
  HexInputWidget *boxHex;
  
  LabelWidget *labelR;
  LabelWidget *labelG;
  LabelWidget *labelB;
  LabelWidget *labelH;
  LabelWidget *labelS;
  LabelWidget *labelV;
  LabelWidget *labelA;
  LabelWidget *labelHex;
  
  void setupLayout();
  void connectSignals();

  void changeRGB();
  void changeHSV();

private Q_SLOTS:
  void changeSVfromGraph(int, int);
  void changeSVfromBoxS(int);
  void changeSVfromBoxV(int);
  void changeAlpha(int);
  void changeHue(int);
  void changeRGBA(RGB, int);
  void changeRed(int);
  void changeGreen(int);
  void changeBlue(int);
};

#endif
