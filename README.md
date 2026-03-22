# 📖 Get Next Line — 42 Project | 125/125

## 📌 Project Goal

**Get Next Line** challenges you to write a function that reads a file descriptor **one line at a time**, returning each line (including the trailing `\n`) on successive calls — just like Python's `readline()` or C's `fgets()`, but implemented entirely from scratch.

```c
char *get_next_line(int fd);
```

The function must work correctly across multiple calls on the same fd, handle multiple different file descriptors simultaneously (bonus), and behave correctly with any **BUFFER_SIZE** — from 1 byte to 100,000+.

---

## 🧠 Key Concepts

### Static Variables
The core of GNL is a **static variable** — a variable that persists its value between function calls without being global. It stores the **leftover** bytes read from the fd that weren't part of the returned line yet.

```c
char *get_next_line(int fd)
{
    static char *leftover;
    // ...
}
```

Each call to `get_next_line` may read more bytes than needed for one line. The excess is saved in `leftover` and used at the start of the next call.

### `read()` and BUFFER_SIZE
The function reads from the fd in chunks of exactly `BUFFER_SIZE` bytes. `BUFFER_SIZE` is defined at compile time:

```bash
cc -D BUFFER_SIZE=42 get_next_line.c
```

Your function must work correctly for **any** BUFFER_SIZE ≥ 1. A buffer of 1 means reading byte by byte; a buffer of 100000 means reading massive chunks. Both must produce identical results.

### The Read Loop
The logic per call:
1. Check if `leftover` already contains a `\n` → if so, extract the line immediately without calling `read()`
2. Otherwise, keep calling `read(BUFFER_SIZE)` and appending to `leftover` until a `\n` is found or `read()` returns 0 (EOF)
3. Extract everything up to and including `\n` as the return value
4. Save the remainder back into `leftover` for the next call
5. On EOF with no remaining data, free `leftover`, set it to NULL, and return NULL

### Memory Management
Every intermediate buffer and the returned line is heap-allocated. You must:
- Free the read buffer after each `read()` call
- Free `leftover` when it's consumed and replaced
- Never leak memory between calls or on EOF/error

### Bonus — Multiple File Descriptors
The bonus requires handling several fds **simultaneously** without them interfering. A single static pointer only tracks one fd's leftover. The solution: use a **static array** (or similar structure) indexed by fd number.

```c
static char *leftover[OPEN_MAX];
```

Each fd gets its own independent `leftover` slot, so reading alternately from fd 3, 4, and 5 works correctly.

---

## 🚀 Usage

### Compilation
```bash
# Mandatory
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl

# Bonus (multiple fds)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
```

### Function Signature
```c
// Mandatory
char *get_next_line(int fd);

// Bonus
char *get_next_line(int fd);  // same signature, multiple fd support internally
```

### Example Usage
```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

### Reading from stdin
```c
// fd = 0 reads from standard input
char *line = get_next_line(0);
```

### Testing with different BUFFER_SIZE values
```bash
cc -D BUFFER_SIZE=1   ... && ./gnl    # byte by byte
cc -D BUFFER_SIZE=42  ... && ./gnl    # typical
cc -D BUFFER_SIZE=9999 ... && ./gnl   # large chunks
```

---

## 🏗️ Implementation Logic

### Call Flow
```
get_next_line(fd)
 ├── 1. Check leftover for '\n' → extract line → update leftover → return line
 ├── 2. read() BUFFER_SIZE bytes into buf
 │    ├── Append buf to leftover
 │    ├── Found '\n' → extract line → update leftover → return line
 │    ├── read() == 0 (EOF) → return leftover as last line (no '\n'), set leftover = NULL
 │    └── read() < 0 (error) → free leftover, set to NULL, return NULL
 └── 3. Loop back to step 2
```

### Key Helper Functions
You are only allowed functions from your `_utils` file. Typical helpers needed:

| Helper | Purpose |
|---|---|
| `ft_strlen` | Length of a string |
| `ft_strchr` | Find `\n` in a string |
| `ft_strjoin` | Concatenate two strings into a new allocation |
| `ft_substr` | Extract a substring |

### Extracting the Line
Once a `\n` is found at position `i` in `leftover`:
- **line** = `leftover[0..i]` (inclusive of `\n`)
- **new leftover** = `leftover[i+1..]` (the remainder, may be empty → set to NULL)

---

## ⚠️ Common Mistakes & Tricky Points

- **BUFFER_SIZE of 1**: Reading one byte at a time is a valid edge case. Make sure your logic doesn't assume the buffer ever contains a full line.
- **EOF with no trailing newline**: The last line of many files has no `\n`. When `read()` returns 0, return whatever is left in `leftover` (if anything) — don't discard it.
- **Freeing `leftover` correctly**: When you split `leftover` into the returned line and the new leftover, free the old `leftover` string — not the new one. A common mistake is freeing after both pointers point to the same allocation.
- **Error handling**: If `read()` returns -1, free `leftover`, set it to NULL, and return NULL. Don't leak.
- **fd validity**: Don't validate fd beyond what `read()` already does — if it errors, handle the return value of `read()`.
- **Empty file**: `read()` returns 0 immediately with `leftover` still NULL → return NULL right away.
- **Bonus fd collisions**: With the array approach, make sure fds above your array size don't cause out-of-bounds access. Use `OPEN_MAX` or a safe upper bound.

---

## ✅ Evaluation Checklist

- [ ] Returns each line with its trailing `\n` (if present)
- [ ] Returns `NULL` on EOF or error
- [ ] Works with BUFFER_SIZE=1, small values, and very large values
- [ ] Works on regular files, stdin, and (optionally) other fds
- [ ] No memory leaks between calls or on EOF
- [ ] Last line without `\n` is returned correctly
- [ ] Empty file returns NULL immediately
- [ ] **Bonus**: multiple fds read concurrently without interference

---

## 🛠️ Useful Debug Commands

```bash
# Check for memory leaks
valgrind --leak-check=full ./gnl

# Test with a file that has no trailing newline
printf "hello\nworld" > no_newline.txt

# Test with an empty file
touch empty.txt

# Test with a very long line
python3 -c "print('a' * 100000)" > long_line.txt

# Test multiple buffer sizes in one script
for bs in 1 5 42 1000 9999; do
    cc -D BUFFER_SIZE=$bs get_next_line.c get_next_line_utils.c main.c && ./a.out
done
```
