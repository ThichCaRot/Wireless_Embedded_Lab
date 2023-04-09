# BÁO CÁO BÀI THỰC HÀNH SỐ 02
Môn học: THIẾT KẾ HỆ THỐNG NHÚNG KHÔNG DÂY 

Tên chủ đề: LẬP TRÌNH CHO OLED SSD1306 TRÊN ESP32

GVHD: Trần Hoàng Lộc

Nhóm: 10 

## 1. THÔNG TIN CHUNG:

|         Họ và tên         |    MSSV  | Email                  |
| ------------------------- | -------- | ---------------------- |
| Ngô Tạ Đình Phong         | 20520688 | 20520688@gm.uit.edu.vn |
| Lê Nguyễn Quang Duy       | 20520467 | 20520467@gm.uit.edu.vn |


## 2. NỘI DUNG THỰC HIỆN: 

|         Nội dung          |Tình trạng|
| ------------------------- | -------- |
| Bài tập 1                 | 100%     |
| Bài tập 2                 | 100%     |
| Bài tập 3                 | 100%     |
|Điểm tự đánh giá           |   10/10  |

### Bài tập 1 (02 điểm) 
Tạo một project từ ví dụ esp-idf/peripherals/i2c/i2c_self_test.
Chỉnh sửa lại project để giao tiếp với ESP32 theo mô tả bên dưới? Giải thích cách cài
đặt? 
-------------------------------------------------------------------------------------
- Đầu tiên, ta cần cấu hình lại pins giao tiếp dựa theo mô tả của đề bài. Ở đây đề bài sử dụng esp32 có chân OLED SDA ở GPIO5 và chân OLED SCL ở GPIO4 :
```C++
#define I2C_MASTER_SCL_IO  4 /*chân số 4 cho I2C master clock*/
#define I2C_MASTER_SDA_IO 5 /*chân số 5 cho I2C master data*/
```
- Tiếp theo ta cần xác định port number cho giao tiếp I2C trên master 
```C++
#define I2C_MASTER_NUM 0
```
- Ta cũng cần phải config tần số clock của I2C master. Nếu giao tiếp với slave là ssd1306 thì tần số clock nên dùng giao động từ 100kHz đến 400kHz. ở đây ta sử dụng tần số clock 400kHz
```C++
#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define EXAMPLE_PIN_NUM_SDA           5
#define EXAMPLE_PIN_NUM_SCL           4
#define EXAMPLE_PIN_NUM_RST           -1
#define EXAMPLE_I2C_HW_ADDR           0x3C
```
#### Pin Assignment(esp32, esp32s2):

**Note:** The following pin assignments are used by default, you can change these  in the `menuconfig` .

|                           | SDA    | SCL    |
| ------------------------- | ------ | ------ |
| ESP32/ESP32-S2 I2C Master | GPIO18 | GPIO19 |
| ESP32/ESP32-S2 I2C Slave  | GPIO4  | GPIO5  |
| BH1750 Sensor             | SDA    | SCL    |

- slave:
  - GPIO4 is assigned as the data signal of I2C slave port
  - GPIO5 is assigned as the clock signal of I2C slave port
- master:
  - GPIO18 is assigned as the data signal of I2C master port
  - GPIO19 is assigned as the clock signal of I2C master port

- Connection:
  - connect GPIO18 with GPIO4
  - connect GPIO19 with GPIO5
  - connect SDA/SCL of BH1750 sensor with GPIO18/GPIO19

**Note:** It is recommended to add external pull-up resistors for SDA/SCL pins to make the communication more stable, though the driver will enable internal pull-up resistors.

#### Pin Assignment(esp32s3):

**Note:** The following pin assignments are used by default, you can change these  in the `menuconfig` .

|                           | SDA    | SCL    |
| ------------------------- | ------ | ------ |
| ESP32-S3 I2C Master       | GPIO1  | GPIO2  |
| ESP32-S3 I2C Slave        | GPIO4  | GPIO5  |
| BH1750 Sensor             | SDA    | SCL    |

- slave:
  - GPIO4 is assigned as the data signal of I2C slave port
  - GPIO5 is assigned as the clock signal of I2C slave port
- master:
  - GPIO1 is assigned as the data signal of I2C master port
  - GPIO2 is assigned as the clock signal of I2C master port

- Connection:
  - connect GPIO1 with GPIO4
  - connect GPIO2 with GPIO5
  - connect SDA/SCL of BH1750 sensor with GPIO18/GPIO19

**Note:** It is recommended to add external pull-up resistors for SDA/SCL pins to make the communication more stable, though the driver will enable internal pull-up resistors.

#### Pin Assignment(esp32c3, esp32c2, esp32h2):

**Note:** The following pin assignments are used by default, you can change these in the `menuconfig` .

|                                             | SDA    | SCL    |
| ------------------------------------------- | ------ | ------ |
| ESP32-C3/ESP32-C2/ESP32-H2 I2C Master(Slave)| GPIO5  | GPIO6  |
| BH1750 Sensor                               | SDA    | SCL    |

- master:
  - GPIO5 is assigned to the data signal of the I2C master port
  - GPIO6 is assigned to the clock signal of the I2C master port

- Connection:
  - connect SDA/SCL of BH1750 sensor to GPIO5/GPIO6

**Note:** There is only one i2c device on ESP32-C3/ESP32-C2/ESP32-H2, so it is not possible to perform any ESP32/ESP32-S2 self-test example from this repo. However it is possible to test I2C with external devices. If you find anything wrong with your device, please try connecting external pull-up resistors.

### Configure the project

Open the project configuration menu (`idf.py menuconfig`). Then go into `Example Configuration` menu.

- In the `I2C Master` submenu, you can set the pin number of SDA/SCL according to your board. Also you can modify the I2C port number and freauency of the master.
- In the `I2C Slave` submenu, you can set the pin number of SDA/SCL according to your board. Also you can modify the I2C port number and address of the slave.
- In the `BH1750 Sensor` submenu, you can choose the slave address of BH1750 accroding to the pin level of ADDR pin (if the pin level of ADDR is low then the address is `0x23`, otherwise it is `0x5c`). Here you can also control the operation mode of BH1750, each mode has a different resolution and measurement time. For example, in the `One Time L-Resolution` mode, the resolution is 4 Lux and measurement time is typically 16ms (higher resolution means longer measurement time). For more information, you can consult the datasheet of BH1750.


### Build and Flash

Enter `idf.py -p PORT flash monitor` to build, flash and monitor the project.

(To exit the serial monitor, type ``Ctrl-]``.)

See the [Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) for full steps to configure and use ESP-IDF to build projects.

## Example Output

```bash
I (6495) i2c-example: TASK[1] test cnt: 1
*******************
TASK[1]  MASTER READ SENSOR( BH1750 )
*******************
data_h: 01
data_l: d0
sensor val: 386.67 [Lux]
I (6695) i2c-example: TASK[0] test cnt: 2
*******************
TASK[0]  MASTER READ SENSOR( BH1750 )
*******************
data_h: 01
data_l: d0
sensor val: 386.67 [Lux]
*******************
TASK[0]  MASTER READ FROM SLAVE
*******************
====TASK[0] Slave buffer data ====
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f
70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f

====TASK[0] Master read ====
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f
70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f

*******************
TASK[1]  MASTER READ FROM SLAVE
*******************
====TASK[1] Slave buffer data ====
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f
70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f

====TASK[1] Master read ====
00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f
20 21 22 23 24 25 26 27 28 29 2a 2b 2c 2d 2e 2f
30 31 32 33 34 35 36 37 38 39 3a 3b 3c 3d 3e 3f
40 41 42 43 44 45 46 47 48 49 4a 4b 4c 4d 4e 4f
50 51 52 53 54 55 56 57 58 59 5a 5b 5c 5d 5e 5f
60 61 62 63 64 65 66 67 68 69 6a 6b 6c 6d 6e 6f
70 71 72 73 74 75 76 77 78 79 7a 7b 7c 7d 7e 7f

*******************
TASK[0]  MASTER WRITE TO SLAVE
*******************
----TASK[0] Master write ----
0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19
1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29
2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39
3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49
4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59
5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69
6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79
7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89

----TASK[0] Slave read: [128] bytes ----
0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 16 17 18 19
1a 1b 1c 1d 1e 1f 20 21 22 23 24 25 26 27 28 29
2a 2b 2c 2d 2e 2f 30 31 32 33 34 35 36 37 38 39
3a 3b 3c 3d 3e 3f 40 41 42 43 44 45 46 47 48 49
4a 4b 4c 4d 4e 4f 50 51 52 53 54 55 56 57 58 59
5a 5b 5c 5d 5e 5f 60 61 62 63 64 65 66 67 68 69
6a 6b 6c 6d 6e 6f 70 71 72 73 74 75 76 77 78 79
7a 7b 7c 7d 7e 7f 80 81 82 83 84 85 86 87 88 89
```

## Troubleshooting

- BH1750 has two I2C address, which is decided by the voltage level of `ADDR` pin at start up. Make sure to check your schemetic before run this example.

(For any technical queries, please open an [issue](https://github.com/espressif/esp-idf/issues) on GitHub. We will get back to you as soon as possible.)
