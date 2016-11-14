#pragma once

#define FAST_TICK_TIMER

typedef enum {
  FAST_TICK_TIMER_SECONDS,
  FAST_TICK_TIMER_MINUTES,
  FAST_TICK_TIMER_HOURS,
  FAST_TICK_TIMER_DAYS,
  FAST_TICK_TIMER_WEEKS,
  FAST_TICK_TIMER_MONTHS,
  FAST_TICK_TIMER_YEARS
} FastTickTimerUnit;

void fast_tick_timer_init(struct tm* start, FastTickTimerUnit unit, int interval);
void fast_tick_timer_init_loop(struct tm* start, struct tm* end, FastTickTimerUnit unit, int interval);

void fast_tick_timer_service_subscribe(TimeUnits units, TickHandler handler);
void fast_tick_timer_service_unsubscribe();

typedef void(*FastTickHandler)(struct tm *tick_time, TimeUnits units_changed, void *context);
int fast_events_tick_timer_service_subscribe(TimeUnits units, TickHandler handler);
int fast_events_tick_timer_service_subscribe_context(TimeUnits units, FastTickHandler handler, void *context);
void fast_events_tick_timer_service_unsubscribe(int handle);
