#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// --- THÔNG TIN CẤU HÌNH ---
const char* ssid = "xxxxxx";
const char* password = "xxxxxxx";
const char* BOTtoken = "xxxxx"; // Dán Token từ BotFather vào đây
const char* CHAT_ID = "xxxxxxx";       // Dán Chat ID từ IDBot vào đây

const int relay1 = 14;
const int relay2 = 12;
const int relay3 = 16;
const int relay4 = 17;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID) {
      bot.sendMessage(chat_id, "Bạn không có quyền điều khiển!", "");
      continue;
    }

    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Chào bạn " + from_name + ",\n";
      welcome += "chúc bạn có một ngày tốt lành:\n";
      welcome += "tôi là bot điều khiển thiết bị chỉ với 1 dòng lệnh cơ bản:\n";
      welcome += "Dùng các lệnh sau để điều khiển thiết bị:\n";
      welcome += "/tbon1 : Bật Thiết bị 1\n";
      welcome += "/tboff1: Tắt Thiết bị 1\n";
      welcome += "/tbon2: Bật Thiết bị 2\n";
      welcome += "/tboff2: Tắt Thiết bị 2\n";
      welcome += "/tbon3: Bật Thiết bị 3\n";
      welcome += "/tboff3: Tắt Thiết bị 3\n";
      welcome += "/tbon4: Bật Thiết bị 4\n";
      welcome += "/tboff4: Tắt Thiết bị 4\n";
      welcome += "/status: Kiểm tra trạng thái thiết bị";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/tbon1") {
      digitalWrite(relay1, LOW); 
      bot.sendMessage(chat_id, "Thiết bị 1 đã BẬT", "");
    }
    if (text == "/tboff1") {
      digitalWrite(relay1, HIGH);
      bot.sendMessage(chat_id, "Thiết bị 1 đã TẮT", "");
    }
    if (text == "/tbon2") {
      digitalWrite(relay2, LOW);
      bot.sendMessage(chat_id, "Thiết bị 2 đã BẬT", "");
    }
    if (text == "/tboff2") {
      digitalWrite(relay2, HIGH);
      bot.sendMessage(chat_id, "Thiết bị 2 đã TẮT", "");
    }
    if (text == "/tbon3")  {
      digitalWrite(relay3, LOW);
      bot.sendMessage(chat_id, "Thiết bị 3 đã BẬT", "");
    }
    if (text == "/tboff3")  {
      digitalWrite(relay3, HIGH);
      bot.sendMessage(chat_id, "Thiết bị 3 đã TẮT", "");
    }
    if (text == "/tbon4")  {
      digitalWrite(relay4, LOW);
      bot.sendMessage(chat_id, "Thiết bị 4 đã BẬT", ""); 
    } 
    if (text == "/tboff4")  {
      digitalWrite(relay4, HIGH);
      bot.sendMessage(chat_id, "Thiết bị 4 đã TẮT", "");
    }
    if (text == "/status") {
      String status = "Trạng thái hiện tại:\n";
      status += (digitalRead(relay1) == LOW) ? "Thiết bị 1: Đang BẬT\n" : "Thiết bị 1: Đang TẮT\n";
      status += (digitalRead(relay2) == LOW) ? "Thiết bị 2: Đang BẬT\n" : "Thiết bị 2: Đang TẮT\n";
      status += (digitalRead(relay3) == LOW) ? "Thiết bị 3: Đang BẬT\n" : "Thiết bị 3: Đang TẮT\n";
      status += (digitalRead(relay4) == LOW) ? "Thiết bị 4: Đang BẬT" : "Thiết bị 4: Đang TẮT";
      bot.sendMessage(chat_id, status, "");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH); // Mặc định tắt
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  WiFi.begin(ssid, password);
  client.setInsecure(); // Cần thiết cho ESP32 kết nối Telegram mà không cần chứng chỉ phức tạp

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void loop() {
  static unsigned long lastTimeBotRan;
  if (millis() > lastTimeBotRan + 1000) { // Kiểm tra tin nhắn mới mỗi 1 giây
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
