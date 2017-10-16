meta:
  id: firmata
  file-extension: firmata
  title: Firmata protocol
seq:
  - id: message
    type: message
    repeat: eos
enums:
  pin_modes:
    0: input
    1: output
    2: analog
    3: pwm
    4: servo
    6: i2c
    7: onewire
    8: stepper
    9: encoder
    10: serial
    11: pullup

types:
  message:
    seq:
      - id: message_header
        type: u1

      # sysEx
      - id: sysex
        if: message_header == 0xF0
        type: sysex
        doc: et


      # normal message
      - id: message_body
        if: message_header != 0xF0
        type:
          switch-on: event_type
          cases:
            0xE0: analog_io
            0x90: digital_io
            0xC0: report_analog_pin
            0xD0: report_digital_port
            0xFE: protocol_version
            0xF4: set_pin_mode
            0xF5: set_digital_pin_value

    instances:
      event_type:
        value: message_header & 0xF0
      channel:
        value: message_header & 0x0F

  analog_io:
    seq:
      - id: lsb
        type: u1
      - id: msb
        type: u1
    instances:
      value:
        value: (0xE0 & lsb) | ((0xE0 & msb) << 7)

  digital_io:
    seq:
      - id: lsb
        type: u1
      - id: msb
        type: u1
    instances:
      value:
        value: (0xE0 & lsb) | ((0xE0 & msb) << 7)

  report_analog_pin:
    seq:
      - id: state
        type: u1

  report_digital_port:
    seq:
      - id: state
        type: u1

  protocol_version:
    seq:
      - id: major
        type: u1
      - id: minor
        type: u1

  set_pin_mode:
    seq:
      - id: pin
        type: u1
      - id: mode
        type: u1
        enum: pin_modes

  set_digital_pin_value:
    seq:
      - id: pin
        type: u1
      - id: value
        type: u1

  sysex:
    seq:
      - id: id
        type: u1
      - id: body
        type: sysex_body

  sysex_body:
    seq:
      - id: body
        type:
          switch-on: _parent.id
          cases:
            0x00: sysex_ext
            0x6F: extended_analog
            0x71: string_bit7
            0x79: report_firmware
            0x7A: sampling_interval
            0x7B: capability_query
            0x7B: capability_response


  sysex_ext:
    seq:
      - id: lsb
        type: u1
      - id: msb
        type: u1
      - id: body
        type: string_bit7

    instances:
      id:
        value: (0xEF & lsb) | (0xEF & msb) << 7

  report_firmware:
    seq:
      - id: major
        type: u1
      - id: minor
        type: u1
      - id: str
        type: string_bit7

  # sysex sampling interval
  sampling_interval:
    seq:
      - id: lsb
        type: u1
      - id: msb
        type: u1

    instances:
      interval:
        value: (0xE0 & lsb) | ((0xE0 & msb) << 7)

  string_bit7:
    seq:
      - id: b
        type: u1
        repeat: until
        repeat-until: _ != 0xFF

  capability_query:
    doc: Request for capability

  pin_capability_list:
    seq:
      - id: modes
        type: pin_capability
        repeat: until
        repeat-until: _.mode != 0x7F
    types:
      pin_capability:
        seq:
        - id: mode
          type: u1
        - id: resolution
          type: u1


  capability_response:
    seq:
      - id: b
        type: u1

  # sysex extended analog
  extended_analog:
    seq:
      - id: pin
        type: u1
      - id: value
        type: string_bit7
