#ifndef USER_H
#define USER_H
#include "User.h"
#include "Book.h"
#include "Record.h"
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <iomanip>
#include <functional>  // 添加这一行
const int pageSize = 10; // 每页显示的数量,可以改
class Library;
class Searcher;
/* 3(1) 可以用用enum(?),它适用于只表达一组有限且互斥的常量
真服了,真会给我找事.
不能直接写,还要 enum class 后面的提取还需要加static_cast
服服服,脑子一拍,苦的是未来的你啊.
*/
enum class UserRole {
ADMIN = 1,
READER = 2
};
/* 3(1) 加个用户的状态 正常/注销 */
enum class UserStatus {
NORMAL,
DEACTIVATED
};
class User {
protected:
std::string account;
std::string password;
UserRole role;
std::string name;
std::string phone;
std::string email;
UserStatus status;
public:
User(const std::string& account, const std::string& password, UserRole role,
const std::string& name, const std::string& phone, const std::string& email,
UserStatus status = UserStatus::NORMAL);
std::string getAccount() const { return account; }
std::string getPassword() const { return password; }
UserRole getRole() const { return role; }
std::string getName() const { return name; }
std::string getPhone() const { return phone; }
std::string getEmail() const { return email; }
UserStatus getStatus() const { return status; }
void setPassword(const std::string& password) { this->password = password; }
void setName(const std::string& name) { this->name = name; }
void setPhone(const std::string& phone) { this->phone = phone; }
void setEmail(const std::string& email) { this->email = email; }
void setStatus(UserStatus status) { this->status = status; }
virtual bool login(const std::string& password) const;
virtual void logout();
bool isActive() const { return status == UserStatus::NORMAL; }
bool isAdmin() const { return role == UserRole::ADMIN; }
virtual std::string toString() const;
static std::shared_ptr<User> fromString(const std::string& str);
/* 2(1) 把原来在reader.cpp里实现的searchBooks迁到user.cpp里,让admin也能用 */
void searchBooks(std::shared_ptr<Library> library, std::shared_ptr<Searcher> searcher);
};
/* 3(1) 把admin.cpp里的分页模板迁到user.cpp里,再把reader.cpp里对应输出部分加个header调用就行了 */
template <typename T>
void paginatePrint(const std::vector<T>& data,
const std::string& header,
std::function<void(const T&)> printLine)
{
int page = 1;
char choice;
do {
system("cls");
int start = (page - 1) * pageSize;
int end   = std::min(start + pageSize, static_cast<int>(data.size()));
std::cout << header;
std::cout << std::string(header.length(), '-') << "\n";
for (int i = start; i < end; ++i) {
printLine(data[i]);
}
std::cout << "\n第 " << page << " 页 / 共 "
<< (data.size() + pageSize - 1) / pageSize
<< " 页  \n【l】上一页   【r】下一页  【0键】返回：";
std::cin  >> choice;
std::cin.ignore();
if (choice == 'l' && page > 1) {
--page;
}
if (choice == 'r' && (start + pageSize) < data.size()) {
++page;
}
} while ((choice == 'l'||choice == 'r'));
}
#endif
/*  ** enum class 的用法  **
1. 声明
enum class 枚举名 [:底层类型] { 常量1 = 值1, 常量2 = 值2, ... };
示例
enum class UserRole : std::uint8_t { ADMIN = 1, READER = 2 };
2. 使用规则
必须带作用域：UserRole::ADMIN
不会隐式转整数：
cint x = role;          // X
int x = static_cast<int>(role); // V
不同枚举之间不能互比较（类型墙）
可前向声明，可指定底层类型（uint8_t/int16_t…）
3. 典型场景
状态机、角色、方向、月份、星期、扑克花色…
只要常量固定、不带附加数据、需要编译期检查，就上 enum class。
4. 与传统 enum 区别
| 老 enum | enum class |
| 名字泄漏到外层 | 被锁在作用域里 |
| 隐式转整型 | 必须显式 static_cast |
| 易冲突、难调试 | 类型安全、可读性好 |
*/
