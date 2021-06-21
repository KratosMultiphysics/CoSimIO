class Singleton
{
    int count_;

public:
    Singleton( ) : count_( 0 ) { }

    void increment( )
    {
        count_++;
    }

    int count( )
    {
        return count_;
    }
};

extern Singleton singleton;

#define DEFINE_SINGLETON_MAIN( ) Singleton singleton

