#ifndef TIMELINE_H
#define TIMELINE_H

#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <algorithm>

class Clock
{
public:
    Clock(double tempo = 120.0, int ticksPerBeat = 480)
        : tempo(tempo), ticksPerBeat(ticksPerBeat), running(false) {}

    void setTempo(double newTempo)
    {
        std::lock_guard<std::mutex> lock(clockMutex);
        tempo = newTempo;
    }

    void start()
    {
        if (running) return;
        running = true;
        clockThread = std::thread(&Clock::run, this);
    }

    void stop()
    {
        running = false;
        if (clockThread.joinable())
        {
            clockThread.join();
        }
    }

    void attachTarget(const std::function<void()> &callback)
    {
        targetCallback = callback;
    }

private:
    void run()
    {
        auto tickDuration = std::chrono::milliseconds(static_cast<int>(60000.0 / (tempo * ticksPerBeat)));
        while (running)
        {
            std::this_thread::sleep_for(tickDuration);
            if (targetCallback)
            {
                targetCallback();
            }
        }
    }

    double tempo;
    int ticksPerBeat;
    std::atomic<bool> running;
    std::thread clockThread;
    std::function<void()> targetCallback;
    mutable std::mutex clockMutex;
};

class Track
{
public:
    explicit Track(const std::string &name) : name(name), isFinished(false) {}

    void tick()
    {
        if (!isFinished)
        {
            std::cout << "Track [" << name << "] ticked." << std::endl;
        }
    }

    void finish()
    {
        isFinished = true;
    }

    bool finished() const
    {
        return isFinished;
    }

    std::string getName() const
    {
        return name;
    }

private:
    std::string name;
    bool isFinished;
};

class Timeline
{
public:
    Timeline(double tempo = 120.0, int ticksPerBeat = 480)
        : tempo(tempo), ticksPerBeat(ticksPerBeat), running(false)
    {
        // clock = std::make_shared<Clock>(tempo, ticksPerBeat);
        // clock->attachTarget([this]() { tick(); });
    }

    ~Timeline()
    {
        stop();
    }

    void start()
    {
        if (running) return;
        running = true;
        clock->start();
    }

    void stop()
    {
        if (!running) return;
        running = false;
        clock->stop();
    }

    void addTrack(const std::shared_ptr<Track> &track)
    {
        std::lock_guard<std::mutex> lock(mutex);
        tracks.push_back(track);
    }

    void tick()
    {
        std::lock_guard<std::mutex> lock(mutex);
        for (auto &track : tracks)
        {
            if (!track->finished())
            {
                track->tick();
            }
        }
        tracks.erase(std::remove_if(tracks.begin(), tracks.end(),
                                    [](const std::shared_ptr<Track> &track) { return track->finished(); }),
                     tracks.end());
    }

private:
    double tempo;
    int ticksPerBeat;
    std::atomic<bool> running;
    std::shared_ptr<Clock> clock;
    std::vector<std::shared_ptr<Track>> tracks;
    mutable std::mutex mutex;
};

#endif
