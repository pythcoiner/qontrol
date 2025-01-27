# Convert pictures to binary .h file

```
xxd -i minus.png minus.h
```

then use it

```cpp
#include "minus.h"
QPixmap pixmap;
pixmap.loadFromData(minus_png, minus_png_len);
QIcon icon(pixmap);
```
