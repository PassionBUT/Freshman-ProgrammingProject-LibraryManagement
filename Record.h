#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <memory>

enum class RecordStatus {
    BORROWED,
    RETURNED
};

class Record {
private:
    int recordId;
    std::string account;
    std::string isbn;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    RecordStatus status;

public:
    Record(int recordId, const std::string& account, const std::string& isbn,
           const std::string& borrowDate, const std::string& dueDate,
           const std::string& returnDate = "", RecordStatus status = RecordStatus::BORROWED);
    
    int getRecordId() const { return recordId; }
    std::string getAccount() const { return account; }
    std::string getIsbn() const { return isbn; }
    std::string getBorrowDate() const { return borrowDate; }
    std::string getDueDate() const { return dueDate; }
    std::string getReturnDate() const { return returnDate; }
    RecordStatus getStatus() const { return status; }
    
    void setReturnDate(const std::string& returnDate) { this->returnDate = returnDate; }
    void setStatus(RecordStatus status) { this->status = status; }
    
    bool isOverdue(const std::string& currentDate) const;
    int calculateOverdueDays(const std::string& currentDate) const;
    
    std::string toString() const;
    static std::shared_ptr<Record> fromString(const std::string& str);
};

#endif

