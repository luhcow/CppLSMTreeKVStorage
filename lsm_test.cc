#include "lsm.hpp"  // 假设主实现在这个头文件中

#include <gtest/gtest.h>
#include <unistd.h>

#include <random>
using namespace LSMTreeKVStorage;
// 工具函数：生成随机字符串
std::string randomString(size_t length) {
  static const char charset[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);

  std::string str;
  str.reserve(length);
  for (size_t i = 0; i < length; ++i) {
    str += charset[dist(generator)];
  }
  return str;
}
// };

// 基本的CRUD操作测试
TEST(LsmKvStoreTest, BasicOperations) {
  LsmKvStore store("/home/rings/LSMTree/wal", 5, 5);
  // 测试设置和获取
  store.set("key1", "value1");
  EXPECT_EQ(store.get("key1"), "value1");

  // 测试更新
  store.set("key1", "value2");
  EXPECT_EQ(store.get("key1"), "value2");

  // 测试删除
  store.remove("key1");
  EXPECT_EQ(store.get("key1"), "");
}

// 测试存储阈值触发持久化
TEST(LsmKvStoreTest, StorageThreshold) {
  LsmKvStore* store = new LsmKvStore("/home/rings/LSMTree/wal", 5, 5);
  // 插入足够多的数据触发持久化
  for (int i = 0; i < 15; ++i) {
    store->set("key" + std::to_string(i),
               "value" + std::to_string(i));
  }

  // 验证所有数据仍然可访问
  for (int i = 0; i < 15; ++i) {
    EXPECT_EQ(store->get("key" + std::to_string(i)),
              "value" + std::to_string(i));
  }
}

// 测试持久化和恢复
TEST(LsmKvStoreTest, PersistenceAndRecovery) {
  LsmKvStore* store = new LsmKvStore("/home/rings/LSMTree/wal", 5, 5);
  // 写入一些数据
  for (int i = 0; i < 28; ++i) {
    store->set("persist_key" + std::to_string(i),
               "persist_value" + std::to_string(i));
  }

  sleep(3);
  // 销毁当前存储实例
  delete store;

  // 创建新的实例，应该能够从磁盘恢复数据
  store = new LsmKvStore("/home/rings/LSMTree/wal", 5, 5);

  // 析构时会丢失一部分数据，并且无法恢复
  // 验证数据是否正确恢复
  for (int i = 0; i < 20; ++i) {
    EXPECT_EQ(store->get("persist_key" + std::to_string(i)),
              "persist_value" + std::to_string(i));
  }
}

// 测试大量数据
TEST(LsmKvStoreTest, LargeDataset) {
  LsmKvStore* store = new LsmKvStore("/home/rings/LSMTree/wal", 5, 5);
  const int numEntries = 1000;
  std::vector<std::pair<std::string, std::string>> testData;

  // 准备测试数据
  testData.reserve(numEntries);
  for (int i = 0; i < numEntries; ++i) {
    std::string key = "large_key_" + std::to_string(i);
    std::string value = randomString(100);
    testData.emplace_back(key, value);
  }

  // 写入数据
  for (const auto& [key, value] : testData) {
    store->set(key, value);
  }

  // 验证数据
  for (const auto& [key, value] : testData) {
    EXPECT_EQ(store->get(key), value);
  }

  // 随机删除一些数据
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, numEntries - 1);

  for (int i = 0; i < numEntries / 2; ++i) {
    int index = dis(gen);
    store->remove(testData[index].first);
    EXPECT_EQ(store->get(testData[index].first), "");
  }
}
