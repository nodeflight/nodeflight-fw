# NodeFlight Control Protocol - NFCP

> Note: This is a draft, and may get breaking changes prior to first stable release of NodeFlight. Parts that expects updates is marked with TBD, however, other parts may get updates too.

NodeFlight Control Protocol is a protocol designed for high reliability and flexibility.

Key features:
- Resilience - handles bit errors
- Extensible - allow for more features to be added, without interfering with existing features
- Stateless - The connection does not have different modes of operation (might be allowed for specific features)
- Modular - Features and behaviour should be possible to integrate to other carriers

To achieve those key features, some structural concepts is used:
- Packet based - to allow checksum validation on all data, acknowledgement on required.
- Functional addressing - All packets are addressed to the given functionality/module, to interleave functionality
- Layered design, so physical layer can be replaced for potential extensions, (for example wireless)

## Structure

NFCP can be described using a layered structure:

| Layer | Name        | Description                                         | Example                        |
| ----- | ----------- | --------------------------------------------------- | ------------------------------ |
| 4     | Application | Specific functionality                              | management, config, monitor    |
| 3     | Transport   | Request/response, session handling, addressing      |                                |
| 2     | Framing     | Serializing packets to stream + checksum validation | HDLC-like asynchronous framing |
| 1     | Physical    | Physical hardware, electrical characteristics...    | UART, USB VCP                  |

## Terminology

Generic terms used in the NFCP protocol:

| Term        | Description                                                                               |
| ----------- | ----------------------------------------------------------------------------------------- |
| Upper layer | The layer directly above (higher number) than the layer described                         |
| Lower layer | The layer directly below (lower number) than the layer described                          |
| Packet      | Container for data, managed as one block. Primary interface for layers between 2, 3 and 4 |
| Frame       | A serialized and encoded block of data available on the physical link as part of a stream |
| RX          | Receiver/receive. Data coming in from lower layer                                         |
| TX          | Transmitter/transmit. Data going out to lower layer                                       |

## Layer 1 - Physical

Currently two different physical interfaces are allowed, UART and USB virtual com port (VCP)

### UART

UART interface uses full duplex, non-inverted UART, configured as:

| Parameter       | Value         | Note                                                                                      |
| --------------- | ------------- | ----------------------------------------------------------------------------------------- |
| Baud rate       | 230400 baud/s |                                                                                           |
| Word length     | 8 bits / word |                                                                                           |
| Parity          | None          | Checksums are available on upper layers                                                   |
| Stop bits       | 1 stop bit    |                                                                                           |
| Handshake       | None          | XON/XOFF bytes is not used by application                                                 |
| Frame delimiter | 0x7e          | Frame delimiter is not part of the UART, but is used to wake up upper layer for reception |

Thus using separate wires for TX and RX.

Interfaces upper layer via a stream interface, providing byte-based rx and tx interface.

### USB VCP

Not yet implemented

## Layer 2 - Framing

Framing is used to allow packet transmission over the link with guarantees (to some degree) for:

- Data integrity - Packets that are received are also valid
- Resilience - Data errors should not freeze up the link, but at most c

Only one framer are allowed for usage on serial links. Other framers may only be used in case the physical layer has specific features that allowes packet based transmission in other ways (for example packet based wireless links, or tunneling over IP network using UDP)

### HDLC-like framing

NFCP adopts the asynchronous framing mode of HDLC. However, it does not adopt the frame format.

#### Cyclic redundancy check/CRC

An incoming packet from upper layer is first padded with 2 bytes a Cyclic redundancy check/CRC using CRC16-CCITT format, MSB first:

CRC polynom used: `0x1021`

```
Packet (hex):   04 7d 7e 03 17 19 08
With CRC (hex): 04 7d 7e 03 17 19 08 7d fb
```

#### Framing

Special bytes not allowed unescaped in a frame is escaped:

| Byte   | Description                                   |
| ------ | --------------------------------------------- |
| `0x7e` | Frame boundary, marks end/start of frame      |
| `0x7d` | Escape char, marks next charachter is escaped |
| `0x17` | XON - reserved for lower layer                |
| `0x19` | XOFF - reserved for lower layer               |

An escaped charachter is replaced by two charachters: First the escape char (`0x7d`), then the charachter to escape xor:ed with `0x20` (flip 5th bit)

The CRC must also be escaped.

Then add a frame boundary (`0x7e`) to the end.

Example:

```
Packet with CRC (hex): 04 7d    7e    03 17    19    08 7d    fb
Framed packet (hex):   04 7d 5d 7d 5e 03 7d 37 7d 39 08 7d 5d fb 7e
```

## Layer 3 - Transport

Three types of packets are available:

- Request packet
- Response packet
- Information packet

For each packet, a class is added to describe the context of the packet.

Upper layer registers classes for which to handle packets. A special class 0 is defined for use by transport layer, called MGMT.

### Packet format

Request and response packets start with 3 byte header header:
```
|         |      7 |      6 |      5 |      4 |      3 |      2 |     1  |      0 |
|---------+-----------------------------------------------------------------------|
| Byte 0  | class                                               | call=1 | resp   |
| Byte 1  | seq nr                                                                |
| Byte 2  | operation                                                             |
| ...     | packet payload                                                        |
```

Information packets starts with 2 byte header: 
```
|         |      7 |      6 |      5 |      4 |      3 |      2 |     1  |      0 |
|---------+-----------------------------------------------------------------------|
| Byte 0  | class                                               | call=0 | resp=0 |
| Byte 1  | operation                                                             |
| ...     | packet payload                                                        |
```

`class` defines packet class. The classes may be added or removed in future versions. Packets with unknown classes should be notified using packet MGMT/UNKNOWN_CLASS (see below)

`seq nr` identifies the request, to correlate the response to a given request and should be incremented for each request. `seq nr` should not directly be reused and should not be reused for different operations. reuse only after wrap-around.

`operation` defines the operation within the class for the packet. `operation` should not have different uses for different packet types. `operation` is however same for both request and response packets.


### Request/reply

A request packet is sent when a response is expected, either containing response data, a status code, or just to get a validation that the packet arrived.

For request packets, a sequence number is included to map the response to the request. The same sequence number is included in the resposnse.

| Host                                                           | Client                                            |
| -------------------------------------------------------------- | ------------------------------------------------- |
| 1. Host increments the local sequence number                   |                                                   |
| 2. Host sends the request to the client including `seq nr`     |                                                   |
|                                                                | 3. Client receives the request                    |
| _other communication may occur_                                | _other communication may occur_                   |
|                                                                | 4. Client sends a response with the same `seq nr` |
| 5. Host processes the response and forwards it to upper layers |                                                   |

When upper layers sends the request, it should specify a timeout for how long until the response should arrive. If timeout, the upper layer are notified with a timeout. If the response arrives late, it will be discarded.

### Connection and session

The connection has two parts:
- host - the part initiating the connection, for example a configuration tool on a computer
- client - the device running NodeFlight

To initiate a session, following sequence should be used: (for packet formats, see below)

| Host                                             | Client                                                 |
| ------------------------------------------------ | ------------------------------------------------------ |
| 1. Generate random session id (not 0)            |                                                        |
| 2. Send an abort sequence (0x7d 0x7e)            |                                                        |
| 3. Send MGMT/SESSION_ID                          |                                                        |
|                                                  | 4. Identify new session id                             |
|                                                  | 5. Send abort sequence                                 |  |
|                                                  | 6. Reply with firmware tag, version and capabilities   |
| 7. Register response and mark session as started |                                                        |
| _session is running_                             | _session is running_                                   |
| 8. Send MGMT/SESSION_ID every 1 second           |                                                        |
|                                                  | 9. Reply with empty ack to indicate session is running |

An abort sequence should always be sent prior to sequence number, to get the client into a known state. In case the client has received any bytes prior to the packet, possibly due to noise from connector, the abort sequence guarantees the previous packet is dropped and the receiver waits for first byte.

The client shall treat the link as closed if:
- If no valid MGMT/SESSION_ID has been received for 3 seconds
- ...or a new MGMT/SESSION_ID is sent opening a new session
- ...or a new MGMT/SESSION_ID with session id=0, explicitly indicating a closed session

The host shall treat the link as lost if:
- No valid MGMT/SESSION_ID response has been received within 3 consecutive requests (indicates lost link)
- ...or a valid response has been received that is not empty (indicates restart of client)

In case of lost link, upper layer should be notified and all ongoing operations should be stopped. No replies for request on the previous link should be sent over.

## Layer 4 - Application

Defines different funcationlity. Each operation may have its own format.

Note that special class 0 - MGMT is described as application packets, but is handled used by Layer 3 - Transport.

A complete list of classes/operations:

| class       | operation             | call=0      | call=1      | Description                       |
| ----------- | --------------------- | ----------- | ----------- | --------------------------------- |
| MGMT (0)    | SESSION_ID (0)        | not allowed | allowed     | Set session id                    |
| MGMT (0)    | LOG_MESSAGE (1)       | allowed     | not allowed | Log message                       |
| MGMT (0)    | INVALID_CLASS (2)     | allowed     | not allowed | Notify invalid class is used      |
| MGMT (0)    | INVALID_OPERATION (3) | allowed     | not allowed | Notify invalid operation is used  |
| FILE (1)    | ...                   | ...         | ...         | TBD: Access virtual file system   |
| MONITOR (2) | ...                   | ...         | ...         | TBD: Monitor state and parameters |

### Packet: MGMT - SESSION_ID

Request packet payload:

| Byte | Content           |
| ---- | ----------------- |
| 0-3  | 4 byte session id |

Response packet payload for first response:

| Byte | Content                                                           |
| ---- | ----------------------------------------------------------------- |
| ...  | TBD: Some extra information about capabilities... not yet defined |
| 0-n  | string, with firmware information                                 |

Response packet for second and upcoming requests:

| Byte | Content |
| ---- | ------- |
| ...  | Empty   |

### Packet: MGMT - LOG_MESSAGE

A printable message. Only allowed as information message.

| Byte  | Content                    |
| ----- | -------------------------- |
| 0     | Context                    |
| 1-... | Message in printable ASCII |

Context is used to color code messages, and/or give diffrent exposure in the UI. Set to 0.

Messages with unknown context should be written to a log, and marked with as unknown context.

The printable message may contain tab characters to identify next field for tabular data

### Packet: MGMT - INVALID_CLASS

Sent as information message when a message is received of unknown class.

| Byte | Content                                                      |
| ---- | ------------------------------------------------------------ |
| 0    | class id + flags (as defined in first byte of packet header) |
| 1    | operation                                                    |
| 2    | seq nr, or 0 if not available                                |

The message should be handled by the receiving node by directly aborting the ongoing request with proper reason. For information messages, this may be used to notify the upper to stop sending the messages.

It may also be logged for inclusion in bug report.

### Packet: MGMT - INVALID_OPERATION

Sent as information message when a message is received of unknown operation, but the class is defined.

| Byte | Content                                                      |
| ---- | ------------------------------------------------------------ |
| 0    | class id + flags (as defined in first byte of packet header) |
| 1    | operation                                                    |
| 2    | seq nr, or 0 if not available                                |

The message should be handled by the receiving node by directly aborting the ongoing request with proper reason, For information messages, this may be used to notify the upper to stop sending the messages.

It may also be logged for inclusion in bug report.