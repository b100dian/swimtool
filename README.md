# Sim Wakeup Interface Manager

Detecting if sims are enabled and if any is an eSIM by connecting to IRadioConfig 1.2

### Example

`swimtool status`

will output

```sh
slots=2
slot1_enabled=true
slot1_eid=
slot1_iccid=<redacted>
slot2_enabled=true
slot2_eid=<redacted>
slot2_iccid=<redacted>
```

meaning: that eid presence is an eSim.

### systemd service

The package installs one `systemd` service:

- `swimtool-disable-slot2.service`
  - Creates `/run/disable-slot2` when `slot2_enabled=true`, `slot2_eid` is non-empty, and `slot2_iccid` is empty (so, 2nd slot is eSIM, but without a profile)
