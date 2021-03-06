﻿//
//  timeline.hpp
//  Animera
//
//  Created by Indiana Kernick on 6/7/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef animera_timeline_hpp
#define animera_timeline_hpp

#include "error.hpp"
#include "cel array.hpp"
#include "group array.hpp"
#include "palette span.hpp"

// TODO: can we make the interface of Timeline smaller?

/*

struct LayerData {
  std::vector<Cel> cels;
  std::string name;
  bool visible;
};

class Layer {
public:
  // signals and slots
 
private:
  LayerData *data;
};

*/

class Timeline final : public QObject {
  Q_OBJECT

public:
  Timeline();

  void initDefault();
  void optimize();
  void change();
  Error openImage(const QString &, PaletteSpan, Format &, QSize &);
  Error importImage(const QString &);

  Error serializeHead(QIODevice &) const;
  Error serializeBody(QIODevice &) const;
  Error serializeTail(QIODevice &) const;

  Error deserializeHead(QIODevice &, Format &, QSize &);
  Error deserializeBody(QIODevice &);
  Error deserializeTail(QIODevice &);

  LayerIdx getLayers() const;
  GroupIdx getGroups() const;
  FrameIdx getFrames() const;
  CelPos getPos() const;
  CelRect getSelection() const;
  
  tcb::span<const Layer> getLayerArray() const;
  tcb::span<const Group> getGroupArray() const;

public Q_SLOTS:
  void initCanvas(Format, QSize);
  void resizeCanvas(QSize);

  void nextFrame();
  void prevFrame();
  void layerBelow();
  void layerAbove();
  
  void beginSelection();
  void continueSelection();
  void endSelection();
  void clearSelection();
  
  void insertLayer();
  void removeLayer();
  void moveLayerUp();
  void moveLayerDown();
  
  void insertFrame();
  void removeFrame();
  
  void clearCel();
  void extendCel();
  void splitCel();
  void growCelImage(QRect);
  void shrinkCelImage(QRect);
  
  void setGroup(FrameIdx);
  void setGroupName(std::string_view);
  void moveGroup(GroupIdx, FrameIdx);
  void splitGroupLeft();
  void splitGroupRight();
  void mergeGroupLeft();
  void mergeGroupRight();
  
  void setPos(CelPos);
  void setVisibility(LayerIdx, bool);
  void isolateVisibility(LayerIdx);
  void setLayerName(LayerIdx, std::string_view);
  void setDelay(int);
  
  void clearSelected();
  void copySelected();
  void pasteSelected();
  
  void lock();
  void unlock();

Q_SIGNALS:
  void posChanged(CelPos);
  void celImageChanged(CelImage *);
  void selectionChanged(CelRect);
  void groupChanged(GroupInfo);
  
  void groupNameChanged(std::string_view);
  void visibilityChanged(LayerIdx, bool);
  void layerNameChanged(LayerIdx, std::string_view);
  
  void groupArrayChanged(tcb::span<const Group>);
  void frameChanged(const Frame &);
  void layerCelsChanged(LayerIdx, tcb::span<const Cel>);
  
  void frameCountChanged(FrameIdx);
  void layerCountChanged(LayerIdx);
  
  void delayChanged(int);
  
  void modified();
  void celImageModified(QRect);
  
private:
  std::vector<Layer> layers;
  std::vector<std::vector<Cel>> clipboard;
  std::vector<Group> groups;
  CelPos pos;
  CelRect selection;
  GroupIdx group;
  FrameIdx frameCount;
  QSize canvasSize;
  Format canvasFormat;
  int delay;
  bool locked = false;
  
  CelImage *getCel(CelPos);
  Frame getFrame(FrameIdx) const;
  LayerIdx layerCount() const;
  
  void changePos();
  void changeFrame();
  void changeLayerCels(LayerIdx);
  void changeLayers(LayerIdx, LayerIdx);
  void changeFrameCount();
  void changeLayerCount();
  void changeCelImage(QRect);
  void changeCelImage();
  GroupInfo changeGroup(FrameIdx);
  void changeGroupArray();
};

#endif
