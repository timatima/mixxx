//
// C++ Interface: woverview
//
// Description:
//
//
// Author: Tue Haste Andersen <haste@diku.dk>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WOVERVIEW_H
#define WOVERVIEW_H

#include <QPaintEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QColor>
#include <QList>

#include "trackinfoobject.h"
#include "widget/wwidget.h"

#include "waveform/renderers/waveformsignalcolors.h"
#include "waveform/renderers/waveformmarkset.h"
#include "waveform/renderers/waveformmarkrange.h"


// Waveform overview display
// @author Tue Haste Andersen
class ControlObjectThreadMain;
class Waveform;

class WOverview : public WWidget {
    Q_OBJECT
  public:
    WOverview(const char* pGroup, ConfigObject<ConfigValue>* pConfig, QWidget *parent=NULL);
    virtual ~WOverview();
    void setup(QDomNode node);

  public slots:
    void setValue(double);
    void slotLoadNewTrack(TrackPointer pTrack);
    void slotTrackLoaded(TrackPointer pTrack);
    void slotUnloadTrack(TrackPointer pTrack);

  signals:
    void trackDropped(QString filename, QString group);

  protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    virtual void dragEnterEvent(QDragEnterEvent* event);
    virtual void dropEvent(QDropEvent* event);

    Waveform* m_pWaveform;

    QImage* m_pWaveformSourceImage;
    QImage m_waveformImageScaled;

    WaveformSignalColors m_signalColors;

    // Hold the last visual sample processed to generate the pixmap
    int m_actualCompletion;

    bool m_pixmapDone;
    float m_waveformPeak;

    int m_diffGain;

  private slots:
    void onEndOfTrackChange(double v);

    void onMarkChanged(double v);
    void onMarkRangeChange(double v);

    void slotWaveformSummaryUpdated();
    void slotAnalyserProgress(int progress);

  private:
    // Append the waveform overview pixmap according to available data in waveform
    virtual bool drawNextPixmapPart() = 0;
    void paintText(const QString &text, QPainter *painter);
    inline int valueToPosition(float value) const {
        return static_cast<int>(m_a * value - m_b + 0.5);
    }
    inline double positionToValue(int position) const {
        return (static_cast<float>(position) + m_b) / m_a;
    }

    const QString m_group;
    ConfigObject<ConfigValue>* m_pConfig;
    ControlObjectThread* m_endOfTrackControl;
    double m_endOfTrack;
    ControlObjectThread* m_trackSamplesControl;
    ControlObjectThread* m_playControl;


    // Current active track
    TrackPointer m_pCurrentTrack;

    // True if slider is dragged. Only used when m_bEventWhileDrag is false
    bool m_bDrag;
    // Internal storage of slider position in pixels
    int m_iPos;

    QPixmap m_backgroundPixmap;
    QString m_backgroundPixmapPath;
    QColor m_qColorBackground;
    QColor m_endOfTrackColor;

    WaveformMarkSet m_marks;
    std::vector<WaveformMarkRange> m_markRanges;

    // Coefficient value-position linear transposition
    float m_a;
    float m_b;

    int m_analyserProgress; // In 0.1%
    bool m_trackLoaded;
};

#endif
