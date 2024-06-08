#pragma once

#include <chrono>
#include <command/Log.hpp>
#include <utility/string.hpp>
#include <utility/platform.hpp>

template<typename T>
concept DurationType = std::same_as<T, std::chrono::duration<typename T::rep, typename T::period>>;

template<Utility::Str::StringLiteral Message = "Process took ", typename Units = std::chrono::seconds, typename Clock = std::chrono::high_resolution_clock>
requires DurationType<Units> && std::chrono::is_clock_v<Clock>
class Timer {
public:
    typename Clock::duration getElapsed() const {
        end = Clock::now();
        return end - begin;
    }

protected:
    typename Clock::time_point begin;
    typename Clock::time_point end;

    void start() {
        begin = Clock::now();
    }

    void stop() {
        end = Clock::now();
        duration = end - begin;
    }
    
    void print() const {
        std::stringstream message;
        message << stem << (stem.back() == ' ' ? "" : " ") << std::chrono::duration_cast<Units>(duration);

        Utility::platformLog(message.str());
        LOG_TO_DEBUG(message.str() + '\n');
    }

private:
    typename Clock::duration duration;

    static constexpr std::string_view stem = Message;
};

template<Utility::Str::StringLiteral Message = "Process took ", typename Units = std::chrono::seconds, typename Clock = std::chrono::high_resolution_clock>
requires DurationType<Units> && std::chrono::is_clock_v<Clock>
class ScopedTimer : public Timer<Message, Units, Clock> {
public:
    ScopedTimer() {
        Timer<Message, Units, Clock>::start();
    }

    ~ScopedTimer() {
        Timer<Message, Units, Clock>::stop();
        Timer<Message, Units, Clock>::print();
    }
};

class ProgramTime {
private:
    using Clock_t = std::chrono::system_clock;
    using TimePoint_t = Clock_t::time_point;
    using Duration_t = Clock_t::duration;

    const TimePoint_t openTime;
    static TimePoint_t getOpenedTime();
    static Duration_t getElapsedTime();

    ProgramTime();
    ~ProgramTime() = default;

public:
    ProgramTime(const ProgramTime&) = delete;
    ProgramTime& operator=(const ProgramTime&) = delete;

    static ProgramTime& getInstance();
    static std::string getTimeStr();
    static std::string getDateStr();
};
