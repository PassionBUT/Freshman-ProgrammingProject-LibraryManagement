#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Book.h"
#include "Record.h"
#include "AdminRecord.h"
#include <vector>
#include <memory>

class Library;

class Admin : public User {
public:
    Admin(const std::string& account, const std::string& password, const std::string& name,
          const std::string& phone, const std::string& email);
    
    void addBook(std::shared_ptr<Library> library, std::shared_ptr<Book> book);
    void removeBook(std::shared_ptr<Library> library, const std::string& isbn);
    void updateBook(std::shared_ptr<Library> library, std::shared_ptr<Book> book);
    void importBooksFromCSV(std::shared_ptr<Library> library, const std::string& filename);
    
    void addUser(std::shared_ptr<Library> library, std::shared_ptr<User> user);
    void removeUser(std::shared_ptr<Library> library, const std::string& account);   
    void updateUser(std::shared_ptr<Library> library, std::shared_ptr<User> user);
    void resetUserPassword(std::shared_ptr<Library> library, const std::string& account);
    void deactivateUser(std::shared_ptr<Library> library, const std::string& account);
    void activateUser(std::shared_ptr<Library> library, const std::string& account);
    
    void viewAllBooks(std::shared_ptr<Library> library) const;
    void viewAllUsers(std::shared_ptr<Library> library) const;
    void viewAllRecords(std::shared_ptr<Library> library) const;
    
    // 记录搜索功能
    void searchRecordsByAccount(std::shared_ptr<Library> library, const std::string& account) const;
    void searchRecordsByISBN(std::shared_ptr<Library> library, const std::string& isbn) const;
    void searchRecordsByDateRange(std::shared_ptr<Library> library, const std::string& startDate, const std::string& endDate) const;
    void searchRecordsByRecordId(std::shared_ptr<Library> library, int recordId) const;
    
    // 新增：管理员操作记录相关功能
    void viewAllAdminRecords(std::shared_ptr<Library> library) const;
    void searchAdminRecordsByAdmin(std::shared_ptr<Library> library, const std::string& adminAccount) const;
    void searchAdminRecordsByAction(std::shared_ptr<Library> library, AdminActionType actionType) const;
    void searchAdminRecordsByDateRange(std::shared_ptr<Library> library, const std::string& startDate, const std::string& endDate) const;
    
private:
    // 新增：记录管理员操作
    void logAdminAction(std::shared_ptr<Library> library, AdminActionType actionType, 
                       const std::string& description, const std::string& targetId, 
                       const std::string& details = "") const;
};

#endif
