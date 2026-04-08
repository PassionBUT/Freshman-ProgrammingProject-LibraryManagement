#include "Utils.h"
#include <sstream>  // ostringstream  --> 按格式拼接字符串 
#include <iomanip>  // setw() setfill() --> 用来补位 
#include <ctime>  // time() localtime() --> 获得本地时间 

std::string Utils::getCurrentDate() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    
    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday;
    
    return oss.str();
}

std::string Utils::getDueDate(int days) {
    std::time_t t = std::time(nullptr);
    t += days * 24 * 60 * 60; 
    
    std::tm* due = std::localtime(&t);
    
    std::ostringstream oss;
    oss << (due->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (due->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << due->tm_mday;
    
    return oss.str();
}

bool Utils::isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    
    int year, month, day;
    if (sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day) != 3) {
        return false;
    }
    
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    return true;
}

int Utils::compareDates(const std::string& date1, const std::string& date2) {
    int y1, m1, d1, y2, m2, d2;
    
    sscanf(date1.c_str(), "%d-%d-%d", &y1, &m1, &d1);
    sscanf(date2.c_str(), "%d-%d-%d", &y2, &m2, &d2);
    
    if (y1 != y2) return y1 - y2;
    if (m1 != m2) return m1 - m2;
    return d1 - d2;
}


/*  ** 如何获得本地时间 **
#include <ctime>
 
std::time_t t = std::time(nullptr); // 1. 获取当前 UTC 秒数（从 1970-01-01 00:00:00 起的秒数）
std::tm* now = std::localtime(&t);  // 2. 根据系统时区，把秒数换算成“本地日历”结构体
// 3. 下面直接用 now->tm_year / tm_mon / tm_mday 拼字符串

*/


/*  ** sscanf的格式化输入 ** 

 sscanf 虽然很适合格式化输入,但是它是C版本的,string是C++;
 所以需要 c_str() 把string转化为C的char[];
  
*/

