#ifndef SEARCHER_H
#define SEARCHER_H

#include "Book.h"
#include "Library.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <map>

/*
1(1) Searcher的实现
(1)  精确查找 
	(1) 题名
	(1) ISBN/ISSN
(1)  模糊查找
	(1) 作者
	(1) 出版社
   3(1) 题名
   3(0) ISBN/ISSN
*/


class Searcher {
private:
    
    std::unordered_map<std::string, std::shared_ptr<Book>> isbnIndex;
    std::map<std::string, std::vector<std::shared_ptr<Book>>> authorIndex;
    std::map<std::string, std::vector<std::shared_ptr<Book>>> titleIndex;
    std::map<std::string, std::vector<std::shared_ptr<Book>>> publisherIndex;
    
public:
    void buildIndexes(std::shared_ptr<Library> library);
    void addBook(std::shared_ptr<Book> book);
    void removeBook(const std::string& isbn);
    
    std::vector<std::shared_ptr<Book>> searchByISBN(std::shared_ptr<Library> library, const std::string& isbn);
    std::vector<std::shared_ptr<Book>> searchByTitleExact(std::shared_ptr<Library> library, const std::string& title);
    std::vector<std::shared_ptr<Book>> searchByTitleFuzzy(std::shared_ptr<Library> library, const std::string& keyword);
    std::vector<std::shared_ptr<Book>> searchByAuthor(std::shared_ptr<Library> library, const std::string& keyword);
    std::vector<std::shared_ptr<Book>> searchByPublisher(std::shared_ptr<Library> library, const std::string& keyword);
    
private:
    bool containsIgnoreCase(const std::string& str, const std::string& keyword);
};

#endif

