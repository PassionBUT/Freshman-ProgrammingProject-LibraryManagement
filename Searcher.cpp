#include "Searcher.h"
#include <algorithm>


void Searcher::buildIndexes(std::shared_ptr<Library> library) {
    auto books = library->getAllBooks();
    
    isbnIndex.clear();
    authorIndex.clear();
    titleIndex.clear();
    publisherIndex.clear();
    
    for (const auto& book : books) {
        addBook(book);
    }
}

void Searcher::addBook(std::shared_ptr<Book> book) {
    isbnIndex[book->getIsbn()] = book;
    
     authorIndex[book->getAuthor()].push_back(book);
    
    titleIndex[book->getTitle()].push_back(book);
    
    publisherIndex[book->getPublisher()].push_back(book);
}

void Searcher::removeBook(const std::string& isbn) {
    auto it = isbnIndex.find(isbn);
    if (it != isbnIndex.end()) {
        auto book = it->second;
        
        isbnIndex.erase(it);

        auto& authorBooks = authorIndex[book->getAuthor()];
        authorBooks.erase(std::remove(authorBooks.begin(), authorBooks.end(), book), authorBooks.end());
        if (authorBooks.empty()) {
            authorIndex.erase(book->getAuthor());
        }
        
        auto& titleBooks = titleIndex[book->getTitle()];
        titleBooks.erase(std::remove(titleBooks.begin(), titleBooks.end(), book), titleBooks.end());
        if (titleBooks.empty()) {
            titleIndex.erase(book->getTitle());
        }
        
        auto& publisherBooks = publisherIndex[book->getPublisher()];
        publisherBooks.erase(std::remove(publisherBooks.begin(), publisherBooks.end(), book), publisherBooks.end());
        if (publisherBooks.empty()) {
            publisherIndex.erase(book->getPublisher());
        }
    }
}

std::vector<std::shared_ptr<Book>> Searcher::searchByISBN(std::shared_ptr<Library>, const std::string& isbn) {
    std::vector<std::shared_ptr<Book>> results;
    auto it = isbnIndex.find(isbn);
    if (it != isbnIndex.end()) {
        results.push_back(it->second);
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Searcher::searchByTitleExact(std::shared_ptr<Library>, const std::string& title) {
    std::vector<std::shared_ptr<Book>> results;
    auto it = titleIndex.find(title);
    if (it != titleIndex.end()) {
        results = it->second;
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Searcher::searchByTitleFuzzy(std::shared_ptr<Library>, const std::string& keyword) {
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : titleIndex) {
        if (containsIgnoreCase(pair.first, keyword)) {
            results.insert(results.end(), pair.second.begin(), pair.second.end());
        }
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Searcher::searchByAuthor(std::shared_ptr<Library>, const std::string& keyword) {
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : authorIndex) {
        if (containsIgnoreCase(pair.first, keyword)) {
            results.insert(results.end(), pair.second.begin(), pair.second.end());
        }
    }
    return results;
}

std::vector<std::shared_ptr<Book>> Searcher::searchByPublisher(std::shared_ptr<Library>, const std::string& keyword) {
    std::vector<std::shared_ptr<Book>> results;
    for (const auto& pair : publisherIndex) {
        if (containsIgnoreCase(pair.first, keyword)) {
            results.insert(results.end(), pair.second.begin(), pair.second.end());
        }
    }
    return results;
}

bool Searcher::containsIgnoreCase(const std::string& str, const std::string& keyword) {
    std::string lowerStr = str;
    std::string lowerKeyword = keyword;
    
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
    
    return lowerStr.find(lowerKeyword) != std::string::npos;
}

