#ifndef RECOMMENDER_H
#define RECOMMENDER_H

#include "Book.h"
#include "Record.h"
#include "Library.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>

/* 2(1) 加个 recommender */
/* 3(1) 把推荐逻辑改成协同过滤(**有过相同借阅记录的书单，则可推荐** --> 原来的实现不了这个) */
/* (0) records里有100+数据 */


const int MAX_R=5; // 最大推荐数量

class Recommender {
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> userBookMatrix;
    std::unordered_map<std::string, std::unordered_map<std::string, int>> bookCooccurrence;
    
public:
    void buildMatrix(std::shared_ptr<Library> library);
    void addRecord(std::shared_ptr<Record> record);
    
    std::vector<std::shared_ptr<Book>> getRecommendations(std::shared_ptr<Library> library, const std::string& userAccount) const;
    
private:
    std::vector<std::string> getUserBorrowedBooks(const std::string& userAccount) const;
    std::vector<std::pair<std::string, int>> getRelatedBooks(const std::vector<std::string>& userBooks) const;
};

#endif



/* ** 协同过滤 的实现方法 **
 代码逻辑: 
    A[初始化推荐器] --> B[调用buildMatrix构建矩阵]
    B --> C[筛选已归还的借阅记录]
    C --> D[调用addRecord填充两个核心矩阵]
    D --> E[用户发起推荐请求，调用getRecommendations]
    E --> F[获取用户已借图书列表]
    F --> G[计算相关图书得分]
    G --> H[过滤已借图书+按得分排序]
    H --> I[返回Top5有效图书]


*/


