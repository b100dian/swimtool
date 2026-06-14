#ifndef SWIMTOOL_RADIO_CONFIG_COMPAT_H
#define SWIMTOOL_RADIO_CONFIG_COMPAT_H

#include <radio_types.h>
#include <radio_config_types.h>

G_BEGIN_DECLS

RadioConfig*
radio_config_new_with_version(
    RADIO_CONFIG_INTERFACE max_version)
    G_GNUC_WARN_UNUSED_RESULT;

void
radio_config_unref(
    RadioConfig* config);

G_END_DECLS

#endif /* SWIMTOOL_RADIO_CONFIG_COMPAT_H */
