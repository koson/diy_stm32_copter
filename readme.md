# About the project
- This is my mini project to design a simple PCB that can be used to control a quad or hexa multicopter Design. 
- Thanks PCB Electronics sub-reddit for consulted me.
- The 'stm_multicopter_gerber_v1.2.rar' was used to make the actual PCB I use in this project. -> The bare PCB board photo 'multicopter_bare_pcb.JPG'

# The firmware
- After finishing soldering the board, I've made several tests on each designed peripherals, you can find them under 'testfirm' folder.
- A video I made to test the ICM AHRS module: https://www.youtube.com/watch?v=TO57MewXJ6E&ab_channel=Edinhthong
- SDcard module code: https://github.com/dinhthong/stm32f4_sd_logger/
# Todos
## 14 March 2020
- Hàn mạch LM1117, đo điện áp ngõ ra
- Hàn minimal ARM STM32 circuit -> test booting: LED blinking, GPIO PWM, UART.
- Hàn mạch chip IMU: ICM -> test đọc dữ liệu, xin code để đọc mẫu.
## 24 March 2020
- Check các nguồn pin Lipo và sạc nó về 3.85V per cell
- Nối thẳng dây tín hiệu PPM vào đầu input của VĐK, sau đó test code PPM
- Kiếm đầu đọc thẻ nhớ SDIO, lắp vào hàn thử, chạy code
- Test UART thông qua CP2012 cắm vào USB, gửi dữ liệu từ Hercules

# Received papers (from Mr Hoang hardware package)
- Certificate of Origin: Bảng list ra nguồn gốc của linh kiện đã mua.
- Packaging list: tên linh kiện, số lượng order.
- Commercial Invoice: tên linh kiện, số lượng, số tiền tương ứng
