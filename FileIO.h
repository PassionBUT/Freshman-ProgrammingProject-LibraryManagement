#ifndef FILEIO_H
#define FILEIO_H

#include "Library.h"
#include <string>

class FileIO {
public:
    
    static void saveToFiles(std::shared_ptr<Library> library);
    static void loadFromFiles(std::shared_ptr<Library> library);
    
private:
    
    static void saveBooks(std::shared_ptr<Library> library);
    static void saveUsers(std::shared_ptr<Library> library);
    static void saveRecords(std::shared_ptr<Library> library);
    static void saveAdminRecords(std::shared_ptr<Library> library); 
    
    static void loadBooks(std::shared_ptr<Library> library);
    static void loadUsers(std::shared_ptr<Library> library);
    static void loadRecords(std::shared_ptr<Library> library);
    static void loadAdminRecords(std::shared_ptr<Library> library);
};

#endif

/*  ** static的作用  **

不需要先创建 FileIO 对象，就能通过类名直接调用。
*/
/*  **  文件管理 **

./ -->同级
../  -->下一级 

*/



