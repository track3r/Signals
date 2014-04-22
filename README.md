Signals
=======

Basic, easy to use, cross platform,  signal class for C++11
* no need to inherit anything
* scoped signal connection - automatically unsubscribe when leaving scope (RAI)
* lambdas and std::function as signal handlers
* useful if you don't want to add boost to your project (mobile platforms, etc)

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
