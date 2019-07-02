//
// Created by cowan on 01/03/19.
//

#ifndef PANDAEXPRESS_TIMER_H
#define PANDAEXPRESS_TIMER_H

#include <unordered_map>
#include <string>

struct Watch {
    float time;
    float target_time;
};

class Timer {
public:
    Timer();
    bool is_done(std::string label);
    bool watch_exists(std::string label);
    void save_watch(std::string label, float time);
    void update(float delta_time);
    void reset_watch(std::string label);
    bool exists(std::string label);
    void remove(std::string label);
    float get_curr_time();
    float get_target_time(std::string label);


private:
    float curr_time;
    std::unordered_map<std::string, Watch> watches;
};


#endif //PANDAEXPRESS_TIMER_H
