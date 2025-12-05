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
size_t cap = 0;
getline(&line, &cap, fp);
```

### `strchr()` – find a character
```c
char *p = strchr(line, '-');
```

### `strstr()` – find substring
```c
if (strstr(line, "foo")) { ... }
```

### `strtol()` / `strtoll()` – safe integer parsing
```c
long long v = strtoll(tok, NULL, 10);
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

### Get one line without `getline()`
```c
char buf[4096];
fgets(buf, sizeof(buf), fp);
```

---

### Quick tokenize helper
```c
char tmp[256];
strcpy(tmp, line);

for (char *t = strtok(tmp, ","); t; t = strtok(NULL, ",")) {
    // use t
}
```
