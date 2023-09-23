đầu tiên 
thay đổi địa chỉ trong thanh ghi stack của chip bằng stack giả của mảng stack_blinky1
lưu ý tự chỉnh lại địa chỉ điền vào stack của chip cho đúng theo cấu trúc 
top của stack là ro-r1-r2-r3-r12-pc-xpsr

bởi vì trỏng mảng stack_blinky1 có chưa
các giá trị r4-r11 kèm theo, 

sau đó chạy 1 lúc

dừng lại ở vị trí cuối systick_hanler

copy các giá trị r4-r11 của chip vào địa chỉ của stack của chip giảm dần
(theo chiều của top stack)
ghi nhớ lại địa chỉ top stack đó (địa chỉ top stack đó nằm quanh quẩn
trong địa chỉ của các phần tử của mảng stack_blinky1)

làm các bước tương tự cho stack_blinky2
