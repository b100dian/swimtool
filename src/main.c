#include <stdio.h>
#include <string.h>

#include <glib.h>

#include <gbinder_reader.h>
#include <gbinder_types.h>
#include <radio_config.h>
#include <radio_config_types.h>
#include <radio_request.h>

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

static
char*
hidl_string_dup(
    const GBinderHidlString* str)
{
    if (str && str->data.str && str->len > 0) {
        return g_strndup(str->data.str, str->len);
    }
    return g_strdup("");
}

static
void
status_ctx_clear(
    StatusCtx* ctx)
{
    gsize i;

    for (i = 0; i < ctx->slot_count; i++) {
        g_free(ctx->slots[i].eid);
        g_free(ctx->slots[i].iccid);
    }
    g_free(ctx->slots);
    g_free(ctx->error);
}

static
void
status_ctx_complete(
    StatusCtx* ctx,
    gboolean ok,
    const char* error)
{
    ctx->ok = ok;
    if (!ok && error) {
        ctx->error = g_strdup(error);
    }
    g_main_loop_quit(ctx->loop);
}

static
void
status_complete_cb(
    RadioRequest* req,
    RADIO_TX_STATUS status,
    RADIO_CONFIG_RESP resp,
    RADIO_ERROR error,
    const GBinderReader* reader,
    gpointer user_data)
{
    StatusCtx* ctx = user_data;
    GBinderReader tmp;
    gsize count = 0;
    gsize elemsize = 0;
    const guint8* slots;
    gsize i;

    if (status != RADIO_TX_STATUS_OK) {
        status_ctx_complete(ctx, FALSE, "getSimSlotsStatus transaction failed");
        return;
    }

    if (resp != RADIO_CONFIG_RESP_GET_SIM_SLOTS_STATUS &&
        resp != RADIO_CONFIG_RESP_GET_SIM_SLOTS_STATUS_1_2) {
        status_ctx_complete(ctx, FALSE, "unexpected getSimSlotsStatus response");
        return;
    }

    if (error != RADIO_ERROR_NONE) {
        status_ctx_complete(ctx, FALSE, "getSimSlotsStatus returned radio error");
        return;
    }

    if (!reader) {
        status_ctx_complete(ctx, FALSE, "empty getSimSlotsStatus response");
        return;
    }

    gbinder_reader_copy(&tmp, reader);
    slots = gbinder_reader_read_hidl_vec(&tmp, &count, &elemsize);
    if (!slots || elemsize < sizeof(RadioSimSlotStatus)) {
        status_ctx_complete(ctx, FALSE, "failed to parse getSimSlotsStatus payload");
        return;
    }

    if (resp == RADIO_CONFIG_RESP_GET_SIM_SLOTS_STATUS_1_2 &&
        elemsize < sizeof(RadioSimSlotStatus_1_2)) {
        status_ctx_complete(ctx, FALSE, "failed to parse getSimSlotsStatus 1.2 payload");
        return;
    }

    ctx->slot_count = count;
    ctx->slots = g_new0(SlotInfo, count);

    for (i = 0; i < count; i++) {
        const guint8* p = slots + (i * elemsize);
        const RadioSimSlotStatus* s = (const RadioSimSlotStatus*)p;

        ctx->slots[i].enabled = (s->slotState == RADIO_SLOT_STATE_ACTIVE);
        ctx->slots[i].online = (s->cardState != RADIO_CARD_STATE_ABSENT);
        ctx->slots[i].iccid = hidl_string_dup(&s->iccid);
        ctx->slots[i].eid = g_strdup("");

        if (resp == RADIO_CONFIG_RESP_GET_SIM_SLOTS_STATUS_1_2) {
            const RadioSimSlotStatus_1_2* s12 = (const RadioSimSlotStatus_1_2*)p;

            g_free(ctx->slots[i].eid);
            ctx->slots[i].eid = hidl_string_dup(&s12->eid);
        }
    }

    status_ctx_complete(ctx, TRUE, NULL);
    (void)req;
}



static
int
command_status(void)
{
    StatusCtx ctx;
    RadioConfig* config;
    RadioRequest* req;
    gsize i;

    memset(&ctx, 0, sizeof(ctx));
    ctx.loop = g_main_loop_new(NULL, FALSE);

    config = radio_config_new_with_version(RADIO_CONFIG_INTERFACE_MAX);
    if (!config) {
        fprintf(stderr, "error: failed to create radio config client\n");
        g_main_loop_unref(ctx.loop);
        return 1;
    }

    req = radio_config_request_new(config,
        RADIO_CONFIG_REQ_GET_SIM_SLOTS_STATUS,
        NULL,
        status_complete_cb,
        NULL,
        &ctx);

    if (!req) {
        fprintf(stderr, "error: failed to build getSimSlotsStatus request\n");
        radio_config_unref(config);
        g_main_loop_unref(ctx.loop);
        return 1;
    }

    radio_request_set_timeout(req, 5000);
    if (!radio_request_submit(req)) {
        fprintf(stderr, "error: failed to submit getSimSlotsStatus request\n");
        radio_request_unref(req);
        radio_config_unref(config);
        g_main_loop_unref(ctx.loop);
        return 1;
    }

    radio_request_unref(req);
    g_main_loop_run(ctx.loop);

    if (!ctx.ok) {
        fprintf(stderr, "error: %s\n", ctx.error ? ctx.error : "getSimSlotsStatus failed");
        status_ctx_clear(&ctx);
        radio_config_unref(config);
        g_main_loop_unref(ctx.loop);
        return 1;
    }



    printf("slots=%u\n", (unsigned)ctx.slot_count);
    for (i = 0; i < ctx.slot_count; i++) {
        printf("slot%u_enabled=%s\n", (unsigned)(i + 1),
            ctx.slots[i].enabled ? "true" : "false");

        if (ctx.slots[i].online != ctx.slots[i].enabled) {
            printf("slot%u_online=%s\n", (unsigned)(i + 1),
                ctx.slots[i].online ? "true" : "false");
        }

        printf("slot%u_eid=%s\n", (unsigned)(i + 1),
            ctx.slots[i].eid ? ctx.slots[i].eid : "");
        printf("slot%u_iccid=%s\n", (unsigned)(i + 1),
            ctx.slots[i].iccid ? ctx.slots[i].iccid : "");
    }

    status_ctx_clear(&ctx);
    radio_config_unref(config);
    g_main_loop_unref(ctx.loop);
    return 0;
}

static
void
print_usage(
    const char* prog)
{
    fprintf(stderr, "usage: %s status\n", prog);
}

int
main(
    int argc,
    char* argv[])
{
    if (argc == 2 && !g_strcmp0(argv[1], "status")) {
        return command_status();
    }

    print_usage(argv[0]);
    return 1;
}
