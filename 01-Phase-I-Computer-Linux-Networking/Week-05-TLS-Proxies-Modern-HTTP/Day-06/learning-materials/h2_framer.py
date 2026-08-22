import struct

# Frame Types
FRAME_DATA = 0x00
FRAME_HEADERS = 0x01
FRAME_SETTINGS = 0x04
FRAME_RST_STREAM = 0x03

# Flags
FLAG_END_STREAM = 0x01
FLAG_END_HEADERS = 0x04

def pack_h2_frame(frame_type: int, flags: int, stream_id: int, payload: bytes) -> bytes:
    """
    Constructs a 9-byte HTTP/2 frame header + payload.
    Header format: Length (24-bit), Type (8-bit), Flags (8-bit), R+StreamID (32-bit).
    """
    length = len(payload)
    if length > 0xFFFFFF:
        raise ValueError("Payload exceeds 24-bit max frame size")
    
    # Pack: 24-bit length (as 3 bytes), 1-byte type, 1-byte flags
    length_bytes = length.to_bytes(3, byteorder='big')
    header_start = struct.pack("!BB", frame_type, flags)
    
    # 31-bit Stream ID (Mask out reserved top bit)
    stream_id_bytes = struct.pack("!I", stream_id & 0x7FFFFFFF)
    
    return length_bytes + header_start + stream_id_bytes + payload

def unpack_h2_frame(raw_bytes: bytes) -> tuple[int, int, int, bytes, bytes]:
    """
    Unpacks a 9-byte HTTP/2 frame header from a byte buffer.
    Returns: (frame_type, flags, stream_id, payload, remaining_buffer)
    """
    if len(raw_bytes) < 9:
        raise ValueError("Incomplete frame header (less than 9 bytes)")

    # Read 24-bit length
    length = int.from_bytes(raw_bytes[0:3], byteorder='big')
    frame_type, flags = struct.unpack("!BB", raw_bytes[3:5])
    stream_id = struct.unpack("!I", raw_bytes[5:9])[0] & 0x7FFFFFFF

    total_frame_len = 9 + length
    if len(raw_bytes) < total_frame_len:
        raise ValueError(f"Buffer has {len(raw_bytes)} bytes, need {total_frame_len}")

    payload = raw_bytes[9:total_frame_len]
    remaining = raw_bytes[total_frame_len:]
    return frame_type, flags, stream_id, payload, remaining

def run_simulation():
    print("=== 1. ENCODING MULTIPLEXED HTTP/2 FRAMES ===")
    
    # Create frames for two separate concurrent streams
    # Stream 1: GET /large-video (Chunk 1)
    frame1 = pack_h2_frame(FRAME_DATA, 0x00, 1, b"[Video Bytes Part 1]")
    
    # Stream 3: GET /style.css (Small file, complete with END_STREAM)
    frame2 = pack_h2_frame(FRAME_DATA, FLAG_END_STREAM, 3, b"body { background: #000; }")
    
    # Stream 1: GET /large-video (Chunk 2, complete with END_STREAM)
    frame3 = pack_h2_frame(FRAME_DATA, FLAG_END_STREAM, 1, b"[Video Bytes Part 2]")

    # Interleave all frames across a single continuous byte stream (Single TCP pipe)
    tcp_wire_buffer = frame1 + frame2 + frame3
    print(f"Total Multiplexed Wire Bytes: {len(tcp_wire_buffer)}")

    print("\n=== 2. RECEIVER DEMULTIPLEXING FRAMES ===")
    buffer = tcp_wire_buffer
    stream_data = {}

    while buffer:
        ftype, flags, stream_id, payload, buffer = unpack_h2_frame(buffer)
        if stream_id not in stream_data:
            stream_data[stream_id] = []
        stream_data[stream_id].append(payload)
        
        is_ended = bool(flags & FLAG_END_STREAM)
        print(f"Read Frame -> Type: {ftype:#04x}, Stream ID: {stream_id}, Payload: {payload.decode('latin1')}, Stream Finished: {is_ended}")

    print("\n=== 3. RECONSTRUCTED STREAMS ===")
    for sid, chunks in stream_data.items():
        full_body = b"".join(chunks)
        print(f"Stream {sid} Final Reassembled Body: {full_body.decode('latin1')}")

if __name__ == "__main__":
    run_simulation()