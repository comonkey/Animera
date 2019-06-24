//
//  composite.hpp
//  Pixel 2
//
//  Created by Indi Kernick on 18/2/19.
//  Copyright © 2019 Indi Kernick. All rights reserved.
//

#ifndef composite_hpp
#define composite_hpp

#include "cell.hpp"

/// Take all of the images that make up a frame and composite them together into
/// a single image
QImage compositeFrame(const Frame &);
/// Composite the overlay on top of the frame
void compositeOverlay(QImage &, const QImage &);

/// Copy an image onto another image at a position
void blitImage(QImage &, const QImage &, QPoint);
/// Copy part of an image into a new image
QImage blitImage(const QImage &, QRect);

/// Copy all pixels of src that are on the mask (shifted to the position) onto
/// onto an image
void blitMaskImage(QImage &, const QImage &, const QImage &, QPoint);
/// Copy all pixels of src that are on the mask (shifted to the position) onto a
/// new image
QImage blitMaskImage(const QImage &, const QImage &, QPoint);

/// Convert a color RGBA image to an RGBA overlay image. Colors are converted
/// to grayscale
void colorToOverlay(QImage &);
/// Convert a color RGBA image to an RGBA overlay image using a mask.
/// Colors are converted to grayscale
void colorToOverlay(QImage &, const QImage &);

#endif
