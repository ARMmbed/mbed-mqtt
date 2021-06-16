/*******************************************************************************
 * Copyright (c) 2014, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Ian Craggs - change Timer member initialization to avoid copy constructor
 *******************************************************************************/

#if !defined(MQTT_MBED_H)
#define MQTT_MBED_H

#include <chrono>

#include "mbed.h"

class Countdown
{
public:
    Countdown() =default;
    
    Countdown(int ms) : t()
    {
        countdown_ms(ms);
    }

    Countdown(std::chrono::milliseconds ms) : t()
    {
        countdown_ms(ms);
    }

    bool expired()
    {
        return t.elapsed_time() >= interval_end_ms;
    }

    void countdown_ms(std::chrono::milliseconds ms)
    {
        t.stop();
        interval_end_ms = std::move(ms);
        t.reset();
        t.start();
    }

    void countdown(std::chrono::seconds seconds)
    {
        countdown_ms(std::move(seconds));
    }
    
    void countdown_ms(unsigned long ms)
    {
        countdown_ms(std::chrono::milliseconds{ms});
    }
    
    void countdown(size_t seconds)
    {
        countdown_ms(std::chrono::seconds{seconds});
    }

    int left_ms()
    {
        using namespace std::chrono;
        return duration_cast<milliseconds>(interval_end_ms - t.elapsed_time()).count();
    }

private:
    Timer t;
    std::chrono::milliseconds interval_end_ms;
};

#endif
