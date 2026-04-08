#include "Reader.h"
#include "Library.h"
#include "Searcher.h"
#include "Recommender.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>

const int DueDate=14; // 最多借14天,可以改
 
Reader::Reader(const std::string& account, const std::string& password, const std::string& name,
               const std::string& phone, const std::string& email)
    : User(account, password, UserRole::READER, name, phone, email) {}

void Reader::changePwd(){
	std::cout<<"\n请输入新的密码: ";
	std::string pwd;std::cin>>pwd;std::cin.ignore();
	setPassword(pwd);
	std::cout<<"\n更改密码成功!\n";
}; 
void Reader::changePhone(){
	std::cout<<"\n请输入新的电话: ";
	std::string phone;std::cin>>phone;std::cin.ignore();
	setPhone(phone);
	std::cout<<"\n更改密码成功!\n";
}
void Reader::changeEmail(){
	std::cout<<"\n请输入新的密码: ";
	std::string email;std::cin>>email;std::cin.ignore();
	setEmail(email);
	std::cout<<"\n更改密码成功!\n";
}

void Reader::borrowBook(std::shared_ptr<Library> library, const std::string& isbn) {
    auto book = library->findBookByISBN(isbn);
    if (!book) {
        std::cout << "未找到该图书！\n";
        return;
    }
    
    if (book->getStock() <= 0) {
        std::cout << "该书库存不足！\n";
        return;
    }
    
    auto records = library->getRecordsByUser(getAccount());
    for (const auto& record : records) {
        if (record->getIsbn() == isbn && record->getStatus() == RecordStatus::BORROWED) {
            std::cout << "您已经借阅了该书，尚未归还！\n";
            return;
        }
    }
    
    int recordId = library->getNextRecordId();
    std::string borrowDate = Utils::getCurrentDate();
    std::string dueDate = Utils::getDueDate(DueDate);  
    
    auto record = std::make_shared<Record>(recordId, getAccount(), isbn, borrowDate, dueDate);
    library->addRecord(record);
    
    book->decrementStock();
    book->incrementBorrowCount();
    
    std::cout << "借阅成功！请在 " << dueDate << " 前归还。\n";
}

void Reader::returnBook(std::shared_ptr<Library> library, const std::string& isbn) {
    auto records = library->getRecordsByUser(getAccount());
    std::shared_ptr<Record> recordToReturn = nullptr;
    
    for (const auto& record : records) {
        if (record->getIsbn() == isbn && record->getStatus() == RecordStatus::BORROWED) {
            recordToReturn = record;
            break;
        }
    }
    
    if (!recordToReturn) {
        std::cout << "未找到该书的借阅记录！\n";
        return;
    }
    
    std::string returnDate = Utils::getCurrentDate();
    recordToReturn->setReturnDate(returnDate);
    recordToReturn->setStatus(RecordStatus::RETURNED);
    
    auto book = library->findBookByISBN(isbn);
    if (book) {
        book->incrementStock();
    }
    
    int overdueDays = recordToReturn->calculateOverdueDays(returnDate);
    if (overdueDays > 0) {
        std::cout << "还书成功！但已逾期 " << overdueDays << " 天。\n";
    } else {
        std::cout << "还书成功！\n";
    }
}

void Reader::viewPersonalRecords(std::shared_ptr<Library> library) const {
    auto records = library->getRecordsByUser(getAccount());
    
    paginatePrint<std::shared_ptr<Record>>(
        records,
        "记录编号  账号      ISBN          借阅日期    应还日期    实还日期    状态\n",
        [](const std::shared_ptr<Record>& r) {
            std::string stat = r->getStatus()==RecordStatus::BORROWED ? "在借" : "已还";
            std::cout << std::left
                      << std::setw(8)  << r->getRecordId()
                      << std::setw(12) << r->getAccount()
                      << std::setw(15) << r->getIsbn()
                      << std::setw(12) << r->getBorrowDate()
                      << std::setw(12) << r->getDueDate()
                      << std::setw(12) << r->getReturnDate()
                      << std::setw(8)  << stat << '\n';
        }
    );
}

void Reader::viewTopBorrowedBooks(std::shared_ptr<Library> library) const {
    auto books = library->getTopBorrowedBooks(10);
    
    std::cout << "\n借阅次数排行榜 Top 10:\n";
    std::cout << std::left << std::setw(4) << "排名" << std::setw(20) << "题名"
              << std::setw(15) << "作者" << std::setw(8) << "借阅次数" << "\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (size_t i = 0; i < books.size(); i++) {
        std::cout << std::left << std::setw(4) << (i + 1)
                  << std::setw(20) << books[i]->getTitle().substr(0, 19)
                  << std::setw(15) << books[i]->getAuthor().substr(0, 14)
                  << std::setw(8) << books[i]->getBorrowCount() << "\n";
    }
}

void Reader::viewTopBorrowers(std::shared_ptr<Library> library) const {
    
    std::vector<std::pair<std::string, int>> topBorrowers = library->getTopBorrowers(10);
    

    std::cout << "\n 借阅次数排行榜 Top 10 :" << std::endl;
    std::cout << std::left; 
    std::cout << std::setw(8) << "排名" 
              << std::setw(15) << "读者账号" 
              << std::setw(8) << "借阅次数" << std::endl;
    std::cout << "--------------------------------------------" << std::endl;
    
    for (size_t i = 0; i < topBorrowers.size(); ++i) {
        std::cout << std::setw(8) << (i + 1) 
                  << std::setw(15) << topBorrowers[i].first 
                  << std::setw(8) << topBorrowers[i].second << std::endl;
    }
}

void Reader::viewLatestBooks(std::shared_ptr<Library> library) const {
    auto books = library->getLatestBooks(10);
    
    std::cout << "\n最新出版图书 Top 10:\n";
    std::cout << std::left << std::setw(4) << "排名" << std::setw(20) << "题名"
              << std::setw(15) << "作者" << std::setw(12) << "出版日期" << "\n";
    std::cout << std::string(55, '-') << "\n";
    
    for (size_t i = 0; i < books.size(); i++) {
        std::cout << std::left << std::setw(4) << (i + 1)
                  << std::setw(20) << books[i]->getTitle().substr(0, 19)
                  << std::setw(15) << books[i]->getAuthor().substr(0, 14)
                  << std::setw(12) << books[i]->getPublishDate() << "\n";
    }
}

void Reader::viewRecommendations(std::shared_ptr<Library> library, std::shared_ptr<Recommender> recommender) const {
    auto recommendations = recommender->getRecommendations(library, getAccount());
    
    if (recommendations.empty()) {
        std::cout << "暂无推荐图书。\n";
        return;
    }
    
    std::cout << "\n为您推荐的图书:\n";
    std::cout << std::left << std::setw(4) << "序号" << std::setw(20) << "题名"
              << std::setw(15) << "作者" << std::setw(8) << "借阅次数" << "\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (size_t i = 0; i < recommendations.size() && i < 5; i++) {
        std::cout << std::left << std::setw(4) << (i + 1)
                  << std::setw(20) << recommendations[i]->getTitle().substr(0, 19)
                  << std::setw(15) << recommendations[i]->getAuthor().substr(0, 14)
                  << std::setw(8) << recommendations[i]->getBorrowCount() << "\n";
    }
}

