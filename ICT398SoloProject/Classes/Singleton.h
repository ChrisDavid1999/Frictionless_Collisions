#pragma once



    template<class T>
    class singleton
    {
    public:
        
        static T& getInstance()
        {
            static T instance;
            return instance;
        }

    private:
        singleton() {}
        
        ~singleton() {}
        
        singleton(singleton const&) = delete;

        singleton& operator=(singleton const&) = delete;
    };
