# Sim Wake Information Monitor

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

meaningi: that eid presence is an eSim.
