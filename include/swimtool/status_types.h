#ifndef SWIMTOOL_STATUS_TYPES_H
#define SWIMTOOL_STATUS_TYPES_H

#include <glib.h>

#include <gbinder_types.h>
#include <radio_config_types.h>

typedef struct slot_info {
    gboolean enabled;
    gboolean online;
    char* eid;
    char* iccid;
} SlotInfo;

typedef struct status_ctx {
    GMainLoop* loop;
    gboolean ok;
    char* error;
    gsize slot_count;
    SlotInfo* slots;
} StatusCtx;

typedef struct radio_sim_slot_status_1_2 {
    RadioSimSlotStatus base RADIO_ALIGNED(8);
    GBinderHidlString eid RADIO_ALIGNED(8);
} RADIO_ALIGNED(8) RadioSimSlotStatus_1_2;

#endif /* SWIMTOOL_STATUS_TYPES_H */
