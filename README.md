[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/zegDotdy)
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
# Name : Chidinma Obi-Okoye
# SID : 1756548
# CCID : obiokoye
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# CMPUT 379 - Assignment 3: UNIX File System Simulator

## Overview

This program simulates a UNIX-like file system with the following features:
- 128 KB virtual disk (128 blocks × 1 KB each)
- Contiguous block allocation for files
- Hierarchical directory structure
- Consistency checking on mount
- Defragmentation support

## Design Choices

### 1. Global State Management
- **Rationale**: File system operations are inherently stateful (mounted disk, current directory, buffer)
- **Implementation**: Used static global variables for `sb`, `buffer`, `current_dir_inode`, etc.
- **Benefit**: Simplified function signatures and avoided passing state through all function calls

### 2. Helper Function Architecture
- **Bit Manipulation Helpers**: Abstract away bitfield operations for maintainability
- **I/O Helpers**: Centralize disk read/write operations to ensure consistency
- **Search Helpers**: DRY principle for common operations like finding inodes

### 3. Consistency Check Ordering
- **Challenge**: Spec requires checking errors in exact order (1-6)
- **Solution**: Implemented as single function with early returns
- **Benefit**: Guarantees only the first error is reported

### 4. Name Handling
- **Challenge**: 5-character names may not be null-terminated
- **Solution**: Always copy to 6-byte buffer and force null termination
- **Functions**: `get_inode_name()`, `set_inode_name()`, `inode_name_equals()`

### 5. Defragmentation Algorithm
- **Approach**: Sort files by current start_block, then compact left-to-right
- **Complexity**: O(n log n) for sorting + O(total_blocks) for moving
- **Memory**: Uses temporary 1KB buffer for block transfers

## System Calls Used

### File Operations
- `open()` - Open virtual disk file (fs_mount)
- `close()` - Close virtual disk file (cleanup)
- `read()` - Read superblock and data blocks
- `write()` - Write superblock and data blocks
- `lseek()` - Position file pointer for block access

### String Operations
- `strcasecmp()` - Case-insensitive name comparison
- `memcpy()` - Copy data between buffers
- `memset()` - Zero out memory regions

### I/O
- `fopen()`, `fgets()`, `fclose()` - Read command file
- `fprintf()` - Error messages to stderr
- `printf()` - Output for fs_ls

## Testing Strategy

### Unit Testing Approach
1. **Bit Manipulation**: Created standalone test for all helper functions
2. **Consistency Checks**: Manually created 6 corrupt disks (one per error type)
3. **Edge Cases**: 5-char names, max file size (127 blocks), full superblock
4. **Command Parsing**: Invalid commands, wrong argument counts, out-of-range values

### Integration Testing
Used provided test suite (`test.py`) with 5 test cases:
- Test 1: Basic create/delete operations
- Test 2: Read/write with buffer operations
- Test 3: Multiple disk mounting
- Test 4: Directory operations and cd
- Test 5: Defragmentation with corrupted disks

### Testing Commands
```bash
# Compile
make clean && make

# Run single test
./fs tests/test1/input > stdout.txt 2> stderr.txt
diff stdout.txt tests/test1/stdout_expected
diff stderr.txt tests/test1/stderr_expected

# Run all tests
python3 test.py

# Memory leak check
valgrind --leak-check=full --show-leak-kinds=all ./fs tests/test1/input
```

### Validation Checklist
-  Compiles with -Wall -Werror (no warnings)
- All 5 provided tests pass
-  No memory leaks (Valgrind clean)
- Handles 5-character names correctly
- Case-insensitive name comparison
- Superblock written after every mutation
- Error messages in specified order
- Blocks zeroed on delete/create

## Known Limitations / Assumptions

1. **Maximum Line Length**: Command parser uses 2048-byte buffer (sufficient for B command with 1024 chars)
2. **Disk Name Length**: Limited to 255 characters
3. **No Fragmentation Prevention**: Files must fit in contiguous space or fail
4. **No File Locking**: Single-threaded; no concurrent access protection
5. **Buffer Not Cleared on Mount**: Per spec, buffer persists across mounts

## Performance Considerations

### Time Complexity
- `fs_create`: O(n) for inode search + O(m) for block scan, where n=126, m=127
- `fs_delete`: O(n × d) for recursive delete, d = max directory depth
- `fs_defrag`: O(n log n + b) where b = total allocated blocks
- `fs_ls`: O(n) to scan all inodes
- Consistency checks: O(n²) for name uniqueness check

### Space Complexity
- Memory footprint: ~1KB (superblock) + 1KB (buffer) + ~100 bytes (state) ≈ 2.1 KB
- No dynamic allocation (everything on stack or static)

## Sources and References

1. **Course Materials**:
   - Assignment 3 specification 
   - Lecture slides on file systems 
   
2. **System Call Documentation**:
   - `man 2 open`, `man 2 read`, `man 2 write`, `man 2 lseek`
   
3. **C Standard Library**:
   - `man 3 strcasecmp`, `man 3 memcpy`

4. **Debugging Tools**:
   - Valgrind documentation (valgrind.org)
   - GDB reference card

## Compilation and Execution
```bash
# Build
make

# Run with input file
./fs 

# Example
./fs tests/test1/input

# Redirect output
./fs tests/test1/input > stdout.txt 2> stderr.txt
```