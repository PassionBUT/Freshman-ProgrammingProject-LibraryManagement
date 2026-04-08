#ifndef LIBRARY_H
#define LIBRARY_H
#include "Book.h"
#include "User.h"
#include "Record.h"
#include "AdminRecord.h"
#include "Admin.h"
#include "Reader.h"
#include <vector>
#include <memory>
#include <unordered_map>
/* 2(1) 改成用unordered_map 储存数据,提升搜索速度
(1)??? 怎么改完成乱序输出了??? 对对对,补sort 在admin.cpp里查看所有users和books */
class Library {
private:
std::unordered_map<std::string, std::shared_ptr<Book>> books;
std::unordered_map<std::string, std::shared_ptr<User>> users;
std::vector<std::shared_ptr<Record>> records;
std::vector<std::shared_ptr<AdminRecord>> adminRecords;  // 新增：管理员操作记录
std::shared_ptr<User> currentUser;
int nextRecordId;
int nextAdminRecordId;  // 新增：管理员记录编号
public:
Library();
void addBook(std::shared_ptr<Book> book);
bool removeBook(const std::string& isbn);
bool updateBook(std::shared_ptr<Book> book);
std::shared_ptr<Book> findBookByISBN(const std::string& isbn);
std::vector<std::shared_ptr<Book>> getAllBooks() const;
std::vector<std::shared_ptr<Book>> getTopBorrowedBooks(int limit) const;
std::vector<std::shared_ptr<Book>> getLatestBooks(int limit) const;
void addUser(std::shared_ptr<User> user);
bool removeUser(const std::string& account);
bool updateUser(std::shared_ptr<User> user);
std::shared_ptr<User> findUser(const std::string& account);
std::vector<std::shared_ptr<User>> getAllUsers() const;
std::vector<std::pair<std::string, int>> getTopBorrowers(int limit) const;
void addRecord(std::shared_ptr<Record> record);
std::vector<std::shared_ptr<Record>> getAllRecords() const;
std::vector<std::shared_ptr<Record>> getRecordsByUser(const std::string& account) const;
int getNextRecordId() { return nextRecordId++; }
void setNextRecordId(int id) { nextRecordId = id; }

// 新增：管理员操作记录相关方法
void addAdminRecord(std::shared_ptr<AdminRecord> record);
std::vector<std::shared_ptr<AdminRecord>> getAllAdminRecords() const;
std::vector<std::shared_ptr<AdminRecord>> getAdminRecordsByAdmin(const std::string& adminAccount) const;
std::vector<std::shared_ptr<AdminRecord>> getAdminRecordsByAction(AdminActionType actionType) const;
int getNextAdminRecordId() { return nextAdminRecordId++; }
void setNextAdminRecordId(int id) { nextAdminRecordId = id; }

bool login(const std::string& account, const std::string& password);
void logout();
std::shared_ptr<User> getCurrentUser() const { return currentUser; }
bool isAdmin() const;
void clearAllData();
};
#endif
