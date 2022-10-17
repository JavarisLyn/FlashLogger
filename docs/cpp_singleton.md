# C++单例对象回收方法
## 单例模式特点：
- 整个class只有一个对象且为static,通过接口获取实例
- 禁止自己声明对象,private构造函数
- 禁止赋值和拷贝
- 线程安全

注：以下实现方法以本项目Logger为例
## 实现方法一:
```
class Logger{
private:
    Logger(){}
    Logger(Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
    static Logger* logger;
public:
    ~Logger(){}
    static Logger* GetInstance(){
        if(logger==nullptr){
              logger = new Logger;
        }
        return logger;
    }
};
 
Logger* Logger::logger = nullptr;
```
## 实现方法二（本项目的方法）:
```
class Logger{
public:
    typedef std::shared_ptr<Logger> share_logger;
    ~Logger(){}
    Logger(Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
    static share_logger GetInstance(){
        // double checked lock
        if(logger_==nullptr){
            std::lock_guard<std::mutex> lk(logger_mutex_);
            if(logger_ == nullptr){
              logger = std::shared_ptr<Logger>(new Logger);
            }
        }
        return logger_;
    }
 
private:
    Logger(){}
    static share_logger logger_;
    static std::mutex logger_mutex_;
};
 
Logger::share_logger Logger::logger_ = nullptr;
std::mutex Logger::logger_mutex_;
```
## 实现方法三(Meyers' Singleton)
```
class Logger
{
public:
    ~Logger(){}
    Logger(const Logger&)=delete;
    Logger& operator=(const Logger&)=delete;
    static Logger& GetInstance(){
        static Logger logger;
        return logger;
    }
private:
    Logger(){}
};
```
*Magic Static: If control enters the declaration concurrently while the variable is being initialized, the concurrent execution shall wait for completion of the initialization.* 
使用时,必须使用引用类型接收
Logger& instance_1 = Logger::GetInstance();