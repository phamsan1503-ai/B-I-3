# STM32F103 - GPIO, EXTI và Timer Demo

## Giới thiệu
Đoạn code này được viết cho vi điều khiển **STM32F103C8T6**, sử dụng thư viện **Standard Peripheral Library (SPL)**.  
Mục đích chính:  
- Điều khiển LED bằng GPIO.  
- Sử dụng ngắt ngoài (EXTI) để thay đổi trạng thái LED khi nhấn nút.  
- Dùng Timer2 để tạo hàm **Delay_ms**.

## Các phần chính của code

### 1. `main.c`
- Khởi tạo hệ thống (`SystemInit()`).
- Gọi các hàm cấu hình:  
  - `GPIO_Config()`  
  - `EXTI_Config()`  
  - `TIM_Config()`  
- Trong vòng lặp vô hạn:
  - LED **PB1** nhấp nháy với chu kỳ 500ms.

---

### 2. Cấu hình GPIO - `GPIO_Config()`
- Bật clock cho **GPIOA, GPIOB và AFIO**.  
- Cấu hình:
  - **PA0**: Input Pull-up (dùng làm nút nhấn).  
  - **PB0, PB1**: Output Push-Pull (dùng để điều khiển LED).  

---

### 3. Cấu hình EXTI - `EXTI_Config()`
- Liên kết **PA0** với ngắt ngoài **EXTI0**.  
- Cấu hình ngắt:  
  - Chế độ: **Interrupt**.  
  - Kích hoạt khi **Falling Edge** (nhấn nút).  
- Bật NVIC cho kênh **EXTI0_IRQn**.

---

### 4. Cấu hình Timer2 - `TIM_Config()`
- Bật clock cho **TIM2**.  
- Cấu hình bộ đếm với tần số **1ms**.  
- Dùng để hiện thực hàm **Delay_ms()**.

---

### 5. Hàm Delay - `Delay_ms(uint32_t ms)`
- Dùng bộ đếm TIM2 để tạo trễ theo mili-giây.

---

### 6. Trình phục vụ ngắt - `EXTI0_IRQHandler()`
- Khi nhấn nút **PA0**:  
  - Thực hiện chống dội phím bằng `Delay_ms(10)`.  
  - Đảo trạng thái biến `led1_state`.  
  - Cập nhật LED **PB0** theo trạng thái mới.  
- Xóa cờ ngắt `EXTI_ClearITPendingBit()`.

---

## Kết quả
- LED **PB1**: nhấp nháy định kỳ 500ms.  
- LED **PB0**: thay đổi trạng thái (bật/tắt) mỗi lần nhấn nút **PA0**.  
