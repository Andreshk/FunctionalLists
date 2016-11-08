# FunctionalLists
C++14 proof-of-concept implementation of immutable functional style lists.

Списъкът `andi::list<...>` съдържа само един умен указател `std::shared_ptr<...>` към главата си, който указател считаме за достатъчно малък, че да копираме списъци смело насам-натам, вместо да ги взимаме по референция. На практика се очаква `sizeof(andi::list<...>) == sizeof(std::shared_ptr<...>) == 2*sizeof(void*)` и копирането на списък да представлява копиране на два указателя + инкрементация на атомарен брояч, т.е. 3 елементарни операции.

Имплементацията може да се подобри със използване на свой тип "умен" броящ референции указател, който да комбинира данните, към които сочи, с т.нар. контролен блок. Този контролен блок ще съдържа само един брояч и ще имаме, че `sizeof(andi::list<...>) == sizeof(special_ptr) == sizeof(void*)` и всяко копиране на списък ще е **точно 2** елементарни операции (съотв. *инструкции*):
```c++
template<class T>
class list
{
  struct node;
  class special_ptr
  {
    node* ptr;
    //...
  };
  struct node
  {
    std::atomic<size_t> refCount;
    const T value;
    special_ptr next;
    //...
  };
  
  special_ptr head;
  //...
};
```
... но това е в to-do :D
