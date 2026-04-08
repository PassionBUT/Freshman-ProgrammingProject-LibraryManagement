#include "FileIO.h"
#include <fstream>  // 打开,写入,读取 txt 文件 

void FileIO::saveToFiles(std::shared_ptr<Library> library){
	///
    try {
        saveBooks(library);
        saveUsers(library);
        saveRecords(library);
        saveAdminRecords(library);
    } catch (const std::exception& ex){
        throw std::runtime_error("保存数据失败: "+std::string(ex.what()));
    }
}

void FileIO::loadFromFiles(std::shared_ptr<Library> library){
	///
    try{
        loadBooks(library);
        loadUsers(library);
        loadRecords(library);
        loadAdminRecords(library); 
    }catch (const std::exception& ex) {
        throw std::runtime_error("加载数据失败: "+std::string(ex.what()));
    }
}

void FileIO::saveBooks(std::shared_ptr<Library> library){
    std::ofstream file("../data/books.txt");
    if (!file.is_open()) {
        throw std::runtime_error("无法打开 books.txt 文件");
    }
    
    file<<"#books\n";
    auto books=library->getAllBooks();
    for(const auto& book:books) {
        file<<book->toString()<<"\n";
    }
    
    file.close();
}

void FileIO::saveUsers(std::shared_ptr<Library> library){
    std::ofstream file("../data/users.txt");
    if (!file.is_open()){
        throw std::runtime_error("无法打开 users.txt 文件");
    }
    
    file<<"#users\n";
    auto users=library->getAllUsers();
    for (const auto& user : users){
        file<<user->toString()<<"\n";
    }
    
    file.close();
}

void FileIO::saveRecords(std::shared_ptr<Library> library){
    std::ofstream file("../data/records.txt");
    if (!file.is_open()){
        throw std::runtime_error("无法打开 records.txt 文件");
    }
    
    file<<"#records\n";
    auto records=library->getAllRecords();
    for (const auto& record : records){
        file<<record->toString()<<"\n";
    }
    
    file.close();
}

void FileIO::saveAdminRecords(std::shared_ptr<Library> library){
    std::ofstream file("../data/adminrecords.txt");
    if (!file.is_open()){
        throw std::runtime_error("无法打开 adminrecords.txt 文件");
    }
    
    file<<"#adminrecords\n";
    auto adminRecords = library->getAllAdminRecords();
    for (const auto& record : adminRecords){
        file << record->toString() << "\n";
    }
    
    file.close();
}

void FileIO::loadBooks(std::shared_ptr<Library> library){
    std::ifstream file("../data/books.txt");
    if (!file.is_open()){
        return; 
    }
    
    std::string line;
    std::getline(file, line); 
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            auto book = Book::fromString(line);
            library->addBook(book);
        }
    }
    
    file.close();
}

void FileIO::loadUsers(std::shared_ptr<Library> library){
    std::ifstream file("../data/users.txt");
    if (!file.is_open()) {
        return; 
    }
    
    std::string line;
    std::getline(file, line); 
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            auto user = User::fromString(line);
            library->addUser(user);
        }
    }
    
    file.close();
}

void FileIO::loadRecords(std::shared_ptr<Library> library){
    std::ifstream file("../data/records.txt");
    if (!file.is_open()){
        return; 
    }
    
    std::string line;
    std::getline(file, line);
    
    int maxRecordId = 0;
    while (std::getline(file, line)){
        if (!line.empty()){
            auto record = Record::fromString(line);
            library->addRecord(record);
            maxRecordId = std::max(maxRecordId, record->getRecordId());
        }
    }
    
    library->setNextRecordId(maxRecordId + 1);   
    file.close();
}

// 新增：加载管理员操作记录
void FileIO::loadAdminRecords(std::shared_ptr<Library> library){
    std::ifstream file("../data/adminrecords.txt");
    if (!file.is_open()){
        return;   // 文件不存在则跳过，不报错
    }
    
    std::string line;
    std::getline(file, line);   // 读取第一行 "#adminrecords"
    
    int maxAdminRecordId = 0;
    while (std::getline(file, line)){
        if (!line.empty()){
            auto adminRecord = AdminRecord::fromString(line);
            library->addAdminRecord(adminRecord);
            maxAdminRecordId = std::max(maxAdminRecordId, adminRecord->getRecordId());
        }
    }
    
    library->setNextAdminRecordId(maxAdminRecordId + 1);
    
    file.close();
}
