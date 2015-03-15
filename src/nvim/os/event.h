#ifndef NVIM_OS_EVENT_H
#define NVIM_OS_EVENT_H

#include <stdint.h>
#include <stdbool.h>

#include "nvim/os/event_defs.h"
#include "nvim/os/job_defs.h"
#include "nvim/os/time.h"

void ui_busy_start(void);
void ui_busy_stop(void);

// Poll for events until a condition or timeout
#define event_poll_until(timeout, condition)                                 \
  do {                                                                       \
    if (timeout < 0 || timeout > 100) {                                      \
      ui_busy_stop();                                                        \
    }                                                                        \
    int remaining = timeout;                                                 \
    uint64_t before = (remaining > 0) ? os_hrtime() : 0;                     \
    while (!(condition)) {                                                   \
      event_poll(remaining);                                                 \
      if (remaining == 0) {                                                  \
        break;                                                               \
      } else if (remaining > 0) {                                            \
        uint64_t now = os_hrtime();                                          \
        remaining -= (int) ((now - before) / 1000000);                       \
        before = now;                                                        \
        if (remaining <= 0) {                                                \
          break;                                                             \
        }                                                                    \
      }                                                                      \
    }                                                                        \
    if (timeout < 0 || timeout > 100) {                                      \
      ui_busy_start();                                                       \
    }                                                                        \
  } while (0)

#ifdef INCLUDE_GENERATED_DECLARATIONS
# include "os/event.h.generated.h"
#endif

#endif  // NVIM_OS_EVENT_H
