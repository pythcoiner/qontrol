# Memory Leak Detection

This document describes how to detect and diagnose memory leaks in the qontrol framework using AddressSanitizer.

## Quick Start

```bash
cmake -B build -DENABLE_ASAN=ON
cmake --build build
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

## Build Options

```
+--------------+-------------------------------------------------------------+
| Option       | Description                                                 |
+--------------+-------------------------------------------------------------+
| ENABLE_ASAN  | AddressSanitizer - comprehensive memory error detection     |
| ENABLE_LSAN  | LeakSanitizer only - lightweight leak detection             |
+--------------+-------------------------------------------------------------+
```

## AddressSanitizer

AddressSanitizer (ASan) is the recommended tool for memory error detection. It detects:

- **Memory leaks** - allocated memory never freed
- **Buffer overflows** - stack and heap out-of-bounds access
- **Use-after-free** - accessing memory after it's been deallocated
- **Use-after-return** - using stack memory after function returns
- **Double-free** - freeing the same memory twice

### Build

```bash
cmake -B build -DENABLE_ASAN=ON
cmake --build build
```

### Run Tests

```bash
# For headless environments (CI, SSH)
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child

# With detailed statistics
ASAN_OPTIONS=detect_leaks=1:print_stats=1 QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

### Environment Variables

```
+--------------+------------------------+----------------------------------------+
| Variable     | Example                | Description                            |
+--------------+------------------------+----------------------------------------+
| ASAN_OPTIONS | detect_leaks=1         | Enable leak detection                  |
| ASAN_OPTIONS | print_stats=1          | Print memory statistics                |
| ASAN_OPTIONS | abort_on_error=1       | Abort on first error (for debugging)   |
| ASAN_OPTIONS | log_path=/tmp/asan.log | Write report to file                   |
+--------------+------------------------+----------------------------------------+
```

Multiple options can be combined with colons:
```bash
ASAN_OPTIONS=detect_leaks=1:print_stats=1:abort_on_error=1 ./your_app
```

## LeakSanitizer Only

For lightweight leak-only detection without full ASan overhead:

```bash
cmake -B build -DENABLE_LSAN=ON
cmake --build build
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

### Environment Variables

```bash
LSAN_OPTIONS=verbosity=1 ./your_app
LSAN_OPTIONS=suppressions=lsan.supp ./your_app
```

## Suppressions

Qt may report false positives for intentional allocations (static objects, caches). Create `lsan.supp`:

```
# Qt platform plugin
leak:libqxcb
leak:QGuiApplicationPrivate

# Qt font database
leak:QFontDatabase
```

Use with:
```bash
LSAN_OPTIONS=suppressions=lsan.supp ./your_app
```

## Interpreting Results

```
+------------------+----------------------------------------------+----------------------+
| Type             | Description                                  | Action               |
+------------------+----------------------------------------------+----------------------+
| Definite leak    | Memory allocated, never freed, unreachable   | Fix required         |
| Indirect leak    | Reachable only through a definite leak       | Fix the definite leak|
| Possible leak    | Ambiguous (interior pointers, complex structs)| Investigate         |
| Still reachable  | Accessible at exit (singletons, caches)      | Usually acceptable   |
+------------------+----------------------------------------------+----------------------+
```

### Example Output

```
==12345==ERROR: LeakSanitizer: detected memory leaks

Direct leak of 64 byte(s) in 1 object(s) allocated from:
    #0 0x7f... in operator new(unsigned long)
    #1 0x55... in MyClass::MyClass() src/MyClass.cpp:42
    #2 0x55... in main src/main.cpp:10

SUMMARY: AddressSanitizer: 64 byte(s) leaked in 1 allocation(s).
```

## Memory Management in qontrol

The framework uses Qt's parent-child ownership model:

```
+----------------------+-------------------------------+-------------------------+
| Pattern              | Ownership                     | Cleanup                 |
+----------------------+-------------------------------+-------------------------+
| push(widget)         | Parent set via setParent(this)| Automatic (Qt)          |
| new Widget(parent)   | Explicit parent               | Automatic (Qt)          |
| new QMap<K,V>        | No parent (non-QObject)       | Manual delete in dtor   |
+----------------------+-------------------------------+-------------------------+
```

### Examples

```cpp
// Safe: parent set automatically by push()
auto *row = new Row(parent);
row->push(new QLabel("text"));  // Label owned by row

// Safe: explicit parent in constructor
auto *label = new QLabel("text", parent);

// Requires manual cleanup: non-QObject types
m_map = new QMap<int, int>;  // Must delete in destructor
```

## CI Integration

```yaml
test-memory:
  script:
    - cmake -B build -DENABLE_ASAN=ON
    - cmake --build build
    - QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

## Troubleshooting

### Tests not found with ctest
Run the test executable directly:
```bash
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

### Qt platform errors
Set the offscreen platform for headless environments:
```bash
QT_QPA_PLATFORM=offscreen ./your_app
```

### Slow execution
ASan adds ~2x overhead. Use `ENABLE_LSAN` for faster leak-only checks.
