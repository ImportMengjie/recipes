#include <ctime>
#include <iostream>

// 归一化函数，将时间归一化到本周一的零点
time_t normalize_ts(struct tm timeinfo) {
  int days_until_monday = (timeinfo.tm_wday + 6) % 7;
  timeinfo.tm_hour = 0;
  timeinfo.tm_min = 0;
  timeinfo.tm_sec = 0;
  return mktime(&timeinfo) - days_until_monday * 24 * 3600;
}

// 打印 GMT 时间
void printGMTTime(const struct tm &timeinfo) {
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
  std::cout << "GMT 时间: " << buffer << std::endl;
}

// 测试函数
void testNormalizeTime() {
  time_t now_time = time(NULL);

  struct tm inputTimeinfo;
  gmtime_r(&now_time, &inputTimeinfo);

  time_t normalizedTime = normalize_ts(inputTimeinfo);

  // 获取 GMT 时间
  struct tm normalizedTimeinfo;
  gmtime_r(&normalizedTime, &normalizedTimeinfo);

  printGMTTime(inputTimeinfo);
  printGMTTime(normalizedTimeinfo);
}

int main() {
  testNormalizeTime();
  return 0;
}
