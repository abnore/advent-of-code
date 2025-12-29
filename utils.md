# Gathering notes Just gathering notes for the remaining week, so i dont have
to man-page everything, and reminding me that these functions exist so I dont
have to reinvent the wheel (sorting algorithm)

## Common `sscanf` Patterns

### Structured input
- Range `A-B`: `%lld-%lld`
- CSV pair `A,B`: `%d,%d`
- Word + number: `%31s %d`
- Skip a value: `%*d` (read but ignore)
- Characters consumed (index of next unread char): `%n`
- Phrase with values: `x=%d y=%d`

### Examples

Ignore first value:
```c
int b;
sscanf("100-200", "%*d-%d", &b);
```

Check success:
```c
int a, b;
if (sscanf(s, "%d-%d", &a, &b) != 2) {
    /* error */
}
```

Count characters consumed:
```c
int value, n;
sscanf("123xyz", "%d%n", &value, &n);  // n = 3
```

---

## Useful Parsing Functions

### `getline()` – read full line (auto-grows buffer)
```c
char *line = NULL;  // getline allocates when line==NULL
size_t cap = 0;     // capacity for line (how much allocated)
getline(&line, &cap, fp);
```

---

## Arrays / Sorting / Manipulation

### `qsort()` – standard sort
```c
static int cmp(const void *a, const void *b) {
    const int *x = a, *y = b;
    return (*x > *y) - (*x < *y);
}

qsort(arr, n, sizeof(arr[0]), cmp);
```

---

## Minimal File Helpers

### Read whole file into a buffer
`fseek`, `rewind` are useful - especially for pt1 and pt2.
`ftell` gives current cursor pos (after `fseek` this equals size in bytes),
`fread` loads everything into memory.

```c
char *read_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    rewind(fp);

    char *buf = malloc(sz + 1);
    fread(buf, 1, sz, fp);
    buf[sz] = '\0';

    fclose(fp);
    return buf; // free() later
}
```

---

### Simple `getline` pattern
```c
char *line = NULL;
size_t cap = 0;
ssize_t n;

while ((n = getline(&line, &cap, fp)) > 0) {
    // use line
}
free(line);
```

---

### Read entire file into a fixed buffer
```c
char bigbuf[500000];
size_t pos = 0;

int c;
while ((c = fgetc(fp)) != EOF) {
    bigbuf[pos++] = (char)c;
}
bigbuf[pos] = '\0';
```

---

### Quick tokenize helper
```c
char tmp[256];
strcpy(tmp, line);
// Never call strtok on original string if you still need it! Modifies!

for (char *t = strtok(tmp, ","); t; t = strtok(NULL, ",")) {
    // use t
}
```
