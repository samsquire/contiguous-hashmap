# contiguous hashmap

I am experimenting with high throughput multithreaded systems and left-right concurrency control.

One of the obstacles of left-right concurrency control is the memory usage, we have to duplicate data structures to provide for thread safety.

This allows every thread to read/write in parallel for maximum throughput, at the cost of memory usage.

I was curious how easy it was to create a hashmap that was contiguous, for cheap copies. On my machine it can copy 52GB in 5 seconds.
