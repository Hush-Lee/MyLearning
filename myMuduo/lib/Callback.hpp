#pragma once 
#include <memory>
#include <functional>
class TcpConnection;
class Buffer;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
using TimerCallback=std::function<void()>;
using ConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
using CloseCallback=std::function<void(const TcpConnectionPtr&)>;
using WriteCompleteCallback=std::function<void(const TcpConnectionPtr&)>;
using HighWaterMarkCallback=std::function<void(const TcpConnectionPtr&,size_t)>;
using MessageCallback=std::function<void(const TcpConnectionPtr&,Buffer*,Timestamp)>;
using DefalutConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
using DefalutMessageCallback=std::function<void(const TcpConnectionPtr&,Buffer*,Timestamp)>;
