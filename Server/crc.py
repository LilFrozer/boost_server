# Используется CRC‑CCITT
def crc_ccitt(data, init=0x0000):
    crc = init
    poly = 0x1021
    for byte in data:
        crc ^= byte << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = (crc << 1) ^ poly
            else:
                crc <<= 1
            crc &= 0xFFFF  # Ограничение до 16 бит
    return crc


# Данные в байтах - 38f6
data = bytes.fromhex("A590006C0000000000000400000038f6")
result = crc_ccitt(data)
print(f"CRC-CCITT: 0x{result:04X}")
