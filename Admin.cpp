#include "Admin.h"
#include "Library.h"
#include "Utils.h"
#include "UI.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <algorithm>

Admin::Admin(const std::string& account, const std::string& password, const std::string& name,
const std::string& phone, const std::string& email)
: User(account, password, UserRole::ADMIN, name, phone, email) {}

//         ¼    Ա      ˽ з   
void Admin::logAdminAction(std::shared_ptr<Library> library, AdminActionType actionType, 
                          const std::string& description, const std::string& targetId, 
                          const std::string& details) const {
    std::string currentTime = Utils::getCurrentDate();
    int recordId = library->getNextAdminRecordId();
    auto adminRecord = std::make_shared<AdminRecord>(recordId, this->account, actionType, 
                                                    description, targetId, currentTime, details);
    library->addAdminRecord(adminRecord);
}

void Admin::addBook(std::shared_ptr<Library> library, std::shared_ptr<Book> book) {
library->addBook(book);
std::cout << "ͼ    ӳɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::ADD_BOOK, "   ͼ  ", book->getIsbn(), 
               "    : " + book->getTitle());
}

void Admin::removeBook(std::shared_ptr<Library> library, const std::string& isbn) {
if (library->removeBook(isbn)) {
std::cout << "ͼ  ɾ   ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::REMOVE_BOOK, "ɾ  ͼ  ", isbn);
} else {
std::cout << "δ ҵ   ͼ 飡\n";
}
}

void Admin::updateBook(std::shared_ptr<Library> library, std::shared_ptr<Book> book) {
if (library->updateBook(book)) {
std::cout << "ͼ    ³ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::UPDATE_BOOK, "    ͼ  ", book->getIsbn(), 
               "    : " + book->getTitle());
} else {
std::cout << "δ ҵ   ͼ 飡\n";
}
}

void Admin::importBooksFromCSV(std::shared_ptr<Library> library, const std::string& filename) {
std::ifstream file("../data/" + filename);
if (!file.is_open()) {
std::cout << " ޷   CSV ļ   \n";
return;
}
std::string line;
int count = 0;
std::getline(file, line);
while (std::getline(file, line)) {
try {
std::istringstream iss(line);
std::string title, isbn, author, publisher, publishDate;
double price;
int stock, borrowCount;
std::getline(iss, title,   ',');
std::getline(iss, isbn,    ',');
std::getline(iss, author,  ',');
std::getline(iss, publisher,',');
iss >> price;
iss.ignore();
std::getline(iss, publishDate, ',');
iss >> stock;
iss.ignore();
iss >> borrowCount;
auto book = std::make_shared<Book>(title, isbn, author, publisher,
price, publishDate, stock, borrowCount);
library->addBook(book);
++count;
} catch (...) {
std::cout << "      Ч  : " << line << '\n';
}
}
std::cout << " ɹ      " << count << "   ͼ 飡\n";
file.close();
//   ¼    Ա    
logAdminAction(library, AdminActionType::IMPORT_BOOKS, "        ͼ  ", filename, 
               "       " + std::to_string(count) + "   ͼ  ");
}

void Admin::addUser(std::shared_ptr<Library> library, std::shared_ptr<User> user) {
library->addUser(user);
std::cout << " û   ӳɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::ADD_USER, "    û ", user->getAccount(), 
               "    : " + user->getName());
}

void Admin::removeUser(std::shared_ptr<Library> library, const std::string& account) {
if (library->removeUser(account)) {
std::cout << " û ɾ   ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::REMOVE_USER, "ɾ   û ", account);
} else {
std::cout << "δ ҵ    û   \n";
}
}

void Admin::updateUser(std::shared_ptr<Library> library, std::shared_ptr<User> user) {
if (library->updateUser(user)) {
std::cout << " û    ³ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::UPDATE_USER, "     û ", user->getAccount(), 
               "    : " + user->getName());
} else {
std::cout << "δ ҵ    û   \n";
}
}

void Admin::resetUserPassword(std::shared_ptr<Library> library, const std::string& account) {
auto user = library->findUser(account);
if (user) {
user->setPassword("123456");
std::cout << "       óɹ         Ϊ  123456\n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::RESET_PASSWORD, "        ", account);
} else {
std::cout << "δ ҵ    û   \n";
}
}

void Admin::deactivateUser(std::shared_ptr<Library> library, const std::string& account) {
auto user = library->findUser(account);
if (user) {
user->setStatus(UserStatus::DEACTIVATED);
std::cout << " û ע   ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::DEACTIVATE_USER, "ע   û ", account);
} else {
std::cout << "δ ҵ    û   \n";
}
}

void Admin::activateUser(std::shared_ptr<Library> library, const std::string& account) {
auto user = library->findUser(account);
if (user) {
user->setStatus(UserStatus::NORMAL);
std::cout << " û  ָ  ɹ   \n";
//   ¼    Ա    
logAdminAction(library, AdminActionType::ACTIVATE_USER, " ָ  û ", account);
} else {
std::cout << "δ ҵ    û   \n";
}
}

void Admin::viewAllBooks(std::shared_ptr<Library> library) const
{
//   ¼ 鿴    
logAdminAction(library, AdminActionType::VIEW_BOOKS, " 鿴    ͼ  ", "ALL", 
               "    Ա 鿴      ͼ   б ");

auto books = library->getAllBooks();
paginatePrint< std::shared_ptr<Book> >(
books,
"                    ISBN                                          ۸                               \n",
[](const std::shared_ptr<Book>& b){
std::cout << std::left
<< std::setw(20) << b->getTitle().substr(0,19)
<< std::setw(15) << b->getIsbn()
<< std::setw(15) << b->getAuthor().substr(0,14)
<< std::setw(15) << b->getPublisher().substr(0,14)
<< std::setw(8)  << std::fixed << std::setprecision(2) << b->getPrice()
<< std::setw(14) << b->getPublishDate()
<< std::setw(8)  << b->getStock()
<< std::setw(10)  << b->getBorrowCount() << '\n';
});
}

void Admin::viewAllUsers(std::shared_ptr<Library> library) const
{
//   ¼ 鿴    
logAdminAction(library, AdminActionType::VIEW_USERS, " 鿴     û ", "ALL", 
               "    Ա 鿴       û  б ");

auto users = library->getAllUsers();
paginatePrint< std::shared_ptr<User> >(
users,
" ˺           ɫ               ֻ                             ״̬\n",
[](const std::shared_ptr<User>& u){
std::string role = u->isAdmin() ? "    Ա" : "    ";
std::string stat = u->isActive() ? "    " : "ע  ";
std::cout << std::left
<< std::setw(12) << u->getAccount()
<< std::setw(8)  << role
<< std::setw(10) << u->getName().substr(0,9)
<< std::setw(12) << u->getPhone()
<< std::setw(20) << u->getEmail().substr(0,19)
<< std::setw(8)  << stat << '\n';
});
}

void Admin::viewAllRecords(std::shared_ptr<Library> library) const
{
//   ¼ 鿴    
logAdminAction(library, AdminActionType::VIEW_RECORDS, " 鿴   н  ļ ¼", "ALL", 
               "    Ա 鿴     н  ļ ¼");

auto records = library->getAllRecords();
paginatePrint< std::shared_ptr<Record> >(
records,
"  ¼      ˺       ISBN                      Ӧ          ʵ          ״̬\n",
[](const std::shared_ptr<Record>& r){
std::string stat = r->getStatus()==RecordStatus::BORROWED ? " ڽ " : " ѻ ";
std::cout << std::left
<< std::setw(8)  << r->getRecordId()
<< std::setw(12) << r->getAccount()
<< std::setw(15) << r->getIsbn()
<< std::setw(12) << r->getBorrowDate()
<< std::setw(12) << r->getDueDate()
<< std::setw(12) << r->getReturnDate()
<< std::setw(8)  << stat << '\n';
});
}

//      ˺Ų  Ҽ ¼
void Admin::searchRecordsByAccount(std::shared_ptr<Library> library, const std::string& account) const
{
    //   ¼        
    logAdminAction(library, AdminActionType::SEARCH_RECORDS, "   ˺       ¼", account, 
                   "    Ա       û  " + account + "  Ľ  ļ ¼");

    auto allRecords = library->getAllRecords();
    std::vector<std::shared_ptr<Record>> filteredRecords;
    
    for (const auto& record : allRecords) {
        if (record->getAccount() == account) {
            filteredRecords.push_back(record);
        }
    }
    
    std::sort(filteredRecords.begin(), filteredRecords.end(),
              [](const std::shared_ptr<Record>& a, const std::shared_ptr<Record>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    if (filteredRecords.empty()) {
        std::cout << "δ ҵ    û  Ľ  ļ ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<Record> >(
        filteredRecords,
        "  ¼      ˺       ISBN                      Ӧ          ʵ          ״̬\n",
        [](const std::shared_ptr<Record>& r){
            std::string stat = r->getStatus()==RecordStatus::BORROWED ? " ڽ " : " ѻ ";
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAccount()
            << std::setw(15) << r->getIsbn()
            << std::setw(12) << r->getBorrowDate()
            << std::setw(12) << r->getDueDate()
            << std::setw(12) << r->getReturnDate()
            << std::setw(8)  << stat << '\n';
        });
}

//     ISBN   Ҽ ¼
void Admin::searchRecordsByISBN(std::shared_ptr<Library> library, const std::string& isbn) const
{
    //   ¼        
    logAdminAction(library, AdminActionType::SEARCH_RECORDS, "  ISBN      ¼", isbn, 
                   "    Ա      ISBN " + isbn + "  Ľ  ļ ¼");

    auto allRecords = library->getAllRecords();
    std::vector<std::shared_ptr<Record>> filteredRecords;
    
    for (const auto& record : allRecords) {
        if (record->getIsbn() == isbn) {
            filteredRecords.push_back(record);
        }
    }
    
    std::sort(filteredRecords.begin(), filteredRecords.end(),
              [](const std::shared_ptr<Record>& a, const std::shared_ptr<Record>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    if (filteredRecords.empty()) {
        std::cout << "δ ҵ   ͼ  Ľ  ļ ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<Record> >(
        filteredRecords,
        "  ¼      ˺       ISBN                      Ӧ          ʵ          ״̬\n",
        [](const std::shared_ptr<Record>& r){
            std::string stat = r->getStatus()==RecordStatus::BORROWED ? " ڽ " : " ѻ ";
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAccount()
            << std::setw(15) << r->getIsbn()
            << std::setw(12) << r->getBorrowDate()
            << std::setw(12) << r->getDueDate()
            << std::setw(12) << r->getReturnDate()
            << std::setw(8)  << stat << '\n';
        });
}

//        ڶβ  Ҽ ¼
void Admin::searchRecordsByDateRange(std::shared_ptr<Library> library, const std::string& startDate, const std::string& endDate) const
{
    if (!Utils::isValidDate(startDate) || !Utils::isValidDate(endDate)) {
        std::cout << "   ڸ ʽ  Ч    ʹ  YYYY-MM-DD  ʽ  \n";
        return;
    }
    
    if (Utils::compareDates(startDate, endDate) > 0) {
        std::cout << "  ʼ   ڲ      ڽ      ڣ \n";
        return;
    }
    
    //   ¼        
    logAdminAction(library, AdminActionType::SEARCH_RECORDS, "     ڶ       ¼", 
                   startDate + "    " + endDate, 
                   "    Ա       " + startDate + "    " + endDate + "  Ľ  ļ ¼");

    auto allRecords = library->getAllRecords();
    std::vector<std::shared_ptr<Record>> filteredRecords;
    
    for (const auto& record : allRecords) {
        std::string targetDate = record->getBorrowDate();
        if (Utils::compareDates(targetDate, startDate) >= 0 && 
            Utils::compareDates(targetDate, endDate) <= 0) {
            filteredRecords.push_back(record);
        }
    }
    
    std::sort(filteredRecords.begin(), filteredRecords.end(),
              [](const std::shared_ptr<Record>& a, const std::shared_ptr<Record>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    if (filteredRecords.empty()) {
        std::cout << "δ ҵ   ʱ    ڵĽ  ļ ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<Record> >(
        filteredRecords,
        "  ¼      ˺       ISBN                      Ӧ          ʵ          ״̬\n",
        [](const std::shared_ptr<Record>& r){
            std::string stat = r->getStatus()==RecordStatus::BORROWED ? " ڽ " : " ѻ ";
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAccount()
            << std::setw(15) << r->getIsbn()
            << std::setw(12) << r->getBorrowDate()
            << std::setw(12) << r->getDueDate()
            << std::setw(12) << r->getReturnDate()
            << std::setw(8)  << stat << '\n';
        });
}

//    ݼ ¼ ž ȷ    
void Admin::searchRecordsByRecordId(std::shared_ptr<Library> library, int recordId) const
{
    //   ¼        
    logAdminAction(library, AdminActionType::SEARCH_RECORDS, "    ¼ ž ȷ    ", 
                   std::to_string(recordId), 
                   "    Ա     ˼ ¼  Ϊ " + std::to_string(recordId) + "  Ľ  ļ ¼");

    auto allRecords = library->getAllRecords();
    std::shared_ptr<Record> foundRecord = nullptr;
    
    for (const auto& record : allRecords) {
        if (record->getRecordId() == recordId) {
            foundRecord = record;
            break;
        }
    }
    
    if (!foundRecord) {
        std::cout << "δ ҵ   ¼  Ϊ " << recordId << "  Ľ  ļ ¼  \n";
        return;
    }
    
    std::cout << "\n===   ¼     ===\n";
    std::cout << "  ¼   : " << foundRecord->getRecordId() << '\n';
    std::cout << " û  ˺ : " << foundRecord->getAccount() << '\n';
    std::cout << "ͼ  ISBN: " << foundRecord->getIsbn() << '\n';
    std::cout << "        : " << foundRecord->getBorrowDate() << '\n';
    std::cout << "Ӧ      : " << foundRecord->getDueDate() << '\n';
    std::cout << "ʵ      : " << (foundRecord->getReturnDate().empty() ? "δ 黹" : foundRecord->getReturnDate()) << '\n';
    std::cout << "״̬: " << (foundRecord->getStatus()==RecordStatus::BORROWED ? " ڽ " : " ѻ ") << '\n';
}

//        鿴   й   Ա      ¼
void Admin::viewAllAdminRecords(std::shared_ptr<Library> library) const
{
    auto adminRecords = library->getAllAdminRecords();
    
    if (adminRecords.empty()) {
        std::cout << "   ޹   Ա      ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<AdminRecord> >(
        adminRecords,
        "  ¼         Ա                                        Ŀ  ID            ʱ        ϸ  Ϣ\n",
        [](const std::shared_ptr<AdminRecord>& r){
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAdminAccount()
            << std::setw(14) << r->getActionTypeString()
            << std::setw(22) << r->getActionDescription().substr(0, 21)
            << std::setw(14) << r->getTargetId()
            << std::setw(12) << r->getActionTime()
            << std::setw(20) << r->getDetails().substr(0, 19) << '\n';
        });
}

//             Ա          ¼
void Admin::searchAdminRecordsByAdmin(std::shared_ptr<Library> library, const std::string& adminAccount) const
{
    auto adminRecords = library->getAdminRecordsByAdmin(adminAccount);
    
    if (adminRecords.empty()) {
        std::cout << "δ ҵ     Ա " << adminAccount << "  Ĳ     ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<AdminRecord> >(
        adminRecords,
        "  ¼         Ա                                        Ŀ  ID            ʱ        ϸ  Ϣ\n",
        [](const std::shared_ptr<AdminRecord>& r){
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAdminAccount()
            << std::setw(14) << r->getActionTypeString()
            << std::setw(22) << r->getActionDescription().substr(0, 21)
            << std::setw(14) << r->getTargetId()
            << std::setw(12) << r->getActionTime()
            << std::setw(20) << r->getDetails().substr(0, 19) << '\n';
        });
}

//                           ¼
void Admin::searchAdminRecordsByAction(std::shared_ptr<Library> library, AdminActionType actionType) const
{
    auto adminRecords = library->getAdminRecordsByAction(actionType);
    
    if (adminRecords.empty()) {
        std::cout << "δ ҵ  ò      ͵ļ ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<AdminRecord> >(
        adminRecords,
        "  ¼         Ա                                        Ŀ  ID            ʱ        ϸ  Ϣ\n",
        [](const std::shared_ptr<AdminRecord>& r){
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAdminAccount()
            << std::setw(14) << r->getActionTypeString()
            << std::setw(22) << r->getActionDescription().substr(0, 21)
            << std::setw(14) << r->getTargetId()
            << std::setw(12) << r->getActionTime()
            << std::setw(20) << r->getDetails().substr(0, 19) << '\n';
        });
}

//            ڶ           ¼
void Admin::searchAdminRecordsByDateRange(std::shared_ptr<Library> library, const std::string& startDate, const std::string& endDate) const
{
    if (!Utils::isValidDate(startDate) || !Utils::isValidDate(endDate)) {
        std::cout << "   ڸ ʽ  Ч    ʹ  YYYY-MM-DD  ʽ  \n";
        return;
    }
    
    if (Utils::compareDates(startDate, endDate) > 0) {
        std::cout << "  ʼ   ڲ      ڽ      ڣ \n";
        return;
    }
    
    auto allAdminRecords = library->getAllAdminRecords();
    std::vector<std::shared_ptr<AdminRecord>> filteredRecords;
    
    for (const auto& record : allAdminRecords) {
        std::string targetDate = record->getActionTime();
        if (Utils::compareDates(targetDate, startDate) >= 0 && 
            Utils::compareDates(targetDate, endDate) <= 0) {
            filteredRecords.push_back(record);
        }
    }
    
    std::sort(filteredRecords.begin(), filteredRecords.end(),
              [](const std::shared_ptr<AdminRecord>& a, const std::shared_ptr<AdminRecord>& b) {
                  return a->getRecordId() > b->getRecordId();
              });
    
    if (filteredRecords.empty()) {
        std::cout << "δ ҵ   ʱ    ڵĹ   Ա      ¼  \n";
        return;
    }
    
    paginatePrint< std::shared_ptr<AdminRecord> >(
        filteredRecords,
        "  ¼         Ա                                        Ŀ  ID            ʱ        ϸ  Ϣ\n",
        [](const std::shared_ptr<AdminRecord>& r){
            std::cout << std::left
            << std::setw(8)  << r->getRecordId()
            << std::setw(12) << r->getAdminAccount()
            << std::setw(14) << r->getActionTypeString()
            << std::setw(22) << r->getActionDescription().substr(0, 21)
            << std::setw(14) << r->getTargetId()
            << std::setw(12) << r->getActionTime()
            << std::setw(20) << r->getDetails().substr(0, 19) << '\n';
        });
}
