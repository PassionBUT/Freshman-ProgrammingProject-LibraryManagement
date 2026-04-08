/*
就自己也想看懂,没门: 

// --> 表示标注逻辑/作用
/* --> 表示未完成/已完成的功能

功能实现注释 int1(int2)
int1:
	1 --> 必要
	2 --> 拓展
	3 --> 不必要,但最好 
	4 --> 游戏玩的不爽可以来实现找找存在感 
int2: 
	1 --> 已完成
	0 --> 未完成 
   10 --> 完成了,但少了 

笔记栏目:
统一写在最下面  /* **  我是题目  **
 
*/ 


/* 3(1) 把.cpp 里的 bits/stdc++.h 改成 对应需要的 头文件,写出在哪里用了 */
/* 3(1) 把 using namespace std; 删去,按照编译器补充std:: ,并总结什么时候加 std:: */

#include <iostream>  // 输出: std::cout,std::endl,std::cerr 
#include <memory>  // 智能指针: std::make_shared.std::shared_ptr 
#include <stdexcept>  // 抓异常: std::exception 
#include "Library.h"
#include "FileIO.h"
#include "UI.h"
#include "Searcher.h"
#include "Recommender.h"
#include "Admin.h"
#include "Reader.h"

/* 2(1) 加个IO类,来管理统一数据的储存 --> 提升写入/写出的速度,方便查询等功能的实现,改写数据的实现 */

/* 1(10) 加个Utils类,放一些小工具-->判断是否合理这样的 */

/* 4(1) 刚弄懂的const,可以加加看 
加加加,真的无语,谢谢你让我对不做人的制造者的怨气撒在你身子上了!!!
我一定不会在玩 the Walking Death 了,再玩我就是狗. 
*/

/* 2(1) IO类V 但是这样还要管理内存很麻烦  ai::可以试试 智能指针
问的那个ai,让我用智能指针,拉黑他!!!
内存是不容易漏了,我脑子快漏水了 
我tm改了整个文件啊!!! */

/* 4(1) 新学的的 try-catch(处理异常情况的),可以加来试试看
加加加,加个头!!! 
为什么? 我就是玩个游戏啊? 为什么???  
/// 表示可以加try-catch(ex); ////表示可以加try-catch(...) */ 

/* 4(1) 感觉数据和代码放在一起感觉好乱,希望可以改改 
改改改,改个头!!! 
本来吃刀就烦,天天要求怎么多.
怎么把任务留给今天的我啊!!! 
./ -->同级
../  -->下一级 
*/

/* 4(0) 其实数字行最好right吧(感觉看的更舒服?) 
没事找事,这又改的必要吗?明明是 =_+ 游戏玩的不开心也不会写的部分 
*/
void initializeSystem(std::shared_ptr<Library> library){
    if (library->getAllUsers().empty()){
        auto admin = std::make_shared<Admin>("admin", "admin123", "管理员", "13800000000", "admin@library.com");
        library->addUser(admin);
        std::cout << "已创建默认管理员账号：admin，密码：admin123\n";
    }
}

int main(){
	
	/// 
	//// 
	
    try {
       
        auto library = std::make_shared<Library>();
        auto searcher = std::make_shared<Searcher>();
        auto recommender = std::make_shared<Recommender>();
        
        std::cout << "加载数据中,请耐心等待...\n";
        FileIO::loadFromFiles(library);
        
        
        initializeSystem(library);
        searcher->buildIndexes(library);
        recommender->buildMatrix(library);
        
        auto ui=std::make_shared<UI>(library,searcher,recommender);
        ui->showMainMenu();
        
        std::cout << "正在保存数据...\n";
        FileIO::saveToFiles(library);
        
    }catch (const std::exception& ex){
        std::cerr<<"执行失败"<<ex.what()<<std::endl;
        return 1;
    }catch (...) {
        std::cerr<<"未知异常导致程序终止"<<std::endl;
        return 1;
    }
    
    return 0;
}



/*  ** 总结什么时候加 std:: **


一句话原则：
只要编译器可能因为"名字查找"找不到它,或者你想明确告诉编译器"这是标准库里的东西",
就加 std::

1. 必加：std::string, std::vector, std::cout, std::cin, 
std::endl, std::move, std::shared_ptr, std::exception, std::runtime_error…

2. 函数参数 / 返回值 / 成员变量类型
只要类型是标准库模板，就加：
void foo(const std::vector<int>&);

*/
/* ** 智能指针的使用方法(大致) **
新建用 make_shared，传参用 shared_ptr，
容器里放 shared_ptr，循环引用加 weak_ptr。

把 new 换成 make_shared，从此不用自己 delete。
函数传参、容器里一律写 std::shared_ptr<类名>，别传裸指针。
当“我只想让别人看一眼，不想共享所有权”时，用 std::weak_ptr 或裸指针做“观察者”。  --> 未涉及 

*/
/*



*/

/*
 *                 _...._
 *              .-"      "-.
 *             /            \
 *            |   O      O   |
 *            |   .  __  .   |    代码之神，速速显灵！
 *            |  .  (OO)  .  |    编译无错，运行如飞！
 *            |   .  ||  .   |    内存不炸，评测全绿！
 *            |_____||_______|
 *             ___  ||  ___
 *            /  |__||__|  \
 *           /              \
 *          /  /|        |\  \
 *         (  ( |        | )  )
 *          \  \|        |/  /
 *           \_/        \_/
 *
 *      急急如律令，BUG 退散！
 *  代码之神在上，弟子今晚只求一次 AC；
 * 若蒙庇佑，愿此生不再写 UB，不再忘 return 0，不再把 == 写成 =；
 * 若因吾辈愚钝仍遭 CE／RE／TLE，亦甘受之，然恳请明示行号；
 * 谨以干净内存、无栈溢、无数据竞之诚，伏惟尚飨！
 */

