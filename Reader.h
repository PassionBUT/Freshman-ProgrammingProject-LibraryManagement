#ifndef READER_H
#define READER_H

#include "User.h"
#include "Book.h"
#include "Record.h"
#include <vector>
#include <memory>

class Library;
class Searcher;
class Recommender;

class Reader : public User {
public:
    Reader(const std::string& account, const std::string& password, const std::string& name,
           const std::string& phone, const std::string& email);
    /* 1(1) 改信息 */
    void changePwd(); 
    void changePhone(); 
    void changeEmail();
    
    /* 1(1) 借书,还书 */
    void borrowBook(std::shared_ptr<Library> library, const std::string& isbn);
    void returnBook(std::shared_ptr<Library> library, const std::string& isbn);
    /* 1(1) 看自已的记录 */
    void viewPersonalRecords(std::shared_ptr<Library> library) const;
    
    /* 1(1) 排行榜 */
    void viewTopBorrowedBooks(std::shared_ptr<Library> library) const;
    void viewLatestBooks(std::shared_ptr<Library> library) const;
    void viewTopBorrowers(std::shared_ptr<Library> library) const;
   /* 2(1) 推荐功能 */
    void viewRecommendations(std::shared_ptr<Library> library, std::shared_ptr<Recommender> recommender) const;
};

#endif

