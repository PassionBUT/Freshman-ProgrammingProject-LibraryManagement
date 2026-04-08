#ifndef UI_H
#define UI_H

#include "Library.h"
#include "Searcher.h"
#include "Recommender.h"
#include <memory>

class UI {
private:
    std::shared_ptr<Library> library;
    std::shared_ptr<Searcher> searcher;
    std::shared_ptr<Recommender> recommender;
    
public:
    UI(std::shared_ptr<Library> lib, std::shared_ptr<Searcher> search, std::shared_ptr<Recommender> rec);
    

    void showMainMenu();
    void showAdminMenu();
    void showReaderMenu();
    void showGuestMenu();

    void handleAdminBookManagement();
    void handleAdminUserManagement();
    void handleAdminRecordManagement();  // 记录管理
    void handleAdminRecordManagementSubMenu(std::shared_ptr<Admin> admin);  // 管理员操作记录子菜单
    void handleReaderOperations();
    
  
    void showLoginScreen();
    void showRegisterScreen();
    
    void pause();
};

#endif
