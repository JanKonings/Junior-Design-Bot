import asyncio
import websockets

async def handler(websocket):
    print("client connected")
    try:
        async for message in websocket:
            print("from arduino:", message)
            # echo back
            await websocket.send("ACK:"+message)
    except:
        print("client disconnected")

async def main():
    async with websockets.serve(handler, "0.0.0.0", 8080):
        print("WS server running on ws://localhost:8080")
        await asyncio.Future()

asyncio.run(main())