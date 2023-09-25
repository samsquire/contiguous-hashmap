# contiguous hashmap

I am experimenting with high throughput multithreaded systems and left-right concurrency control.

One of the obstacles of left-right concurrency control is the memory usage, we have to duplicate data structures to provide for thread safety.

This allows every thread to read/write in parallel for maximum throughput, at the cost of memory usage.

I was curious how easy it was to create a hashmap that was contiguous, for cheap copies. On my machine it can copy 52GB in 5 seconds.

# credits

Hash function is by djb2 from https://stackoverflow.com/a/7666577/10662977

Everything else is 

# LICENCE

BSD Zero Clause License

Copyright (C) 2023 by Samuel Michael Squire sam@samsquire.com

Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
