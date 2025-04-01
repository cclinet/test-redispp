#include <bs_thread_pool.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <sw/redis++/redis++.h>
using namespace sw::redis;

int main() {
  try {
    // 配置Redis连接和连接池
    ConnectionOptions conn_opts;
    conn_opts.host = "127.0.0.1";
    conn_opts.port = 6379;
    conn_opts.socket_timeout = std::chrono::milliseconds(200);

    ConnectionPoolOptions pool_opts;
    pool_opts.size = 10;

    Redis redis(conn_opts, pool_opts);

    BS::thread_pool pool(4);

    for (int i = 10; i < 100; ++i) {
      // 提交异步SET任务
      auto set_task = pool.submit_task(
          [&redis, i]() { redis.set("key", std::to_string(i)); });

      // 等待SET完成
      set_task.get();

      // 提交异步GET任务
      auto get_task = pool.submit_task([&redis]() { return redis.get("key"); });

      // 获取并处理结果
      auto value = get_task.get();
      if (value) {
        std::cout << "Value: " << *value << std::endl;
      } else {
        std::cout << "Key does not exist." << std::endl;
      }
    }
  } catch (const Error &e) {
    std::cerr << "Redis error: " << e.what() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}