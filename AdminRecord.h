#ifndef ADMIN_RECORD_H
#define ADMIN_RECORD_H

#include <string>
#include <memory>

// 管理员操作类型枚举
enum class AdminActionType {
    ADD_BOOK,           // 添加图书
    REMOVE_BOOK,        // 删除图书
    UPDATE_BOOK,        // 更新图书
    IMPORT_BOOKS,       // 批量导入图书
    ADD_USER,           // 添加用户
    REMOVE_USER,        // 删除用户
    UPDATE_USER,        // 更新用户
    RESET_PASSWORD,     // 重置密码
    DEACTIVATE_USER,    // 注销用户
    ACTIVATE_USER,      // 恢复用户
    VIEW_BOOKS,         // 查看图书
    VIEW_USERS,         // 查看用户
    VIEW_RECORDS,       // 查看借阅记录
    SEARCH_RECORDS      // 搜索记录
};

class AdminRecord {
private:
    int recordId;                    // 记录编号
    std::string adminAccount;        // 管理员账号
    AdminActionType actionType;      // 操作类型
    std::string actionDescription;   // 操作描述
    std::string targetId;            // 操作目标ID（如ISBN、用户账号等）
    std::string actionTime;          // 操作时间
    std::string details;             // 详细描述

public:
    AdminRecord(int recordId, const std::string& adminAccount, 
                AdminActionType actionType, const std::string& actionDescription,
                const std::string& targetId, const std::string& actionTime,
                const std::string& details = "");
    
    int getRecordId() const { return recordId; }
    std::string getAdminAccount() const { return adminAccount; }
    AdminActionType getActionType() const { return actionType; }
    std::string getActionDescription() const { return actionDescription; }
    std::string getTargetId() const { return targetId; }
    std::string getActionTime() const { return actionTime; }
    std::string getDetails() const { return details; }
    
    // 获取操作类型的字符串描述
    std::string getActionTypeString() const;
    
    std::string toString() const;
    static std::shared_ptr<AdminRecord> fromString(const std::string& str);
};

#endif
