#include "Recommender.h"
#include <algorithm>

 

void Recommender::buildMatrix(std::shared_ptr<Library> library) {
    userBookMatrix.clear();
    bookCooccurrence.clear();
    
    auto records = library->getAllRecords();
    
    for (const auto& record : records) {
        if (record->getStatus() == RecordStatus::RETURNED) {
            addRecord(record);
        }
    }
}

void Recommender::addRecord(std::shared_ptr<Record> record) {
    const std::string& user = record->getAccount();
    const std::string& book = record->getIsbn();
    
    userBookMatrix[user].insert(book);
    
    for (const auto& otherBook : userBookMatrix[user]) {
        if (otherBook != book) {
            bookCooccurrence[book][otherBook]++;
            bookCooccurrence[otherBook][book]++;
        }
    }
}

std::vector<std::shared_ptr<Book>> Recommender::getRecommendations(std::shared_ptr<Library> library, const std::string& userAccount) const {
    std::vector<std::shared_ptr<Book>> recommendations;
    
    std::vector<std::string> userBooks = getUserBorrowedBooks(userAccount);
    
    if (userBooks.empty()) {
        return recommendations;
    }
    
    auto relatedBooks = getRelatedBooks(userBooks);
    
    std::unordered_set<std::string> userBookSet(userBooks.begin(), userBooks.end());
    
    std::vector<std::pair<std::string, int>> filteredRecommendations;
    for (const auto& pair : relatedBooks) {
        if (userBookSet.find(pair.first) == userBookSet.end()) {
            filteredRecommendations.push_back(pair);
        }
    }
    
    std::sort(filteredRecommendations.begin(), filteredRecommendations.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second > b.second; });
    
    for (size_t i = 0; i < filteredRecommendations.size() && i < MAX_R; i++) {
        auto book = library->findBookByISBN(filteredRecommendations[i].first);
        if (book) {
            recommendations.push_back(book);
        }
    }
    
    return recommendations;
}

std::vector<std::string> Recommender::getUserBorrowedBooks(const std::string& userAccount) const {
    auto it = userBookMatrix.find(userAccount);
    if (it != userBookMatrix.end()) {
        return std::vector<std::string>(it->second.begin(), it->second.end());
    }
    return std::vector<std::string>();
}

std::vector<std::pair<std::string, int>> Recommender::getRelatedBooks(const std::vector<std::string>& userBooks) const {
    std::unordered_map<std::string, int> bookScores;
    
    for (const auto& book : userBooks) {
        auto it = bookCooccurrence.find(book);
        if (it != bookCooccurrence.end()) {
            for (const auto& pair : it->second) {
                bookScores[pair.first] += pair.second;
            }
        }
    }
    
    return std::vector<std::pair<std::string, int>>(bookScores.begin(), bookScores.end());
}

