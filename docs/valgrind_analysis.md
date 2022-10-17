# Valgrind Analysis
## 问题一:
单例对象(Logger)未正常释放,修改方法见[C++单例对象回收方法](./cpp_singleton.md)
## 问题二：
循环队列环形引用导致资源未正确释放
```
cur = head;
while(cur!=nullptr){
    std::shared_ptr<BufferNode> next = cur->next;
    cur = nullptr;
    cur = next;
}
```
```
cur = head;
head = nullptr;
while(cur!=nullptr){
    std::shared_ptr<BufferNode> next = cur->next;
    if(next != nullptr){
        next->prev = nullptr;
    }
    cur = next;
}
tail = nullptr;
```