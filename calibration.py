import asyncio
import websockets

CALIBRATION_SAMPLES = 80       
PROGRESS_EVERY = 20             

def parse_sample(message: str):
    """
    Parse 'mag,deg,mag2,deg2' from the Arduino message.
    Returns (magL, degL, magR, degR) as floats, or None on error.
    """
    try:
        parts = message.strip().split(',')
        if len(parts) < 4:
            return None
        magL = float(parts[0])
        degL = float(parts[1])
        magR = float(parts[2])
        degR = float(parts[3])
        return magL, degL, magR, degR
    except ValueError:
        return None


async def handler(websocket):
    print("client connected")

    # Calibration state
    sample_count = 0

    left_mag_min = float("inf")
    left_mag_max = float("-inf")
    left_deg_min = float("inf")
    left_deg_max = float("-inf")

    right_mag_min = float("inf")
    right_mag_max = float("-inf")
    right_deg_min = float("inf")
    right_deg_max = float("-inf")

    def reset_stats():
        nonlocal sample_count
        nonlocal left_mag_min, left_mag_max, left_deg_min, left_deg_max
        nonlocal right_mag_min, right_mag_max, right_deg_min, right_deg_max

        sample_count = 0
        left_mag_min = float("inf")
        left_mag_max = float("-inf")
        left_deg_min = float("inf")
        left_deg_max = float("-inf")
        right_mag_min = float("inf")
        right_mag_max = float("-inf")
        right_deg_min = float("inf")
        right_deg_max = float("-inf")

    reset_stats()

    try:
        async for message in websocket:
            parsed = parse_sample(message)
            if parsed is None:
                # Bad packet; just ignore it quietly
                continue

            magL, degL, magR, degR = parsed
            sample_count += 1

            # Update mins/maxes
            left_mag_min = min(left_mag_min, magL)
            left_mag_max = max(left_mag_max, magL)
            left_deg_min = min(left_deg_min, degL)
            left_deg_max = max(left_deg_max, degL)

            right_mag_min = min(right_mag_min, magR)
            right_mag_max = max(right_mag_max, magR)
            right_deg_min = min(right_deg_min, degR)
            right_deg_max = max(right_deg_max, degR)

            # Optional small progress print (no raw data)
            if PROGRESS_EVERY and (sample_count % PROGRESS_EVERY == 0):
                print(f"[CALIB] collected {sample_count} samples...")

            # When enough samples are collected, print summary
            if sample_count >= CALIBRATION_SAMPLES:
                print("\n========== CALIBRATION SUMMARY ==========")
                print(f"Samples collected: {sample_count}")
                print("")
                print("LEFT SENSOR:")
                print(f"  MAG_MIN  = {left_mag_min}")
                print(f"  MAG_MAX  = {left_mag_max}")
                print(f"  DEG_MIN  = {left_deg_min}")
                print(f"  DEG_MAX  = {left_deg_max}")
                print("")
                print("RIGHT SENSOR:")
                print(f"  MAG_MIN  = {right_mag_min}")
                print(f"  MAG_MAX  = {right_mag_max}")
                print(f"  DEG_MIN  = {right_deg_min}")
                print(f"  DEG_MAX  = {right_deg_max}")
                print("=========================================\n")

                reset_stats()

    except Exception as e:
        print("client disconnected or error:", e)


async def main():
    async with websockets.serve(handler, "0.0.0.0", 8080):
        print("WS server running on ws://0.0.0.0:8080")
        await asyncio.Future()

asyncio.run(main())
