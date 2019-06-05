#ifndef _THECURE_SINGLETON_H_
#define _THECURE_SINGLETON_H_


template<class T>
class Singleton {
private:
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton&) = delete;
protected:
    Singleton(){}
    ~Singleton(){}
    static T* ms_inst;
public:
    static T* get_inst(){
        if(!Singleton::ms_inst){
            Singleton::ms_inst = new T();
        }
        return Singleton::ms_inst;
    }
    static void release(){
        if(Singleton::ms_inst){
            delete Singleton::ms_inst;
            Singleton::ms_inst = nullptr;
        }
    }
};

template<class T>
T* Singleton<T>::ms_inst = nullptr;

#endif
