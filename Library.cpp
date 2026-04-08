#include "Library.h"
#include <algorithm>

Library::Library() : nextRecordId(1), nextAdminRecordId(1) {}

void Library::addBook(std::shared_ptr<Book> book) {
    books[book->getIsbn()] = book;
}

bool Library::removeBook(const std::string& isbn) {
    return books.erase(isbn) > 0;
}

bool Library::updateBook(std::shared_ptr<Book> book) {
    if (books.find(book->getIsbn()) != books.end()) {
        books[book->getIsbn()] = book;
        return true;
    }
    return false;
}

std::shared_ptr<Book> Library::findBookByISBN(const std::string& isbn) {
    auto it = books.find(isbn);
    return (it != books.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<Book>> Library::getAllBooks() const {
    std::vector<std::shared_ptr<Book>> result;
    for (const auto& pair : books) {
        result.push_back(pair.second);
    }

    std::sort(result.begin(), result.end(),
              [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b) {
                  return a->getTitle() < b->getTitle();
              });

    return result;
}

std::vector<std::shared_ptr<Book>> Library::getTopBorrowedBooks(int limit) const {
    auto allBooks = getAllBooks();
    std::sort(allBooks.begin(), allBooks.end(),
              [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b) {
                  return a->getBorrowCount() > b->getBorrowCount();
              });
    
    if (limit > allBooks.size()) {
        limit = allBooks.size();
    }
    
    return std::vector<std::shared_ptr<Book>>(allBooks.begin(), allBooks.begin() + limit);
}

std::vector<std::pair<std::string, int>> Library::getTopBorrowers(int limit) const {
    std::unordered_map<std::string, int> borrowCount;

    for (const auto& record : records) {
        if (record && (record->getStatus() == RecordStatus::RETURNED || 
            record->getStatus() == RecordStatus::BORROWED)) {
            borrowCount[record->getAccount()]++;
        }
    }
    
    std::vector<std::pair<std::string, int>> topBorrowers(borrowCount.begin(), borrowCount.end());
    std::sort(topBorrowers.begin(), topBorrowers.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
                  return a.second > b.second; 
              });
    
    if (topBorrowers.size() > static_cast<size_t>(limit)) {
        topBorrowers.resize(limit);
    }
    
    return topBorrowers;
}

std::vector<std::shared_ptr<Book>> Library::getLatestBooks(int limit) const {
    auto allBooks = getAllBooks();
    std::sort(allBooks.begin(), allBooks.end(),
              [](const std::shared_ptr<Book>& a, const std::shared_ptr<Book>& b) {
                  if (a->getPublishDate() != b->getPublishDate()) {
                      return a->getPublishDate() > b->getPublishDate();
                  }
                  return a->getTitle() < b->getTitle();
              });
    
    if (limit > static_cast<int>(allBooks.size())) {
        limit = allBooks.size();
    }
    
    return std::vector<std::shared_ptr<Book>>(allBooks.begin(), allBooks.begin() + limit);
}

void Library::addUser(std::shared_ptr<User> user) {
    users[user->getAccount()] = user;
}

bool Library::removeUser(const std::string& account) {
    return users.erase(account) > 0;
}

bool Library::updateUser(std::shared_ptr<User> user) {
    if (users.find(user->getAccount()) != users.end()) {
        users[user->getAccount()] = user;
        return true;
    }
    return false;
}

std::shared_ptr<User> Library::findUser(const std::string& account) {
    auto it = users.find(account);
    return (it != users.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<User>> Library::getAllUsers() const {
    std::vector<std::shared_ptr<User>> result;
    for (const auto& pair : users) {
        result.push_back(pair.second);
    }

    std::sort(result.begin(), result.end(),
              [](const std::shared_ptr<User>& a, const std::shared_ptr<User>& b) {
                  bool aAdmin = a->isAdmin(), bAdmin = b->isAdmin();
                  if (aAdmin != bAdmin) return aAdmin;
                  return a->getAccount() < b->getAccount();
              });

    return result;
}

void Library::addRecord(std::shared_ptr<Record> record) {
    records.push_back(record);
}

std::vector<std::shared_ptr<Record>> Library::getAllRecords() const {
    return records;
}

std::vector<std::shared_ptr<Record>> Library::getRecordsByUser(const std::string& account) const {
    std::vector<std::shared_ptr<Record>> result;
    for (const auto& record : records) {
        if (record->getAccount() == account) {
            result.push_back(record);
        }
    }
    
    std::sort(result.begin(), result.end(),
              [](const std::shared_ptr<Record>& a, const std::shared_ptr<Record>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    return result;
}

// 新增：管理员操作记录相关方法
void Library::addAdminRecord(std::shared_ptr<AdminRecord> record) {
    adminRecords.push_back(record);
}

std::vector<std::shared_ptr<AdminRecord>> Library::getAllAdminRecords() const {
    return adminRecords;
}

std::vector<std::shared_ptr<AdminRecord>> Library::getAdminRecordsByAdmin(const std::string& adminAccount) const {
    std::vector<std::shared_ptr<AdminRecord>> result;
    for (const auto& record : adminRecords) {
        if (record->getAdminAccount() == adminAccount) {
            result.push_back(record);
        }
    }
    
    std::sort(result.begin(), result.end(),
              [](const std::shared_ptr<AdminRecord>& a, const std::shared_ptr<AdminRecord>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    return result;
}

std::vector<std::shared_ptr<AdminRecord>> Library::getAdminRecordsByAction(AdminActionType actionType) const {
    std::vector<std::shared_ptr<AdminRecord>> result;
    for (const auto& record : adminRecords) {
        if (record->getActionType() == actionType) {
            result.push_back(record);
        }
    }
    
    std::sort(result.begin(), result.end(),
              [](const std::shared_ptr<AdminRecord>& a, const std::shared_ptr<AdminRecord>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    return result;
}

bool Library::login(const std::string& account, const std::string& password) {
    auto user = findUser(account);
    if (user && user->login(password) && user->isActive()) {
        currentUser = user;
        return true;
    }
    return false;
}

void Library::logout() {
    currentUser = nullptr;
}

bool Library::isAdmin() const {
    return currentUser && currentUser->isAdmin();
}

void Library::clearAllData() {
    books.clear();
    users.clear();
    records.clear();
    adminRecords.clear();
    currentUser = nullptr;
    nextRecordId = 1;
    nextAdminRecordId = 1;
}
