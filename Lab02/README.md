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
#define EXAMPLE_PIN_NUM_SDA           5
#define EXAMPLE_PIN_NUM_SCL           4
```
- Tiếp theo ta cần xác định port number cho giao tiếp I2C trên master 
```C++
#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_HOST  0
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
### Bài tập 2 + 3 (gộp)
Sử dụng font, thư viện và các hàm cho sẵn theo tài nguyên đính kèm, viết chương trình hiển thị MSSV của các thành viên trong nhóm lên OLED SSD1306

Vẽ logo UIT lên OLED SSD1306. Giải thích cách làm và kết quả
------------------------------------------------------------------------------------

Link github: https://github.com/Kaze2508/Wireless_Embedded_Lab/tree/main/Lab02

Giải thích cách thức hoạt động: 

```C++
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "ssd1306.h"
#include "font8x8_basic.h"
#include <string.h>
#include "cc.h"
```
Các thư viện được tích hợp vào và sử dụng. Ngoài thư viện nhập xuất stdio.h  tiêu chuẩn, các thư viện esp_log, i2c, sdkconfig, ssd1306 để thiết lập và sử dụng với giao thức I2C với OLED SSD1306. File font8x8_basic để sử dụng font in text lên OLED. File cc chứa khai báo hàm dùng để in logo UIT
```C++
static const char *TAG = "i2c-example";

#define _I2C_NUMBER(num) I2C_NUM_##num
#define I2C_NUMBER(num) _I2C_NUMBER(num)
#define I2C_HOST  0

#define DATA_LENGTH 512                  /*!< Data buffer length of test buffer */
#define RW_TEST_LENGTH 128               /*!< Data length for r/w test, [0,DATA_LENGTH] */
#define DELAY_TIME_BETWEEN_ITEMS_MS 1000 /*!< delay time between different test items */
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define EXAMPLE_PIN_NUM_SDA           5
#define EXAMPLE_PIN_NUM_SCL           4
#define EXAMPLE_PIN_NUM_RST           -1
#define EXAMPLE_I2C_HW_ADDR           0x3C
```
Define các giá trị sử dụng xuyên suốt chương trình. Ở đây chúng ta kết nối và giao tiếp với OLED qua 2 pin 4 và pin 5 (như trong hình)
```C++
void i2c_config()
{
    ESP_LOGI(TAG, "Initialize I2C bus");
    i2c_config_t i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = EXAMPLE_PIN_NUM_SDA,
        .scl_io_num = EXAMPLE_PIN_NUM_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_HOST, &i2c_conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_HOST, I2C_MODE_MASTER, 0, 0, 0));
}
```
Hàm dùng để thiết lập giao thức I2C giữ ESP và OLED. Chúng ta cần sử dụng master mode để ghi tín hiệu ra OLED. Các chân giá trị thiết lập được sử dụng từ khai báo của phần define.
Nếu không có lỗi nào xảy ra, thiết bị sẽ tiến hành cài đặt driver I2C để hiển thị (như trong hướng dẫn thực hành Lab02)
```C++
void ssd1306_init() 
{
    esp_err_t espRc;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);

    i2c_master_write_byte(cmd, OLED_CMD_SET_CHARGE_PUMP, true);
    i2c_master_write_byte(cmd, 0x14, true);

    i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP, true); // reverse left-right mapping
    i2c_master_write_byte(cmd, OLED_CMD_SET_DISPLAY_CLK_DIV, true); // reverse up-bottom mapping

    i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_NORMAL, true);
    i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_OFF, true);
    i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_ON, true);
    i2c_master_stop(cmd);

    espRc = i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    if (espRc == ESP_OK) 
    {
        ESP_LOGI(TAG, "OLED configured successfully");
    } else {
        ESP_LOGE(TAG, "OLED configuration failed. code: 0x%.2X", espRc);
    }
    i2c_cmd_link_delete(cmd);
}
```
Thiết lập màn OLED.

Tạo một command link với hàm i2c_cmd_link_create(), sau đó điền vào các dữ liệu cần thiết để gửi đến slave, bao gồm:

o Start bit: i2c_master_start().

o Địa chỉ slave: i2c_master_write_byte(). Đối số là một byte địa chỉ đính kèm với bit READ (vào bit ngoài cùng bên phải).

o Data: one hoặc nhiều byte được gửi vào đối số của hàm i2c_master_write().

o Stop bit: i2c_master_stop().

Cả 2 hàm i2c_master_write_byte() và i2c_master_write() đều có một đối số bổ sung chỉ định liệu master có đảm bảo rằng nó nhận được bit ACK hay không.

• Kích hoạt việc thực thi command link bằng bộ điều khiển I2C với hàm i2c_master_cmd_begin(). Một khi việc thực thi được kích hoạt, command link sẽ không được chỉnh sửa nữa.

• Sau khi lệnh được gửi, trả lại tài nguyên được sử dụng bởi command link bằng cách gọi hàm i2c_cmd_link_delete().

```C++
void task_ssd1306_display_text(const void *arg_text) 
{
    char *text = (char*)arg_text;
    uint8_t text_len = strlen(text);

    i2c_cmd_handle_t cmd;

    uint8_t cur_page = 0;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
    i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
    i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
    i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    for (uint8_t i = 0; i < text_len; i++) 
    {
        if (text[i] == '\n') 
        {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

            i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
            i2c_master_write_byte(cmd, 0x00, true); // reset column
            i2c_master_write_byte(cmd, 0x10, true);
            i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

            i2c_master_stop(cmd);
            i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
        } 
        else 
        {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

            i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
            i2c_master_write(cmd, font8x8_basic_tr[(uint8_t)text[i]], 8, true);
            
            i2c_master_stop(cmd);
            i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
        }
    }
    vTaskDelete(NULL);
}
```
In text ra màn hình OLED
Khi sử dụng hàm, hàm sẽ nhận text được input và parse xuống biến **arg_text**
```C++
void task_ssd1306_display_text(const void *arg_text) 
```
Biến text_len dùng để xác định chiều dài đoạn text để khởi tạo vòng lặp in ra màn hình 
```C++
uint8_t text_len = strlen(text);
```
Các hàm để khởi tạo command link và start bit
```C++
i2c_cmd_handle_t cmd;
cmd = i2c_cmd_link_create();
i2c_master_start(cmd);
```
Truyền giá trị dạng byte qua command link 
```C++
i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
```
Reset con trỏ in ra màn hình về column 0
```C++
i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
```
Reset con trỏ in ra màn hình về line 0
```C++
i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
```
Reset trang in đang sử dụng
```C++
i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page
```
Kết thúc command link và delete biến cmd với thời gian delay thực thi 10 milisecond cho 1 Tick
```C++
i2c_master_stop(cmd);
i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
i2c_cmd_link_delete(cmd);
```
Khởi chạy vòng lặp với i từ 0 đến chiều dài của chuỗi text được nhập vào

Mục đích nhằm quét hết chuỗi ký tự, đối chiếu giá trị ký tự tương dương trong file font8x8 và in ra màn hình OLED dựa vào tham số hex
```C++
for (uint8_t i = 0; i < text_len; i++)
```
Nếu chuỗi giá trị quét trúng ký tự "\n" thì sẽ khởi chạy command link reset cột và dòng tương tự lúc initialize trước lúc in text
```C++
if (text[i] == '\n') 
        {
            cmd = i2c_cmd_link_create();
            i2c_master_start(cmd);
            i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

            i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
            i2c_master_write_byte(cmd, 0x00, true); // reset column
            i2c_master_write_byte(cmd, 0x10, true);
```
Nhưng dòng cuối cùng sẽ tăng giá trị page thông qua biến cur_page để chương trình xuống dòng, sau đó kết thúc command link tương tự như lúc initialize đầu hàm
```C++
            i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

            i2c_master_stop(cmd);
            i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
            i2c_cmd_link_delete(cmd);
        } 
```
Còn trường hợp khác, chương trình sẽ hoạt động gần như tương tự, chỉ khác bước tham chiếu giá trị tương đương sau khi quy đổi về dạng biến uint_8t trong bảng font8x8 để in ra màn hình

Trong đó cmd là command link để ghi giá trị, font8x8_basic_tr là bảng theo định dạng mảng 2 chiều, 8 là độ rộng của 1 mảng
```C++
            i2c_master_write(cmd, font8x8_basic_tr[(uint8_t)text[i]], 8, true);
```
Về cách thức hoạt động, ví dụ khi chương trình quét được ký tự **O**, hàm sẽ dò tham chiếu giá trị của O trong mảng font8x8_basic_tr
```C++
    { 0x1C, 0x3E, 0x63, 0x41, 0x63, 0x3E, 0x1C, 0x00 },   // U+004F (O)
```
Lần lượt in ra màn hình khi quy đổi sang dạng hex sẽ là

1C -> 00011100

3E -> 00111110

63 -> 01100011

41 -> 01000001

63 -> 01100011

3E -> 00111110

1C -> 00011100

00 -> 00000000

```C++
void task_ssd1306_display_image() 
{
    
    i2c_cmd_handle_t cmd;

    uint8_t cur_page = 0;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

    i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
    i2c_master_write_byte(cmd, 0x00, true); // reset column - choose column --> 0
    i2c_master_write_byte(cmd, 0x10, true); // reset line - choose line --> 0
    i2c_master_write_byte(cmd, 0xB0 | cur_page, true); // reset page

    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);

    for (int n = 0 ; n < 8 ; n++)
    {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);

        for (int i = 0; i < 8; i++ )
        {
            i2c_master_write(cmd, uit[8 * n + i ], 16, true);
        }
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);

        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
        i2c_master_write_byte(cmd, 0x00, true); // reset column
        i2c_master_write_byte(cmd, 0x10, true);
        i2c_master_write_byte(cmd, 0xB0 | ++cur_page, true); // increment page

        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
    }
    vTaskDelete(NULL);
}
```

```C++
void task_ssd1306_display_clear(void *ignore) 
{
    i2c_cmd_handle_t cmd;

    uint8_t clear[128];
    for (uint8_t i = 0; i < 128; i++) {
        clear[i] = 0;
    }
    for (uint8_t i = 0; i < 8; i++) {
        cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (OLED_I2C_ADDRESS << 1) | I2C_MASTER_WRITE, true);
        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_SINGLE, true);
        i2c_master_write_byte(cmd, 0xB0 | i, true);

        i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);
        i2c_master_write(cmd, clear, 128, true);
        i2c_master_stop(cmd);
        i2c_master_cmd_begin(I2C_NUM_0, cmd, 10/portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);
    }
    vTaskDelete(NULL);
}
```
Phần chương trình display_clear tụi em không nắm rõ cách xài mà cũng không thấy ảnh hưởng quá nhiều đến code kết quả nên đã không tìm hiểu kĩ cách hoạt động cũng như dùng đến trong bài tập

```C++
void app_main(void)
{
    i2c_config();
    ssd1306_init();
    task_ssd1306_display_text("20520688\nNgo Ta Dinh\nPhong \n20520467\nLe Nguyen\nQuang Duy");
    vTaskDelay(100/portTICK_PERIOD_MS);
    //task_ssd1306_display_image() ;
}
```
Phần chương trình nếu sử dụng để in họ tên và mã số sinh viên thành viên trong nhóm lên màn hình OLED

```C++
void app_main(void)
{
    i2c_config();
    ssd1306_init();
    //task_ssd1306_display_text("20520688\nNgo Ta Dinh\nPhong \n20520467\nLe Nguyen\nQuang Duy");
    //vTaskDelay(100/portTICK_PERIOD_MS);
    task_ssd1306_display_image() ;
}
```
Phần chương trình nếu sử dụng để in logo UIT lên màn hình OLED

```C++
const unsigned char uit [64][16] = {
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x3F},
{0x1F, 0x0F, 0x0F, 0x8F, 0x87, 0xC7, 0xC3, 0xE3, 0xE3, 0xF3, 0xF3, 0xFB, 0xFB, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x9F, 0x9F, 0x1F, 0xDF, 0xCF},
{0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xC7, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x87, 0xBF},
{0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x8F, 0xC3, 0xC1, 0x60, 0xB0, 0xD8, 0x78, 0xAC},
{0xF6, 0xDE, 0xEF, 0xFF, 0xF7, 0x7F, 0x3F, 0x3D, 0x1F, 0x9E, 0x8F, 0xCF, 0xC7, 0xE7, 0xE3, 0xF3},
{0xF1, 0xF9, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD},
{0xFB, 0xF7, 0xEF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFE, 0xFC, 0xF9, 0xC3, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xEC, 0xF6, 0xDB, 0xED, 0xF6, 0x7B, 0x3D, 0x3F, 0x1E, 0x0F, 0x87},
{0xC3, 0xE3, 0xF1, 0xF0, 0xF8, 0xFC, 0x3E, 0x3E, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F},
{0x7F, 0x3F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x83, 0xC1, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF},
{0xFF, 0xFF, 0xBF, 0x87, 0xC1, 0xE0, 0x70, 0x3C, 0x1E, 0x06, 0x07, 0x03, 0x01, 0x01, 0x03, 0x03},
{0x07, 0x0E, 0x1C, 0x78, 0xF0, 0xC1, 0x83, 0x8F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x0F},
{0x03, 0x00, 0x00, 0xE0, 0xF0, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1, 0xC0, 0xC1, 0xC1, 0xDD},
{0xFF, 0xFB, 0xFD, 0xDF, 0xC3, 0xC1, 0xC3, 0xE3, 0xE3, 0xF7, 0xFE, 0xFE, 0xFC, 0xFC, 0xFE, 0xFE},
{0xF7, 0xF7, 0xE3, 0xC3, 0xC3, 0xC3, 0xC7, 0xDF, 0xF9, 0xFF, 0xFF, 0xC1, 0xC1, 0xC1, 0xC1, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x01, 0x00, 0x00, 0x00},
{0x00, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xCF, 0x8F, 0x0F, 0x0F, 0x0F, 0x1F, 0x3E},
{0x3C, 0x7D, 0xF8, 0xF1, 0xE0, 0xC1, 0xC3, 0x87, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0x3F, 0x1F, 0x8F, 0x83, 0xC1, 0xE0, 0xF1, 0xF0, 0x79, 0x7D, 0x3E, 0x1F, 0x0F, 0x0F, 0x0F, 0x8F},
{0xCF, 0xE7, 0xF7, 0xFD, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00},
{0x00, 0x00, 0x07, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8},
{0xF8, 0xF0, 0xF0, 0xE0, 0xE1, 0xE1, 0xC1, 0xC3, 0xC3, 0xC3, 0xC0, 0xC0, 0xC3, 0xC3, 0xC1, 0xC0},
{0xC2, 0xC3, 0xC3, 0x81, 0xC1, 0xC1, 0xE0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8},
{0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xC1, 0xC1, 0xC1, 0xE3, 0xE3, 0xE3, 0xE3},
{0xE3, 0xE3, 0xE3, 0xE3, 0xF3, 0xF3, 0xF3, 0xF3, 0xF9, 0xF9, 0xF9, 0xFD, 0xFD, 0xFC, 0xFE, 0xFE},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}
};
```
File hex theo grid để hiển thị logo UIT trong file cc.h

Link YouTube: https://youtu.be/lvGm1jAH1YU