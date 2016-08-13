#pragma once
#include <string>
#include <vector>
#include <memory>
#include <time.h>



bool StringHasSuffix(const std::string &str, const std::string &suffix);
bool StringStartsWith(const std::string& str, const std::string& prefix);
std::vector<std::string> StringSplit(const std::string &s, char delim);
std::vector<std::string> StringSplit(const std::string &s, const std::string& delim);

std::string StringReplace(std::string subject, const std::string& search,
                          const std::string& replace);


void StringUpper(std::string& str);


template<typename ... Args>
std::string StringFormat( const std::string& format, Args ... args )
{
    size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


class TBaseException : public std::exception
{
public:
   TBaseException(const std::string& message) : Message(message) {}
   virtual ~TBaseException() throw() {}
   virtual const char* what() const throw() {return Message.c_str();}
protected:
   std::string Message;
};

class TMaybeNotDefined_ {};
#define NotDefinedMaybe TMaybeNotDefined_();

template<typename T>
class TMaybe
{
public:
    inline TMaybe(const T& val) {Init(val);}
    inline TMaybe<T>& operator=(const T& val) { Init(val); return *this; }

    inline TMaybe() {};
    inline TMaybe(const TMaybeNotDefined_&) {};


    inline bool Defined() const { return !!Data_; }
    inline bool Empty() const { return !Data_(); }
    bool operator !() const { return Empty(); }


    const T* Get() const { return Defined() ? Data() : nullptr; }
    T* Get() { return Defined() ? Data() : nullptr; }

    inline const T& GetRef() const { EnsureDefined(); return *Data(); }
    inline T& GetRef() { EnsureDefined();  return *Data(); }

    inline const T& operator*() const { return GetRef();}
    inline T& operator*() { return GetRef(); }

    const T* operator->() const {  return &GetRef();}
    inline T* operator->() { return &GetRef(); }


    inline void EnsureDefined() const {
        if (!Defined()) {
            throw TBaseException("TMaybe is not defined");
        }
    }


private:
    inline void Init(const T& val) { Data_.reset(new T(val)); };
    inline T* Data() const { return Data_.get(); };


    std::unique_ptr<T> Data_;
};



bool TopicMatchesSub(const char * pattern, const char * topic);
bool TopicMatchesSub(const std::string & pattern, const char * topic);
bool TopicMatchesSub(const std::string & pattern, const std::string & topic);

