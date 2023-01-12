/* 
        Copyright Product by 12A1.2
          Collabs with 12A2 & 12D2
        School: Binh Long Highschool
      Source code: Kay Chip ( Monitor )
*/
// Khai báo thư viện Arduino
#include "Arduino.h"
// Khai báo thư viện DFPlayerMini
#include "DFRobotDFPlayerMini.h"
// Khai báo biến
int gas_sensor = 14;    // Cảm biến khí gas
int value = 0;          // Giá trị cảm biến khí gas
int count = 0;          // Biến đếm ( dùng để chạy điều kiện )
unsigned long timer = 0;    // Biến chạy theo thời gian thực ( phụ thuộc *hàm millis* )
DFRobotDFPlayerMini player;   // Biến để thực thi tác vụ của DFPlayer

void setup() {
  Serial2.begin(9600);         // Khởi tạo tác vụ truyền sóng #2 ( *DFPlayer Mini* )
  Serial.begin(9600);          // Khởi tạo tác vụ truyền sóng #1 ( *ESP32 NodeMCU* )
  Serial.println();            // Cách ra
  Serial.println(F("DFRobot DFPlayer Mini"));   // In ra màn hình
  Serial.println(F("Đang kiểm tra trạng thái ... ( Có thể mất 3~5 giây)"));
// Kiểm tra giao thức kết nối ( Tx - Rx )
  if (!player.begin(Serial2)) {     
    Serial.println(F("Không thể khởi động:"));
    Serial.println(F("1. Vui lòng kiểm tra lại kết nối !"));
    Serial.println(F("2. Kiểm tra lại thẻ nhớ đã được lắp vào hay chưa !"));
    while(true);
  }
  // Nếu nhận dạng được thiết bị ( kết nối thành công ) thì thực thi các tác vụ bên dưới
  Serial.println(F("DFPlayer Mini hoạt động."));    // In ra
  player.setTimeOut(2000);  // Độ trễ
  player.volume(30);  // Mức độ volume của DFPlayer (10 ~ 30) || MAX = 30
  player.EQ(DFPLAYER_EQ_NORMAL);  // Tần số ~ 50Hz 
  player.outputDevice(DFPLAYER_DEVICE_SD);  // Định dạng thẻ: thẻ nhớ SD
  player.play(3);   // Phát đoạn giới thiệu
  pinMode(gas_sensor, INPUT);   // Định dạng chân của cảm biến khí gas: *INPUT*
}

void loop() {
  // Đặt điều kiện: Nếu kể từ lúc khởi động, phát đoạn giới thiệu qua loa. Sau 20 giây, thiết bị sẽ bắt đầu xử lí
  if(millis() - timer > 20000)
  {
    value = digitalRead(gas_sensor);    // Cho biến value đọc giá trị của cảm biến khí gas theo *định dạng Digital*
    Serial.println(value);              // In ra biến value
    // Đặt điều kiện: nếu phát hiện khí gas thì...:
    if(value == 1)
    {
      count++;    // Biến đếm sẽ tăng liên tục
      player.play(2);     // Phát âm thanh báo động
      delay(6000);
    }
    // Điều kiện khác: nếu không phát hiện khí gas và biến đếm lớn hơn 0 ( cảm biến phát hiện khí gas trước lúc đó đã được xử lí ) 
    else if(value == 0 && count > 0)
    {
      player.play(1);   // Phát âm thanh báo cáo trạng thái an toàn
      count = 0;    // trả biến đếm về 0
    }
    delay(200);  // *Độ trễ* của tất cả tác vụ là 200 mili giây
  }
}

/////////////////// Các thuật ngữ trong Code: //////////////////////
// *millis()* : hàm đếm thời gian thực kể từ lúc vi điều khiển khởi động, giới hạn khoảng 50 ngày đêm liên tục
// *ESP32 NodeMCU* : Vi điều khiển, ngoài thực hiện xử lí các tác vụ nhúng, còn có khả năng phát triển IoT (Internet vạn vật)
// *DFPlayer Mini* : Mạch mã hóa âm thanh định dạng MP3, WAV để phát ra loa
// *INPUT* : định dạng để nhận giá trị từ cảm biến
// *Digital* : định dạng đọc được chỉ có 2 giá trị: 0 và 1 
// *Delay*: độ trễ