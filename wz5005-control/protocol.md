## Command / response structure

Each command seems to be exactly 20 bytes long.

Commands trigger a response from the device. The response has the exact same structure, except command parameters are substituted with response values.

Here's a byte breakdown of contents of a message:

| Bytes | Meaning |
| :-: | - |
| 1 | Header; Static value |
| 2 | Device number |
| 3 | Command |
| 4-19 | Command arguments / response values |
| 20 | Checksum byte |

### Header

Always set to the same value 0xAA

### Device number

There's a command to set a device's number - I haven't tested this, but it seems we can have multiple devices on the serial bus.
In this case this byte should tell them which one we are talking to.

### Command 

Number if the command we want to issue. Check the commands table for possible values.

### Command arguments

If a command takes arguments (e.g. set voltage, set device number) - this is where we place them.

Unused arguments should be set to 0x00.

### Checksum

Single-byte checksum value. Not yet tested, but most likely the device will reject commands with bad checksums. TODO: Determine checksum algo.


## Available commands

### `0x20` Set operating mode
Argument bytes: 
| Byte | 1 | 2-16 |
|-|-|-|
| Request | `0` for manual operation, `1` for remote control | 0x00 |
| Response | TODO | TODO |

Example: Put the device in remote control mode:
0xAA 0x01 0x20 0x01 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0xCC

Device can be set either in manual or remote control mode. Remote control seems to disable physical buttons on the device.

TODO: What if device is disconnected from bus? Will it default back to manual operation?

TODO: Can the device be remotely controlled in manual mode?

### `0x21` Set device address

| Byte | 1 | 2-16 |
|-|-|-|
| Request | New device address (0x00 - 0xFF) | 0x00 |
| Response | TODO | TODO |

TODO: Is this correct?

In multi-device scenario, this command should be used to give each device on the same bus a unique address (basically an ID).

The device address must then be used when communicating with this device.

For a new device, the device address is `0x01`.

### `0x22` Set default output status

| Byte | 1 | 2-16 |
|-|-|-|
| Request | `0` - Off, `1` - On | 0x00 |
| Response | TODO | TODO |

Turns the power output on or off. TODO: Verify

### `0x23` Get status

| Byte | 1 | 2 | 3 | 4-16 |
|-|-|-|-|-|
| Request | 0x00 | 0x00 | 0x00 | 0x00 |
| Response | Output status | CC/CV Status | Abnormal state | TODO |

**Output status**: 
- `0` = off
- `1` = on
- 
**CC/CV Status**:
- `0` = CV Mode (constant voltage)
- `1` = CC Mode (constant current)

**Abnormal state**:
- `0` = No abnormal state
- `1` = Overvoltage (output voltage crossed the overvoltage value)
- `2` = Overcurrent (output current crossed the overcurrent value)

Requests various status flags from the device.

### `0x24` Get device factory info

| Byte | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8-16 |
|-|-|-|-|-|-|-|-|-|
| Request | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 | 0x00 |
| Response | Model | Version | Version | Item ID | Item ID | Item ID | Item ID | TODO |

TODO: Is this correct?

Retrieves some factory values, like device model and ID (serial number?)

Version is 2 bytes (high to low), Item ID is 4 bytes (highest to lowest). Data type unknown - TODO

### `0x25` / `0x26` Various settings

TODO

### `0x29` Get power output values

TODO

### `0x2A` Get cumulative statistics

TODO

### `0x2B` / `0x2C` Get/Set voltage/current/protection

Example: Get the voltage/current/protection
0xAA 0x01 0x2B 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0xD6

Example: Set the voltage to 5V
0xAA 0x01 0x2C 0x13 0x88 0x12 0xAB 0x01
0xF4 0x00 0x04 0x00 0x00 0x00 0x42 0x00
0x00 0x00 0x00 0x6A

