# Pebble Time Machine
A Pebble TickTimerService replacement for testing transitions and animations more quickly by speeding up the passage of time.

I found that I needed something like this for testing in every watchface that has animations or unusual ways to show the time. So I made a package out of it.

To use the TimeMachine you have to call one of the init functions to configure the start time, which unit increase you want to accelerate and the interval how quickly you want this to happen.

If you set the interval to `0` you will get exactly one tick with the start time. Use this to set a specific time for testing or screen shots.

If you want to test a specific transition, you can also configure a loop to wrap around at a specified end time. This is also very useful when recording GIF animations for the app store assets.

You have to replace the TickTimerService API calls with the TimeMachine API, which just adds a `time_machine_` prefix to the front. For convenience I also included a modified `pebble-events` package API, but be aware that packages that use the `pebble-events` version of TickTimerService will not use the TimeMachine version automatically.

You can use the `TIME_MACHINE` define to determine whether or not TimeMachine is used. The define is set if `pebble-time-machine.h` is included. When used correctly you can easily switch between accelerated and real time by just commenting out the `pebble-time-machine.h` include.

`pebble-time-machine.h`:

    #pragma once

    #define TIME_MACHINE

    typedef enum {
      TIME_MACHINE_SECONDS,
      TIME_MACHINE_MINUTES,
      TIME_MACHINE_HOURS,
      TIME_MACHINE_DAYS,
      TIME_MACHINE_WEEKS,
      TIME_MACHINE_MONTHS,
      TIME_MACHINE_YEARS
    } TimeMachineUnit;

    void time_machine_init(struct tm* start, TimeMachineUnit unit, int interval);
    void time_machine_init_loop(struct tm* start, struct tm* end, TimeMachineUnit unit, int interval);

    void time_machine_tick_timer_service_subscribe(TimeUnits units, TickHandler handler);
    void time_machine_tick_timer_service_unsubscribe();

    typedef void(*TimeMachineTickHandler)(struct tm *tick_time, TimeUnits units_changed, void *context);
    int time_machine_events_tick_timer_service_subscribe(TimeUnits units, TickHandler handler);
    int time_machine_events_tick_timer_service_subscribe_context(TimeUnits units, TimeMachineTickHandler handler, void *context);
    void time_machine_events_tick_timer_service_unsubscribe(int handle);


Usage examples:

    #include <pebble-time-machine/pebble-time-machine.h>

    static void prv_tick_handler(struct tm* tick_time, TimeUnits units_changed) {
      /* do something with the time */
    }

    static void my_window_load(Window *window) {
      /* init your layers and stuff */

      time_t now = time(NULL);
      struct tm* tick_time = localtime(&now);
      prv_tick_handler(tick_time, 0);
    #ifdef TIME_MACHINE
      /* accelerate minutes to seconds */
      time_machine_init(tick_time, TIME_MACHINE_MINUTES, 1000);
      time_machine_tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
    #else
      tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
    #endif
    }

#

    #include <pebble-time-machine/pebble-time-machine.h>

    static void prv_tick_handler(struct tm* tick_time, TimeUnits units_changed) {
      /* do something with the time */
    }

    static void my_window_load(Window *window) {
      /* init your layers and stuff */

      time_t now = time(NULL);
      struct tm* tick_time = localtime(&now);
      prv_tick_handler(tick_time, 0);
    #ifdef TIME_MACHINE
      /* loop 10 minutes around noon with a minute tick every 2 seconds */
      struct tm end_time = *tick_time;
      /* start 5 minutes to noon */
      tick_time->tm_hour = 11;
      tick_time->tm_min = 55;
      /* wrap around at 5 minutes past noon */
      end_time.tm_hour = 12;
      end_time.tm_min = 5;
      time_machine_init_loop(tick_time, &end_time, TIME_MACHINE_MINUTES, 2000);
      time_machine_tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
    #else
      tick_timer_service_subscribe(MINUTE_UNIT, prv_tick_handler);
    #endif
    }

#Note
If you do large time spans, be aware that leap years are currently ignored and will not be handled correctly.
