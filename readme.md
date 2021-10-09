### 简介

**snow data base** 是一个基于Bitcask理论的key value数据库

数据库可以内嵌式使用，或者使用Client-Server的方式使用，client采用了阻塞的方式进行远程rpc调用
所依赖的rpc工具为作者自研的snow rpc，如果您需要，也可以单独将snow rpc部分抽出来，发布自己的rpc方法
当采用Client和Server使用时，调用协议依赖google protobuf，详情见proto文件



### 构建方式

仓库里提供了cmakeLists，makefile和xmake.lua，三种方式均可构建
但是需要您提前安装protobuf



### 底层原理

本key-value数据库底层数据结构基于 跳表 ,持久化基于LSM-Tree理论，可以高速插入数据，但是在读取数据上略有短板



### 目前功能

支持单个key-value查询，支持范围查询

支持内嵌式

支持阻塞式-远程rpc调用，rpc为作者自研snow rpc



### 使用场景

目前使用hash/skipList-lsm Tree理论的数据库有：
BeansDB，豆瓣公司使用的存储日记的数据库
RocksDB，k-v数据库热榜排名前十



### TODO

支持事务
支持异步调用
设置bufferPool缓存，增快读取速度



### 结语

如果您对该项目感兴趣，可以一起参与构建
