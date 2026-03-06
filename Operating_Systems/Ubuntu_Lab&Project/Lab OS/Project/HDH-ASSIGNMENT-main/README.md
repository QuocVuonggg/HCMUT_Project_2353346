# OS LamiaAtrium - Simple Operating System Simulator

**Môn học**: Hệ điều hành (CO2018)  
**Trường**: ĐH Bách Khoa TP.HCM - Khoa Khoa học và Kỹ thuật Máy tính

## Mục tiêu

Mô phỏng các thành phần chính của hệ điều hành đơn giản:
- **Scheduler**: Bộ lập lịch CPU
- **Synchronization**: Đồng bộ hóa tài nguyên
- **Memory Management**: Quản lý bộ nhớ ảo và vật lý
- **System Calls**: Giao diện lập trình hệ thống

## Tổng quan

Hệ điều hành quản lý 2 tài nguyên ảo chính:
- **CPU(s)**: Thông qua Scheduler và Dispatcher
- **RAM**: Thông qua Virtual Memory System

### Cấu trúc Source Code

**Header files**: `timer.h`, `cpu.h`, `queue.h`, `sched.h`, `common.h`, `bitops.h`, `os-mm.h`, `mm.h`, `mm64.h`

**Source files**: `timer.c`, `cpu.c`, `queue.c`, `sched.c`, `os.c`, `loader.c`, `mm.c`, `mm64.c`, `mm-vm.c`, `mm-memphy.c`, `syscall.c`, `sys_mem.c`

### Cấu trúc PCB (Process Control Block)

```c
struct pcb_t {
    uint32_t pid;           // Process ID
    uint32_t priority;      // Độ ưu tiên mặc định
    char path[100];         // Đường dẫn process
    struct code_seg_t *code; // Code segment
    addr_t regs[10];        // Thanh ghi, lưu địa chỉ vùng nhớ
    uint32_t pc;            // Program counter
    uint32_t prio;          // Ưu tiên động (MLQ)
    struct page_table_t *page_table; // Bảng trang
    uint32_t bp;            // Break pointer
};
```

### Các lệnh Process

- **CALC**: Thực hiện tính toán
- **ALLOC**: `alloc [size] [reg]` - Cấp phát bộ nhớ
- **FREE**: `free [reg]` - Giải phóng bộ nhớ
- **READ**: `read [source] [offset] [destination]` - Đọc byte từ bộ nhớ
- **WRITE**: `write [data] [destination] [offset]` - Ghi giá trị vào bộ nhớ
- **SYSCALL**: Gọi system call

### Format file Process

```
[priority] [N = số lượng instructions]
instruction 0
instruction 1
...
```

**Ví dụ**: `input/proc/p1s`
```
1 10
calc
alloc 100 0
write 5 0 10
```

## Cài đặt và Build

### Bước 1: Cài MSYS2
```bash
# Tải và cài MSYS2: https://www.msys2.org/
# Mở MSYS2 MINGW64, cài các tools:
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make
```

### Bước 2: Build Project
```bash
cd /e/baocao/BTLHDH/os_lamiaatrium
export PATH=/mingw64/bin:$PATH

mingw32-make clean
mingw32-make os
```

### Bước 3: Chạy Simulation

**Format file config** (trong thư mục `input/`):
```
[time_slice] [N = Số CPU] [M = Số Process]
[RAM_size] [SWAP0_size] [SWAP1_size] [SWAP2_size] [SWAP3_size]
[time_0] [path_0] [priority_0]
...
```

**Chạy**:
```bash
./os os_1_mlq_paging
```

## Các thành phần chính

### 1. SCHEDULER (Bộ lập lịch) - 3 điểm

**Giải thuật**: Multi-Level Queue (MLQ)

- Hệ thống có `MAX_PRIO = 140` mức độ ưu tiên
- Giá trị càng nhỏ, độ ưu tiên càng cao
- Mỗi mức ưu tiên tương ứng với 1 hàng đợi
- **Công thức Slot**: `slot[i] = MAX_PRIO - prio`
- Khi hết slot, chuyển sang queue khác để đảm bảo công bằng

---

### 2. MEMORY MANAGEMENT (Quản lý bộ nhớ) - 2 điểm

**Virtual Memory**:
```c
struct vm_area_struct {
    unsigned long vm_id, vm_start, vm_end, sbrk;
    struct vm_rg_struct *vm_freerg_list;  // Danh sách vùng trống
    struct vm_area_struct *vm_next;
};

struct mm_struct {
    uint32_t *pgd;                   // Page Global Directory
    struct vm_area_struct *mmap;     // Danh sách VMA
    struct vm_rg_struct symrgtbl[PAGING_MAX_SYMTBL_SZ];
};
```

**Physical Memory**:
- **RAM**: Bộ nhớ chính
- **SWAP**: Bộ nhớ phụ (tối đa 4 thiết bị)
- **Swap-out**: Chuyển page từ RAM ra SWAP khi hết chỗ
- **Swap-in**: Đưa page từ SWAP vào RAM khi cần truy cập

**YÊU CẦU QUAN TRỌNG**: Tách biệt User/Kernel Space - KHÔNG truy cập trực tiếp PCB từ userspace, phải qua `struct krnl_t` và sử dụng PID.

---

### 3. MULTI-LEVEL PAGING (Phân trang đa cấp 64-bit) - 2 điểm

**Lược đồ địa chỉ 64-bit - 5 cấp**:
```
bit 56-48: PGD (Page Global Directory)
bit 47-39: P4D (Page Level 4 Directory)
bit 38-30: PUD (Page Upper Directory)
bit 29-21: PMD (Page Middle Directory)
bit 20-12: PT  (Page Table)
bit 11-0:  Offset
```

**Page Table Entry (PTE) - 32 bit**:
```
bit 31: PRESENT (page có trong RAM?)
bit 30: SWAPPED (page đã swap?)
bit 28: DIRTY (page đã modify?)
bit 27-15: USRNUM
bit 12-0: FPN (Frame Page Number) hoặc SWPOFF (Swap Offset)
```

---

### 4. SYSTEM CALLS (Lời gọi hệ thống)

**Kernel Structure**:
```c
struct krnl_t {
    struct queue_t *ready_queue, *mlq_ready_queue, *running_list;
    struct mm_struct *mm;
    struct memphy_struct *mram, **mswp, *active_mswp;
};
```

**Các System Call**:
- `listsyscall`: Liệt kê tất cả syscalls
- `memmap`: Hỗ trợ các thao tác ánh xạ bộ nhớ (SYSMEM_MAP_OP, SYSMEM_INC_OP, SYSMEM_SWP_OP, SYSMEM_IO_READ, SYSMEM_IO_WRITE)

---

### 5. SYNCHRONIZATION (Đồng bộ hóa)

**Shared Resources cần bảo vệ**: `queue_lock`, `mmvm_lock`

**Cơ chế**:
```c
pthread_mutex_lock(&queue_lock);
// Critical section
pthread_mutex_unlock(&queue_lock);
```

## Phân chia công việc

**Cấu trúc điểm**: Scheduler (3đ), Memory (2đ), 64-bit Paging (2đ), Report (3đ) = **10 điểm**

### Task 1: Scheduler & Dispatcher - Tấn Vương (3 điểm)

**Files**: `src/queue.c`, `src/sched.c`

**Nhiệm vụ**:
- Implement `enqueue()` - Thêm process vào queue theo priority
- Implement `dequeue()` - Lấy process có priority cao nhất
- Implement `get_mlq_proc()` - Logic MLQ với slot mechanism
- Đảm bảo CPU chọn đúng process theo slot

**Test**: `./os sched`, `./os sched_0`, `./os sched_1`

---

### Task 2: Memory Management - Anh Vũ (2 điểm)

**Files**: `src/mm-vm.c`, `src/libmem.c`

**Nhiệm vụ**:
- Quản lý Virtual Memory Areas (VMA)
- Implement `inc_vma_limit()` - Tăng giới hạn VMA
- Xử lý ALLOC, FREE, READ, WRITE operations
- Implement swap mechanism (swap-in/swap-out)
- **QUAN TRỌNG**: Đảm bảo tách biệt User/Kernel space (không truy cập trực tiếp PCB, sử dụng krnl_t và PID)

**Test**: `./os os_1_mlq_paging_small_1K`, `./os os_1_mlq_paging_small_4K`

---

### Task 3: 64-bit Paging - Tấn Phát (2 điểm)

**Files**: `src/mm64.c`

**Nhiệm vụ**:
- Implement `init_mm()` - Khởi tạo 5-level page tables (pgd, p4d, pud, pmd, pt)
- Implement `alloc_pages_range()` - Cấp phát frames từ RAM
- Implement `vmap_page_range()` - Map virtual pages → physical frames
- Implement `vmap_pgd_memset()` - Giả lập tạo page directory
- Thống kê: Số lần truy cập bộ nhớ và kích thước page tables

**Test**: `./os os_0_mlq_paging`, `./os os_1_mlq_paging`, `./os os_1_singleCPU_mlq`, `./os os_1_singleCPU_mlq_paging`

---

### Task 4: System Calls - Quốc Vương

**Files**: `src/sys_mem.c`, `src/syscall.c`, `src/syscall.tbl`

**Nhiệm vụ**:
- Implement `__sys_memmap()` - Xử lý memory syscalls
- **FIX**: Lookup process thật từ running_list bằng PID (không dùng dummy)
- Implement các operations: SYSMEM_MAP_OP, SYSMEM_INC_OP, SYSMEM_SWP_OP, SYSMEM_IO_READ/WRITE
- Đảm bảo synchronization: Dùng locks cho shared resources

**Test**: `./os os_syscall`

---

### Task 5: Testing & Reporting - Lương Việt (3 điểm)

**Nhiệm vụ**:
- Tạo test cases trong `input/proc/`
- Chạy mô phỏng và kiểm tra logs
- Viết báo cáo: Biểu đồ Gantt chart, trạng thái bộ nhớ, phân tích kết quả
- Trả lời Questionnaire
- Đóng gói source code và báo cáo

---

## Test Cases

| Test File | Mô tả | Thành phần kiểm tra |
|-----------|-------|---------------------|
| `sched`, `sched_0`, `sched_1` | Scheduler cơ bản đến nâng cao | MLQ algorithm, slot mechanism |
| `os_0_mlq_paging` | Basic MLQ + Paging | Cơ bản |
| `os_1_mlq_paging` | MLQ với nhiều processes | 64-bit paging, multiple processes |
| `os_1_mlq_paging_small_1K` | RAM nhỏ (1KB) | Swap mechanism |
| `os_1_mlq_paging_small_4K` | RAM nhỏ (4KB) | Swap mechanism |
| `os_1_singleCPU_mlq` | Single CPU MLQ | Scheduler với 1 CPU |
| `os_1_singleCPU_mlq_paging` | Single CPU MLQ + Paging | Toàn bộ hệ thống |
| `os_syscall` | System calls | System call interface |

## Debug & Configuration

**Bật debug trong** `include/os-cfg.h`:
```c
#define MLQ_SCHED 1      // Bật MLQ scheduler
#define MM_PAGING        // Bật paging-based memory
#define MM64             // Bật chế độ 64-bit (comment nếu dùng 32-bit)

// Debug flags
#define VMDBG 1          // Debug virtual memory
#define MMDBG 1          // Debug memory management
#define IODUMP 1         // Dump I/O operations
#define PAGETBL_DUMP 1   // Dump page tables
```

**Xem output chi tiết**:
```bash
./os os_1_mlq_paging > output.log 2>&1
cat output.log
```

## Thành viên nhóm

| STT | Họ tên | Nhiệm vụ |
|-----|--------|----------|
| 1 | Lương Việt | Testing & Reporting 
| 2 | Tấn Vương | Scheduler & Dispatcher 
| 3 | Anh Vũ | Memory Management 
| 4 | Tấn Phát | 64-bit Paging 
| 5 | Quốc Vương | System Calls 


---


## Running code
# First cmd
    make clean
    make all
# Run testcase
    ./os os_0_mlq_paging
    ./os os_1_mlq_paging
    ./os os_1_mlq_paging_small_1K
    ./os os_1_mlq_paging_small_4K
    ./os os_1_singleCPU_mlq
    ./os os_1_singleCPU_mlq_paging
    ./os os_sc
    ./os os_syscall
    ./os os_syscall_list
    ./os sched_0
    ./os sched_1