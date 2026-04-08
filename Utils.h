#ifndef UTILS_H
#define UTILS_H

#include <string>


class Utils {
public:
    static std::string getCurrentDate();
    static std::string getDueDate(int days);
    static bool isValidDate(const std::string& date);
    static int compareDates(const std::string& date1, const std::string& date2);
};

#endif

