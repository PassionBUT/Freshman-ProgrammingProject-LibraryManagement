#include "UI.h"
#include "Admin.h"
#include "Reader.h"
#include <iostream>
/* 4(0) system("cls") 写个函数,处理不同系统
ps: 虽然感觉没有必要,但是ai写的都用,是不是显得很严谨(? */
/* 4(0) pause() 可以试试改成 pause(" ") */
UI::UI(std::shared_ptr<Library> lib,std::shared_ptr<Searcher> search,std::shared_ptr<Recommender> rec)
:library(lib),searcher(search),recommender(rec){}
void UI::showMainMenu(){
while (true){
system("cls");
std::cout<<"\n=== 图书馆管理系统 ===\n";
std::cout<<"[1] 管理员模式\n";
std::cout<<"[2] 读者模式\n";
/* 1(1) 游客那么大两个字,是看不见吗?
直接用不存储的reader搞吧,正好是值看的操作在reader里都有 */
std::cout<<"[3] 游客模式(只读)\n";
std::cout<<"[0] 退出\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
switch (choice){
case 1:
showLoginScreen();
if(library->getCurrentUser()&&library->isAdmin()){
showAdminMenu();
}
break;
case 2:
showLoginScreen();
if (library->getCurrentUser()&&!library->isAdmin()){
showReaderMenu();
}
break;
case 3:
showGuestMenu();
break;
case 0:
std::cout<<"感谢使用！再见！\n";
return;
default:
std::cout<<"无效选择！\n";
pause();
}
library->logout();
}
}
void UI::showAdminMenu(){
while (true){
system("cls");
std::cout<<"\n=== 管理员菜单 ===\n";
std::cout<<"[1] 图书管理\n";
std::cout<<"[2] 用户管理\n";
std::cout<<"[3] 记录管理\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
switch (choice){
case 1:
handleAdminBookManagement();
break;
case 2:
handleAdminUserManagement();
break;
case 3:
handleAdminRecordManagement();
break;
case 0:
return;
default:
std::cout<<"无效选择！\n";
pause();
}
}
}
void UI::showReaderMenu(){
while (true){
system("cls");
auto reader = std::static_pointer_cast<Reader>(library->getCurrentUser());
std::cout<<"\n=== 读者操作 ===\n";
std::cout<<"[1] 图书搜索\n";
std::cout<<"[2] 借阅图书\n";
std::cout<<"[3] 归还图书\n";
std::cout<<"[4] 查看个人记录\n";
std::cout<<"[5] 排行榜\n";
std::cout<<"[6] 推荐图书\n";
std::cout<<"[7] 更改信息\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
switch (choice){
case 1:
reader->searchBooks(library, searcher);
break;
case 2:{
std::string isbn;
std::cout<<"请输入要借阅的图书ISBN: ";
std::getline(std::cin,isbn);
reader->borrowBook(library,isbn);
break;
}
case 3:{
std::string isbn;
std::cout<<"请输入要归还的图书ISBN: ";
std::getline(std::cin,isbn);
reader->returnBook(library,isbn);
break;
}
case 4:
reader->viewPersonalRecords(library);
break;
case 5:{
std::cout<<"\n[1] 借阅排行榜\n";
std::cout<<"[2] 最新图书\n";
std::cout<<"[3] 读者排行榜\n";
std::cout<<"请选择: ";
int subChoice;
std::cin>>subChoice;
std::cin.ignore();
if (subChoice == 1){
reader->viewTopBorrowedBooks(library);
} else if (subChoice == 2){
reader->viewLatestBooks(library);
}else if(subChoice == 3){
reader->viewTopBorrowers(library);
}
break;
}
case 6:
recommender->buildMatrix(library);
reader->viewRecommendations(library,recommender);
break;
case 7:
std::cout<<"[1] 密码\n";
std::cout<<"[2] 邮箱\n";
std::cout<<"[3] 电话\n";
std::cout<<"请选择: ";
int subChoice;
std::cin>>subChoice;
std::cin.ignore();
if (subChoice == 1){
reader->changePwd();
} else if (subChoice == 2){
reader->changePhone();
}else if(subChoice == 3){
reader->changeEmail();
}
break;
case 0:
return;
default:
std::cout<<"无效选择！\n";
}
pause();
}
}
void UI::showGuestMenu(){
while (true){
system("cls");
auto reader = std::static_pointer_cast<Reader>(library->getCurrentUser());
std::cout<<"\n=== 游客操作(只读) ===\n";
std::cout<<"[1] 图书搜索\n";
std::cout<<"[2] 排行榜\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
switch (choice){
case 1:
reader->searchBooks(library,searcher);
break;
case 2:{
std::cout<<"\n[1] 借阅排行榜\n";
std::cout<<"[2] 最新图书\n";
std::cout<<"[3] 读者排行榜\n";
std::cout<<"请选择: ";
int subChoice;
std::cin>>subChoice;
std::cin.ignore();
if (subChoice == 1){
reader->viewTopBorrowedBooks(library);
} else if (subChoice == 2){
reader->viewLatestBooks(library);
} else if(subChoice == 3){
reader->viewTopBorrowers(library);
}
break;
}
case 0:
return;
default:
std::cout<<"无效选择！\n";
}
pause();
}
}
void UI::handleAdminBookManagement(){
while (true){
system("cls");
std::cout<<"\n=== 图书管理 ===\n";
std::cout<<"[1] 添加图书\n";
std::cout<<"[2] 删除图书\n";
std::cout<<"[3] 更新图书\n";
std::cout<<"[4] 批量导入(CSV)\n";
std::cout<<"[5] 查看所有图书\n";
std::cout<<"[6] 搜索图书\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
if(choice==0) return;
auto admin=std::static_pointer_cast<Admin>(library->getCurrentUser());
switch (choice){
case 1:{
std::string title,isbn,author,publisher,publishDate;
double price;
int stock;
std::cout<<"请输入图书信息:\n";
std::cout<<"题名: "; std::getline(std::cin,title);
std::cout<<"ISBN: "; std::getline(std::cin,isbn);
std::cout<<"作者: "; std::getline(std::cin,author);
std::cout<<"出版社: "; std::getline(std::cin,publisher);
std::cout<<"价格: "; std::cin>>price; std::cin.ignore();
std::cout<<"出版日期(YYYY-MM-DD): "; std::getline(std::cin,publishDate);
std::cout<<"库存: "; std::cin>>stock; std::cin.ignore();
auto book = std::make_shared<Book>(title,isbn,author,publisher,price,publishDate,stock);
admin->addBook(library,book);
pause();
break;
}
case 2:{
std::string isbn;
std::cout<<"请输入要删除的图书ISBN: ";
std::getline(std::cin,isbn);
admin->removeBook(library,isbn);
pause();
break;
}
case 3:{
std::string isbn;
std::cout<<"请输入要更新的图书ISBN: ";
std::getline(std::cin,isbn);
auto book = library->findBookByISBN(isbn);
if (book){
std::string title,author,publisher,publishDate;
double price;
int stock;
std::cout<<"请输入新的图书信息:\n";
std::cout<<"题名("<<book->getTitle()<<"): ";
std::getline(std::cin,title);
if (!title.empty()) book->setTitle(title);
std::cout<<"作者("<<book->getAuthor()<<"): ";
std::getline(std::cin,author);
if (!author.empty()) book->setAuthor(author);
std::cout<<"出版社("<<book->getPublisher()<<"): ";
std::getline(std::cin,publisher);
if (!publisher.empty()) book->setPublisher(publisher);
std::cout<<"价格("<<book->getPrice()<<"): ";
std::cin>>price; std::cin.ignore();
if (price > 0) book->setPrice(price);
std::cout<<"出版日期("<<book->getPublishDate()<<"): ";
std::getline(std::cin,publishDate);
if (!publishDate.empty()) book->setPublishDate(publishDate);
std::cout<<"库存("<<book->getStock()<<"): ";
std::cin>>stock; std::cin.ignore();
if (stock >= 0) book->setStock(stock);
admin->updateBook(library,book);
} else{
std::cout<<"未找到该图书！\n";
}
pause();
break;
}
case 4:{
std::string filename;
std::cout<<"请输入CSV文件名: ";
std::getline(std::cin,filename);
admin->importBooksFromCSV(library,filename);
pause();
break;
}
case 5:
admin->viewAllBooks(library);
pause();
break;
case 6:
admin->searchBooks(library,searcher);
break;
default:
std::cout<<"无效选择！\n";
pause();
}
}
}
void UI::handleAdminUserManagement(){
while (true){
system("cls");
std::cout<<"\n=== 用户管理 ===\n";
std::cout<<"[1] 添加用户\n";
std::cout<<"[2] 删除用户\n";
std::cout<<"[3] 更新用户\n";
std::cout<<"[4] 重置密码\n";
std::cout<<"[5] 注销用户\n";
std::cout<<"[6] 恢复用户\n";
std::cout<<"[7] 查看所有用户\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
if (choice == 0) return;
auto admin = std::static_pointer_cast<Admin>(library->getCurrentUser());
switch (choice){
case 1:{
std::string account,password,name,phone,email;
int role;
std::cout<<"请输入用户信息:\n";
std::cout<<"账号: "; std::getline(std::cin,account);
std::cout<<"密码: "; std::getline(std::cin,password);
std::cout<<"角色(1-管理员,2-读者): "; std::cin>>role; std::cin.ignore();
std::cout<<"姓名: "; std::getline(std::cin,name);
std::cout<<"手机号: "; std::getline(std::cin,phone);
std::cout<<"邮箱: "; std::getline(std::cin,email);
std::shared_ptr<User> user;
if (role == 1){
user = std::make_shared<Admin>(account,password,name,phone,email);
} else{
user = std::make_shared<Reader>(account,password,name,phone,email);
}
admin->addUser(library,user);
pause();
break;
}
case 2:{
std::string account;
std::cout<<"请输入要删除的用户账号: ";
std::getline(std::cin,account);
admin->removeUser(library,account);
pause();
break;
}
case 3:{
std::string account;
std::cout<<"请输入要更新的用户账号: ";
std::getline(std::cin,account);
auto user = library->findUser(account);
if (user){
std::string name,phone,email;
std::cout<<"请输入新的用户信息:\n";
std::cout<<"姓名("<<user->getName()<<"): ";
std::getline(std::cin,name);
if (!name.empty()) user->setName(name);
std::cout<<"手机号("<<user->getPhone()<<"): ";
std::getline(std::cin,phone);
if (!phone.empty()) user->setPhone(phone);
std::cout<<"邮箱("<<user->getEmail()<<"): ";
std::getline(std::cin,email);
if (!email.empty()) user->setEmail(email);
admin->updateUser(library,user);
} else{
std::cout<<"未找到该用户！\n";
}
pause();
break;
}
case 4:{
std::string account;
std::cout<<"请输入要重置密码的用户账号: ";
std::getline(std::cin,account);
admin->resetUserPassword(library,account);
pause();
break;
}
case 5:{
std::string account;
std::cout<<"请输入要注销的用户账号: ";
std::getline(std::cin,account);
admin->deactivateUser(library,account);
pause();
break;
}
case 6:{
std::string account;
std::cout<<"请输入要恢复的用户账号: ";
std::getline(std::cin,account);
admin->activateUser(library,account);
pause();
break;
}
case 7:
admin->viewAllUsers(library);
pause();
break;
default:
std::cout<<"无效选择！\n";
pause();
}
}
}

// 新增：记录管理菜单
void UI::handleAdminRecordManagement(){
while (true){
system("cls");
std::cout<<"\n=== 记录管理 ===\n";
std::cout<<"[1] 查看所有借阅记录\n";
std::cout<<"[2] 按账号搜索借阅记录\n";
std::cout<<"[3] 按ISBN搜索借阅记录\n";
std::cout<<"[4] 按日期段搜索借阅记录\n";
std::cout<<"[5] 按记录号精确查找借阅记录\n";
std::cout<<"[6] 查看管理员操作记录\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
if (choice == 0) return;
auto admin = std::static_pointer_cast<Admin>(library->getCurrentUser());
switch (choice){
case 1:
admin->viewAllRecords(library);
pause();
break;
case 2:{
std::string account;
std::cout<<"请输入要查询的用户账号: ";
std::getline(std::cin,account);
admin->searchRecordsByAccount(library,account);
pause();
break;
}
case 3:{
std::string isbn;
std::cout<<"请输入要查询的图书ISBN: ";
std::getline(std::cin,isbn);
admin->searchRecordsByISBN(library,isbn);
pause();
break;
}
case 4:{
std::string startDate,endDate;
std::cout<<"请输入开始日期(YYYY-MM-DD): ";
std::getline(std::cin,startDate);
std::cout<<"请输入结束日期(YYYY-MM-DD): ";
std::getline(std::cin,endDate);
admin->searchRecordsByDateRange(library,startDate,endDate);
pause();
break;
}
case 5:{
int recordId;
std::cout<<"请输入要查询的记录编号: ";
std::cin>>recordId;
std::cin.ignore();
admin->searchRecordsByRecordId(library,recordId);
pause();
break;
}
case 6:
handleAdminRecordManagementSubMenu(admin);
break;
default:
std::cout<<"无效选择！\n";
pause();
}
}
}

// 新增：管理员操作记录子菜单
void UI::handleAdminRecordManagementSubMenu(std::shared_ptr<Admin> admin) {
while (true){
system("cls");
std::cout<<"\n=== 管理员操作记录 ===\n";
std::cout<<"[1] 查看所有管理员操作记录\n";
std::cout<<"[2] 按管理员账号搜索\n";
std::cout<<"[3] 按操作类型搜索\n";
std::cout<<"[4] 按日期段搜索\n";
std::cout<<"[0] 返回\n";
std::cout<<"请选择: ";
int choice;
std::cin>>choice;
std::cin.ignore();
if (choice == 0) return;
switch (choice){
case 1:
admin->viewAllAdminRecords(library);
pause();
break;
case 2:{
std::string adminAccount;
std::cout<<"请输入管理员账号: ";
std::getline(std::cin,adminAccount);
admin->searchAdminRecordsByAdmin(library,adminAccount);
pause();
break;
}
case 3:{
std::cout<<"请选择操作类型:\n";
std::cout<<"[1] 添加图书\n";
std::cout<<"[2] 删除图书\n";
std::cout<<"[3] 更新图书\n";
std::cout<<"[4] 批量导入图书\n";
std::cout<<"[5] 添加用户\n";
std::cout<<"[6] 删除用户\n";
std::cout<<"[7] 更新用户\n";
std::cout<<"[8] 重置密码\n";
std::cout<<"[9] 注销用户\n";
std::cout<<"[10] 恢复用户\n";
std::cout<<"[11] 查看图书\n";
std::cout<<"[12] 查看用户\n";
std::cout<<"[13] 查看借阅记录\n";
std::cout<<"[14] 搜索记录\n";
std::cout<<"请选择: ";
int actionChoice;
std::cin>>actionChoice;
std::cin.ignore();
AdminActionType actionType;
switch (actionChoice){
case 1: actionType = AdminActionType::ADD_BOOK; break;
case 2: actionType = AdminActionType::REMOVE_BOOK; break;
case 3: actionType = AdminActionType::UPDATE_BOOK; break;
case 4: actionType = AdminActionType::IMPORT_BOOKS; break;
case 5: actionType = AdminActionType::ADD_USER; break;
case 6: actionType = AdminActionType::REMOVE_USER; break;
case 7: actionType = AdminActionType::UPDATE_USER; break;
case 8: actionType = AdminActionType::RESET_PASSWORD; break;
case 9: actionType = AdminActionType::DEACTIVATE_USER; break;
case 10: actionType = AdminActionType::ACTIVATE_USER; break;
case 11: actionType = AdminActionType::VIEW_BOOKS; break;
case 12: actionType = AdminActionType::VIEW_USERS; break;
case 13: actionType = AdminActionType::VIEW_RECORDS; break;
case 14: actionType = AdminActionType::SEARCH_RECORDS; break;
default:
std::cout<<"无效选择！\n";
pause();
return;
}
admin->searchAdminRecordsByAction(library,actionType);
pause();
break;
}
case 4:{
std::string startDate,endDate;
std::cout<<"请输入开始日期(YYYY-MM-DD): ";
std::getline(std::cin,startDate);
std::cout<<"请输入结束日期(YYYY-MM-DD): ";
std::getline(std::cin,endDate);
admin->searchAdminRecordsByDateRange(library,startDate,endDate);
pause();
break;
}
default:
std::cout<<"无效选择！\n";
pause();
}
}
}

void UI::showLoginScreen(){
std::string account,password;
std::cout<<"\n=== 登录 ===\n";
std::cout<<"账号: ";
std::getline(std::cin,account);
std::cout<<"密码: ";
std::getline(std::cin,password);
if (library->login(account,password)){
std::cout<<"登录成功！欢迎 "<<library->getCurrentUser()->getName()<<"！\n";
} else{
std::cout<<"登录失败！账号或密码错误，或账号已被注销。\n";
pause();
}
}
void UI::pause(){
std::cout<<"\n按回车键继续...";
std::cin.get();
}
/*  **  ignore()与get()的世子之争 **
总的来说:
1. 清残留 -> ignore() --> 在 数字->字符串 [先 cin >> number 再 getline(cin, str)]
2. 暂停屏 -> get() --> 在 暂停屏幕"按回车继续"的场景中用 (不关心具体字符，只要知道用户按了即可)
3. 读字符 -> get(ch) --> 不太常用,至少在这个项目中我没有用
*/
