//==========================================================================
//  LAYOUTERENV.CC - part of
//
//                     OMNeT++/OMNEST
//            Discrete System Simulation in C++
//
//  Implementation of
//    inspectors
//
//==========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 1992-2015 Andras Varga
  Copyright (C) 2006-2015 OpenSim Ltd.

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/

#include <cstring>
#include <cstdlib>
#include <cmath>
#include <cassert>

#include "omnetpp/cmodule.h"
#include "omnetpp/cdisplaystring.h"
#include "layouterenv.h"
#include "qtutil.h"
#include "qtenv.h"
#include <QDebug>
#include <QApplication>

namespace omnetpp {
namespace qtenv {

QtenvGraphLayouterEnvironment::QtenvGraphLayouterEnvironment(cModule *parentModule, const cDisplayString& displayString)
    : parentModule(parentModule), displayString(displayString)
{
}

bool QtenvGraphLayouterEnvironment::getBoolParameter(const char *tagName, int index, bool defaultValue)
{
    return resolveBoolDispStrArg(displayString.getTagArg(tagName, index), parentModule, defaultValue);
}

long QtenvGraphLayouterEnvironment::getLongParameter(const char *tagName, int index, long defaultValue)
{
    return resolveLongDispStrArg(displayString.getTagArg(tagName, index), parentModule, defaultValue);
}

double QtenvGraphLayouterEnvironment::getDoubleParameter(const char *tagName, int index, double defaultValue)
{
    return resolveDoubleDispStrArg(displayString.getTagArg(tagName, index), parentModule, defaultValue);
}

void QtenvGraphLayouterEnvironment::clearGraphics()
{
    if (scene)
        scene->clear();
}

void QtenvGraphLayouterEnvironment::showGraphics(const char *text)
{
    bbox = nextbbox;
    nextbbox = QRectF();
    auto item = new QGraphicsTextItem(text);
    item->setTextWidth(view->viewport()->size().width());
    item->setFont(getQtenv()->getCanvasFont());
    scene->addItem(item);
    scene->setSceneRect(scene->itemsBoundingRect());
    QApplication::processEvents();
}

void QtenvGraphLayouterEnvironment::drawText(double x, double y, const char *text, const char *tags, const char *color)
{
    scaleCoords(x, y);
    auto item = new QGraphicsSimpleTextItem(text);
    item->setPos(x, y);
    item->setBrush(parseColor(color));
    item->setFont(getQtenv()->getCanvasFont());
    scene->addItem(item);
}

void QtenvGraphLayouterEnvironment::drawLine(double x1, double y1, double x2, double y2, const char *tags, const char *color)
{
    scaleCoords(x1, y1);
    scaleCoords(x2, y2);
    auto item = new QGraphicsLineItem(x1, y1, x2, y2);
    item->setPen(parseColor(color));
    scene->addItem(item);
}

void QtenvGraphLayouterEnvironment::drawRectangle(double x1, double y1, double x2, double y2, const char *tags, const char *color)
{
    scaleCoords(x1, y1);
    scaleCoords(x2, y2);
    auto item = new QGraphicsRectItem(x1, y1, x2-x1, y2-y1);
    item->setPen(parseColor(color));
    scene->addItem(item);
}

bool QtenvGraphLayouterEnvironment::okToProceed()
{
    QApplication::processEvents();
    return !stopFlag;
}

void QtenvGraphLayouterEnvironment::cleanup()
{
    if (view)
        view->setTransform(QTransform());
    clearGraphics();
}

void QtenvGraphLayouterEnvironment::stop()
{
    stopFlag = true;
}

void QtenvGraphLayouterEnvironment::scaleCoords(double &x, double &y)
{
    int vMargin = QFontMetrics(getQtenv()->getCanvasFont()).height()*2;
    int hMargin = 20;

    if (view) {
        nextbbox = nextbbox.united(QRectF(x, y, 1, 1));
        auto viewRect = view->viewport()->geometry();
        x -= bbox.left();
        y -= bbox.top();
        float scale =std::min(1.0,
                              std::min((viewRect.width()-hMargin*2) / bbox.width(),
                                       (viewRect.height()-vMargin*2) / bbox.height()));
        x *= scale;
        y *= scale;
        x += hMargin;
        y += vMargin;
    }
}

} // namespace qtenv
} // namespace omnetpp
