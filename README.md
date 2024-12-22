# CppLSMTreeKVStorage

基于 [从0开始：500行代码实现 LSM 数据库 - 肖汉松](https://mp.weixin.qq.com/s/kCpV0evSuISET7wGyB9Efg) 实现，按照自己的理解补充了空缺的部分，使之成为一个可以使用的完整头文件。

使用了 [nlohmann/json](https://github.com/nlohmann/json.git) 处理 json 数据。

目前有一个基本使用上的 BUG，在对象被销毁时，内存中的数据不会被持久化，也不能从 WAL 中恢复。

需要 C++17