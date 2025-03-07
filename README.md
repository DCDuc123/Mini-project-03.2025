# Giải thích cách chạy chương trình:
-	Bitmask: Quản lý trạng thái ON/OFF, lỗi, cảnh báo bằng các bit trong một byte.
-	Function Pointer: Mỗi module có hàm điều khiển riêng (engine_control, abs_control).
-	setjmp/longjmp: Xử lý lỗi bằng cách nhảy về trạng thái an toàn khi phát hiện lỗi.
-	Struct & Union: Tối ưu bộ nhớ bằng cách lưu dữ liệu đặc thù cho từng loại module trong union.
  
Khi chạy, chương trình sẽ thử bật động cơ 2 lần, lần thứ hai gây lỗi và kích hoạt cơ chế phục hồi!

## 1. File bitmask_utils.c và File bitmask_utils.h
Ban đầu ta định nghĩa các trạng thái bằng bitmask 
 - #define STATUS_ON      (1 << 0)  // Bit thứ 0: Trạng thái ON (0000 0001)
 - #define STATUS_ERROR   (1 << 1)  // Bit thứ 1: Lỗi (0000 0010)
 - #define STATUS_WARNING (1 << 2)  // Bit thứ 2: Cảnh báo (0000 0100)
   
Có 3 hàm lần lượt là 
### 1)hàm bật một Bit
```cpp
void set_status_bit(uint8_t *status, uint8_t flag) {
    *status |= flag;
}
```
Toán tử OR (|): Bật bit tại vị trí flag

ví dụ: 
- status = 0000 0001 (STATUS_ON)
- flag = 0000 0010 (STATUS_ERROR)
- Kết quả: 0000 0011 (STATUS_ON + STATUS_ERROR)  
### 2)hàm tắt một bit 
```cpp
void clear_status_bit(uint8_t *status, uint8_t flag) {
    *status &= ~flag; 
}
```
 Toán tử AND (&) với NOT (~): Tắt bit tại vị trí flag
 
 Ví dụ:
- status = 0000 0011 (STATUS_ON + STATUS_ERROR)
- flag = 0000 0010 (STATUS_ERROR)
- ~flag = 1111 1101
- Kết quả: 0000 0001 (STATUS_ON)
### 3)hàm kiểm tra bit có được bật không  
```cpp
int is_status_set(uint8_t status, uint8_t flag) {
    return (status & flag) != 0;  
}
```
Toán tử AND (&): Kiểm tra bit
   
   Ví dụ:
- status = 0000 0011 (STATUS_ON + STATUS_ERROR)
- flag = 0000 0010 (STATUS_ERROR)
- Kết quả: 0000 0010 != 0 → Trả về 1 (true)

### dùng bitmask để:
- Tiết kiệm bộ nhớ
- Tốc độ cao
- Dễ mở rộng thêm trạng thái

  
## 2. File  module_manager.h và module_manager.c 
- module_manager.h
  - Định nghĩa cấu trúc dữ liệu và khai báo hàm để quản lý module.
  - Dùng union để tiết kiệm bộ nhớ cho dữ liệu module.
  - Dùng con trỏ hàm để gọi hành động điều khiển riêng cho từng module.
- module_manager.c
  - Triển khai mảng động để linh hoạt thêm/xóa module.
  - Cấp phát động (malloc/realloc/free) để quản lý bộ nhớ hiệu quả.
  - Xử lý logic tìm kiếm, thêm, xóa module.
    
**a. Tạo danh sách module (create_module_list):** 
```cpp
ModuleList* create_module_list(size_t initial_capacity) {
    // Cấp phát bộ nhớ cho danh sách
    ModuleList* list = malloc(sizeof(ModuleList));
    // Cấp phát mảng động chứa các module
    list->modules = malloc(sizeof(Module) * initial_capacity);
    list->count = 0;       // Ban đầu chưa có module nào
    list->capacity = initial_capacity; // Sức chứa ban đầu 
    return list;
}
```
Ví dụ trong bài này giống như mua một cái hộp (bộ nhớ) để chứa tối đa 5 module. 
```cpp
    ModuleList* list = create_module_list(5);
```

**b. Thêm module (add_module):**
```cpp
int add_module(ModuleList* list, Module module) {
    // Nếu hộp đầy, mở rộng hộp gấp đôi
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        Module* new_modules = realloc(list->modules, sizeof(Module) * list->capacity);
        if (!new_modules) return -1; // Lỗi
        list->modules = new_modules;
    }
    // Thêm module vào cuối mảng
    list->modules[list->count++] = module;
    return 0;
}
```
Ví dụ như thêm một module "Engine" vào hộp. Nếu hộp đầy, đổi sang hộp lớn gấp đôi = 5*2=10 

**c. Xóa module (remove_module):**
```cpp
// Tạo danh sách chứa tối đa 2 module
ModuleList* list = create_module_list(2);

// Tạo module động cơ
Module engine = {
    .id = 1,
    .name = strdup("Engine"), // Cấp phát bộ nhớ cho tên
    .status = 0,
    .control_action = engine_control // Hàm điều khiển động cơ
};
// Thêm module vào danh sách
add_module(list, engine);
// Tìm module để điều khiển
Module* found = find_module(list, 1);
if (found) {
    found->control_action(found, 1); // Bật động cơ
}
// Hủy danh sách khi kết thúc
destroy_module_list(list);
```
Ví dụ: Xóa module có ID 1 khỏi hộp, các module sau sẽ dồn lên để lấp khoảng trống.
**d. Tìm module (find_module):**
```cpp
Module* find_module(ModuleList* list, int id) {
    // Duyệt qua từng module để tìm ID trùng
    for (size_t i = 0; i < list->count; i++) {
        if (list->modules[i].id == id) {
            return &list->modules[i]; // Trả về địa chỉ module
        }
    }
    return NULL; // Không tìm thấy
}
```
Ví dụ: Tìm kiếm trong hộp xem có module nào có ID là 1 không.

**e. Hủy danh sách (destroy_module_list):**
```cpp
void destroy_module_list(ModuleList* list) {
    if (list) {
        // Giải phóng bộ nhớ từng tên module
        for (size_t i = 0; i < list->count; i++) {
            free(list->modules[i].name);
        }
        free(list->modules); // Giải phóng mảng module
        free(list);          // Giải phóng danh sách
    }
}
```
Ví dụ: Khi không cần dùng hộp nữa, vứt bỏ toàn bộ hộp và các module bên trong.

## 3. File error_handler.h và file error_handler.c

## 4. File function_handler.h và file function_handler.c:
- function_handler.h: Khai báo các hàm điều khiển để module khác gọi.
- function_handler.c:
  - Triển khai logic bật/tắt module, kiểm tra lỗi bằng bitmask.
  - Kết hợp với error_handler.c để xử lý lỗi bằng setjmp/longjmp.
```cpp
#include "function_handler.h"
#include "bitmask_utils.h"  // Để dùng bitmask (STATUS_ON, ...)
#include "error_handler.h"  // Để gọi handle_error()
#include <stdio.h>

// Hàm điều khiển động cơ
void engine_control(Module* module, int action) {
    if (action == 1) { // Bật động cơ
        // Kiểm tra xem động cơ đã BẬT chưa
        if (is_status_set(module->status, STATUS_ON)) {
            printf("Engine đã BẬT! Không thể bật lại.\n");
            // Đánh dấu lỗi và xử lý
            set_status_bit(&module->status, STATUS_ERROR);
            handle_error(); // Nhảy về trạng thái an toàn
        } else {
            // Bật trạng thái ON
            set_status_bit(&module->status, STATUS_ON);
            printf("Engine đã khởi động.\n");
        }
    } else if (action == 0) { // Tắt động cơ
        clear_status_bit(&module->status, STATUS_ON);
        printf("Engine đã tắt.\n");
    }
}

// Hàm điều khiển ABS (triển khai tương tự)
void abs_control(Module* module, int action) { ... }
```
Với các hàm điều khiển như abs,light, ta có thể mở rộng thêm ở đây  

## 5. Giải thích luồng chạy trong main 




















```cpp

```

```cpp

```

```cpp

```

```cpp

```

```cpp

```

```cpp

```

```cpp

```
