#include "User.h"
#include "Library.h"
#include "Searcher.h"
#include "User.h"
#include "Library.h"
#include "Searcher.h"
#include <sstream> 
#include <memory>   
#include <string>  
#include <vector>   
#include <iostream>  
#include <iomanip>   

User::User(const std::string& account, const std::string& password, UserRole role,
           const std::string& name, const std::string& phone, const std::string& email,
           UserStatus status)
    : account(account), password(password), role(role), name(name),
      phone(phone), email(email), status(status) {}

bool User::login(const std::string& password) const {
    return this->password == password && isActive();
}

void User::logout() {
 }

std::string User::toString() const {
    std::ostringstream oss;
    oss << account << "," << password << "," << static_cast<int>(role) << ","
        << name << "," << phone << "," << email << "," << static_cast<int>(status);
    return oss.str();
}

std::shared_ptr<User> User::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string account, password, name, phone, email;
    int roleInt, statusInt;
    
    std::getline(iss, account, ',');
    std::getline(iss, password, ',');
    iss >> roleInt;
    iss.ignore();
    std::getline(iss, name, ',');
    std::getline(iss, phone, ',');
    std::getline(iss, email, ',');
    iss >> statusInt;
    
    UserRole role = static_cast<UserRole>(roleInt);
    UserStatus status = static_cast<UserStatus>(statusInt);

    return std::make_shared<User>(account, password, role, name, phone, email, status);
}

void User::searchBooks(std::shared_ptr<Library> library, std::shared_ptr<Searcher> searcher) {
    std::cout << "\n=== 图书搜索 ===\n";
    std::cout << "[1] 精确查询 (ISBN)\n";
    std::cout << "[2] 精确查询 (书名)\n";
    std::cout << "[3] 模糊查询 (书名)\n";
    std::cout << "[4] 模糊查询 (作者)\n";
    std::cout << "[5] 模糊查询 (出版社)\n";
    std::cout << "[0] 返回\n";
    std::cout << "请选择: ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 0) return;
    
    std::string keyword;
    std::cout << "请输入关键词: ";
    std::getline(std::cin, keyword);
    
    std::vector<std::shared_ptr<Book>> results;
    
    switch (choice) {
        case 1:
            results = searcher->searchByISBN(library, keyword);
            break;
        case 2:
            results = searcher->searchByTitleExact(library, keyword);
            break;
        case 3:
            results = searcher->searchByTitleFuzzy(library, keyword);
            break;
        case 4:
            results = searcher->searchByAuthor(library, keyword);
            break;
        case 5:
            results = searcher->searchByPublisher(library, keyword);
            break;
        default:
            std::cout << "无效选择\n";
            return;
    }
    
    std::cout << "\n搜索结果:\n";
    std::cout << std::left << std::setw(20) << "书名" << std::setw(15) << "ISBN"
              << std::setw(15) << "作者" << std::setw(15) << "出版社"
              << std::setw(8)  << "价格" << std::setw(6)  << "库存" << "\n";
    std::cout << std::string(90, '-') << "\n";
    
    for (size_t i = 0; i < results.size() && i < 10; i++) {
        const auto& book = results[i];
        std::cout << std::left << std::setw(20) << book->getTitle().substr(0, 19)
                  << std::setw(15) << book->getIsbn()
                  << std::setw(15) << book->getAuthor().substr(0, 14)
                  << std::setw(15) << book->getPublisher().substr(0, 14)
                  << std::setw(8)  << std::fixed << std::setprecision(2) << book->getPrice()
                  << std::setw(6)  << book->getStock() << "\n";
    }
}
