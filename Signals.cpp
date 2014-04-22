//
//  Signal.cpp
//
//  Created by Alexandr Dem'yanenko on 8/28/13.
//
//

#include "Signals.h"

SignalConnection::SignalConnection(SignalConnection&& other)
{
    m_data.swap(other.m_data);
    std::swap(m_id, other.m_id);
}

SignalConnection::SignalConnection(SignalSharedDataPointer ptr, size_t id)
    :m_data(ptr)
    ,m_id(id)
{

}

void SignalConnection::Disconnect()
{
    if(m_data)
    {
        if(m_data->parent != nullptr)
        {
            m_data->parent->Disconnect(m_id);
            m_id = 0;
            m_data.reset();
        }
    }
}

const SignalConnection& SignalConnection::operator=(SignalConnection&& other)
{
    Disconnect();
    m_data = other.m_data;
    other.m_data.reset();

    m_id = other.m_id;
    other.m_id = 0;

    return *this;
}

ScopedConnection::ScopedConnection(SignalConnection&& connection)
    :m_connection(std::move(connection))
{

}

ScopedConnection::ScopedConnection(ScopedConnection&& connection)
    :m_connection(std::move(connection.m_connection))
{

}

void ScopedConnection::Disconnect()
{
    m_connection.Disconnect();
}

const ScopedConnection& ScopedConnection::operator=(ScopedConnection&& other)
{
    m_connection.Disconnect();
    m_connection = std::move(other.m_connection);
    return *this;
}

ScopedConnection::~ScopedConnection()
{
    m_connection.Disconnect();
}

BaseSignal::~BaseSignal()
{
    m_shared->parent = nullptr;
}

BaseSignal::BaseSignal()
{
    m_shared = std::make_shared<SignalSharedData>();
    m_shared->parent = this;
}

void BaseSignal::Disconnect(size_t id)
{
    m_garbage.push_back(id);
}

void BaseSignal::Gc()
{
    for(auto it : m_garbage)
    {
        _Disconnect(it);
    }

    m_garbage.clear();
}


size_t BaseSignal::GenId()
{
    return ++m_counter;
}


SignalSharedDataPointer BaseSignal::GetShared()
{
    return m_shared;
}