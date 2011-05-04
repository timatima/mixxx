/**
 * @file soundmanagerconfig.h
 * @author Bill Good <bkgood at gmail dot com>
 * @date 20100709
 */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SOUNDMANAGERCONFIG_H
#define SOUNDMANAGERCONFIG_H

#ifndef SOUNDMANAGERCONFIG_FILENAME
#define SOUNDMANAGERCONFIG_FILENAME "soundconfig.xml"
#endif

#include <QtCore>

#include "soundmanagerutil.h"

class SoundDevice;
class SoundManager;

#define DEFAULT_API "None"

const unsigned int MAX_LATENCY = 6; // this represents latency values from 1 ms to about
                                    // 180 ms, should be sufficient -- bkgood
// 20100726 latencies ~80 and ~170 are utterly broken right now, at least on
// linux/ALSA (although I think this is a Mixxx bug somewhere around
// enginebuffer), as such, disabling these latencies -- bkgood
// no DEFAULT_API because we have to check that the api is available -- bkgood
const unsigned int DEFAULT_SAMPLE_RATE = 48000;
// latency=5 means about 21 ms of latency which is default in trunk r2453 -- bkgood
const int DEFAULT_LATENCY = 5;

class SoundManagerConfig {
public:
    enum Defaults {
        API = (1 << 0),
        DEVICES = (1 << 1),
        OTHER = (1 << 2),
        ALL = (API | DEVICES | OTHER),
    };
    SoundManagerConfig();
    ~SoundManagerConfig();
    bool readFromDisk();
    bool writeToDisk() const;
    QString getAPI() const;
    void setAPI(const QString &api);
    bool checkAPI(const SoundManager &soundManager);
    unsigned int getSampleRate() const;
    void setSampleRate(unsigned int sampleRate);
    bool checkSampleRate(const SoundManager &soundManager);
    unsigned int getLatency() const;
    unsigned int getFramesPerBuffer() const;
    void setLatency(unsigned int latency);
    void addOutput(const QString &device, const AudioOutput &out);
    void addInput(const QString &device, const AudioInput &in);
    QMultiHash<QString, AudioOutput> getOutputs() const;
    QMultiHash<QString, AudioInput> getInputs() const;
    void clearOutputs();
    void clearInputs();
    // I'd prefer for these to be const but SoundManager::getDeviceList isn't
    void filterOutputs(SoundManager *soundManager);
    void filterInputs(SoundManager *soundManager);
    void loadDefaults(SoundManager *soundManager, unsigned int flags);
private:
    QFileInfo m_configFile;
    QString m_api;
    // none of our sample rates are actually decimals, this avoids
    // the weirdness using floating point can introduce
    unsigned int m_sampleRate;
    // m_latency is an index > 0, where 1 is a latency of 1ms and
    // higher indices represent subsequently higher latencies (storing
    // latency as milliseconds or frames per buffer is bad because those
    // values very with sample rate) -- bkgood
    unsigned int m_latency; // this is an index
    QMultiHash<QString, AudioOutput> m_outputs;
    QMultiHash<QString, AudioInput> m_inputs;
};
#endif