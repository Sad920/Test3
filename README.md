# Matrix Multiplier GUI

A simple Qt-based desktop application that multiplies two matrices (up to 10x10 each) via a clear graphical interface. Use the spin boxes to set matrix dimensions, edit the table cells to change values, and click **Multiply** to compute the product.

## Building

```bash
cmake -S . -B build
cmake --build build
./build/matrix_multiplier
```

Qt 5 or Qt 6 with the Widgets module is required.
