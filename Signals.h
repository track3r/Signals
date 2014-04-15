//
//  Signal.h
//  BattleForThrone
//
//  Created by Alexandr Dem'yanenko on 8/28/13.
//
//

#ifndef __BattleForThrone__Signal__
#define __BattleForThrone__Signal__

#include <vector>
#include <functional>
#include <memory>
#include <unordered_map>

class BaseSignal;

struct SignalSharedData
{
    BaseSignal* parent = nullptr;
};

typedef std::shared_ptr<SignalSharedData>  SignalSharedDataPointer;

class SignalConnection
{
public:
    SignalConnection(){}
    SignalConnection(SignalConnection&& other);
    SignalConnection(SignalSharedDataPointer ptr, size_t id);
    const SignalConnection& operator=(SignalConnection&& other);
    void Disconnect();
    
private:
    SignalSharedDataPointer m_data;
    size_t m_id = 0;
};

class ScopedConnection
{
public:
    ScopedConnection(){}
    ScopedConnection(SignalConnection&& connection);
    ScopedConnection(ScopedConnection&& connection);
    const ScopedConnection& operator=(ScopedConnection&& other);
    ~ScopedConnection();
    void Disconnect();

private:
    SignalConnection m_connection;
};

class BaseSignal
{
public:
    virtual ~BaseSignal();
    void Disconnect(size_t id);
    
protected:
    BaseSignal();
    size_t GenId();
    SignalSharedDataPointer GetShared();
    void Gc();
    virtual void _Disconnect(size_t id) = 0;
    
private:
    size_t m_counter = 0;
    SignalSharedDataPointer m_shared;
    std::vector<size_t> m_garbage;
};


template<typename... Args>
class Signal : public BaseSignal
{
public:
    SignalConnection Connect(const std::function<void(Args...)>& function)
    {
        const auto id = GenId();
        m_list[id] = function;
        
        return SignalConnection(GetShared(), id);
    }
    void _Disconnect(size_t id) override
    {
        m_list.erase(id);
    }
    
    void operator()(Args... args)
    {
        Call(args...);
    }
    
    void Call(Args... args)
    {
        Gc();
        for(const auto& it : m_list)
        {
            it.second(args...);
        }
    }
private:
    std::unordered_map<size_t, std::function<void(Args...)>> m_list;
};

#endif /* defined(__BattleForThrone__Signal__) */
