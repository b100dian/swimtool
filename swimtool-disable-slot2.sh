#!/bin/sh
TMPFILE="$(mktemp)"
rm -f /run/disable-slot2

/usr/bin/swimtool status 1> "$TMPFILE"
. "$TMPFILE"
rm -f "$TMPFILE"

if [ "${slot2_enabled:-false}" = "true" ] && [ -n "${slot2_eid:-}" ] && [ -z "${slot2_iccid:-}" ]; then
    echo "We recommend that you sim2 is disabled."
    : > /run/disable-slot2
else
    echo "sim2 looks healthy"
fi
