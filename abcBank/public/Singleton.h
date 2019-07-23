#pragma once

namespace PUBLIC
{


// 通过Singleton<T>::getInstance() 来获得这个单例
template <typename T>
class Singleton
{
public:
  static T & getInstance()
  {
    static T instance;
    return instance;
  }
private:
  Singleton();
  ~Singleton();
};
}// end namespace PUBLIC
