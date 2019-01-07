= BLE Midi Helper

This library provides utility functions for reconstructing Midi messages from BLE notifications. It uses a ring buffer to store messages until they are dispatched. Once the ring buffer is full incoming Midi messages will be silently discarded.

