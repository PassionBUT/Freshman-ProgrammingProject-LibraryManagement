#include "AdminRecord.h"
#include <sstream>
#include <iomanip>

AdminRecord::AdminRecord(int recordId, const std::string& adminAccount,
                         AdminActionType actionType, const std::string& actionDescription,
                         const std::string& targetId, const std::string& actionTime,
                         const std::string& details)
    : recordId(recordId), adminAccount(adminAccount), actionType(actionType),
      actionDescription(actionDescription), targetId(targetId), 
      actionTime(actionTime), details(details) {}

std::string AdminRecord::getActionTypeString() const {
    switch (actionType) {
        case AdminActionType::ADD_BOOK:
            return "添加图书";
        case AdminActionType::REMOVE_BOOK:
            return "删除图书";
        case AdminActionType::UPDATE_BOOK:
            return "更新图书";
        case AdminActionType::IMPORT_BOOKS:
            return "批量导入图书";
        case AdminActionType::ADD_USER:
            return "添加用户";
        case AdminActionType::REMOVE_USER:
            return "删除用户";
        case AdminActionType::UPDATE_USER:
            return "更新用户";
        case AdminActionType::RESET_PASSWORD:
            return "重置密码";
        case AdminActionType::DEACTIVATE_USER:
            return "注销用户";
        case AdminActionType::ACTIVATE_USER:
            return "恢复用户";
        case AdminActionType::VIEW_BOOKS:
            return "查看图书";
        case AdminActionType::VIEW_USERS:
            return "查看用户";
        case AdminActionType::VIEW_RECORDS:
            return "查看借阅记录";
        case AdminActionType::SEARCH_RECORDS:
            return "搜索记录";
        default:
            return "未知操作";
    }
}

std::string AdminRecord::toString() const {
    std::ostringstream oss;
    oss << recordId << "," 
        << adminAccount << "," 
        << static_cast<int>(actionType) << ","
        << actionDescription << ","
        << targetId << ","
        << actionTime << ","
        << details;
    return oss.str();
}

std::shared_ptr<AdminRecord> AdminRecord::fromString(const std::string& str) {
    std::istringstream iss(str);
    std::string recordIdStr, adminAccount, actionTypeStr, actionDescription, 
                targetId, actionTime, details;
    
    std::getline(iss, recordIdStr, ',');
    std::getline(iss, adminAccount, ',');
    std::getline(iss, actionTypeStr, ',');
    std::getline(iss, actionDescription, ',');
    std::getline(iss, targetId, ',');
    std::getline(iss, actionTime, ',');
    std::getline(iss, details, ',');
    
    int recordId = std::stoi(recordIdStr);
    int actionTypeInt = std::stoi(actionTypeStr);
    AdminActionType actionType = static_cast<AdminActionType>(actionTypeInt);
    
    return std::make_shared<AdminRecord>(recordId, adminAccount, actionType, 
                                         actionDescription, targetId, actionTime, details);
}
