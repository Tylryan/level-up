# LOGGER
## Purpose
A logger macro that will print out the file, function, and line where the macro was called.

## Example
See at `example.c` and run `make example`

### Input
```c
	LOG(INFO, "Just lettin' you know: %d\n", 5);
	LOG(WARN, "Whoo there nelly!");
	LOG(ERR, "Sound the alarms!!");
```

### Output
```bash
| INFO | [Wed Nov 22 17:04:44 2023] | example.c:main():6        | Just lettin' you know: 5
| WARN | [Wed Nov 22 17:04:44 2023] | example.c:main():7        | Whoo there nelly!
| ERR | [Wed Nov 22 17:04:44 2023] | example.c:main():8 | Sound the alarms!!
```
