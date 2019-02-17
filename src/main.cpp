//
//  main.cpp
//  Pixel 2
//
//  Created by Indi Kernick on 3/2/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#include <iostream>
#include <QtGui/qevent.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qscrollarea.h>
#include <QtWidgets/qdockwidget.h>
#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qapplication.h>
#include <QtWidgets/qstylepainter.h>
#include <QtWidgets/qdesktopwidget.h>

/*
In case I decide that we need KC filters

#include <pixman-1/pixman.h>

#define d2f pixman_double_to_fixed
    const double frac = 1.0/5.0;
    pixman_fixed_t filterParams[] = {
      d2f(3), d2f(3),
      d2f(-1), d2f(-1), d2f(0),
      d2f(-1), d2f(0), d2f(1),
      d2f(0), d2f(1), d2f(1)
 
      //d2f(5), d2f(5),
      //d2f(1), d2f(0), d2f(0), d2f(0), d2f(0),
      //d2f(0), d2f(1), d2f(0), d2f(0), d2f(0),
      //d2f(0), d2f(0), d2f(0), d2f(0), d2f(0),
      //d2f(0), d2f(0), d2f(0), d2f(-1), d2f(0),
      //d2f(0), d2f(0), d2f(0), d2f(0), d2f(-1),
 
      //d2f(3), d2f(3),
      //d2f(frac), d2f(frac), d2f(frac),
      //d2f(frac), d2f(frac), d2f(frac),
      //d2f(frac), d2f(frac), d2f(frac),
    };
 
    pixman_image_set_filter(
      pixman_img,
      PIXMAN_FILTER_CONVOLUTION,
      filterParams,
      std::size(filterParams)
    );
    pixman_image_composite32(
      PIXMAN_OP_SRC,
      pixman_img,
      nullptr,
      pixman_filtered,
      0, 0, 0, 0, 0, 0,
      w, h
    );
    QImage filtered(filteredDat, w, h, QImage::Format_ARGB32);
 
    pixman_image *pixman_img = pixman_image_create_bits(
      PIXMAN_a8r8g8b8,
      w, h,
      reinterpret_cast<uint32_t *>(img.bits()),
      img.bytesPerLine()
    );
    uint8_t *filteredDat = new uint8_t[w * h * 4];
    pixman_image *pixman_filtered = pixman_image_create_bits(
      PIXMAN_a8r8g8b8,
      w, h,
      reinterpret_cast<uint32_t *>(filteredDat),
      w * 4
    );
*/


/*
 
 Performing basic graphics operations on QImages and rendering them
 
    setWindowTitle("Pixel 2");
    setupMenubar();
 
    int w = 256;
    int h = 256;
 
    uint8_t *imgDat = new uint8_t[4 * w * h];
    QImage img{imgDat, w, h, QImage::Format_ARGB32};
    std::memset(imgDat, 0, 4 * w * h);

    int x = 50;
    int y = 40;

    {
      QPainter painter{&img};
      painter.setRenderHint(QPainter::Antialiasing, false);
      painter.setCompositionMode(QPainter::CompositionMode_Source);
      painter.fillRect(x, y, 101, 101, QColor{0, 0, 255, 127});
 
      painter.setBrush(QBrush{QColor{255, 0, 0}});
      painter.setPen(QColor{0, 255, 0});
      painter.drawEllipse({x + 50, y + 50}, 50, 50);
 
      painter.setPen(QColor{255, 0, 255});
      painter.drawLine(x + 20, y + 30, x + 68, y + 85);
    }

    int index = ((y + 0) * w + (x + 0)) * 4;
    std::cout << int(imgDat[index]) << ' ' << int(imgDat[index + 1]) << ' ' << int(imgDat[index + 2]) << ' ' << int(imgDat[index + 3]) << '\n';

    union {
      uint32_t rgba;
      uint8_t comp[4];
    };

    rgba = (127 << 24) | (255 << 0);
    std::cout << int(comp[0]) << ' ' << int(comp[1]) << ' ' << int(comp[2]) << ' ' << int(comp[3]) << '\n';

    uint8_t *maskDat = new uint8_t[w * h];
    START_TIMER(MemsetMask);
    std::memset(maskDat, 255, w * h);
    STOP_TIMER(MemsetMask);
    QImage mask{maskDat, w, h, QImage::Format_Alpha8}; // for drawing and masking
    QImage realMask{maskDat, w, h, QImage::Format_Grayscale8}; // for rendering

    START_TIMER(RenderMask);
    {
      QPainter painter{&mask};
      painter.setRenderHint(QPainter::Antialiasing, false);
      painter.setCompositionMode(QPainter::CompositionMode_Source);
      painter.fillRect(x, y, 70, 70, QColor{0, 0, 0, 0});
    }
    STOP_TIMER(RenderMask);

    QImage maskAnd = mask;
    maskAnd.detach();

    START_TIMER(AndMask);
    andMask(maskAnd, mask);
    STOP_TIMER(AndMask);

    START_TIMER(CopyMask);
    QImage copy = mask;
    copy.detach();
    STOP_TIMER(CopyMask);

    START_TIMER(NotMask);
    notMask(mask);
    notMask(mask);
    STOP_TIMER(NotMask);

    START_TIMER(ApplyMask);
    applyMask(img, mask);
    STOP_TIMER(ApplyMask);

    START_TIMER(ToTexture);
    pixmap = QPixmap::fromImage(img, Qt::NoFormatConversion);
    STOP_TIMER(ToTexture);
    //pixmap = QPixmap::fromImage(realMask, Qt::NoFormatConversion);

    //pixmap = QPixmap::fromImage(realMask);
    //pixmap.load("/Users/indikernick/Library/Developer/Xcode/DerivedData/Pixel_2-gqoblrlhvynmicgniivandqktune/Build/Products/Debug/Pixel 2.app/Contents/Resources/icon.png");

    label.setPixmap(pixmap);
    label.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label.setMinimumSize(0, 0);
    label.resize(w, h);
    label.show();

    layout.addWidget(&button, 0, 0);
    layout.addWidget(&label, 1, 0);
    setLayout(&layout);

    show();

*/

#include <cmath>

struct Color {
  uint8_t r, g, b, a;
};

struct ColorF {
  float r, g, b, a;
};

float toFloat(const uint8_t component) {
  return static_cast<float>(component) / 255.0f;
}

uint8_t fromFloat(const float component) {
  return static_cast<uint8_t>(std::clamp(std::round(component * 255.0f), 0.0f, 255.0f));
}

ColorF toFloat(const Color color) {
  return {toFloat(color.r), toFloat(color.g), toFloat(color.b), toFloat(color.a)};
}

Color fromFloat(const ColorF color) {
  return {fromFloat(color.r), fromFloat(color.g), fromFloat(color.b), fromFloat(color.a)};
}

// straight alpha with float precision
Color compositeF(const Color aInt, const Color bInt, const uint8_t afInt, const uint8_t bfInt) {
  const float aF = toFloat(afInt);
  const float bF = toFloat(bfInt);
  const ColorF a = toFloat(aInt);
  const ColorF b = toFloat(bInt);
  
  const float cA = a.a*aF + b.a*bF;
  if (cA == 0.0f) {
    return {0, 0, 0, 0};
  } else {
    const float cR = (a.a*aF*a.r + b.a*bF*b.r) / cA;
    const float cG = (a.a*aF*a.g + b.a*bF*b.g) / cA;
    const float cB = (a.a*aF*a.b + b.a*bF*b.b) / cA;
    return fromFloat({cR, cG, cB, cA});
  }
}

// straight alpha with uint8 precision
Color compositeI(const Color a, const Color b, const uint8_t aF, const uint8_t bF) {
  const uint32_t cA = a.a*aF + b.a*bF;
  if (cA == 0) {
    return {0, 0, 0, 0};
  } else {
    const uint8_t cR = (a.a*aF*a.r + b.a*bF*b.r) / cA;
    const uint8_t cG = (a.a*aF*a.g + b.a*bF*b.g) / cA;
    const uint8_t cB = (a.a*aF*a.b + b.a*bF*b.b) / cA;
    return {cR, cG, cB, static_cast<uint8_t>(cA / 255)};
  }
}

// premultiplied alpha with uint8 precision
Color compositeM(const Color a, const Color b, const uint8_t aF, const uint8_t bF) {
  const uint8_t cR = (aF*a.r + bF*b.r) / 255;
  const uint8_t cG = (aF*a.g + bF*b.g) / 255;
  const uint8_t cB = (aF*a.b + bF*b.b) / 255;
  const uint8_t cA = (aF*a.a + bF*b.a) / 255;
  return {cR, cG, cB, cA};
}

Color mulAlpha(const Color color) {
  const uint8_t r = (color.r * color.a) / 255;
  const uint8_t g = (color.g * color.a) / 255;
  const uint8_t b = (color.b * color.a) / 255;
  return {r, g, b, color.a};
}

Color divAlpha(const Color color) {
  if (color.a == 0) {
    return {0, 0, 0, 0};
  } else {
    const uint8_t r = (color.r * 255) / color.a;
    const uint8_t g = (color.g * 255) / color.a;
    const uint8_t b = (color.b * 255) / color.a;
    return {r, g, b, color.a};
  }
}

std::ostream &operator<<(std::ostream &stream, const Color color) {
  stream.width(3);
  stream << int(color.r) << ' ';
  stream.width(3);
  stream << int(color.g) << ' ';
  stream.width(3);
  stream << int(color.b) << ' ';
  stream.width(3);
  stream << int(color.a);
  return stream;
}

std::ostream &operator<<(std::ostream &stream, const ColorF color) {
  stream.width(3);
  stream << color.r << ' ';
  stream.width(3);
  stream << color.g << ' ';
  stream.width(3);
  stream << color.b << ' ';
  stream.width(3);
  stream << color.a;
  return stream;
}

bool operator==(const Color a, const Color b) {
  return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

bool operator!=(const Color a, const Color b) {
  return !(a == b);
}

bool different(const uint8_t a, const uint8_t b) {
  const int aI = a;
  const int bI = b;
  return aI + 2 < bI || bI + 2 < aI;
}

bool different(const Color a, const Color b) {
  return different(a.r, b.r) ||
         different(a.g, b.g) ||
         different(a.b, b.b) ||
         different(a.a, b.a);
}

void testComposite() {
  std::cout << "A                   B                   Float               Int                 Premul\n";

  uint8_t vals[] = {0, 63, 127, 191, 255};
  for (int cA = 0; cA != sizeof(vals); ++cA) {
    for (int aA = 0; aA != sizeof(vals); ++aA) {
      for (int cB = 0; cB != sizeof(vals); ++cB) {
        for (int aB = 0; aB != sizeof(vals); ++aB) {
          const Color a = {vals[cA], 0, 0, vals[aA]};
          const Color b = {vals[cB], 0, 0, vals[aB]};
          const uint8_t aF = 255;
          const uint8_t bF = 255 - a.a;
          const Color cF = compositeF(a, b, aF, bF);
          const Color cI = compositeI(a, b, aF, bF);
          const Color cM = divAlpha(compositeM(mulAlpha(a), mulAlpha(b), aF, bF));
          if (different(cF, cI) || different(cI, cM) || different(cF, cM)) {
            std::cout << a << " \t" << b << " \t" << cF << " \t" << cI << " \t" << cM << '\n';
          }
        }
      }
    }
  }
}

void applyMask(QImage &image, const QImage &mask) {
  assert(image.size() == mask.size());
  assert(image.format() == QImage::Format_ARGB32);
  assert(mask.format() == QImage::Format_Alpha8);
  
  image.detach();
  
  const ptrdiff_t dstPitch = image.bytesPerLine();
  uchar *dstData = image.bits();
  const ptrdiff_t maskPitch = mask.bytesPerLine();
  const uchar *maskData = mask.bits();
  const ptrdiff_t width = image.width();
  const ptrdiff_t height = image.height();
  
  for (ptrdiff_t y = 0; y != height; ++y) {
    for (ptrdiff_t x = 0; x != width; ++x) {
      uchar *const dstPixel = dstData + x * 4;
      const uchar maskPixel = maskData[x];
      dstPixel[0] &= maskPixel;
      dstPixel[1] &= maskPixel;
      dstPixel[2] &= maskPixel;
      dstPixel[3] &= maskPixel;
    }
    dstData += dstPitch;
    maskData += maskPitch;
  }
}

// each scanline is 32-bit aligned

void notMask(QImage &dst) {
  assert(dst.format() == QImage::Format_Alpha8);
  
  dst.detach();
  
  const ptrdiff_t pitch = dst.bytesPerLine();
  uchar *data = dst.bits();
  const ptrdiff_t width = dst.width() / 4;
  const ptrdiff_t height = dst.height();
  
  for (ptrdiff_t y = 0; y != height; ++y) {
    for (ptrdiff_t x = 0; x != width; ++x) {
      uint32_t *const pixel = reinterpret_cast<uint32_t *>(data) + x;
      *pixel ^= 0xFFFFFFFF;
    }
    data += pitch;
  }
}

template <typename Op>
void binaryMaskOp(QImage &dst, const QImage &src, Op &&op) {
  assert(dst.size() == src.size());
  assert(dst.format() == QImage::Format_Alpha8);
  assert(src.format() == QImage::Format_Alpha8);
  assert(dst.bytesPerLine() == src.bytesPerLine());
  
  dst.detach();
  
  const ptrdiff_t pitch = dst.bytesPerLine();
  uchar *dstData = dst.bits();
  const uchar *srcData = src.bits();
  const ptrdiff_t width = dst.width() / 4;
  const ptrdiff_t height = dst.height();
  
  for (ptrdiff_t y = 0; y != height; ++y) {
    for (ptrdiff_t x = 0; x != width; ++x) {
      uint32_t *const dstPixel = reinterpret_cast<uint32_t *>(dstData) + x;
      const uint32_t srcPixel = reinterpret_cast<const uint32_t *>(srcData)[x];
      op(dstPixel, srcPixel);
    }
    dstData += pitch;
    srcData += pitch;
  }
}

void andMask(QImage &dst, const QImage &src) {
  binaryMaskOp(dst, src, [](uint32_t *const dstPixel, const uint32_t srcPixel) {
    *dstPixel &= srcPixel;
  });
}

void orMask(QImage &dst, const QImage &src) {
  binaryMaskOp(dst, src, [](uint32_t *const dstPixel, const uint32_t srcPixel) {
    *dstPixel |= srcPixel;
  });
}

void xorMask(QImage &dst, const QImage &src) {
  binaryMaskOp(dst, src, [](uint32_t *const dstPixel, const uint32_t srcPixel) {
    *dstPixel ^= srcPixel;
  });
}

uint32_t composeRGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
  return (uint32_t{a} << 24) | (uint32_t{r} << 16) | (uint32_t{g} << 8) | uint32_t{b};
}

#define START_TIMER(NAME) \
  const auto NAME##_timer_start = std::chrono::high_resolution_clock::now()
#define STOP_TIMER(NAME) \
  const auto NAME##_timer_end = std::chrono::high_resolution_clock::now(); \
  const auto NAME##_timer_diff = std::chrono::duration_cast<std::chrono::microseconds>(NAME##_timer_end - NAME##_timer_start); \
  std::cout << #NAME << " \t" << NAME##_timer_diff.count() / 1000.0 << "ms\n"

class ToolsWidget : public QWidget {
public:
  ToolsWidget(QWidget *parent)
    : QWidget{parent} {
    layout = new QVBoxLayout{this};
    setLayout(layout);
    setFixedSize(24*2+2, (24*2+2) * 11);
  }

private:
  QVBoxLayout *layout = nullptr;
};

class TimelineWidget : public QWidget {
public:
  TimelineWidget(QWidget *parent)
    : QWidget{parent} {
    setMinimumHeight(128);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
  }
};

class RenderWidget : public QScrollArea {
public:
  RenderWidget(QWidget *parent)
    : QScrollArea{parent} {
    setMinimumSize(128, 128);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setAlignment(Qt::AlignCenter);
    view = new QLabel{this};
    img.load("/Users/indikernick/Library/Developer/Xcode/DerivedData/Pixel_2-gqoblrlhvynmicgniivandqktune/Build/Products/Debug/Pixel 2.app/Contents/Resources/icon.png");
    view->setPixmap(img.scaled(64, 64));
    view->resize(64, 64);
    setWidget(view);
  }
  
private:
  QLabel *view = nullptr;
  QPixmap img;
};

class EditorWidget : public QScrollArea {
public:
  EditorWidget(QWidget *parent)
    : QScrollArea{parent},
      cursor{Qt::CrossCursor} {
    setMinimumSize(128, 128);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setAlignment(Qt::AlignCenter);
    view = new QLabel{this};
    img.load("/Users/indikernick/Library/Developer/Xcode/DerivedData/Pixel_2-gqoblrlhvynmicgniivandqktune/Build/Products/Debug/Pixel 2.app/Contents/Resources/icon.png");
    view->setPixmap(img.scaled(1024, 1024));
    view->resize(1024, 1024);
    setWidget(view);
    setCursor(cursor);
  }
  
private:
  QLabel *view = nullptr;
  QPixmap img;
  QCursor cursor;
};

class CentralWidget : public QWidget {
public:
  CentralWidget(QWidget *parent)
    : QWidget{parent} {
    layout = new QHBoxLayout{this};
    layout->addWidget(new EditorWidget{this});
    layout->addWidget(new RenderWidget{this});
    setLayout(layout);
  }

private:
  QHBoxLayout *layout = nullptr;
};

class Window : public QMainWindow {
public:
  Window() {
    setupMenubar();
    setupUI();
    show();
  }

  void fileOpen(QFileOpenEvent *event) {
    //button.setText(static_cast<QFileOpenEvent *>(event)->file());
  }

private:
  QMenuBar *menubar = nullptr;
  QDockWidget *toolDock = nullptr;
  QDockWidget *timelineDock = nullptr;
  
  void setupUI() {
    toolDock = new QDockWidget{this};
    toolDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    toolDock->setWidget(new ToolsWidget{this});
    toolDock->setTitleBarWidget(new QWidget{});
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);
    
    timelineDock = new QDockWidget{this};
    timelineDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    timelineDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    timelineDock->setWidget(new TimelineWidget{this});
    timelineDock->setTitleBarWidget(new QWidget{});
    addDockWidget(Qt::BottomDockWidgetArea, timelineDock);
    
    setCentralWidget(new CentralWidget{this});
  }
  
  void setupMenubar() {
    menubar = new QMenuBar{this};
    QMenu *file = menubar->addMenu("File");
    QAction *open = file->addAction("Open");
    QAction *save = file->addAction("Save");
    file->addSeparator();
    file->addAction("Export");
    connect(open, &QAction::triggered, this, &Window::openDoc);
    connect(save, &QAction::triggered, this, &Window::saveDoc);
  }

  void openDoc() {
    std::cout << "Open document\n";
    QString path = QFileDialog::getOpenFileName(
      this,
      "Open Image",
      QDir::homePath(),
      "PNG Files (*.png)",
      nullptr,
      QFileDialog::ReadOnly | QFileDialog::HideNameFilterDetails
    );
    if (!path.isEmpty()) {
      //pixmap.load(path);
      //label.setPixmap(pixmap);
      //label.resize(pixmap.size());
    }
  }
  void saveDoc() {
    std::cout << "Save document\n";
    QString path = QFileDialog::getSaveFileName(
      this,
      "Save Image",
      QDir::homePath() + "/Frame 0.png",
      "",
      nullptr,
      QFileDialog::HideNameFilterDetails
    );
    if (!path.isEmpty()) {
      //pixmap.save(path);
    }
  }
  
  void resizeEvent(QResizeEvent *event) override {
    
  }
};

class Application : public QApplication {
public:
  Application(int &argc, char **argv)
    : QApplication{argc, argv} {
    window.setMinimumSize(640, 360);
    window.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    window.setGeometry(
      QStyle::alignedRect(
        Qt::LeftToRight,
        Qt::AlignCenter,
        window.size(),
        desktop()->availableGeometry()
      )
    );
  }
  
private:
  Window window;
  
  bool event(QEvent *event) override {
    if (event->type() == QEvent::FileOpen) {
      window.fileOpen(static_cast<QFileOpenEvent *>(event));
      return true;
    } else {
      return QApplication::event(event);
    }
  }
};

#include "image.hpp"

int main(int argc, char **argv) {
  Image img;
  img.data.load("/Users/indikernick/Library/Developer/Xcode/DerivedData/Pixel_2-gqoblrlhvynmicgniivandqktune/Build/Products/Debug/Pixel 2.app/Contents/Resources/icon.png");
  img.xform.angle = 1;
  img.xform.posX = 3;
  img.xform.posY = 5;
  img.xform.flipX = true;
  QImage xformed = img.transformed();
  xformed.save("/Users/indikernick/Desktop/test.png");
  //return 0;
  
  Application app{argc, argv};
  return app.exec();
}