from pathlib import Path

def mp3_to_c_array(mp3_path, output_path, array_name="audio_data"):
    binary_data = Path(mp3_path).read_bytes()
    
    hex_values = [f"0x{b:02x}" for b in binary_data]
    
    # Format into rows of 16 bytes each
    rows = [hex_values[i:i+16] for i in range(0, len(hex_values), 16)]
    formatted = ",\n    ".join(", ".join(row) for row in rows)
    
    c_code = f"""#ifndef {array_name.upper()}_H
#define {array_name.upper()}_H

#include <stdint.h>

const uint8_t {array_name}[] = {{
    {formatted}
}};
const uint32_t {array_name}_len = {len(binary_data)};

#endif
"""
    
    Path(output_path).write_text(c_code, encoding="utf-8")
    print(f"Length: {len(binary_data)} bytes")
    print(f"Saved to {output_path}")

mp3_to_c_array("audio_out1.raw", "audio_data1.h")


"""
Check the audio file sampling rate:
ffprobe filename.mp3 2>&1 | grep Audio

Convert to .raw:
ffmpeg -i original_mp3_filename.mp3 -ar 16000 -ac 1 -f s16le -acodec pcm_s16le output_filename.raw

"""