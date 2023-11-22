# DESCRIPTION
A simple vector struct

# Usage
Most of the vector's functions are pretty self explanitory. However, there are a couple that could do with an explanation.

## v_insert(), v_remove()
Any function where the vector is altered by index, the index is allowed to be negative. In the case where an index is negative, it's position is relative to the end of the vector.
```c
vector_t vec = v_create(2);
v_add(&vec, (void)*1);
v_add(&vec, (void*)2);
v_insert(&vec, (void*)5, -1);    // This is the same thing as v_add()
v_remove(&vec, -2);              // Remove the second to last element in the vector.
v_free(&vec);
```

# Note
Types without pointers (int, char, etc) need to be cast to `void *` before being added to the vector and cast back to their respective types when interacting with them to avoid warnings. However, `char *`, `structs`, etc can just be added as normal. This is probably a bug.
```c

vector_t vec = v_create(2);
v_add(&vec, (void*)5);
int * val = (int*) v_get(&vec, -1);

```
