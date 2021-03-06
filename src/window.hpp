﻿//
//  window.hpp
//  Animera
//
//  Created by Indiana Kernick on 10/3/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef animera_window_hpp
#define animera_window_hpp

#include "animation.hpp"
#include <QtWidgets/qmainwindow.h>

class QWidget;
class QSplitter;
class UndoObject;
class SampleObject;
class EditorWidget;
class PaletteWidget;
class ToolColorsWidget;
class ToolSelectWidget;
class ToolParamBarWidget;
class TimelineWidget;
class StatusBarWidget;
class ColorPickerWidget;
class QMenuBar;
class ExportDialog;
class StatusObject;
struct ExportParams;

class Window final : public QMainWindow {
  Q_OBJECT

public:
  Window(QWidget *, const Window *);
  
  void newFile(Format, QSize);
  void openFile(const QString &);
  void openImage(const QString &);

private Q_SLOTS:
  void modify();

private:
  Animation anim;
  QWidget *central = nullptr;
  QWidget *bottom = nullptr;
  QWidget *right = nullptr;
  QSplitter *splitter = nullptr;
  UndoObject *undo = nullptr;
  SampleObject *sample = nullptr;
  EditorWidget *editor = nullptr;
  PaletteWidget *palette = nullptr;
  ToolColorsWidget *colors = nullptr;
  ToolSelectWidget *tools = nullptr;
  ToolParamBarWidget *toolParams = nullptr;
  TimelineWidget *timeline = nullptr;
  StatusBarWidget *statusBar = nullptr;
  ColorPickerWidget *colorPicker = nullptr;
  StatusObject *status = nullptr;
  QMenuBar *menubar = nullptr;
  ExportDialog *exporter = nullptr;
  bool showQuitDialog = true;
  bool closeAfterSave = false;
  
  void setPosition(const Window *);
  void createWidgets();
  void setupLayouts();
  void initStyles();
  void populateMenubar();
  void connectSignals();
  
  void saveToPath(const QString &);
  void saveFile();
  void saveFileDialog();
  void exportAnimation(const ExportParams &);
  void exportDialog();
  void exportFrame(const QString &);
  void exportFrameDialog();
  void exportCel(const QString &);
  void exportCelDialog();
  void importCel(const QString &);
  void importCelDialog();
  void openPalette(const QString &);
  void savePalette(const QString &);
  void openPaletteDialog();
  void savePaletteDialog();
  void resetPalette();
  void keysDialog();
  void resizeDialog();
  
  void closeEvent(QCloseEvent *) override;
};

#endif
