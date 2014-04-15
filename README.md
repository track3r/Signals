Signals
=======

Basic, easy to use, cross platform,  signal class for C++11
``` cpp
void SignalExample()
{
    Signal<int> signal;
    
    auto func = [](int i){std::cout << i * 2 << std::endl;};
    auto func2 = [](int i){std::cout << i << std::endl;};
    
    auto connection = signal.Connect(func);
    
    {
        auto scoped = ScopedConnection(signal.Connect(func2));
        signal(1);
    }
    
    signal(2);
}
```    
Output:

    1
    2
    4
