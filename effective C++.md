# effective C++

## 1.视C++为一个语言联邦

1. C：C++实在C语言的基础上发展而来的。

2. Object-Oriented C++：这是C++中不同于C的部分，这里主要指面向对象。

3. Template C++：C++中的泛型编程。

4. STL：这是一个标准模板库，它用模板实现了很多容器、迭代器和算法，使用STL往往事半功倍。

## 2. 尽量以const、enum、inline替换#define

* 以编译器代替预处理
* 例如，你使用预处理定义了圆周率，在预处理时， 所有使用PI的地方都将被替换，之后编译器在编译时从未看到过PI。这时如果遇到错误，报错时给出的是301415926，而不是PI，因为PI从未进入到符号表，这将**导致错误难以理解。**
* 一个枚举类型（enumerated type）的数值可充当static int使用
* enum hack（对enum访问地址是不合法的）的行为某方面比较像#define而不像const，有时候这正是你想要的，例如取一个const的地址时合法的，但取一个enum的地质就不合法，而取一个#define的地质通常也不合法。如果你不想让别人获得一个pointer或referrnce指向你的某个整数常量，enum可以帮助你实现这个约束。
* 当用常量替换#define时，有两点要注意

  1. 替换字符串时，要定义成常量指针，而不是指向常量的指针，例如定义一个名字

  2. 专属于class作用域的常量。专属于class的常量将这个常量限定在class的作用域内，而#define定义的常量没有作用域的限制，一旦在某一处有个宏定义，在其后面都有效（除非#undef）。

## 3. 尽可能使用const

* 使用const修饰指针或指针指向的对象时要注意：

  1. const T * point或T const* point表示point指向的对象是常量。

  2. T * const point;表示point指针是常量指针。

* const成员函数

  * const成员函数的目的是确认该函数可以用到const对象上，const成员函数使得:

    1. class接口更加容易理解，确认哪些接口可以修改class成员。

    2. 使操作const对象成为可能。
       1. const对象只能调用const成员函数；但是非const对象既可以调用普通成员函数，也可以调用const成员函数
       2. 这是因为this指针可以转换为const this，但是const this不能转换为非const this。

  * 一个函数是不是const是可以被重载的。

## 4. 确定对象使用前已先被初始化

* 如果使用未初始化的对象，可能会导致不确定的行为。所以，在使用对象之前，一定要将其初始化。

   对于内置的数据类型

* 对于内置类型以外的数据类型，则通过构造函数完成初始化。在构造函数中完成初始化时，要区分赋值和初始化。

* 在**构造函数体内的是赋值，在初始化列表（初始化顺序要和声明顺序一致）中的才是初始化。**

* 初始化效率往往高于赋值。赋值是先定义变量，在定义的时候可能已经调用了变量的构造函数，之后赋值是调用了赋值操作符。而初始化是直接调用了复制构造函数。

* 在一些情况下必须使用初始化方式，有些变量在定义时就要求初始化，例如const和引用。

* 在C++的类继承中，基类先于派生类初始化。

* “**不同编译单元内定义的non-local static对象**”的初始化：

  * static对象的生命周期为从其构造出来到程序结束，堆和栈上均不满足，存储与全局数据区
  * static对象包括全局（global）对象（虽然没有用static修饰）、定义在namespace作用域内、在class、函数内、以及在file作用域内声明的static对象。
  * 函数体内的static对象是local static对象，其他static对象是non-local static对象。
  * 编译单元是指产出单一目标文件的源码，是源码文件加上其包含的头文件（#include files）。现在有至少两个源码文件，每一个里面至少含有一个non-local static对象。如果一个编译单元的non-local static对象初始化使用了另外一个编译单元的non-local static对象，它所用到的这个对象可能尚未被初始化，因为C++对“定义在不同编译单元内的non-local static对象”初始化次序无明确规定。
  * 一个改动即可消除上述问题。既然static对象只有一份拷贝，且只初始化一次，很容易想到“单例模式”。使用local static对象，**首次使用时初始化，返回其引用即可**（local static声明周期是整个程序），以后再使用无需再次初始化。

* 为了避免使用未初始化的对象，要做三件事：

  1. 手动初始化non-member对象。
  2. 使用初始化列表初始化member对象。
  3. 消除初始化次序的不确定性。 ﻿

## 5. 了解C++默认编写并调用哪些函数

*  加入编写一个空类，那么经过编译之后，会有默认构造函数、复制构造函数、赋值操作符和析构函数。这些函数都是**public**且**inline**。
* 如果没有构造函数，编译器将会创建一个默认构造函数，由它来调用基类和non-static成员变量的构造函数。
* 析构函数是否是虚函数，如果没基类，那么默认是non-virtual。如果有继承基类，析构函数会调用基类和non-static成员变量的析构函数。
* 两个成员变量，一个是引用：初始化后不能更改，一个是常量：也是初始化后不能更改。
  编译器合成赋值操作符后，在调用赋值操作符时能更改这两个变量的值吗？显然不可以。编译器会拒绝编译重新赋值的动作。
  如果自己编写赋值操作符，且合法，编译不会出错。
* 如果其基类的赋值操作符是private，编译器将拒绝为派生类合成赋值操作符。

## 6. 若不想使用编译器自动生成的函数，就该明确拒绝

* 如果复制构造函数和赋值操作符不应该使用，但编译器默认帮我们合成了这两个函数。而且编译器合成的函数都访问都是public。如果我们想阻止使用这两个函数，我们可以**声明它们为private**。

* 将错误提前到编译阶段是最好的，毕竟越早出现错误越好。可以通过继承来实现，在这设计一个不可以复制的类 ：

  `class U`

  `{`

  `public:`

  ​	`U() {};`

  ​	`~U() {};`

  `private:`

  ​	`U(const U&):U(u);`

  ​	`U& operator= (const U&):`

  `};`

* 让其他类来继承此类即可

  `class T ：public U`

  `{`

  ​	`....`

  `}`

## 7. 为多态基类声明virtual析构函数

* 在创建有层次的类时，往往要把基类的析构函数声明为虚函数。
  这是因为在使用这些类时，往往是**通过基类指针或者引用使用**的（类的实例在堆上），如果在析构对象时，析构函数不是虚函数，将不会调用当前指针指向对象的析构函数。
  同理可知，要实现多态的函数，在基类也要声明为虚函数。

*  当一个类不用做基类时，如果把其析构函数声明为虚函数是个馊主意。因为**虚函数是通过虚函数表调用的**，在调用虚函数时多一步指针操作；除此之外，其对象占用的内存空间也会多一个虚函指针。

* 一个类不含虚函数，一般不适合做基类。这时如果通过基类指针使用，在delete时，可能会造成内存泄漏。

  要记住，**STL中的容器都没有虚的析构函数**，即STL容器均不能作为基类。

* **纯虚函数就是没有函数体，同时在定义的时候，其函数名后面要加上“= 0”**。定义纯虚函数的目的在于，使派生类仅仅只是继承函数的接口。一个含有纯虚函数的类叫做抽象类，不能被实例化。即不想让基类实例化对象，仅允许通过继承的方式来作为该基类的接口。

* 一个析构函数为纯虚函数的抽象类：

  `class U`

  `{`

  `public:`

  ​	`virtual ~U()=0;`

  `}`

* 如果把它当作基类，会有问题，因为其析构函数只有声明。析构函数的调用时从派生类到基类，如果没定义，会发生链接错误，这是要定义个空的析构函数: 

  `U :: ~U () {};`

## 8. 别让异常逃离析构函数

* 当容器销毁时会调用析构函数，这时如果析构函数抛出异常，容器中剩余的元素还是应该被销毁，否则可能会有内存泄露。这时如果继续销毁其他元素，又出现异常的话，会同时存在两个异常。两个异常同时存在会导致不明确的行为。

* 析构函数绝对不要抛出异常。如果一个被析构函数调用的函数可能抛出异常,析构函数应该捕捉该异常，然后吞下它们(不传播)或结束程序。
   如果使用者需要对某个操作函数运行期间抛出的异常做出反应，那么类应该提供一个普通函数(而非在析构函数中）执行该操作。

*  有两个方法可以避免传播异常：

  1. 如果抛出异常，就终止这个程序。

     通常通过调用abort（exit和abort都是用来终止程序的函数）完成。通常当“析构期间发生错误”后无法继续执行，终止程序是个合理的选项。毕竟这样可以确保异常从析构函数传播出去。

  2. 吞下这个异常，try-catch语句记录概念异常。吞下这个异常不是个好主意，它压制了某些失败动作的重要信息。

* 一个比较好的策略是重新设计类接口，使客户能对可能出现的异常做出反应。例如该类可以自己提供一个函数，可以给客户一个机会来处理“因该操作而发生的异常”。

  把调用函数的责任从析构函数手上转移到客户手上，这样会多出一个保险。客户自己调用close函数并不会给它们带来负担，而且给了他们一个处理错误的机会，否则他们没机会响应。

## 9. 绝不在构造和析构过程中调用virtual函数

* 在构造或析构函数中调用virtual函数不会呈现出多态。
* 构造时，先构造基类后构造派生类
* 析构时，先析构派生类后析构基类
* 在构造和析构函数中调用virtual函数并不会指向所需版本

## 10. 令operator=返回一个reference to *this

* 赋值操作符运算是由右向左运算的。
*  若要实现连锁赋值，操作符必须**返回一个指向操作符左侧的实参**（以引用的方式）。
* 其实，如果operator=不返回一个引用，返回一个临时对象，照样可以实现连锁赋值。但这个临时对象的构建会调用    拷贝构造函数。
  * 这时将会调用拷贝构造函数，返回一个临时变量，临时变量再给左侧变量赋值，多出了一步拷贝，浪费资源
*  operator=是改变左侧操作数的，与其类似的operator+=、operator-=等改变左侧操作符的运算，都应该返回引用。这是一个协议，应该去遵守。

 ## 11. 在operator=中实现“自我赋值”

* 自我赋值是指对象给自己赋值。

* `class Widget{`

  `public:`

  ​	`Widget& operator=(const Widget &rhs){`

  ​		`delete p;`

  ​		`p=new int(rhs->p);`

  ​		`return *this;`

  ​	`}`

  ​	`int *p;`

  `}`

* 如果上面代码自我赋值，在使用指针p之前已经将其释放掉了。 

   防止这种问题发生的办法是“证同测试”，在删除前判断是不是自我赋值。

* `class Widget{`

  `public:`

  ​	`Widget& operator=(const Widget &rhs){`

  ​		`if(this==&rhs)`

  ​			`return *this;`

  ​		`delete p;`

  ​		`p=new int(rhs->p);`

  ​		`return *this;`

  ​	`}`

  ​	`int *p;`

  `}`

* 这个版本的operator=可以解决自我赋值的问题。但是还有个问题：异常安全。如果delete p成功，而p=new int(rhs.)失败会发生什么？ 

  这时，widget对象会持有一个指针，这个指针指向了被释放的内存。下面方法可以实现异常安全。 

* `class Widget{`

  `public:`

  ​	`Widget& operator=(const Widget &rhs){`

  ​		`int temp=p;`

  ​		`p=new int(rhs->p);`

  ​		`delete temp;`

  ​		`return *this;`

  ​	`}`

  ​	`int *p;`

  `}`

## 12. 复制对象时勿忘记每一部分

*   在一个类中，有两个函数可以给复制对象：复制构造函数和赋值操作符，统称为copying函数。如果我们自己不编写者两个函数，编译器会帮我们实现这两个函数，编译器生成的版本会将对象的所有成员变量做一份拷贝。编译器生成的copying函数的做法通常是浅拷贝。

  如果我们自己实现了copying函数，编译器就不再帮我们实现。但是编译器不会帮我们检查copying函数是否给对象的每一个变量都赋值。 

* 一旦给类添加变量，那么自己编写的copying函数也要修改，因为编译器不会提醒你。

*   在派生类中构造函数没有初始化的基类部分是通过基类默认构造函数初始化的（没有默认构造函数就会报错）。 

   但是在copy assignment操作符中，不会调用基类的默认构造函数，因为copy assignment只是给对象重新赋值，不是初始化，因此不会调用基类的构造函数，除非我们显示调用，即在赋值运算符函数中调用基类的拷贝构造函数。

* 复制构造函数和赋值操作符有类似的代码。但是这两个函数不能相互调用。

  复制构造函数是构造一个不存在的对象，而赋值操作符是给一个存在的对象重新赋值。

## 13. 以对象管理资源

* 为了确保资源总是在出了作用域内被释放，可以用对象来控制资源。把对象建到栈上，当对象出了作用域时自然会析构，在这个析构函数释放资源。这就是对象管理资源的机制。

* 上面以资源管理对象的关键在于两点

  1. RAII：资源获取即初始化(resource acquisition is initialization)。获取资源后立即放进对象内进行管理。

  2. 管理对象运用析构函数确保资源释放。管理对象是开辟在栈上面的，离开作用域系统会自动释放管理对象，自然会调用管理对象的析构函数。

## 14. 在资源管理类中小心coping行为

* 在条款13中，讲述了资源获取即初始化（resource acquisition is initialization;RAII）的资源管理方法，并介绍了智能指针auto_prt和shared_ptr指针。上述方法管理的资源都是开辟在堆上（heap-based），但是并不是所有资源都是开辟在堆上，有时候我们需要自己建立资源管理类。

  `class Lock{`

  `public:`

  ​	`explicit Lock(Mutex *mu) : mutexPtr(mu){`

  ​		`lock(mutexPtr);`

  ​	`}`

  ​	`~Lock(){`

  ​		`unlock(mutexPtr);`

  ​	`}`

  `private:`

  ​	`Mutex *mutexPtr;`

  `}`

* 当一个RAII对象被复制，通常会有几种做法：

  1. 禁止复制

     许多情况下，复制RAII对象并不合理。例如Lock类，这时候便可以禁止复制，只需将coping函数设为私有

  2. 对管理资源使用引用计数法。

     有时候我们希望保持资源直到最后一个使用者。这时RAII对象复制时，应该将持有资源的引用计数器加一。例如shared_ptr。

     通常只需要在RAII  class中包含一个shared_ptr成员变量便可实现引用计数的方法。如果上述Lock类使用引用计数器的话，只需把mutexPrt类型从Mutex*变为shared<Mutex>即可。但shared_ptr默认是当引用计数为0时，删除多指向对象，这不是我们想要的，我们想要的是调用unlock函数。幸运的是在shared_ptr中允许指定“删除器”，即引用计数为0时调用的函数。

     `share_ptr<Mutex> mutexPtr;`

  3. 复制底部资源。
     使用资源管理类的目的是保证不需要这个资源时能正确释放。如果这种资源可以任意复制，我们只需编写好适当的copying函数即可，确保复制时是深度复制。
     C++中的string类，内部是指向heap的指针。当string复制时，底层的指针指向的内容都会多出一份拷贝。

  4. 转移底层资源的拥有权。
     有时候资源的拥有权只能给一个对象，这时候当资源复制时，就需要剥夺原RAII类对该资源的拥有权。在C++11新标准中的std::move便是这个功能。可以把一个左值转换为一个右值。

     copying函数如果你不编写，编译器会帮你合成。要记住的是不论是自己编写还是编译器合成，都要符合自己资源管理类的需要。

## 15. 在资源管理类中提供对原始资源的访问

* 通常我们使用资源管理类来屏蔽原始资源，对抗内存泄露等问题，避免使用原始资源（raw resources）。

  但是在现实中并不是这样，许多APIs要求使用原始资源。

* 如果一个API是这样的

  `int dayheld(const Investment *pi);`

* 显然是无法使用shared_ptr对象的。这时候资源管理类需要一个函数，将管理的原始资源暴露出来（即返回原始资源的直接访问方法）。 
  shared_ptr和auto_ptr都提供一个get函数，用于执行这样的显示转换。这时如果在调用上面的API时： 

  `dayheld(pInv.get());`

* 为了使智能指针使用起来像普通指针一样，它们要重载**指针取值操作符**（operator **->** 和operator ***** )，它们允许转换至底部原始指针。 

* 有时候我们必须使用RAII class内的原始资源。

  `FontHandle getFont();`

  `void releaseFont(FontHandle fh);`

  `class Font{`

  `public:`

  ​	`explicit Font(FontHandle fh) : f(fh){};`

  ​	`~Font(){ releaseFont(f); };`

  `private:`

  ​	`FontHandle f;`

  `};`

* 如果有大量的API，它们要求处理的是FontHandle，那么将Font转换为FontHandle是一个比较频繁的需求。

​	1. 这时可以提供一个显示的转换函数，像shared_ptr中get那样 

​		`FontHandle get() const { return f; };`
* 如果需要使用FontHandle，直接调用get函数即可。 
  如果到处都要使用get函数，这样看起来很难受，也增加了内存泄露的可能，因为我们把原始资源返回给其他API了。 

2. 另外一种方法就是提供**隐式转换：`operator 目标类型() {return 源类型;};`**

   `operator FontHandle() const { return f; };`

* 那么在实际中，是使用显示get这样的转换呢，还是使用隐式转换？

  * 答案是取决于RAII class被设计用来执行那么工作，在哪些情况下使用。通常来说，**get比较受欢迎，因为它避免了隐式转换带了的问题**。
  * 现在看来，RAII class内的返回资源的函数和封装资源之间有矛盾。的确是这样，但这样不是什么灾难。**RAII class不是为了封装资源，而是为确保资源释放。**当然可以在这个基本功能之上再加上一层封装，但是通常不是必要的。但是也有些RAII class结合十分松散的底层资源封装，以获得真正的封装实现。

  * 例如shared_ptr将引用计数器封装起来，但是外界很容易访问其所内含的原始指针。它**隐藏了客户看不到的部分，但是具备客户需要的东西**。良好的class就应该这样。 

## 16. 成对使用new和delete时要采取型相同形式

* 看下面一段代码：

  `string *stringArray=new string[100];`

  `delete stringArray;`

* 程序使用了new开辟内存，然后使用delete释放内存，貌似没有什么问题。但是有某样东西完全错误：程序有不明确行为。stringArray内含有100个string对象，但是delete只是删除了一个，剩余的99个没有删除，它们的析构函数没有被调用。 

* 当使用new（即用new动态创建一个对象），有两步。第一步，开辟内存（通过operator new）。第二步，在开辟的内存处调用构造函数。

  同样使用delete时也有两步，第一步调用析构函数，第二步释放内存（通过operator delete）。当使用new时，会指定有多少个对象被创建，使用delete时也需要指定有多少对象被删除（有多少对象的析构函数被调用）。

* 这个问题可以再简单一些：即被删除的那个指针，所指的是单一对象还是数组。单一对象的内存布局就是单个对象，而数组还要多出一个参数：多少个单一对象（即数组大小）。通常在数组指针（第一个指针，即指向数组头的指针）的前一个字节保存了数组的大小，以前面例子为例：*((int *)stringArray-1)的值为100。但使用delete[]时，会取得这个值，然后依次在头指针后面调用delete。

* 一个非数组指针，对它调用delete[]的结果是未定义的

* **如果使用new开辟内存，就使用delete释放。如果使用new[]开辟内存，就使用delete[]释放。**

* 在编写含有指针的class时，上面规则尤为重要。因为析构函数只有一个，但是构造函数可以有多个。构造函数可以使用new或new[]来初始化指针，但是析构函数只能使用一种方法来释放delete/delete[]。

* 这个规则对于喜欢使用typedef的人也很重要，因为typedef的作者要说清楚，当程序员用new创建typedef类型对象时，应该使用delete/delete[]释放。

  `typedef string AddressLines[4];`

  `string *val=new AddressLines;`

* 那么在释放时，就应该是delete[]，而不是delete了。 

  为了避免这样的错误，最好尽量不使用对数组做typedef动作。在C++的STL中有string、vector等templates，可以将数组需求降至几乎为零。

## 17. 以独立语句将newed对象置入智能指针

* 在使用智能指针时，应该用独立的语句把新创建的对象指针放入智能指针，否则可能会造成内存泄露。

* 假设有个函数了处理某个程序，它有两个参数，一个是动态分配的Widget，另一个是优先级。

  `int processWidget(shared_ptr<Widget> pw,int priority);`

  `int priority(); //该函数可以获取优先级`

  `processWidget(shared_ptr<Widget> (new Widget),priority()); //调用processWidget`

* 虽然上面使用了对象管理资源，但是却可能会有资源泄露。

  编译器在产出processWidget之前，必须先核算即将被传进去的各个参数。上面第二个参数是对函数priority()的调用；第一个参数确实有两部分组成，已不是是执行new Widget表达式，另一部分是执行shared_ptr构造函数。所以在调用processWidget之前有三件事：

  1、执行priority()函数

  2、执行new Widget

  3、执行shared_ptr构造函数

  C++编译器会以什么样的次序来完成这些事情呢？弹性很大。在Java和C#中，总是以特定的次序来完成这样函数参数的计算，但在C++中却不一定。唯一可以确定的是new Widget在shared_ptr之前调用。但是函数priority排在第几却不一定。假设排在第二，那么顺序就是：1、执行new Widget。2、执行函数priority()。3执行shared_ptr构造函数。

  如果对函数priority()调用出现异常，那么new Widget返回的指针还没来得及放入shared_ptr中。这样会造成内存泄露。

* 这是因为在创建资源（new Widget)和把资源放置到资源管理对象两个操作之间发送异常。避免这个问题很简单：将创建Widget和将Widget放入一个智能指针这两步不要分开

  `shared_ptr<Widget> pw(new Widget);`

  `processWidget(pw,priority());`

## 18. 让接口容易被使用，不容易被误用

* 接口是客户和你的代码交换的唯一手段。如果客户正确使用你开发的接口，那自然很好；但是如果你的接口被误用，你也要负一部分责任。理想上，**如果客户使用了某个接口却没有获得他想要的行为，那么不应该编译通过；如果编译通过了，那么客户就应该得到他想要的行为。**

  欲开发一个“容易被正确使用，不容易被误用”的接口，首先要考虑客户可能会出现什么样的错误。假如设计一个class来表示日期，那么它的构造函数应该怎么设计？

* 以一个日期类作为例子

  `class Date{`

  `public:`

  ​	`void Date(int month,int day,year);`

  ​	`...`

  `};`

  这个接口看似是正确的，但客户可能会犯下错误

  `Data d(30,3,1995); //月和日搞混了`

  `Data d(2,30,1995); //二月没有30天`

  像这样客户可能错误输入的错误可以通过引入新的数据类型来预防

  `struct Day{`

  `explicit Day(int d):val(d) {}`

  `int val;`

  `};`

  `struct Month{`

  `explicit Month(int m):val(m) {}`

  `int val;`

  `};`

  `struct Year{`

  `explicit Year(int y):val(y) {}`

  `int val;`

  `};`

  再重写Date的构造函数

  `void Date(const Month& m,const Day& d,const Year& y);`	 

  这样在使用起来，如果年月日的顺序不对，编译器会报错。但是这样还是无法避免客户端使用时的第二个错误。

  在参数类型定位会后，就可以限制其值了。一年只有12个月份，所以Month应该反映这样的事实。办法之一就是利用enum表现月份，但是enum不具有类型安全，因为enum本质是一个int类型。比较安全的一个办法是**预先定义所有的月份**。

  `class Month{`

  `public:`

  ​	`static Month Jan(){return Month(1);}`

  ​	`static Month Feb(){return Month(2);}`

  ​	`...`

  `private:`

  ​		`explicit Month(int m); //只能在类内使用，防止生成其他月份`

  `}`

  以函数代替对象

* 下面是另一个设计原则：**让types容易被正确使用，不容易被误用**。

  * 除非你有好的理由，否则应该尽量让你的types的行为于内置types保持一致。例如，客户已经知道像int这样的type有些什么样的行为，所以你应该努力让你的types在合情合理的前提下也有相同的表现。
  * 避免于内置类型发生冲突，这样做的目的是**提供行为一致性的接口**。“一致性”最容易导致接口被正确使用。STL容器的接口十分一致（虽然不是完美地一致），这样使得它们非常容易被使用。例如，每个STL容易都有一个名为size的成员函数，来表示当前容器内有多少对象。

  * 任何接口，如果要求客户必须记得做某些事，那么这个接口就有着不正确使用的倾向，因为客户很可能忘记做那件事。

* 总结：

  1. 好的接口容易被正确使用，不容易被误用。

  2. ”促进正确使用“的办法包括接口一致性，以及于内置类型兼容。

  3. ”阻止误用“方法包括建立新类型、限制类型上的操作、束缚对象值，以及消除客户的资源管理责任。

  4. shared_ptr支持特定的删除器。可以防范cross-DLL problem。

## 19. 设计class犹如设计type

* 在面向对象的语言中，定义一个新class时，也就定义了一个新type。

  在开发C++时，许多时间都是在扩张类型系统，这意味着程序员不只是class的设计者，还是type的设计者。重载函数和操作符、内存分配于释放、对象的创建和销毁……这些全都控制在程序员手上。因此应该带着和”语言设计者当初设计语言内置类型时“一样的谨慎来研讨class的设计。

* 在设计新的class时，应该回答一下问题。

  * 新type的对象应该**如何被创建和销毁**？

    这会影响到class的构造函数和析构函数以及内存的分配和释放。

  * 对象**初始化和对象赋值**该有什么样的区别？

    这个是你构造函数和赋值操作符的行为以及它们的差异。不要混淆初始化和赋值，它们对应不同的函数

  * 新type的对象如果被pass by value（**值传递**），意味着什么？

    这决定于你的copy构造函数。

  * 什么是新**type的合法值**？

    对于class的成员变量而言，通常只有某些数据集是有效的。这些数据集决定了你的class要维护的约束条件，也决定了你的某些成员函数进行的错误检查的工作，它也影响函数抛出异常的明细。

  * 新type需要配合某个**继承图系**（inheritance graph）吗？

    如果你继承自某些既有的classes，特别是受到那些classes的virtual或non-virtual的影响。如果你允许其他classes继承你的class，那会影响你所声明的函数，尤其是析构函数（是否为virtual）。

  * 新type需要什么样的**类型转换**？

    你的type生存在其他type之间，彼此之间需要转换行为吗？如果允许类型T1转换为类型T2，那么就必须在class T1内写一个类型转换函数operator T2，或在class T2内写一个non-explicit-one-argument（可被单一实参调用）的构造函数。如果只允许explicit构造函数存在，必须写出专门负责执行转换的函数，且不得为类型转换操作符（type-conversion operators）或non-explicit-one-argument构造函数。条款15有隐式和显示转换的范例。

  * 什么样的**操作符和函数**对此新type而言是合理的？

    这取决于你为class声明哪些函数。其中你的type生存在其他type之间，彼此之间需要转换行为吗？如果允许类型T1转换为类型T2，那么就必须在class T1内写一个类型转换函数operator T2，或在class T2内写一个non-explicit-one-argument（可被单一实参调用）的构造函数。如果只允许explicit构造函数存在，必须写出专门负责执行转换的函数，且不得为类型转换操作符（type-conversion operators）或non-explicit-one-argument构造函数。

    什么样的操作符和函数对此新type而言是一些是member函数，一些不是。

  * 什么样的函数应该被**驳回**？

    即哪些函数应该声明为private。

  * 谁该**取用**新type的成员？

    这个问题帮助你决定哪个成员是public，哪个是private。也帮你解决哪一个class或fuction应该是friends，以及将它们嵌套于另一个是否合理。

  * 什么是新type的“未声明接口”（undeclared interface）？

    它对效率、异常安全性以及资源运用（例如多任务和动态内存）提供何种保证？你在这些方面提供的保证将为你的class实现代码上加上响应约束条件。

  * 你的新type有多么**一般化**？

    也许你并非定义一个新type，而是定义一整个types家族。如果是这样就不应该定义一个新class，而是应该定义一个新的class template。

  * 你**真的需要**一个新type吗？

    如果只是定义新的derived class以便为既有的class添加机能，那么说不定单纯定义一个或多个non-member function或templates，更能达到目标。

## 20. 宁以pass-by-reference-to-const替换pass-by-value

* 在默认情况下，C++函数传递参数是继承C的方式，是值传递（pass by value)。这样传递的都是实际实参的副本，这个副本是通过调用复制构造函数来创建的。

* 有时候创建副本代价非常昂贵，用以下例子说明

  `class Person{`

  `public:`

  ​	`Person();`

  ​	`~Person();`

  `private:`

  ​	`string name;`

  ​	`string address;`

  `};`

  `class Student : public Person{`

  `public:`

  ​	`Student();`

  ​	`~Student();`

  `private:`

  ​	`string schoolName;`

  ​	`string schoolAddress;`

  `};`

  现在考虑一个函数validateStudent，它需要一个Student实参，以pass by value方式传递。

  `bool validateStudent(Student s);`

  `Student plato;`

  `bool platIsok=validateStudent(plato);`

  当函数被调用时，copy构造函数会被调用，用plato构造s。在返回时，s会被析构。那么pass by value的代价就是Student的一次构造和一次析构。但是Student构造和析构时又发生了什么？它内部有两个string对象，所以会有两个string对象的构造和析构。Student继承自Person，又加上Person的构造和析构，Person内又有两个string对象，因此还要加上2个string对象的构造和析构。总共是六次构造和六次析构。

  pass  by value是正确的，但是其效率低下。以pass by reference-to-const方式传递，可以回避所有构造函数和析构函数。

  `bool validateStudent(const Student &s);`

  这种方式传递，没有新对象创建，所以自然没有构造和析构函数的调用。参数中，以const修饰是比较重要的，原先的pass by value，原先的值自然不会被修改。现在以pass by reference方式传递，函数validateStudent内使用的对象和传进来的同同一个对象，为了**防止在函数内修改，加上const限制**。

  以pass by reference方式传递，还可以避免对象切割（slicing）问题。一个**派生类**（derived class）对象以pass by value方式传递，当**被视为一个基类对象（base class）时**，基类对象的copy构造函数会被调用，此时**派生类部分全部被切割掉了，仅仅留下一个base class部分**。

  **在C++编译器的底层，reference往往以指针实现出来，所以pass by reference通常意味着真正传递是指针**。**但是对于内置类型，pass by value往往比pass by reference更高效。所以在使用STL函数和迭代器时，习惯上都被设计出pass by value。当设计迭代器和函数时，设计者有责任查看哪种传递方式更为高效，是否会有切割问题的影响。**这个规则的改变适用于你使用C++的哪一部分。

  通常，内置类型都比较小，因此有人认小型types都适合pass by value，用户自己定义的class亦然。但是对象小并不意味着copy构造函数代价小。许多对象（包括STL容器），内涵的成员只不过是一两个指针，但是复制这种对象时，要复制指针指向的每一样东西，这个代价很可能十分昂贵。

  还有一个理由，某些编译器对待内置类型和用户自定义类型的”态度“截然不同，即使两者有着相同的底层描述。例如，某些编译器拒绝把一个double组成的对象放进缓存器内，但是却乐意在一个正规基础上光秃秃的doubles上这么做。当这种事情发生时，应该以by reference方式传递此对象，因为编译器当然会把指针放进缓存器。

  用户自定义的小型types，可能还会发生变化，将来也许会变大，其内部实现可能会改变，所以用户自定义的小型type在使用pass by value时要慎重。

  一般情况下，**可以假设内置类型和STL迭代器和函数对象以pass by value是代价不昂贵。其他时候最好以pass by reference to const替换掉pass by value。**

## 21. 必须返回对象时，别妄想返回其reference

* 有时候我们会传递一些reference指向不存在的对象。

* 考虑一个用以表现有理数乘积的class。

  `class Rational{`

  `public:` 	

  ​	`Rational(int numerator=0,int denominator=1);`

  `private:`

  ​	`int n,d;`

  ​	`friend const Rational operator* (const Rational &lhs,const Rational &rhs);`

  `};`

  这个版本的operator*用by value的方式返回其计算结果（对象）。这样返回的代价是一个对象的创建+析构+另一个对象的创建。一个对象的创建是指，在这个operator*函数中，**创建一个新对象来保存结果，之后用这个用这个新对象返回，返回时用它初始化另一个对象，之后这个新对象析构**。

  但是如果用by reference方式传递就不会有任何代价。但是reference只是名称，代表一个已经存在的对象，任何时候看到reference都应该问自己，它的另一个名称是什么？如果上述operator*返回一个reference，那么它一定指向一个存在的Rational对象。

  `Rational a(1,2);`

  `Rational b(3,5);`

  `Rational c=a*b;`

  这时返回一个值为3/10的Rational对象。但是这个对象原先并不存在，这时如果返回reference，那么必须在函数operator*内创建这个Rational对象。

  函数创建新对象有两种途径，在stack上或在heap上创建。如果定义local变量，那么就在stack上创建

  `const Rational operator* (const Rational &lhs,const Rational &rhs)`

  `{`

  ​	`Rational result(lhs.n * rhs.n,lhs.d * rhs.d);`

  ​	`return result;`

  `}`

  上面的做法，没有避免调用构造函数，result像任何对象一样由构造函数构造起来。上面还有一个错误：**这个函数返回reference执行result，但是result是个local对象，它在函数退出前被销毁了。**

  考虑在heap上创建对象

  `const Rational operator* (const Rational &lhs,const Rational &rhs)`

  `{`

  ​	`Rational *result=new Rational(lhs.n * rhs.n,lhs.d * rhs.d);`

  ​	`return *result;`

  `}`

  还是要付出一个构造函数的代价，分配的内存将以一个适当的构造函数初始化。现在又面临另一个问题：谁负责给你new出来的对象实施delete？

  在上面的两种做法中（在stack和在heap上创建对象），都因为operator*返回结果调用构造函数而受到惩罚。我们最初的目标是避免如此的构造函数的调用。还有一个办法避免任何构造函数的调用：让operator*返回一个指向在函数内部定义的static Rational对象：

  `const Rational operator* (const Rational &lhs,const Rational &rhs)`

  `{`

  ​	`static Rational result;` 

  ​	`Rational result=new Rational(lhs.n * rhs.n,lhs.d * rhs.d);`

  ​	`return result;`

  `}`

  对该函数的调用

  `bool operator== (const Rational &lhs,const Rational &rhs)`

  `{`

  ​	`Rational a,b,c,d;`

  ​	`if((a * b)==(c * d))`

  ​		`doSomething();`

  ​	`else`

  ​	 	`doOtherthing();`

  `}`

  上述表达式(a*b)==(c*d)总是返回true。因为operator*返回的对象都是指向operator*内部定义的static对象。这个对象只有一个，当计算后者时，前者被覆盖。因此永远是两个相同的Rational对象的作比较。

  一个必须返回新对象的函数的正确写法，就是让那个函数**返回一个新对象**。

  `inline const Rational operator* (const Rational &lhs,const Rational &rhs)`

  `{`

  ​	`return Rational(lhs.n * rhs.n,lhs.d * rhs.d);`

  `}`

  这样，你需要承受operator*返回值的构造和析构成本，但是从长远来看，那只是为了获得正确的行为而付出的一个小小的代价。但万一代价比较恐怖，承受不起，这是别忘了C++和所有编程语言一样，允许编译器实现者施行最优化，用以改善产出代码的效率，却不改变其可观察的行为。如果编译器施行优化，你的程序将保持它们该有的行为，但运行起来比预期更快。

  以上可以总结为：在返回一个reference和返回一个object之间抉择时，挑出行为正确的那个。让编译器厂商为你尽可能降低成本吧！

## 22. 将成员变量声明为private

* 首先从语法一致性开始，如果成员变量不是public，那么客户访问该成员的唯一方法就是通过成员函数（如果没有友函数）。如果public接口内的每样东西都是函数，客户在使用这个对象时，就不需要疑问到底是访问变量还是函数了，因为这个时候不能访问成员变量。
  或许一致性不是令你信服的理由。还有一个理由：使用函数可以让你对成员变量的处理有更加精确的控制。如果成员变量为public，那么每个人都能读和写，但是如果通过函数读或写其值，那么就能实现“不准访问”、“只读访问”以及“读写访问”，甚至实现“惟写访问”。
* 还有一个更重要的理由：封装。如果通过函数访问成员变量，日后可以用某个计算替换这个变量，这时class的客户却不知道内部实现已经变化。
* protected成员的封装貌似高于public，但是事实并非如此，修改protected成员变量，多少derived类需要修改或多少使用derived对象的客户代码需要修改。
* 总结：
   1、将成员变量声明为private。这可以赋予客户访问数据的一致性、可细微划分访问控制、允许约束条件获得保证，并提供class作者以充分弹性实现。
   2、protected并不比public更具有封装性。

## 23. 宁以non-member,non-friend替代member函数

* 这个条款讲解成员函数和友函数的区别。

* 考虑一个class用来清除浏览器的一些记录，这个class中有清除告诉缓存区的函数，有清除访问过URLs的函数，还有清除cookies的函数：

  `class WebBrowser{`

  `public：`

  ​	`void clearCash();`

  ​	`void clearHistory();`

  ​	`void clearCookies();`

  `};`

  一般情况下，需要同时执行这三个动作，因此WebBrowser可以提供这样一个函数：

  `class WebBrowser{`

  ​	`void clearEverything(){`

  ​		`clearCash();`

  ​		`clearHistory();`

  ​		`clearCookier();`

  ​	`}`

  `};`

  另一种做法是用一个non-member函数调用适当的member函数

  `void clearBrowser(WebBroser &wb){`

  ​	`wb.clearCash();`

  ​	`wb.clearHistory();`

  ​	`wb.clearCookier();`

  `}`

  上面两种做法，哪种比较好呢？答案是**non-member函数比较好**。

  面向对象思想要求，数据尽可能被封装，member函数clearEverything带来的封装性比non-member函数clearBrowser低。提供non-member函数，对class相关机能有较大包裹弹性（packaging flexibility），因此带来了较低的编译相依度，增加了class的可延展性。

  封装意味着不可见。愈多东西被封装，欲少人可以看到它，我们就有愈大的弹性去改变它。愈少代码可以看到数据（访问数据），愈多数据可被封装，我们就更有自由来改变对象数据。愈多函数可以访问它，数据的封装性就愈低。

  成员变量应该是private，否则就有无限多函数可以访问它，毫无封装可言。能访问private成员变量的函数只有class的member函数、friend函数而已。在一个member函数和一个non-member、non-friend函数之间做抉择，如果两者提供相同的机能，显然后者提供了更大的封装，这个就是上面选择clearBrowser函数的原因。

  在封装这点上，需要注意两点:

  1. 这个论述只适用于non-member、non-friend函数。
  2. 因为封装，让函数成为class的non-member函数，但这并不意味着它不可以是另一个class的member函数

  在C++中，实现上述功能，比较自然的做法是把clearBrowser函数和WebBrowser类放到一个命名空间内：

  `namespace WebBrowserstuff{`

  ​	`class WebBrowserstuff{};`

  ​	`void clearBrowser(WebBrowser *wb);`

  `}`

  这不仅仅是看起来整齐。namespace可以跨越多个源码文件，class不能。像clearBrowser这样的函数只是为了提供便利，它是non-member、non-friend，没有对WebBrowser的特殊访问权力。一个像WebBrowser这样的class可能拥有大量便利函数，例如某些与书签相关，某些与打印有关，某些与cookies相关……。通常客户使用是时只是对其中一些感兴趣。在编码时通常分离它们：将书签相关便利函数声明于一个头文件，将cookie相关函数声明于另一个头文件，再将打印相关函数声明到第三个头文件……。

  这也正是C++标准库的组织方式。标准库有数十个头文件（<vector>,<algorithm>,<memroy>等等），每个头文件声明std的某些机能。如果客户想使用vector相关机能，只需要#include<vector>即可。这也允许客户只对他们所用的那一小部分形成编译相依

  **将所有便利函数放到多个文件夹但隶属同一个命名空间**，意味着客户可以轻松扩展这一组便利函数，他们要做的就是往命名空间添加更多non-member函数和non-friend函数，这也是class无法做到的。当然客户可以继承类来扩展出新类，但是derived class无法访问base class中封装的private成员，因此扩展的机能拥有的只是次级身份。

  总结：用non-member、non-friend函数替换member函数，这样可以增加封装性、包裹弹性和机能扩充性。

## 24. 若所有参数均需要类型转换，请为此采用non-member函数

通常情况，**class不应该支持隐式类型转换**，因为这样可能导致我们想不到的问题。这个规则也有例外，最常见的例外是建立数值类型时。例如编写一个分数管理类，允许隐式类型转换：

`class Rational{`

`public:` 

​	`Rational(int numerator=0,int denominator=1);`

`};`

如果要支持加减乘除等运算，这时重载运算符时是应该重载为member函数还是non-member函数呢，或者non-member friend函数？
如果写成member函数

`Rational::const Rational operator* (const Rational &rhs);`

这样可以实现两个有理数相乘

`Rational one(1,3);`

`Rational two(3,5);`

`Rational result=one*two;`

如果进行混合运算

`result=one*2;   //正确，相当于one.operator*(2);`

`result=2*one;  //错误,相当于2.operator(one);`

如果要支持混合运算，可以让operator*成为一个non-member函数，这样编译器可以在实参身上执行隐式类型转换。

const Rational operator*(const Rational& lhs,const Rational& rhs);

这样就可以进行混合运算了。那么还有一个问题就是，是否应该让operator*成为friend函数。如果可以通过public接口，来获取内部数据，那么可以不是friend函数，否则，如果读取private数据，那么要成为friend函数。

这里还有一个重要结论：member函数的反面是non-member函数，不是friend函数。如果可以避免成为friend函数，那么最好避免，因为friend的封装低于非friend。

如果需要为某个函数的所有参数（包括this指针所指向的隐喻参数）进行类型转换，这个函数必须是个non-member函数。

## 25. 考虑一个不抛出异常的swap函数

swap是STL中的标准函数，用于交换两个对象的数值。后来swap成为异常安全编程的脊柱，也是实现**自我赋值**的一个常见机制。

`namespace std{`

​	`template <class T>`

​	`void swap(T &a,T &b){`

​		`T temp(a);`

​		`a=b;`

​		`b=temp;`

​	`}`

`}`

只要T支持copying函数（copy构造函数和copy assignment操作符）就能允许swap函数。这个版本的实现非常简单，a复制到temp，b复制到a，最后temp复制到b。

但是对于某些类型而言，这些复制可能无一必要。例如，class中含有指针，指针指向真正的数据。这种设计常见的表现形式是所谓的“pimpl手法“（**pointer to implementation**）。如果以这种手法设计Widget class

`class WidgetImpl{`

`private:`

​	`int a,b,c;`

​	`std::vector<double> b;`

`}`

下面是pimpl实现

`class WIdget{`

`public:`

​	`Widget(const Widget &rhs);`

​	`Widget& operator=(const Widget &rhs)`

​	`{`

​		`*pImpl= *(rhs.pImpl);  //复制Widget时，复制pImpl对象`

​	`}`

`private:`

​	`WidgetImpl *pImpl;  //指针，含有Widget对象`

`}`

如果置换两个Widget对象值，只需要置换其pImpl指针，但STL中的swap算法不知道这一点，它不只是复制三个Widgets，还复制WidgetImpl对象，非常低效。

我们希望告诉std::swap，当Widget被置换时，只需要置换其内部的pImpl指针即可，下面是基本构想，但这个形式无法编译（不能访问private）。

`namespace {`

​	`template <>  //表示Widget的全特化版本`

​	`void swap<Widget> (Widget &a,Widget &b){`

​		`swap(a.pImpl,b.pImpl);`

​	`}`

`}`

其中template<>表示std::swap的一个全特化（total template specialization），函数名之后的<Widget>表示这一特化版本系针对T是Widget而设计的。我们被允许改变std命名空间的任何代码，但是可以为标准的template编写特化版本，使它专属于我们自己的class。

上面函数试图访问private数据，因此无法编译。我们可以将swap函数声明为friend或者令Widget的swap函数为public，然后将std::swap全特化

`class Widget{`

`public:`

​	`void swap(Widget &other)`
​	`{`

​		`using std::swap`

​		`swap(pImpl,other.pImpl);`

​	`}`

`};`

`namespace std{`

​	`template<>`

​	`void swap(Widget &a,Widget &b){`

​			`a.swap(b);`

​	`}`

`}`

这个做法还跟STL容器保持一致，因为STL容器也提供`public swap`和特化的`std::swap`。 
 刚刚假设Widget和WidgetImpl都是class，而不是class template，如果是template时：

`template<class T>`

`class WidgetImpl{...};`

`template<class T>`

`class Widget{...};`

可以在Widget内或WidgetImpl内放个swap成员函数，像上面一样。但是在特化`std:swap` 时会遇到麻烦

`namespce std{`

​	`template<class T>`

​	`void swap(Widget<T> &a,Widget<T> &b){ //不合法，错误`

​		`a.swap(b);`

​	`}`

`}`

看起来合理却不合法。上面是企图偏特化（partially specialize）一个function template(std::swap)，但C++只允许对class template偏特化，在function templates身上偏特化行不通，这段代码不该通过编译。
当偏特化一个function template时，通常简单地为它添加一个重载版本

（类模板可以进行偏特化，函数模板不能偏特化，因为这能用函数重载来实现）

`namespace std{`

​	`template<class T>`

​	`void swap(Widget<T> &a,Widget<int> &b)`

​	`{`	

​		`a.swap(b);`	

​	`}`

`}`

一般而言，重载function template没有任何问题，但std是个特殊的命名空间，其管理规则也比较特殊。客户可以全特化std内的templates，但是不可以添加新的classes或functions到std里面。std的内容有c++标准委员会决定，标准委员会禁止我们膨胀那些已经声明好的东西。
正确的做法是声明一个non-member swap 让他来调用member swap，但不再将那个non-member swap声明为std::swap。把Widget相关机能都置于命名空间WidgetStuff

`namespace WidgetStuff{`

​	`template<class T>`

​	`void swap(Widget<T> &a,Widget<int> &b)`

​	`{`	

​		`a.swap(b);`	

​	`}`

`}`

上面的做法对于class和class template都适用，但是如果你想让你的“class专属版”swap在尽可能多的语境下被调用，你需要同时在该class所在命名空间内写一个non-member版本以及一个std::swap版本。
现在所做的都与swap相关，换位思考一下，从客户角度来看，假设你正在写一个function template，其内需要置换两个对象的值
这时应该调用哪个swap？是std既有的，还是某个可能存在的特化版本，再或则是可能存在一个可能存在的T专属版本且可能栖身于某个命名空间。我们希望首先调用T的专属版本，当该版本不存在的情况下调用std的一般版本。

`template<class T>`
`void doSomething(T &obj1,T &obj2){`

​	`using std::swap;  //令std::swap在此函数中可用`

​	`swap(obj1,obj2);  //为T型对象调用最佳swap版本；即若没有专有版本，则调用std中的一般版本`

`}`

当编译器看到对swap调用时，便去找最合适的。C++的名称查找法则（name lookup rules）确保找到**global作用域或T所在命名空间内的任何T专属的swap**。如果T是Widget并在命名空间WidgetStuff内，编译器或使用“实参取决之查找规则”（argument-dependent lookup）找到WidgetStuff内的swap，如果没有专属版的swap，那么会调用std内的swap（因为使用了using std::swap)。

现在已经讨论了dufault swap、member swap、non-member swaps、std::swap特化版本、以及对swap的调用，下面做个总结。

* 首先，如果swap的缺省实现对我们的class或class template提供可接受的效率，那么无需做任何事。
* 其次，如果swap缺省实现版本的效率不足（例如，你的class或template使用了某种pimple手法），试着做以下事情：
  1. 提供一个public swap成员函数，让它高效置换两个对象值。这个函数不应该抛出异常。
  2. 在你的class或template所在命名空间提供一个non-member swap，并令它调用上述swap成员函数。
  3. 如果你编写的是class（不是class template），为你的class 特化std::swap，并令它调用你的swap成员函数。
     如果调用swap，那么要使用using声明式，确保让std::swap在你的函数内可见。

* 另外，成员版的swap函数决不能抛出异常，因为swap的一个最好应用是帮助class或class template提供强烈的异常安全性（exception-safety）保障。条款29对此提供细节，但此技术基于一个假设：成员版swap绝不抛出异常。这个约束只施行在成员版，不用于非成员版。因为std::swap是以copy函数为基础，而copy函数允许抛出异常。
  当我们编写自定版的swap时，不是仅仅提供高效的置换对象值的方法，还要不抛出异常。这两个特性总是连在一起，因为高效的swap几乎总是基于对内置类型（例如pimple手法的底层指针），而内置类型上操作不允许抛出异常。

总结:

1. 如果std::swap不高效时，提供一个swap成员函数，并且确定这个函数不抛出异常。
2. 如果提供一个member-swap,也应该提供一个non-member swap来调用前者。对于class（非class template），要特化std::swap。
3. 调用swap时，针对std::swap使用using形式，然后调用swap并且不带任何命名空间资格修饰。
4. 为“用户定义类型”进行std template全特化时，不要试图在std内加入某些对std而言是全新的东西

## 26. 尽可能延后变量定义式的出现时间

在程序中定义一个变量，当控制流（control flow）到达这个变量定义时，程序就要承受变量的构造成本，当控制流离开这个作用域时，程序也要承受析构成本。无论这个变量是否使用，都要承受这些成本。应该尽量避免这种情形。或许你认为自己不会这样使用，但也未必。例如要写一个加密函数，但加密的密码要足够长。如果密码太短，会抛出一个异常logic_error

`std::string encryptPassword(const std::string &Password)`

`{`

​	`using namespace std;`

​	`string encrypted;`

​	`if(password.length()<MinimumPasswordLength)`

​		`throw logic_error("Password is to short");`

​	`...  //将加密结果置入encrypted；`

​	`return encrypted;`

`}`

如果该函数抛出一个异常，对象encrypted就没有被使用过，也就是说，无论该函数是否抛出异常，都得付出encrypted的构造和析构版本，所以最好延后encrypt的定义，直到确实需要使用他为止

`std::string encryptPassword(const std::string &Password)`

`{`

​	`using namespace std;`

​	`if(password.length()<MinimumPasswordLength)`

​		`throw logic_error("Password is to short");`

​	`string encrypted;`

​	`...  //将加密结果置入encrypted；`

​	`return encrypted;`

`}`

但这段代码任何不够高效。使用变量encrypted时，先定义，之后再给它赋值。在**条款**4曾经提到过，通过default构造函数构造一个对象，然后再用赋值操作符赋值，其效率不如使用一个直接指定初值的构造函数。

`string encrypted(Password);`

这个条款“尽可能延后”的意义，不仅仅是把变量定义延迟到变量的使用前一刻为止，甚至该**尝试延后这份定义到能够给它赋初值为止**。这样还可以避免无意义的default构造函数。

**总结**：尽可能延后变量定义式的出现。这样可以增加程序清晰性并改善效率。



## 27. 尽量少做转型动作

C++规则的设计目标之一是保证“类型错误”不发生。如果编译时出现与类型转换相关的警告，别轻易放弃它，要确保这个警告是否安全。
C++中的转型（casts）会破坏类型系统（type system）。C++不同于C、Java和C#，这些语言中的转型比较必要，且难以避免，也比较不危险。 

C风格转型：

​	`(T) expression`

函数风格的转型：

​	`T(expression)`

上面两种形式无差别，只是小括号的位置不同而已。 
 C++中提供四种新式转型：

`const_cast<T> (expression)`

`dynamic_cast<T> (expression)`

`reinterpret_cast<T> (expression)`

`static_cast<T> (expression)`

这四种各有不同用法

1. const_cast通常被用来将对象的**常量特性转除**（cast away the constness）。它也是唯一由此能力的C++-style转型的操作符。

2. dynamic_cast主要用来执行“安全向下转型”（safe downcasting），也就是用来决定某对象是否归属继承体系中的某个类型。其主要目的是：**将基类的指针或引用安全地转换成派生类的指针或引用**，并用派生类的指针或引用调用非虚函数。如果是基类指针或引用调用的是虚函数无需转换就能在运行时调用派生类的虚函数。

3. reinterpret_castreinterpret_cast 用于进行各种不同类型的**指针之间、不同类型的引用之间以及指针和能容纳指针的整数类型之间的转换**。转换时，执行的是**逐个比特复制**的操作。

   这种转换提供了很强的灵活性，但转换的安全性只能由程序员的细心来保证了。例如，程序员执意要把一个 int* 指针、函数指针或其他类型的指针转换成 string* 类型的指针也是可以的，至于以后用转换后的指针调用 string 类的成员函数引发错误，程序员也只能自行承担查找错误的烦琐工作

4. static_cast用于比较自然和低风险的转换（implicit conversions）。例如将int转为double，non-const转为constant等。static_cast 不能用于在不同类型的指针之间互相转换，也不能用于整型和指针之间的互相转换，当然也不能用于不同类型的引用之间的转换，因为这些属于风险比较高的转换。它也可以用来执行一些转换的反响转换，但无法将const转为non-const。

为了兼容，旧式的转型仍然合法，但是更提倡用新式的形式。因为：

1. 新式转型**很容易被辨识**出来，可以很快找到代码中有哪些转型。
2. 新式转型动作的**目标愈窄化**，编译器愈可能**诊断出错误**的运用。

但是当条用explicit构造函数将一个对象传给函数时， 常常使用旧式转型：

`class Widget{`

`public:`

​	`explicit Widget(int size);`

`};`

`void do(const Widget &w);  //一个形参使用Widget的函数`

`do(Widget(15));  //函数转换`

`do(static_cast<Widget>(15));  //C++转换`

使用C++风格转型时，不怎么像生成对象，而函数风格看起来更自然些。
转型在表面看起来是把一种类型视为另一种类型，但是编译器做的远远比这个多。编译器在编译器期间会真的编译出运行期间的代码。例如下面代码：

`class Base{};`

`class Dervied:public Base{};`

`Derived d;`

`Base *b=&d;`

上面代码是实现多态的手段之一，经常看到。Base对象和Derived对象的大小并不相同。这时，通常有个偏移量（offset）在运行期间施与Derived*指针身上，用于取得正确的Base*指针。
上面也说明单一对象，可能有多个地址，例如Derived对象，Derived*指针指向它和Base*指针指向它。在继承中经常发生这样事。通常你不应该假设对象在内存中如何布局，更不该以此假设为基础进行转型。例如把Base*转型为char*进行字符操作。
但是有时候我们需要一个偏移量，知道对象的布局，并以此进行了转型。但是编译器不同可能会导致对象布局不同，这意味着在一个平台编译通过的代码，在另一个平台未必能行。
有时候很容易写出似是而非的代码。比如，在许多应用框架中（application frameworks）都要求Derived classes内的virtual函数代码第一行都是调用base class对应的virtual函数。例如有个Window base class和SpecialWindow derived class。两者都定义了onResize函数，specialWindow中的onResize函数要先调用Window中的onResize函数。下面是一种实现方式，看起来对，但其实是错的。

`class Window{`

`public:`

​	 `virtual void onResize(){};`

`};`

`class specialWindow{`

`public:`

​	`virtual void onResize(){`

​			`static _cast<Window>(*this).onResize();//将this转换成Window,然后调用，这样其实是行不通的`

​	`}`

`}`

这段代码中，使用了转型动作，我们所预期的是将this转为Window，之后调用Window::onResize。但实际上并不是这样，static_cast<Window>(*this).onResize()所调用的是this中“base class成分”的暂时副本身上的onResize()（函数只有一份，调用那个对象身上的函数没有什么关系，关键是函数中隐藏有this指针，会影响函数操作的数据）。要想实现想要的行为，其实很简单，就是使用域操作符::`class specialWindow{`

`public:`

​	`virtual void onResize(){`

​			`Window::onResize();`

​	`}`

`}`

探究dynamic_cast之前，要注意的是**dynamic_cast的许多实现版本运行非常慢**。有一个很普遍的实现版本基于“class名称之字符串比较”。如果你在四层继承的单继承体系中的某个对象上执行dynamic_cast，刚刚所说的那个实现版本每一次dynamic_cast调用可能会耗用多达四次的strcmp调用，用以比较class名称。如果是深度继承或多重继承，成本更高。当然，某些版本这样实现有些原因（必须只是动态链接）。所以要对一般转型保持猜疑，尤其是对注重效率的代码中使用dynamic_cast。
之所以要使用dynamic_cast是因为我们手头只有一个Base指针或引用，但是要在Derived对象身上执行Derived class操作。通常有两个一般性操作可以避免这个问题。
1、使用容器其中存储指向Derived的指针。这样就可以消除“通过base class接口处理”Derived对象的需要。但是这种实现无法再同一个容器内存储指针“指向所有可能的派生类”。通常会需要多个容器，每个容器存储一种类型，它们必须具备类型安全（type-safe）。
2、可以通过base class接口处理“所有可能之各种Window派生类”，就是在base virtual函数做你想对派生类做的事。例如，可以在base class声明一个空的virtual函数，在不同的派生类有不同的实现，通过多态来实现想做的事。 

优良的C++代码应该很少使用转型，但是完全摆脱转型又不切实际。在一开始的将int转为double是一个通情达理的使用，虽然我们可以一开始声明为double类型来避免转型。像面对众多构造函数一样，通常我们应该隔离转型，**把转型隐藏在函数内，通过接口保证调用者不受转型的影响**。
总结
1、应该尽量少使用转型，尤其是在注重效率的代码中使用dynamic_cast。如果需要转型，试着设计无需转型代替。
2、如果必须使用转型，把它隐藏在函数内，客户通过接口使用，而不是由客户来实现转型。
3、使用新式的C++ style转型来代替旧的转型，因为新式的转型很容易辨识出来，而且它们有分类。

## 28. 避免返回handle指向对象内部成分

这里的handles指的是引用、指针、迭代器等可以修改对象的传递方法。（避免返回值修改对象的内部成分）

假设现在编写一个表示矩形的class，每个矩形有其左上角和右下角表示。先定义平面内的点

`class Point{`

`public:`

​	`Point(int x,int y);`

​	`void setX(int newVal);`

​	`void setY(int newVal);`

`}`

再定义矩形对象

`struct RectData{`

​	`Point ulhc;  //upper left-hand corner`	

​	`Point lrhc;  //low right-hand corner`

`}`

后面定义使用的类

`class Rectangle{`

`public：`

​	`Point& upperLeft() const {return pData->ullhc;};`

​	`Point& lowerRight() const {return pData->lrhc;};`

`private:`

​	`std::tr1::shared<RectData> pData;  //tr1::shared<>是一个引用计数型智慧指针`

`};`

这样设计，语法上没有问题。但是从逻辑上来看，upperLeft和lowerRight被声明为const成员函数，它们被设计用来给客户提供Rectangle坐标，而不是让客户修改Rectangle。但是返回的引用的数据是private内部数据，客户可以通过引用修改内部数据。因此我们得到两个教训：

1. 成员变量的封装性最多等于“返回其reference”的函数访问级别。上面的例子中，ulhc和lrhc虽然都被声明为private，但实际却是public，因为public函数upperLeft和lowerRight传出了它们的reference。
2. 如果成员函数传出一个reference，reference所指数据与对象自身关联，且存储在对象之外，通过这个引用可以修改其所指数据。

如果把上面的reference换成指针、迭代器等handles，同样适用。返回对象内部数据的handle会带来降低对象封装性的风险。

我们都知道对象的“内部”是指其成员变量，其实非public成员函数（即protected和private）也是对象“内部”数据的一部分。因此要留心不要返回它们的handles。也就是说，不要让成员函数返回一个指针指向“访问级别较低”的成员函数；如果这样做，后者的访问级别会提高到与前者一致。虽然返回成员函数的指针情况不多见，但是也要注意这一点。

下面讨论怎么解决Rectangle class的成员函数upperLeft和lowerRight的问题。我们遇到的问题是客户可能会修改这两个函数的返回值，那么如果其返回值是const，那么就可以解决这个问题。


class Rectangle{`

`public：`

​	`const Point& upperLeft() const {return pData->ullhc;};`

​	`const Point& lowerRight() const {return pData->lrhc;};

`};`

这样一来，客户可以读取矩形Points的值，但是不能修改它们。封装时，我们总是愿意让客户看到Rectangle的外围Point，所以这里蓄意放松了封装。但是即便如此，upperleft和lowerRight还是返回了“代表对象内部”的handles，它可能导致dangling handles（空悬的号码牌）：handles所指对象不存在。这种不复存在的对象，最常见的来源就是函数返回值。例如某个函数返回对象外框（bounding box），这个外框采用矩形形式：

`class GUIObject{};`

`const Rectangle bouningBox(const GUIObject& obj);`

现在客户这样使用

`GUIObject *pgo;`

`const Point* pUpperLeft=&(bouningBox(*pgo),upperLeft);`

调用boundingBox获得一个新的、临时的Rectangle对象，临时对象没有名字，暂且成为temp，随后upperLeft作用于temp身上，返回一个reference指向temp的一个内部成分。于是，pUpperLeft指向这个Point对象。问题出在temp是一个临时对象，`当这个语句结束后，临时对象便会析构，这时pUpperLeft指向一个不再存在的对象`。pUpperLeft变成空悬、虚吊（dangling）。

所以，“返回一个handle代表对象内部成分”总是危险的，不论这个handle是不是const。问题关键是：handle一旦被传出去，此handle的寿命可能比起所指对象更长。

但这并不意味着你绝对不可以让成员函数返回handle，有时候你还必须这样做。例如重载operator[]，必须返回其reference，但是要牢记，`reference所指对象会随着class对象的销毁而销毁`。

**总结**：避免返回handles（reference、指针、迭代器）指向对象内部。遵守这个条款可以增加封装性，帮助const成员函数的行为像个const，并将发生“虚吊号码牌”（dangling handles)的风险降到最低。

## 29. 为“异常安全”而努力是值得的

假设在多线程环境中，一个class用来表现夹带背景图案的GUI菜单，它应该有个互斥量（mutex）来控制并发（concurrency control）

`class PrettyMenu{`

`public:`

​	`void changeBackground(std::istream &imgSrc);`

`private:`

​	`Mutex mutex;`

​	`Image *bgImage;`

​	`int ImageChanges;`

`};`

下面是PrettyMenu的changeBackground函数的一个实现方式

`void PrettyMenu::changeBackground(std::istream &imgSrc){`	

​	`lock(&mutex);`

​	`delete bgImage;`

​	`++imageChanges;`

​	`bgImage=new Image(imgSrc);`

​	`unlock(&mutex);`

`}`

从“异常安全性”来看，这个函数没有满足“异常安全”两个条件中的任何一个：

1. 不泄露任何资源。上面，一旦在lock和unlock之间发生异常，unlock的调用就不会执行，互斥量永远被锁住了。

2. 不允许数据败坏。上述代码中，如果new Image(imgSrc)抛出异常，bgImage就会指向不存在的对象，但imageChanges已经累加，其实并没有新的图像安装起来。

``void PrettyMenu::changeBacground(std::istream& imgSrc)` 
 `{` 
 	`Lock m(&mutex);` 
 	`deelte bgImage;` 
 	`++imageChanges;` 
 	`bgImage=new Image(imgSrc);` 
 }` 

这个实现代码更短，一个一般性的规则：较少的代码就是较好的代码，因为出错机会比较少，且一旦有所改变，被误解的机会也比较少。

在解决了资源泄露后，来看看如何解决数据败坏。首先来定义一些术语。

异常安全函数（Exception-safe functions）提供以下三个保证之一：

1、基本承诺：如果异常被抛出，程序内的任何事物仍然保持在有效状态下。没有任何对象或数据结构因此而破坏，所有对象都处于一种内部前后一致的状态。例如，刚刚的changeBackground如果抛出异常，PrettyMenu对象应该可以继续使用原背景图像，或令它拥有某个缺省背景图像。
2、强烈保证如果抛出异常，程序状态不变。这点有点像事物，成功则完全成功，失败则回到调用函数之前的状态。和第一个保证不同，第一个保证只能保证程序处于一个合法状态，而强烈保证能保证程序处于两个状态的一种。
3、不抛掷（nothrow）保证，承诺绝不抛出异常，因为它们总是能够完成它们原先承诺的功能。作用于内置类型（int，指针等）身上的所有操作都提供nothrow保证。这时异常安全码中一个必不可少的关键基础材料。

异常安全码（Exception-safe code）必须提供上述三个保证之一，如果不这样做，我们认为其不具备安全性。上面三个保证依次增强，我们需要做的是应该为所写的每一个函数提供哪一种保证。

一般而言你应该会想提供可实施之最强烈保证，从异常安全性的观点视之，nothrow函数很棒，但我们很难从C part of C++领域中完全没有调用一个可能抛出异常的函数。任何使用动态内存的东西，如果无法找到足够内存来满足需求，通常便会抛出一个bad_alloc异常

对于changeBackground来说，提供强烈保证并不困难。使用智能指针，改变语句顺序即可

`class PrettyMenu{`

​	`std::tr1::shared_ptr<Image>  bgImage;`

`}`

`void PrettyMenu::changeBackground(std::istream &imgSrc){`

​	`Lock m1(&mutex);`

​	`bgImage.reset(new Image(imSrc));`

​	`++imageChanges;`

`}`

智能指针使用reset，当new Image(imgSrc)成功后，才会删除旧图像。这些改变几乎让changeBackground提供强烈异常安全保证。美中不足的是参数imgSrc，如果Image构造函数抛出异常，有可能是输入流（input stream）的读取记号（read marker）已被移走，而这样的搬移对程序其余部分是一种可见状态的改变。所以changeBackground在解决这个问题之前只提供异常安全保证。

有个一般化的设计可以很典型的导致强烈保证，这个策略是copy and swap。原则是：为你打算改变的对象（原件）做出一份副本，然后再副本身上做一切必要修改。如果修改动作抛出异常，原对象仍保持未改变状态。当所有修改都成功后，再讲修改过的副本和原对象在一个不抛出异常的操作中置换（swap）。

在实现上，通常将所有“隶属对象的数据”从原对象放进另一个对象内，然后赋予原对象一个指针，指向那个所谓的实现对象（implementation object，即副本）。这种手法叫做pimpl idiom，对PrettyMenu来说，实现如下：

`struct PMImpl{`

​	`std::tr1::shared_ptr<Image> bgImage;`

​	`int imageChanges;`

`}`

`class PrettyMenu{`

`private:`

​	`Mutex mutex;`

​	`std::tr1::shared_ptr<PImpl> pImpl;`

`};`

`void PrettyMenu::changeBackground(std::istream &imgSrc){`

​	`using std::swap;`

​	`Lock m1(&mutex);`

​	`std::tr1::shared_ptr<PImpl> pNew(new PMIpl(*pImpl));`

​	`pNew->bgImage.reset(new Image(imageSrc));`

​	`++pNew->imageChanges;`

​	`swap(pImpl,pNew);`

`}`

上面代码中，PMImpl是一个struct，而不是class，因为PrettyMenu的数据封装性已经有“pImpl是private”而获得保证。当然如果PMImpl是class，虽然一样好，但是有时却不太方便（但也保持了对象纯度）。也可以将PMImpl嵌套于PrettyMenu内，但打包问题（packaging，例如独立撰写异常安全码）是这里所考虑的事。

虽然“copy-and-swap”的策略是改变对象全有或全无的一个好办法，但一般而言，它并不保证整个函数有强烈的异常安全。为了原因，把changeBackground换成一个抽象概念：someFunc

`void someFunc(){`

​	`...  //对local状态做备份`

​	`f1();`

​	`f2();`

​	`...  //将修改后的状态置换过来`

`}`

很显然，这个函数的异常安全性和函数f1、f2有关。如果它们异常安全性更低，那么会拉低someFunc()的异常安全性。例如，如果f1只提供基本保证，为了让someFunc提供强烈保证，我们必须写代码获得调用f1之前的整个程序状态、捕捉f1所有可能异常、然后恢复状态。

即使f1和f2都是强烈异常安全，情况也不会好转。如果f1圆满结束，程序状态在任何方面都有可能有所改变；如果f2随后抛出异常，程序状态和someFunc被调用之前也并不相同，甚至当f2没有改变任何东西时也是如此。

这个问题的原因是“连带影响”（side effects）。函数对“非局部性数据”（non-local data）有连带影响时，就很难提供强烈保证。例如当f1函数修改了数据库，那么其他客户就能看到这一笔修改了，很难再提供强烈保证了。

既然强烈保证有这么多麻烦，或许你不再想为函数提供强烈保证了。还有一个理由支持你这个想法，那就是效率。copy-and-swap的关键在于“修改对象数据的副本，然后在一个不抛出异常的函数中将修改后的数据和原数据置换”，这需要耗费新建数据副本的空间以及时间。因此从效率上来看，“强烈保证”未必显得那么实际。

为实际考虑，有时放弃“强烈保证”，可以退而求其次的追求“基本保证”。对于许多函数来说，“异常安全性之基本保证”是一个比较适当的选择。

假如我们写的函数不提供异常安全，那么他人又可以假设我们在这方面有缺失，除非我们证明自己。我们应该写出异常安全的代码，但有时我们也可以有理由不这么做。例如someFunc函数中调用f1和f2,如果其中一个没有提供异常安全保证，甚至没有提供最基本的异常安全，那么someFunc是无法补偿这些问题的。因为`someFunc调用函数没有提供任何异常安全保证，someFunc自身也不可能提供任何保证。`

一个系统要么具备异常安全性，要么不具备，没有“局部异常安全系统”。许多老旧C++代码不具备异常安全性，所以今天 许多系统仍然不是异常安全的，因为它们使用了一些非异常安全的代码。

因此当我们编写或修改代码时，应该考虑如何让它具备异常安全性。首先是“以对象管理资源”，组织那些可能泄露的资源。然后挑选三个“异常安全保证”中的一个实施到我们所编写的每一个函数身上。如果我们调用非异常安全代码，那么我们就别无选择了，只能将它设为“无任何保证”。将我们做的选择写成文档，为了给客户看，以及为了将来的维护代码。函数的“异常安全性保证”是其可见接口的一部分，我们应该慎重选择。

goto代码以前被视为美好的时间，如今我们却致力写出结构化控制流（structured control flows）；以前全局数据结构（globally accessible data）被视为美好的实践，如今我们却致力于数据封装。以前我们没考虑过异常安全，如今我们却致力于写出异常安全代码。时间在前进，我们与时俱进。

**总结**

1. 异常安全函数（Exception-safe functions）即使发生异常，也不会发生资源泄露或数据结构败坏。这样的函数分为三种：基本型、强烈型、不抛异常型。
2. “强烈保证”往往能够以copy-and-swap实现，但是“强烈保证”未必那么必要或有现实意义。
3. 函数提供的“异常安全保证”通常最高只等于其所调用函数中“异常安全性”最低的决定。

## 30. 透彻了解inlining的里里外外

inline函数是特殊的函数，它有宏的优点，却克服了宏的缺点。inline函数可以免除函数调用所招致的额外开销，但你实际获得的好处可能比你想象的还多，编译器会对inline函数本体执行语境相关最优化。

但使用inline函数会导致目标码（object code）变大，因为对inline函数的调用都会以函数本体替换。在内存比较小的机器上，不宜过多使用inline函数。即使使用虚拟内存，也会导致额外的换页行为（paging），降低指令高速缓存装置的击中率（instruction cache hit rate），以及伴随而来的效率损失。如果inline函数本体很小，编译器对函数本体产出的码可能比函数调用所产出的码更小；如果这样，那么将函数inlining确实会减小目标码和提高高速指令高速缓存装置的击中率。

在函数前面加上inline关键字不是强制这个函数变为inline函数，这只是向编译器提一个申请。这个申请有时是隐喻的，例如将函数定义在class内。**如果把friend函数定义在class内，那么它们也将隐喻声明为inline**。

明确声明为inline函数的的做法是在其定义式前加上关键字inline。例如标准的max template（来自）是这样的：

`template<class T>`

`inline const T& std::max(T &a,T &b){`

​	`return (a<b?b:a);`

`}` 

（添加一点：const放在函数前说明函数返回值不能修改，const函数不修改数据成员）

**inline函数和templates通常都被定义于头文件**。这不是巧合。大多数建置环境（build environment）在编译过程中进行inlining，将一个“函数调用”替换为“被调用函数的本体”，在替换时需要知道这个函数长什么样子。Inlining在大多数C++中是编译期行为。

Templates通常也放置在头文件，因为它一旦被使用，编译器为了将它具体化，也需要知道它长什么样子。（有些编译环境可以在链接期间才执行template具体化，但是编译期间完成的更常见）。

大部分编译器拒绝太过复杂的inlining函数（例如有循环或递归）。virtual函数也不能是inline函数，因为virtual函数是直到运行时才确定调用哪个函数，而inline是执行前将调用动作替换为函数本体。所以表面上是inline函数，实际上未必是，很大程度上取决于编译器。大多数编译器提供了一个诊断级别：如果无法将你要求的函数inline化，会给你一个警告信息。

编译器将inline函数调用替换为inline函数本体的同时，还是可能会为该函数生成一个函数本体。如果程序要取某个inline函数的地址，编译器通常必须为此函数生成一个outlined函数本体。如果inline函数本体不存在，自然就不会有这个函数的地址。但是，**通过函数指针调用inline函数，这时inline函数一般不会被inlined，这取决于调用的实施方式。**

`inline void f(){};  //内联函数f`

`void (*pf)()=f;  //指向函数的指针`

`f();  //该函数会被内联，因为是正常调用`

`pf();  //这个调用可能不会被内联，因为他是通过函数指针来调用`

就算你未使用函数指针，程序有时也会使用。例如，编译器会生成构造函数和析构函数的outline副本，这样就可以获得这些函数的指针，在array内部元素的构造和析构过程中使用。

实际上，把构造函数和析构函数做为inline函数未必合适，表面上看并不可以看出原因。考虑下面Derived class构造函数

`class Base{`

`public:` 

​	`...`

`private:`

​	`std::string bm1,bm2;` 

`};`

`class Derived:public Base{`

`public:`

​	`Derived(){};`

`private:`

​	`std::string dm1,dm2,dm3;` 	

`};`

class Derived的构造函数不含任何代码，应该是inlining函数的绝佳候选。但实际上未必。C++对于“对象被创建和销毁时都发了什么事”做了各种保证。例如，当你创建一个对象，Base class和Derived class的每一个成员变量都会被自动构造；当你销毁一个对象，会有反向的析构过程。这是正常运行时的情况，但是**如果对象在构造期间有异常被抛出，那么该对象已经构造好的那一部分应该自动销毁**。当然，这是编译器负责的事情，但是编译器是怎么实现的呢？那就是编译器在你的程序中插入了某些代码，通常就在构造函数和析构函数内。我们可以想象一下，那个空的构造函数到底应该是怎么样的：

`Dervied::Dervied{   //概念实现`

​	`Base::Base();`

​	`try{dm1.std::string::string();}   //构造dm1`

​	`catch(...){`

​				`Base::~Base();  //销毁Base class部分`

​				`throw;   //抛出异常`

​	`}`

​	`try{dm2.std::string::string();}`

​	`catch(...){`

​				`dm1.std::string::string();`

​				`Base::~Base();`

​				`throw;`

​	`}`

​	`try{dm3.std::string::string();}   `

​	`catch(...){`

​				`dm2.std::string::string();`

​				`dm1.std::string::string();`

​				`Base::~Base();  `

​	`}`

`}`

上面代码并不是编译器生成的，编译器生成的应该会更加精致复杂，处理异常也没这么简单。但这也足够反应空白的Derived构造函数提供的行为。不论编译器怎么优化，Derived构造函数都会调用Base class的构造函数和其成员变量的构造函数，而那些调用（它们自身也可能被inlined）会影响编译器是否对此空白函数inlining。这个道理同样适用于Base构造函数，也同样适用于析构函数。

程序员还要考虑将函数声明为inline带了的其他影响：inline函数无法随着程序库的升级而升级。例如，fun是个inline函数，客户讲fun编进其程序中，一旦程序库设计者升级程序库，所有用到函数fun的客户端程序多必须重新编译。但是如果fun是non-inline的，客户端只需重新连接即可；如果是动态链接库，客户端甚至感觉不到程序库升级。还有一个影响就是调试。大部分调试器无法调试inline函数，因为你不能再一个不存在的函数内设立断点。

**总结**

1. 将大多数inlining限制在小型、被频繁调用的函数身上。这可使日后的调试过程和二进制升级（binary upgradability）更容易，使代码膨胀问题最小化，使程序提升效率机会最大化。
2. function tempates出现在头文件，但是不一定必须是inline。

## 31. 将文件间的编译依存关系降至最低

假如你在修改程序，只是修改了某个class的实现，而且修改的是private部分。之后，你编译时，发现好多文件都被重新编译了。这种问题的发生，在于没有把“将接口从实现中分离”。Class的定义不只是详细叙述class接口，还包括许多实现细目：

`class Person{`

`public://接口`

​	`Person(const std::string &nam,const Date &birthday,const Address &addr);`

​	`std::string name() const;`

​	`std::string birthDate() const;`

​	`std::string address() const;`

`private://实现`

​	`std::string theName;`

​	`Date theBirthDate；`

​	`Address theAddress;`

`};`

要想编译，还要把class中用到的string、Date、Address包含进来。在`Person`定义文件的最前面，应该有：

`#include <string>`

`#include "date.h"`

`#include "address.h"`

这样一来，Person定义文件和其含入文件之间形成了一种**编译依存关系**（compilation dependency）。**如果这些头文件有一个修改，那么使用Person class的文件要重新编译**。这样的连串编译依存关系（cascading compliation dependencies）会给项目造成许多不便。
那么为什么C++把class的实现细目置于class定义式中？可以把实现细目分开：

`namespace std{`

​	`class string;  //前置声明（其实是错的）`

`}`

`class Date;//前置声明`

`class Address;`

`class Person{`

`public:`

​	`Person(const std::string &nam,const Date &birthday,const Address &addr);`

​	`std::string name() const;`

​	`std::string birthDate() const;`

​	`std::string address() const;`

`}；`

首先不讨论前置声明是否正确（实际上是错误的），如果可以这么做，**Person的客户只需要在Person接口被修改时才重新编译**。但是这个想法有两个问题。

1. string不是个class，它是个typedef，定义为basic_string。上面对string的前者声明并不正确，正确的前置声明比较复杂，因为涉及额外的templates。实际上，我们不应该声明标准库，**使用#include即可**。**标准头文件一般不会成为编译瓶颈**，尤其是在你的建置环境中允许使用预编译头文件（precompiled headers）。如果解析（parsing）标准头文件是个问题，一般情况是你需要修改你的接口设计。

2. 前置声明的每一件东西，编译器必须在编译期间知道对象的大小。例如

`int main(){`

​	`int x;`

​	`Person p(params);`

`}`

当编译器看到x定义式，必须知道给x分配多少内存；之后当编译器看到p的定义时，也应该知道必须给p分配多少内存。如果class的定义式不列出实现细目，编译器无法知道给p分配多少空间。

在C++中，也可以这样做，将对象实现细目隐藏在一个指针背后。针对Person，可以把它分为两个classes，一个负责提供接口，另一个负责实现该接口。

负责实现的接口取名为PersonImpl（Person implementation）：

`#include <string>`

`#include <memory>`

`class PersonImpl;`

`class Date;`  

`class Address;`

`class Person{`

`public:`

​	`Person(const std::string &nam,const Date &birthday,const Address &addr);`

​	`std::string name() const;`

​	`std::string birthDate() const;`

​	`std::string address() const;`

`private:`

​	`std::tr1::share_ptr<PersonImpl> pImpl;  //指针，指向实现物`

`};`

这样的设计称为pimpl idiom（pimpl：pointer to implementation）。**Person的客户和Date、Address以及Person的实现细目分离了**。classes的任何实现修改都不要客户端重新编译。此外，客户还无法看到Person的实现细目，也就不会写出“取决于那些细目的代码”，真正实现了“接口与实现分离”。

这个分离的关键在于“**声明的依存性**”替换了“定义的依存性”，这正是编译依存性最小化的本质。现实中，让头**文件尽可能自我满足，万一做不到，则让它与其他文件内的声明（不是定义）相依存**。其他都源于以下设计策略：

1. 如果使用object references或object pointers可以完成任务，就不要使用object。因为，**定义references或pointers只需要一个声明，而定义objects需要使用该类型的定义**。

2. 如果可以，**尽量以class声明式替换class定义式**。但是，当声明函数使用某个class时，即使是by value方式传递该类型参数/返回值，都不需要class定义。但是在使用这些函数时，这些classes在调用函数前一定要先曝光。客户终究是要知道classes的定义式，但是这样做的意义在于：如果你能将“提供class定义式”（通过#include完成）的义务，从函数声明所在文件，转移到函数调用的客户文件，便可将“并非真正必要之类型定义”与客户端之间的编译依存关性去除。

3. 为声明式和定义式提供两个不同的头文件。程序中，程序作者一般提供两个头文件，一个用于声明式，一个用于定义式。这些文件必须保持一致性，如果有个声明式被改变了，两个文件都得改变。因此客户应该总是#include一个声明文件，而非前置声明若干函数。

   在C++标准库的头文件中,<iosfwd>内含iostream各组件的声明式，其对应定义分布在不同文件件，包括<sstream>,<streambuf>,<fstream>,<iostream>。

像Person这样使用pimpl idiom的classes叫做Handle  classes。这样的class真正做事的方法之一是将他们所有的函数转交给相应的实现类（implementation  classes），由实现类完成实际工作。例如Person的实现：

`#include "Person.h"`

`#include "PersonImpl.h"`

`Person::Person(cosnt std::string &name,const Date &birthday,const Address &addr):pImpl(new PersonImpl(name,birthday,addr)){};`

`std::string Person::name() const{`

​	`return pImpl->name();`

`}`

**在PersonImpl中，有着和Person完全相同的成员函数，两者接口完全相同。**

还有一种实现Handle class的办法，那就是令Person成为一种特殊的abstract base class（抽象基类），称作Interface class。这样的class成员变量，只是描述derived classes接口（条款 34），也没有构造函数，只有一个virtual析构函数（ 条款 7）和这一组pure virtual函数，用来叙述整个接口。

像Person这样的Interface class可以这些写：

`class Person{`

`public:`

​	`virtual ~Person();`

​	`virtual std::string name() const =0;`

​	`virtual std::string birthday() const =0;`

​	`virtual std::string address() const =0;`

`};`

**这个class的客户必须使用Person的pointers或references，因为内含pure virtual函数的class无法实例化**（抽象类不能定义对象，但是能定义指针）。这样一来，只要Interface class的接口不被修改，其他客户就不需要重新编译。

（抽象类可以通过指针或引用来调用非虚函数，但无法通过函数来为成员变量赋值，总结：使用抽象类只有通过指针或引用来调用非虚函数）

Interface class的客户在为class创建新对象时，通常使用一个特殊函数，这个函数扮演“真正将被具体化”的那个derived classes的构造函数的角色。这样的函数叫做工程函数factory或virtual构造函数，它们返回指针（更有可能为智能指针），指向动态分配所得对象，这个对象支持Interface class接口。factory函数通常声明为static

`class Person{`

`public:`

​	`static std::tr1::share_ptr<Person> create(const std::string &name,const Date &birthday,const 		    Address &addr);`

`};`

客户这样使用

`std::string name;`

`Date dateOfBirth;`

`Address address;`

`//创建一个指针，支持Person接口`

`std::tr1::share_ptr<Person> pp(Person::create(name,dateOfBirth,address));  `

`std::cout<<pp->name()<<...;`

支持Interface class接口的那个**具体类（concrete classes）在真正的构造函数调用之前要被定义好**。例如，有个RealPerson继承了Person

`class RealPerson: public Person{`
`public:`
    `RealPerson(const std::string& name, const Date& birthday, const Address& addr)`
        `:theName(name), theBirthDate(birthday), theAddress(addr)`
        `{}`
    `virtual ~RealPerson(){};`
    `std::string name() const;`
    `……`
`private:`
    `std::string theName;`
    `Date theBirthDate;`
    `Address theAddress;`
`};`

有了RealPerson之后，就可以写Person::create了

 `std::tr1::shared_ptr<Person> Person::create(const std::string& name, const Date& birthday,const 						Address& addr)`
`{`
    `return std::tr1::shared_ptr<Person>(new RealPerson(name, birthday, addr));`
`}`

RealPerson实现Interface class的机制是：**从Interface class继承接口，然后实现出接口所覆盖的函数。**（基类相当于接口，派生类相当于实现）还有一种实现方法，设计多重继承。

Handle classes和Interface classes解除了接口和实现之间的耦合关系，从而降低了编译依存性。但是为了也带了一些代价：使你丧失了运行期间若干速度，又开辟了超出对象若干内存。

在Handle classes身上，成员函数通过implementation pointer取得对象数据。这样为访问增加了一层间接性，内存也增加了implementation pointer的大小。implementation pointer的初始化，还要带了动态开辟内存的额外开销，蒙受遭遇bad_alloc异常的可能性。

在Interface classes身上，每个函数都是virtual的，所以每次调用要付出一个间接跳跃（indirect jump）成本。其派生对象会有一个vptr（virtual table pointer，**条款**7），增加了对象所需内存。

Handle classes和Interface classes，一旦脱离inline函数，都无法有太大作为。**条款**30说明为什么inline函数要置于头文件，但Handle classes和Interface classes被设计用来隐藏实现细节。

我们要做的是，**在程序中使用Handle classes和Interface classes，以求实现代码有所变化时，对其客户带来最小影响**。但如果它们导致的额外成本过大，例如导致运行速度或对象大小差异过大，以至于classes之间的耦合相比之下不成为关键时，就以具体类（concrete classes）替换Handle classes和Interface classes。

**总结**

1. 支持“编译依存性最小化”的一般构想是：**相依于声明式，不要相依于定义时**。基于此构想的两个手段是**Handle classes和Interface classes**。

2. **程序库头文件应该以“完全且仅有声明式”（full and declaration-only forms）的形式存在**。不论是否这几templates，这种做法都是适用。

## 32. 确定你的public继承塑造出is-a关系

以C++面向对象编程，最重要一个规则是：public inheritance（公开继承）意味着“is-a”（是一种）的关系。在这里是“直译”,例如class D: public B直译就是D是一种B。

如果让class D（“Derived”）以public形式继承class B（“Base”），这便意味着一个类型为D的对象同时也是一个类型为B的对象（或者说D对象含有B对象），但是反之不成立。B比D表现出更一般化的概念，而D比B表现出更特殊化的概念。可以使用B对象的地方，也可以使用D对象。

以一个具体例子来说明：

`class Person{};`

`class Student:public Person{};`

由生活经验可以知道，每个学生都是人，但是并非每个人都是学生。这就是这个继承体系的主张。可以预期，对人成立的每件事，对学生也都成立。但是对学生成立的事对人未必成立。在C++中，任何函数如果期望接受类型为Person对象的实参（或pointer to person，或reference to person），也都可以接受一个类型为Student对象的实参（或pointer to student，或reference to student）。

有时public和is-a之间的关系会误导我们。例如，企鹅（penguin）是一种鸟，这是事实；鸟可以飞，这也是事实。如果以C++描述这层关系：

`class Bird{`

`public:`

​	`virtual void fly();`

`};`

`class Penguin:public Bird{`

`...`

`};`

但是我们知道，企鹅不会飞，这个是事实。这个问题的原因是语言（英语）不严谨。当我们说鸟会飞时，我们表达的意思是一般的鸟都会飞，并不是表达所有的鸟都会飞。我们还应该承认一个事实：有些鸟不会飞。这样可以塑造一下继承关系

`class Bird{`

`...`

`};`

`class flyBird:public Bird{`

`public:`

​	`virtual void fly();`

`};`

`class Penguin:public Bird{`

`...`

`};`

这样的设计能更好的反映我们真正要表达的意思。但是这时，我们仍未完全处理好这些鸟事。例如，如果你的系统不会区分鸟会不会飞，你关心的是鸟啄和鸟翅，这样的话，原先的“双class继承体系”更适合你的系统。并不存在完美设计，具体问题要具体讨论。

还有一个方法来处理“所有鸟都会飞，企鹅是鸟，但企鹅不会飞”这个问题，我们可以在企鹅类重新定义fly函数，让它在产生一个运行期错误：

`void error(const std::string &msg);`

`class Penguin{`

`public:`

​	`virtual void fly(){`

​		`error("Attemp to make a penguin fly");`

​	`}`

`};`

这里前面的解决方法不同，这里不说企鹅不会飞，当你说企鹅会飞时，会告诉你这是一个错误。但是这种解决方法之间有什么差异？从错误被侦测出来的时间来看，第一种解决方法“企鹅不会飞”这个限制条件在编译期强加事实；第二个解决方法，“企鹅会飞是错误”是在运行期检测出来的。第一种解决方法更好，**条款**18说过，好的接口可以防止无效的代码通过编译，相比之下，我们应该选择在编译期来找出这个问题。

在考虑一个例子，基础几何我们都学过，那么正方形和矩形的关系有多么复杂呢？先看下面这个例子：class Square应该以public形式基础class Rectangle吗？我们都知道正方形是特殊的矩形，如果以public继承

在考虑一个例子，基础几何我们都学过，那么正方形和矩形的关系有多么复杂呢？先看下面这个例子：class Square应该以public形式基础class Rectangle吗？我们都知道正方形是特殊的矩形，如果以public继承

**总结** ：“public”继承意味着**is-a**。适用于base classes身上的每一件事一定也适用于derived身上，因为每一个derived对象也是一个base class 对象。



## 33. 避免遮掩继承而来的名称

这里说的名称，是和继承以及作用域有关。先看一个和作用域有关的例子：

`int x;`

`void someFunc(){`

​	`double x;`

​	`std::cin>>x;`

`}`

这个cin是给local变量x赋值，而不是global变量x，因为内层作用域名称会遮掩外围作用域名称。当编译器在someFunc作用域内遇到名称x时，它在local作用域内查找是否有这个变量定义，如果找不到就再去找其他作用域。这个例子中的变量x类型不同，local的是double类型，而global的是int类型；但是这个并不要紧，C++的名称遮掩规则（name-hiding rules）所做的唯一事情就是：遮掩名称，至于类型并不重要。

现在来看一下继承。当一个derived class成员函数内指涉（refer to） base class内的某物（成员函数、成员变量、typedef等）时，编译器可以找到所指涉的东西，因为derived class继承了声明在base class内的所有东西。**derived class的作用域被嵌套的base class作用域内**。

`class Base{`
    `private:`
        `int x;`
    `public:`
        `virtual void mf1()=0;`
        `virtual void mf2();`
        `void mf3();`
        `……`
    `};`
    `class Derived: public Base{`
    `public:`
        `virtual void mf1();`
        `void mf4();`
        `……`
    `};`

这个例子中既有public，又有private。成员函数有pure virtual、impure virtual和non-virtual，这是为了强调我们讨论的是名称，和其他无关。这个例子是单一继承，了解单一继承很容易推断多重继承。假设在derived class的mf4内调用mf2
 `void Derived::mf4()`
    `{`
        `mf2();`
    `}`

当编译器看到mf2时，要知道它指涉（refer to）什么东西。首先在local作用域内（即mf4覆盖的作用域）查找有没有名称为mf2的东西；如果找不到，再查找外围作用域（class Derived覆盖的作用域）；如果还没找到，再往外围找（base class覆盖作用域），在这里找到了。如果base内还是没找到，之后继续在base那个namespace作用域内找，最后往global作用域找。

下面把这个例子变得稍微复杂一点，重载mf1和mf3，且添加一个新版mf3到Derived中。

 `class Base{`
    `private:`
        `int x;`

​    `public:`
​        `virtual void mf1()=0;`
​        `virtual void mf1(int);`
​        `virtual void mf2();`
​        `void mf3();`
​        `void mf3(double);`
​        `……`
​    `};`
​    `class Derived: public Base{`
​    `public:`
​        `virtual void mf1();`
​        `void mf3();`
​        `void mf4();`
​        `……`
​    `};`

因为以作用域为基础的“名称遮掩规则”，base class内所有名称为mf1和mf3的函数都被derived class内的mf1和mf3函数遮掩掉了。从名称查找观点来看，Base::mf1和Base::mf3都不再被Derived继承。

​    `Derived d;`
​	`int x;`
​    `d.mf1();//正确，调用Derived::mf1`
​    `d.mf1(x);//错误，因为Derived::mf1遮掩了Base::mf1`
​    `d.mf2();//正确，调用Base::mf2`
​    `d.mf3();//正确，调用Derived::mf3`
​    `d.mf3(x);//错误，因为Derived::mf3遮掩了Base::mf3`

**条款\**32中说过public继承是\**is-a**关系，如果使用public继承而又不继承那些重载函数，就是违反了**is-a**关系。要想上面的函数调用都正确，可是使用using声明

 `class Base{`
    `private:`
        `int x;`
    `public:`
        `virtual void mf1()=0;`
        `virtual void mf1(int);`
        `virtual void mf2();`
        `void mf3();`
        `void mf3(double);`
        `……`
    `};`
    `class Derived: public Base{`
    `public:`
        `//让Base class内名为mf1和mf3的所有东西在Derived作用域内都可见，且为public`
        `using Base::mf1;`
        `using Base::mf3;`
        `virtual void mf1();`
        `void mf3();`
        `void mf4();`
        `……`
    `};`

这样，下面的调用都不会出错了。

`Derived d;`
`int x;`
`d.mf1();//正确，调用Derived::mf1`
`d.mf1(x);//调用Base::mf1`
`d.mf2();//正确，调用Base::mf2`
`d.mf3();//正确，调用Derived::mf3`
`d.mf3(x);//调用Base::mf3`

如果你继承base class，且加上重载函数；你又希望重新定义或覆写其中一部分，那么要把被遮掩的每个名称引入一个using声明。

public继承暗示base和derived class之间是一种is-a关系，这也是上述using声明放在derived class的public作用域内的原因：base class内的public名称在publicly derived class内也应该是public。

如果想要private继承Base，而Derived唯一想继承的是时Base内mf1无参数的那个版本，using声明在这派不上用场，因为**using声明会使继承而来的某个名称所有函数在derived class都可以见**。这样的实现需要一个不同的技术，一个简单的转交函数（forwarding function）:

`class Base{`
`public:`
    `virtual void mf1()=0;`
    `virtual void mf1(int);`
`};`
`class Derived: private Base{`
`public:`
    `virtual void mf1()//转交函数（forwarding function）`
    `{Base::mf1();};//隐式成为inline`
`};`

`Derived d;`
`int x;`
`d.mf1();//调用Derived::mf1`
`d.mf1(x);//错误，Base::mf1被遮掩了`

上面所述都是不含templates。当继承结合templates时，又会面临“继承名称被遮掩”，关于以“角括号定界”的东西，在**条款**43讨论。

**总结**

1. derived classes内的名称会遮掩base classes内的名称。在public继承下从来没有人希望如此。
2. 为了让遮掩的名称在derived class内重见天日，可以使用using声明或转交函数(forwarding function）。
   

## 34. 区分接口继承和实现继承

public继承的概念，由两部分构成：函数接口（function Interface）继承和函数实现（function implementation）继承。这两种继承的差异有点像函数的声明和函数的定义之间的差异。

**=》pure virtual -> 只继承接口        impure virtual -> 继承接口+实现（也称为默认实现）**

我们在设计class时，有时希望derived class只继承函数的接口（即函数声明）；有时候希望derived class继承函数接口和实现，但又覆写它们所继承的实现；又有时候希望derived class同时继承函数的接口和实现，但不覆写任何东西。

为了更好理解上述差异，用一个绘图程序来说明：

`class Shape{`
`public:`
    `virtual void draw() const=0;`
    `virtual void error(const std::string& msg);`
    `int objectID() const;`
    `……`
`};`
`class Rectangle: public Shape{……};`
`class Ellipse:public Shape{……};`

Shape中有pure virtual函数，所以它是个抽象类，不能创建Shape对象，但Shape强烈影响了所有以public继承它的derivedclass，因为
- 成员函数的接口总会被继承，public继承意味着is-a。

Shape class有三个函数：draw是pure virtual函数；error是impure pure函数；objectID是non-virtual函数。

pure virtual函数有两个特点：它们必须被继承了它们的具体class重新声明，而且在抽象class中通常没有定义。

这也就是说明：**声明一个pure virtual函数的目的是为了让derived class只继承函数接口**。

这也是合情合理的，因为Shape::draw并不知道我们要画什么图像，当然无法给出实现了。但是我们可以为pure virtual函数提供定义，即为Share::draw提供一份实现，C++不会发出怨言，但是调用这个函数的唯一途径是调用时指明其class名称：

`Shape sp=new Shape;  //错误，Shape是抽象类`

`Shape ssp=new Rectangle;  //ok`

`ssp->draw();  //调用Rectangle::draw`

`ssp->Shape::draw();  //调用Shape::draw`

impure virtual函数和pure virtual函数有所不同，derived classes继承其函数接口，但impure virtual函数会提供一份实现代码，derived class可能覆写（override）它。

**声明（非纯）impure virtual函数的目的，是让derived classes继承该函数的接口和缺省实现。**（缺省==实现）

考虑Shape::error这个例子，error接口表示，每个class都必须支持一个遇上错误时可调用的函数，但每个class可以自由处理错误。如果某个class不想针对错误做出特殊行为，可以退回到Shape class提供的缺省错误处理行为。也就是说Shape::error的声明式告诉derived class设计者：你必须支持一个error函数，但如果你不想自己写，可以使用Shape class提供的缺省版本。

如果允许impure virtual函数同时指定函数声明和函数缺省行为，有可能造成危险。考虑一个具体例子，一个XYZ航空公司设计飞机继承体系，该公司有A型和B型两种飞机，都以相同方式飞行，可以考虑这样设计继承体系：


`class Airport{ ……};`
`class Airplane{`
`public:`
    `virtual void fly(const Airport& destation);`
    `……`
`};`
`void Airplane::fly(const Airport& destation)`
`{`
    `//缺省行为，将飞机飞到指定的目的地`
`}`
`class ModelA: public Airplane{……};`
`class ModelB: public Airplane{……};`

因为不同型飞机不需要不同的fly实现，Airplane::fly被声明为virtual；为了避免在ModelA和ModelB重新撰写相同代码，缺省的飞行行为有Airplane::fly提供。

上面这种设计方式是典型的面向对象设计。两个classes共享的性质放到base class中，然后被这两个class继承。这样可以突出共同性质，避免代码重复。

但是如果XYZ要购买一种新型飞机C，C和A、B飞行方式不同。XYZ公司程序员给C型飞机添加了一个class,但是没有重新定义fly函数

`class ModelC: public Airplane{……};`

然后又写了如下代码 	

 `Airport PDX()；//某个机场`
 `Airplane* pa=new ModelC;`
 `……`
 `pa->fly(PDX);//调用了Airplane::fly`

这会造成大灾难，因为程序员试图以ModelA或ModelB的方式来飞ModelC。问题不在于Airplane::fly有缺省行为，在于ModelC在未搞清楚的情况下就使用了这个缺省行为。幸运的是可以做到：提供缺省实现给derived classes，但是除非derived classes真的要用。这个做法是**切断virtual函数接口和其缺省实现之间的连接**。

`class Airplane{`
    `public:`
        `virtual void fly(const Airport& destation)=0;`
    `protected:`
        `void defaultFly(const Airport& destation);`
    `};`

 `void Airplane::deFaultFly(const Airport& destation)`
 `{`
      `//将飞机飞到指定目的地`
 `}`

这里将Airplane::fly改为pure virtual函数，只提供接口。但是缺省的行为在Airplane::defaultFly函数中出现。如果要使用其缺省行为，可以在fly函数调用defaultFly函数。

`class ModelA: public Airplane{`
`public:`
    `virtual void fly(const Airport& destation)`
    `{ defaultFly(destation)}`
`};`
`class ModelB: public Airplane`
`……`

`class ModelC: public Airplane{`
`public:`
    `virtual void fly(const Airport& destination);`
`};`
`void ModelC:fly(const Airport& destination)`
`{`
    `//将C型飞机飞到指定目的地`
`}`

上面设计中，Airplane::defaultFly是个non-virtual，derived classes不用重新定义。如果Airplane::defaultFly是virtual函数，就会出现循环问题：万一derived classes忘记重新定义defaultFly函数会怎样？

有的人返回以不同的函数分别将提供接口和缺省实现，这样会因为过度雷同的函数名称引起class命名空间污染问题；但是他们同意接口和缺省实现应该分开。我们可以利用“pure virtual函数必须在derived classes中重新声明，但它们可以拥有自己的实现”这个特点

`class Airplane{`
`public:`
    `virtual void fly(const Airport& destination)=0;`
    `……`
`};`
`void Airplane::fly(const Airport& destination)//pure virtual函数实现`
`{`
    `//缺省实现`
`}`

`class ModelA: public Airplane{`
`public:` 
    `virtual void fly(const Airport& destination)`
    `{Airplane::fly(destination);}`
    `……`
`};`
`class ModelB:public Airplane`
`……`

`class ModelC: public Airplane`
`{`
`public:`
    `virtual void fly(const Airport& destination);`
    `……`
`};`
`void ModelC::fly(const Airport& destination)`
`{`
    `//ModelC的实现`
`}`

这个实现和上一个不同之处在于，用pure virtual函数Airplane::fly替换了独立函数Airplane::defaultFly。现在的fly被分割为两个基本要素：其声明部分表现为接口（derived classes必须使用），定义部分表现为缺省行为（derived classes明确提出申请才可以用）。

最后来看看Shape的non-virtual函数objectID；Shape::objectID是个non-virtual函数，这意味着它不打算在derived class中有不同行为。

**声明non-virtual函数的目的是为了令derived classes继承函数的接口及一份强制性实现。**

可以把Shape::objectID看做“每个Shape对象都有一个用来产生识别码的函数，这个识别码采用相同计算方法。non-virtual函数代表的意义是不变性（invariant）凌驾特异性（specialization），所以不应该在derived classes中被重新定义。

**pure virtual函数对应只继承接口；simple（impure） virtual函数对应继承接口和一份缺省实现；non-virtual函数对应继承接口和一份强制实现。**

在设计classes时，要分清这些区别和联系，否则容易犯两个错误：

* 第一个错误是将所有函数声明为non-virtual。这会使derived classes没有空间进行特化工作；non-virtual析构函数会给析构带了问题。如果关心virtual函数的成本问题，典型的程序有80%时间在执行20%代码，函数中有80%的virtual函数不一定会给程序带了多大效率损失，将心力花在20%代码上才是关键。
* 第二个错误是将所有成员函数声明为virtual。有时候这样是正确的，例如**条款**31的Interface classes。如果有些函数在derived classes中不应该被重新定义，那么就应该将这些函数声明为non-virtual。

**总结**

1. 接口继承和实现继承不同。在public继承下，derived classes总是继承base classes的接口。
2. pure virtual函数只具体指定接口继承。
3. impure virtual函数具体指定接口继承和缺省实现继承。
4. non-virtual函数具体指定接口继承和强制性实现继承。

## 35. 考虑virtual函数以外的选择

virtual函数在派生中经常用到，在遇到一些问题时用virtual函数没问题，但是有时候我们应该思考一下是否有替代方案，以此来拓宽我们的视野。

假如现在正在写一个游戏，游戏中人物的血量随着战斗而减少，用一个函数healthValue返回这个血量值。因为不同人物血量值计算方法不同，所以应该讲healthValue声明为virtual：

`class GameCharacter{`
`public:`
    `virtual int healthValue() const;  //derived classes可以重新定义`
    `……`
`};`

`healthValue`不是pure virtual，这暗示我们有个计算血量的缺省算法。

这是个很明白清楚的设计，正是因为如此，我们可能没有考虑其他替代方案。我们来考虑一些其他解法。

### 藉由Non-virtual Interface手法实现Template Method模式

我们将从一个有趣的流派开始，这个流派主张：virtual函数应该几乎总是private。这个主张建议，较好的设计是保留healthValue为public non-virtual成员函数，让它调用一个private virtual函数来做实际工作：

 `class GameCharacter{`
    `public:`
        `int healthValue() const`
        `{`
            `……  //做事前工作`
            `int retVal=doHealthValue();//真正做实际工作`
            `……  //做事后工作`
            `return retVal;`
        `}`
        `……`
    `private:`
        `virtual int doHealthValue() const {……}   //derived classes可以重新定义`
    `};`

**这个设计是让客户通过public non-virtual成员函数间接调用private virtual函数**，成为non-virtual interface（NVI）手法。它是所谓Template Method设计模式（与C++ templates没关系）的一个独特表现形式。这个non-virtual函数叫做virtual函数的外覆器（wrapper）。

Template Method：定义一个操作中的算法的骨架，而将一些步骤延迟到子类中。TemplateMethod使得子类可以不改变一个算法的结构即可重定义该算法的某些特定步骤。

**NVI的优点在于“做事前工作”和“做事后工作”，这可以确保virtual函数在调用之前和调用之后做些工作，为virtual函数调动做准备，且在调用之后做些清理。**例如事前工作可以包括锁定互斥器、制造日志记录项、验证class约束条件、验证函数先决条件等；事后工作可以包括解除互斥器、验证函数事后条件等。

这里有必要解释一下重新定义virtual函数和调用virtual函数。重新定义virtual函数表示某事如何被完成，调用virtual函数则表示何时完成某事。这并不矛盾。derived classes重新定义virtual函数，赋予它们如何实现的控制能力；但base class保留何时调用函数的权利。

NVI手法未必一定让virtual函数是private，某些classes继承体系要求derived class是protected。但是如果virtual函数是public，这样就不能实施NVI手法了。

### 藉由Function Pointers实现**Strategy**模式

NVI使用了virtual函数来计算每个人的健康指数。另一个更戏剧性的主张：人物健康指数的计算和人物类型无关；这样计算不需要“人物”这个成分。例如可以在人物的构造函数接受一指针，指向健康计算函数：

`class GameCharacter;//前置声明（forward declaration）`
`int defaultHealthCalc(const GameCharacter& gc);//健康计算缺省算法`
`class GameChaaracter{`
`public:`
    `typedef int(*HealthCalcFunc)(const GameCharacter&);
    explicit GameCharacter(HealthCalcFunc hcf=defaultHealthCalc)
        :healthFunc(hcf){}
    int healthValue()const
    { return healthFunc(*this); }`
    `……`
`private:`
    `HealthCalcFunc healthFunc;//函数指针`
`};`

健康指数计算函数不再是GameCharacter可提供一个成员函数，这意味着这些计算函数并非特别访问即将被计算健康指数的那个对象的内部成分

其实这是Strategy设计模式的应用。和使用virtual函数比较，有更多弹性

* 同一人物类型之不同实体可以有不同的健康计算函数。也就是说同一人物类型不同的对象可以有不同的健康计算，例如射击游戏中，**一些购买防弹衣的玩家使用的对象**，血量可以减少更慢。
* 某已知人物健康计算函数可以在运行期间变更。即健康计算函数不再是GameCharacter继承体系内的成员函数。

实际上，class内的某个机能（也许是某个成员函数）替换为class的外部某个等价机能（例如某个non-member、non-friend函数或另个一class的non-friend成员函数），都有争议。

由外部函数访问内部成员时，有时需要弱化class的封装。这也就带来了缺点，但是优点是否足以弥补缺点，要视情况而定。

### 藉由tr1::function完成Strategy模式

如果习惯了templates以及它们对隐式接口的使用，基于函数指针的实现便有点死板了。

可以不用函数指针，而是用一个类型为tr1::function的对象。这样的对象可以有（保存）任何可调用物（callable entity，即函数指针、函数对象、成员函数指针），只要其签名式兼容于需求端。用tr1::function实现

## 36. 绝不重新定义继承而来的non-virtual函数

以一个例子来展开本条款阐述内容。假设class D是class B的派生类，class B中有一个public成员函数mf：

`class B{`
`public:`
    `void mf();`
    `……`
`};`

`class D: public B {`
`public:`
    `void mf();  //遮掩了B::mf。`
	`……`

`};`

`pB->mf();  //调用B::mf`
`pD->mf();  //调用D::mf）。`

之所以行为不一致，是因为non-virtual函数是静态绑定的。pB被声明为一个pointer-to-B，通过pB调用的non-virtual函数永远是B所定义的版本。但是virtual函数是动态绑定，所以virtual函数不受这个约束，即通过指针调用，实际调用的函数是指针真正指向对象的那个函数。

如果你打算在class D中重新定义继承自class B的non-virtual函数，D对象很可能会出现行为不一致行径。更明确一点，即任何一个D对象都可能表现出B或D的行为；决定因素不在对象自身，而在于“指向该对象之指针”当初声明类型。References也会展现出和指针一样难以理解的行径。

前面已经说过，public继承是is-a 关系（条款 32）。**条款**34说过，class内声明一个non-virtual函数会为该class建立一个不变性（invariant），它凌驾其特异性（specialization）。将这两个观点施行到class B和class D上以及non-virtual函数B::mf上，那么

适用于B对象的每一件事，也适用于D对象。（is-a 关系）
B的derived classes一定会继承mf的接口和实现，因为mf是一个non-virtual函数。

现在在D中重新定义mf，就会有矛盾。1、如果D真的有必要重新实现mf（不同于B的），那么is-a 关系就不成立，因为每个D都是B不再为真；既然这样，就不应该以public形式继承。2、如果D必须以public方式继承B，且D有需求实现不同的mf，那么久不能反映出不变性凌驾特异性；既然这样就应该声明为virtual函数。3、如果每个D是一个B为真，且mf真的可以反映出不变性凌驾特异性的性质，那么D久不需要重新定义mf了。

不论上面那个观点，结论都相同：任何情况下都不应该重新定义一个基础而来的non-virtual函数。

在条款 7已经知道，base class内的析构函数应该是virtual；如果你违反了条款 7，你也就违反了本条款，因为析构函数每个class都有，即使你没有自己编写。


## 37. 绝不要定义继承来的缺省参数值

在继承中，只能继承两种函数：virtual和non-virtual。在条款 36中我们学到，不能重新定义一个继承而来的non-virtual函数。本条款讨论的是继承virtual函数问题，再具体一点：**继承一个带有缺省参数值的virtual函数**。

我们应该知道，virtual函数是动态绑定（dynamically bound），缺省参数值却是静态绑定（statically bound）。

对象的静态类型（static type）是它在程序中被声明时采用的类型，例如

`class Shape{`
`public:`
    `enum ShapeColor{ Red, Green, Blue};`
    `virtual void draw(ShapeColor color=Red) const=0;`
    `……`
`};`
`class Rectangle: public Shape{`
`public:`
    **`virtual void draw(ShapeColor color=Green) const;//不同缺省参数值，很糟糕`**
    `……`
`};`
`class Circle: public Shape{`
`public:`
    `virtual void draw(ShapeColor color) const;`
    **`/*客户调用上面函数时，如果使用对象调用，必须指定参数值，因为静态绑定下这个函数不从base继承缺省值。*/`**
    **`/*如果使用指针或引用调用，可以不指定缺省参数值，动态绑定会从base继承缺省参数值*/`**
    `……`
`};`

这个继承很简单。现在这样使用

`Shape* ps;`
`Shape* pc=new Circle;`
`Shape* pr=new Rectangle;`

这些指针类型都是pointer-to-Shape类型，都是静态类型Shape*。对象的动态类型是指“目前所指对象类型”。动态类型可以表现出一个对象将会有什么行为。pc动态类型是Circle*，pr动态类型是Rectangle*，ps没有动态类型（它没有指向任何对象）。动态类型可以在执行过程中改变，重新赋值可以改变动态类型。

virtual函数是动态绑定的，调用哪一份函数实现的代码，取决于调用的那个对象的动态类型。

`pc->draw(Shape::Red);`
`pr->draw(Shape::Red);`

这样调用无可非议，都带有参数值。但是如果不带参数值呢

pr->draw();   //调用Rectangle::draw(Shape::Red)

上面调用中，**pr动态类型是Rectangle *，所以调用Rectangle的virtual函数。Rectangle::draw函数缺省值是GREEN，但是pr是静态类型Shape *，所以这个调用的缺省参数值来自Shape class**，不是Rectangle class。这次调用两个函数各出了一半的力。

C++之所以使用这么怪异的运作方式，是因为效率问题。如果缺省参数值动态绑定，编译器必须有某种办法在运行期为virtual函数决定适当的参数缺省值。这比目前实行的“在编译器决定”的机制更慢且更复杂。为了执行速度和编译器实现上的简易度，C++做了这样的取舍。

我们尝试遵守这个规则，给base class和derived class提供相同参数值

`class Shape{`
`public:`
    `enum ShapeColor{ Red, Green, Blue};`
    `virtual void draw(ShapeColor color=Red) const=0;`
    `……`
`};`
`class Rectangle: public Shape{`
`public:`
    `virtual void draw(ShapeColor color=Red) const;`
    `……`
`};`

这样问题又来了，**代码重复且带着相依性**（with dependencies）：如果Shape内缺省参数值改变了，那么derived classes的缺省参数值也要改变，否则就会导致重复定义一个继承而来的缺省参数值。

当时如果的确需要derived classes的缺省参数值，那么就需要替代方法。条款 35列出了一些virtual函数的替代方法，例如NVI手法：

`class Shape{`
`public:`
    `enum ShapeColor{ Red, Green, Blue};`
    `void draw(ShapeColor=Red) const`
    `{`
        `doDraw(color);`
    `}`
    `……`
`private:`
    `virtual void doDraw(ShapeColor color) const=0;//真正在这里完成工作`
`};`
`class Rectangle: public Shape{`
`public:`
    `……`
`private:`
    `virtual void draw(ShapeColor color) const;`
    `……`
`};`

因为non-virtual函数不会被derived覆写（**条款** 36），这个设计很清楚的使得draw函数的color缺省参数值总是Red。

**总结**

- 不要重新定义一个继承而来的缺省参数值，因为缺省参数值是静态绑定，而virtual函数（你唯一应该覆写的东西）是动态绑定。

## 38. 通过复合塑模出has-a或“根据某物实现出”

复合（composition）是类型之间的一种关系，**一个类型的对象包含其他类型对象**便是这种关系：

`class Address{ …… };`
`class PhoneNumber{ …… };`
`class Person{`
`public:`
    `……`
`private:`
    `std::string name;`
    `Address address;`
    `PhoneNumber mobilePhone;`
`};`

Person对象中包含string，Address，PhoneNumber对象，这就是复合。还有几个同义词：layering（分层），containment（内含），aggregation（聚合），embedding（内嵌）。

条款 32中提到，public是is-a关系，复合是has-a（有一个）或is-implemented-in-terms-of（根据某物实现出）。在程序中，大概可以分为两个领域（domains）。程序中对象相当于你所塑造现实世界中某物，例如地址、电话号码，这样的对象属于应用域（application domain）。还有一些是实现细节上的人工复制品，例如缓冲区（buffers）、互斥器（mutexes）、查找树（search tree）等，这些是实现域（implementation domain）。当复合发生在应用域对象之间时，表现出has-a关系；发生在实现域表现出is-implemented-in-terms-of关系。

区分is-a和is-implemented-in-terms-of比较麻烦。通过一个例子来说明，假设你需要一个template，用来构造一组classes来表示不重复对象组成的sets。首先我们想到用标准程序库提供的set template。

标准程序库的set由平衡查找树（balance search tree）实现，每个元素使用了三个指针的额外开销。这样可以使查找、插入、移除等操作时间复杂度为O(logN)（对数时间，logarithmic-time）。如果速度比空间重要，这样做合理，但是如果空间比速度重要，那么标准库提供的set将不满足我们需求。

set实现方法很多，可以在底层使用linked lists来实现，标准库中有list template，于是我们复用它。、

`template<typename T>`
`class Set: public std::list<T>`
`{`
    `……`
`};`

上面看起来很美好，其实是错误的。条款 32曾说过，public继承是is-a关系，即set是一种list并不对。例如set不能包含重复元素，但是list可以。

因为这两个classes之间并非is-a关系，所以public继承并不适用。set对象可以根据一个list对象来实现出来：

`template<calss T>`
`class Set{`
`public:`
    `bool member(const T& item) const;`
    `void insert(const T& item);`
    `void remove(const T& item);`
    `std::size_t size() const;`
`private:`
    `std::list<T> rep;`
`};`

`template<class T>`

`bool set<T>::member (const T& item) const`

`{`

​	`return std::find(rep.begin(),rep.end(),item)!=rep.end();`

`}`

`...`

只要熟悉list，便很快可以实现上面几个接口函数。

条款 18主张，接口容易被正确适用，不易被误用。这里没有让set遵循STL容器的协议，如果遵循的话，要为set添加许多东西，这就模糊了set和list之间的关系。这里只是澄清set和list之间的关系。

**总结**

复合（composition）的意义和public继承完全不同。
在应用域（application domain），复合意味has-a；在实现域（implementation domain），复合意味is-implemented-in-terms-of（根据某物实现出）。

## 39. 明智而审慎的使用private继承

public继承是**is-a**关系，条款32曾讲过并给出例子，如果把那个例子用private继承会怎样？

`class Person{……};`
`class Student: private Person{……};`
`void eat(const Person& p);`
`void study(const Student& s);`

`Person p;`
`Student s;`

`eat(p);`
`eat(s);`

上面的eat(s)会出错，因为private继承不是is-a关系。如**果继承关系是private，那么编译器不会自动将一个derived class对象转换为base class对象**；**继承base的所有成员，在derived class中都是private**。

private继承意味implemented-in-terms-of（根据某物实现出）。如果class D以private形式继承class B，我们的用意是采用class B内已经具备的某些特性。private继承纯粹只是一种实现技术（这也是为什么derived class中，base class成员都是private的：因为它们都只是实现枝节而已）。**private继承意味只有实现部分被继承，接口部分应略去**。D以private形式继承B，意思是D对象是根据B对象实现而得。

private继承意味is-implemented-terms-of（根据某物实现出），和**条款**38的复合意义相同。那么如何在两者之间取舍?答案是**尽可能的复合，必要时才使用private继承**。何时才是必要的？当protected成员或virtual函数牵扯进来的时候。还有一种激进情况，当空间方面厉害关系足以踢翻private继承支柱时，这个稍后讨论。

现在有个Widget class，我们想记录每个成员函数调用次数，在运行期间周期性审查这份信息。为了完成这项工作，需要用到定时器：

`class Timer{`
`public:`
    `explicit Timer(int tickFrequency);`
    `virtual void OnTick() const;//定时器滴答一次，此函数调用一次`
    `……`
`};`

上面是可以调整频率的订一起，每次滴答调用某个virtual函数，我们可以重新定义那个virtual函数，来取出Widget当时状态。为了重新定义Timer内的virtual函数，Widget必须继承Timer。因为Widget不是Timer，因此不适用public继承。还有一个观点支持不适用public，Widget对象调用onTick有点奇怪，会违反条款18：让接口容易被正确使用，不容易被误用。

`class Widget: private Timer{`
`private:`
    `virtual void onTick() const;//查看Widget的数据等操作`
    `……`
`};`

这个设计也可以用复合实现

`class Widget{`
`private:`
    `class WidgetTimer: public Timer{`
    `public:`
        `virtual void onTick() const;`
        `……`
    `};`
    `WidgetTimer timer;` 
    `……`
`}；`

这个设计稍微复杂一点，涉及到了public继承和复合，以及导入一个新class。我们有理由来选择这个复合版本，而不是private继承版本。

第一，Widget可能会有派生类，但是我们可能会想阻止在派生类中重新定义onTick。如果是使用private继承，上面的想法就不能实现，因为derived classes可以重新定义virtual函数（**derived classes可以重新定义virtual函数，即使它们无法调用它**）。如果采用复用方案，WidgetTimer是Widget内部的一个private成员，Widget的derived classes将无法取用WidgetTimer，自然也就无法继承或重新定义它的virtual函数了。

第二，采用复合方案，还可以降低编译依存性。如果Widget继承自Timer，当Widget编译时Timer的定义必须可见，所以Widget所在的定义文件必须包含Timer的定义文件。复合方案可以将WidgetTimer移出Widget，而只含有一个指针即可。

private继承主要用于“当一个意欲成为derived class者想访问一个意欲成为base class者的protected成分，或为了重新定义一个或多个virtual函数”。这时候，两个classes之间关系是is-implemented-in-terms-of，而不是is-a。有一种激进情况涉及空间最优化，会促使你选择private继承，而不是继承加复合。

这个情况真够激进，只适用于你所处理的class不带任何数据。它不包含non-static变量、virtual函数，没有继承virtual base class。这样的empty classes对象没使用任何空间，因为它没有任何数据对象要存储。但是因为技术原因，C++对象都必须有非零大小

`class Empty{}；`
`class HoldsAnInt{`
`private:`
    `int x;`
    `Empty e;`
`};`

sizeof(HoldsAnInt)>sizeof(int)。大多数编译器中，sizeof(Empty)为1，通常C++官网勒令安插一个char到对象内，但class大小还有字节对其需求。

“独立（非附属）”对象大小一定不为零，这个约束不适用于derived class对象内的base成分，因为它们不独立，如果继承Empty，而不是复合

`class HoldsAnInt: private Empty{`
`private:`
    `int x;`
`};`

这时，几乎可以确定sizeof(HoldsAnInt)==sizeof(int)。这是所谓的EBO（empty base optimization；空白基类最优化）。如果客户非常在意空间，那么使用EBO。EBO一般只在单一继承下才行，统治C++对象布局的那些规则通常表示EBO无法被施行余“拥有多个base”的derived classes身上。

empty class并不是真的empty。它们内往往含有typedef、enum、static或non-virtual函数。SLT有许多技术用途的empty classes，其中内含有的成员（通常是typedefs），包括base classes unary_function和binary_function，这些是“用户自定义之函数对象”，通常会继承的classes。

前面提到，只要可以尽可能选择复合，但这也不是全部。当面对并不存在is-a关系的两个classes，其中一个需要访问另一个的protected成员，或需要重新定义其一个或多个virtual函数，private继承可能成为正统设计策略。在考虑了其他方案后，仍然认为private继承是“表现两个classes之间的关系”的最佳办法，那就使用它。

**总结**

private继承意味着is-implemented-in-terms-of（根据某物实现出）。它通常比复合（composition）的级别低。但是当derived class需要访问protected base class的成员，或需要重新定义继承而来的virtual函数时，使用private是合理的。
和复合（composition）不同，private继承可以造成empty base最优化。这对致力于“对象占用空间最小化”的程序库开发者而言，可能很重要。

## 40. 明智而审慎的使用多重继承

多重继承（multiple inheritance：MI）在C++阵营中有不同主张。一个是：如果认为单一继承（single inheritance：SI）是好的，那么多重继承一定也是好的；另一个是：单一继承是好的，但是多重继承不是。本条款主要让大家了解这两个观点。

当使用MI时，程序可能从一个以上base classes继承相同名称（函数、typedef等），这会导致歧义（ambiguity）

`clas BorrowableItem{`
`public:`
    `void checkOut();`
    `……`
`};`
`class ElectronicGadgent{`
`private:`
    `bool checkOut() const;`
    `……`
`};`

`class MP3Player: public BorrowableItem, public ElectronicGadget`
`{`
    `……`
`};`
`MP3Player mp;`
`mp.checkOut();//歧义，调用哪个checkOut`

注意，虽然上面两个函数一个是public，一个是private，但还是有歧义。这与C++用来解析（resolving）重载函数调用的规则相符：在看到是否有个函数可取用之前，C++首先确认这个函数对此调用是最佳匹配。上面两个checkOut有相同匹配度。为了解决歧义，必须指明调用哪一个base class内的函数

`mp.BorrowableItem::checkOut();`

当然可以调用ElectronicGadget::checkOut，但你会得到一个“尝试调用private成员函数”错误。

多重继承还可能会导致“菱形继承”

`class File{……};`
`class InputFile: public File{……};`
`class OutputFile: public File{……};`
`class IOFile:public InputFile, public OutputFile{……};`

在上面这个继承体系中，IOFile会有File的两重拷贝，因为InputFile和OutFile都有一份拷贝，而IOFile有这两个class的拷贝。假如File class有个成员变量fileName，那么IOFile内有两个这样的数据。IOFile继承base classes，所以应该有两份，但是这两个变量是重复的，fileName只需一份即可。

C++在是否需要2份拷贝的辩论中没有立场，两个方案它都支持，缺省方案是第一个方案。如果要用第二个方案，那么就要让带此数据的class（即File）成为一个virtual base class。

`class File{……};`
`class InputFile: virtual public File{……};`
`class OutputFile:virtual public File{……};`
`class IOFile:public InputFile, public OutputFile`
`{……};`

C++标准库有一个多重继承体系，是个class templates。名字分别是basic_ios，basic_istream，basic_ostream和basic_iostream。

从上面例子看，好像public继承都应该是virtual。但是这个观点不正确。为了避免成员变量重复，编译器付出了代价，使用virtual继承的对象比non-virtual继承的对象体积大，访问virtual base classes成员变量时，也比访问non-virtual base classes成员变量速度慢。

virtual继承成本还包括virtual base classes初始化。virtual base的初始化由继承体系中最底层（most derived）class负责。这表示（1）classes派生自virtual bases需要初始化，必须认知其virtual bases；（2）当一个新的derived class加入继承体系中，它必须承担virtual bases（直接或间接）的初始化责任。

对virtual继承的忠告是：（1）**非必要时，不使用virtual继承，平时使用non-virtual继承。**（2）如果**必须使用virtual继承，那么尽量避免在base内放置数据**，这样就不用担心在这些classes身上的初始化和赋值带来的诡异事情了。Java和.NET的Interfaces在许多方面兼容C++的virtual base classes，它不允许带有任何数据。

现在看一下**条款**31中用来塑造人的Interface class

`class IPerson{`
`public:`
    `virtual ~IPerson();`
    `virtual std::string name() const=0;`
    `virtual std::string birthDate() const=0;`
`};`

客户使用IPerson的pointer或reference来编程，因为抽象类无法实例化。在创建对象时，IPerson的客户可以使用factory function（工厂函数）来将派生自IPerson的classes实例化。

`//根据ID创建一个Person对象`
`std::tr1::shared_ptr<IPerson> makePerson(DatabaseID personIdentifier);`
`//从使用者手上取得一个数据库的ID`
`DatabaseID askUserForDatabaseID();`

`DatabaseID id(askUserForDatabaseID());`
`std::tr1::shared_ptr<IPerson> pp(makePerson(id));`

假设makePerson创建的具体class为CPerson，那么CPerson必须提供继承自IPerson的pure virtual函数实现代码。假设现在还有与数据库相关的class PersonInfo，提供CPerson所需的一些东西

 `class PersonInfo{`
 `public:`
        `explicit PersonInfo(DatabaseID pid);`
        `virtual ~PersonInfo();`
        `virtual const char* theName() const;`
        `virtual const char* theBirthdayDate() const;`
        `……`
 `private:`
        `virtual const char* valueDelimOpen() const;`
        `virtual const char* valueDelimClose() const;`
        `……`
 `};`

对于我们来说这是个好消息，我们可以使用PersonInfo的一些接口来完成相关设计，但是有两个函数不满足要求，valueDelimOpen()和valueDelimClose()，这两个函数分别返回”[“和”]”，影响了其他函数，要重新定义。

这样可以得到CPerson和PersonInfo的关系是PersonInfo刚好有若干函数可以帮助CPerson比较容易来实现。它们关系是is-implemented-in-terms-of（根据某物实现出），这种关系可以由两种技术实现：复合和private继承。**条款**39提到复合是比较受欢迎的，但如果要重新定义virtual 函数，那么必须继承。本例中，要重新定义函数，所以无法使用复合。

CPerson也要实现IPerson接口，所以要public继承它。这导致了多重继承

`class IPerson{`
`public:`
    `virtual ~IPerson();`
    `virtual std::string name() const=0;`
    `virtual std::string birthDate() const=0;`
`};`
`class DatabaseID{……};`
`class PersonInfo{`
`public:`
    `explicit PersonInfo(DatabaseID pid);`
    `virtual ~PersonInfo();`
    `virtual const char* theName() const;`
    `virtual const char* theBirthdayDate() const;`
    `……`
`private:`
    `virtual const char* valueDelimOpen() const;`
    `virtual const char* valueDelimClose() const;`
    `……`
`};`
`class CPerson: public IPerson, private PersonInfo{`
`public:`
    `explicit CPerson(DatabaseID pid): PersonInfo(pid){}`
    `virtual std::string name() const`
    `{`
        `return PersonInfo::theName();`
    `}`
    `virtual std::string birthDate()`
    `{`
        `return PersonInfo::theBirthDate();`
    `}`
`private:`
    `const char* valueDelimOpen() const{return "";}`
    `const char* valueDelimClose() const{return "";}`
`};`

这个例子告诉我们多重继承也有它合理用途。

多重继承只是面向对象的一个工具而已。和单一继承比较，它比较复杂，也难以理解，所以如果有一个单一继承方案和一个多重继承方案，那么单一继承方案比较受欢迎。但是如果通过多重继承可以完成任务，而且最简洁、最易维护、最合理，那么就不用怕使用它。

总结

1. 多重继承比单一继承复杂。它可能导致新的歧义性，以及对virtual继承的需要。
2. virtual继承会在对象大小、速度、初始化、赋值造成成本增加。如果virtual base class不带任何数据，那么将是最具有使用价值情况。
3. 多重继承的确有正当用途。其中一个情节设计public继承某个Interface class和private继承某个协助实现的class。

## 41. 了解隐式接口和编译器多态

面向对象编程总是以显示接口（explicit int	rfaces）和运行期多态（runtime polymorphism）来解决问题。例如

 `class Widget{`
    `public:`
        `Widget();`
        `virtual ~Widget();`
        `virtual std::size_t size() const;`
        `virtual void normalize();`
        `void swap(Widget& other);`
        `……`
    `};`

`void doProcessing(Widget& w)`
`{`
    `if(w.size()>10 && w!=someNasyWidget)`
    `{`
        `Widget temp(w);`
        `temp.normalize();`
        `temp.swap(w);`
    `}`
`}`

可以这样说doProcessing内的w

w的类型被声明为Widget，所以w必须支持Widget接口。
Widget的某些成员函数是virtual，w对于这样函数的调用将表现出运行期多态（runtime polymorphism）。

在Templates和泛型编程的世界中，显示接口和运行期多态仍然存在，但是更要到的是隐式接口（implicit interface）和编译器多态（compile-time polymorphism）。将doProcessing从函数变为函数模板（function template）

`template<typename T>`
    `void doProcessing(T& w)`
    `{`
        `if(w.size()>10 && w!=someNasyWidget)`
        `{`
            `Widget temp(w);`
            `temp.normalize();`
            `temp.swap(w);`
        `}`
    `}`

现在再来看doProcessing内的w

w必须支持哪种接口，由template中执行于w身上的操作来决定。
凡涉及w的任何函数调用，例如operator>和operator!=，有可能造成template的具现化（instantiated，实体化），使这些调用得以成功。这样的局现化发生在编译期。以不同template参数具现化function template会导致调用不同的函数，这就是编译期多态（compile-time polymorphism）。

编译期多态和运行期多态不难区分，其区别类似于“哪一个重载函数应该被调用”和“哪一个virtual函数应该被绑定”

现在来看显式接口和隐式接口的区别。通常显式接口有函数的签名式（函数名称、参数类型、返回值）构成。

`class Widget{`
`public:`
    `Widget();`
    `virtual ~Widget();`
    `virtual std::size_t size() const;`
    `virtual void normalize();`
    `void swap(Widget& other);`
    `……`
`};`

**显式接口**由一个构造函数、一个析构函数、函数size，normalize、swap以及其参数、返回值、常量性（constness）构成，还包括编译器产生的copy构造函数和copy assignment操作符。（正常函数调用）

**隐式接口**就完全不同，它不是由函数签名决定，而是**由有效表达式（valid expression）组成**。（运算符）

`template<typename T>`
    `void doProcessing(T& w)`
    `{`
        `if(w.size()>10 && w!=someNasyWidget)`
        `{`
            `Widget temp(w);`
            `temp.normalize();`
            `temp.swap(w);`
        `}`
    `}`

可以看出T（w类型）的隐式接口好像有这些约束

1. 必须提供一个名为size的函数，该函数返回一个整数值
2. 必须支持一个operator!=函数，用来比较两个对象。

其实并不是必须满足这两个约束。T必须支持size成员函数，但是这个函数可能从base class继承。这个函数不需要返回一个整数值，甚至不需要返回一个定义有operator>的类型。它唯一要做的就是返回一个类型为X的对象，而X对象加上一个int（10的类型）必须能够调用一个operator>。这个operator>不需要非得取得一个类型为X的参数，它可以取得类型为Y的参数，只要存在一个隐式转换能够将类型X的对象转换为类型为Y的对象。同理T不需要支持operator!=。

（以上分析还没有考虑operator&&被重载，一个连接词的改变或许完全不同的某种东西，可能改变上述表达式的意义。）

第一次以此种方式思考隐式接口会感觉不习惯。隐式接口仅仅由一组有效表达式构成，这个表达式可能看起来很复杂，但它们要求的约束条件一般而言相当直接又明确，例如：

`if(w.size()>10 && w!=someNasyWidget)`

关于函数size、operator>、operator&&、operator!=身上的约束条件，很难再说太多；但整体确认表达式约束条件很容易。if表达式必须为布尔值，因此整体表达式必须与bool兼容。这时template doProcessing中类型参数T隐式接口的一部分，doProcessing要求其他隐式接口：copy构造函数、normalize和swap也必须对T型对象有效。

**加诸于template参数身上的隐式接口和加诸于class对象身上接口一样真实，都是在编译期完成检查**，如果template中使用不支持template所要求的隐式接口，代码不能编译通过

**总结**

1. class和template都支持接口和多态。
2. 对class的接口而言是显式的，以函数签名为中心；多态则是通过virtual函数发生于运行期。
3. 对template的参数而言，接口是隐式的，基于有效表达式。多态是通过template具体化和函数重载解析（function overloading resolution）发生在编译期。

## 42. 了解typename的双重意义

 使用模板时，可以用typename，也可以用class

 `template<class T> class Widget;`
 `template<typename T> class Widget;`

两者没有什么不同。作为template的类型参数，意义完全相同。在使用习惯上来说，很多人喜欢使用typename，因为这暗示参数并非一定要是个class类型。

C++有时不会把class和typename等价。有时候一定要用typename；为了了解原理，先来谈谈在template内指涉（refer to）的两种名称。

用一个例子来说明，现在有个template function，接收STL兼容容器为参数，容器内对象可被赋值为int，这个函数打印第二个元素的值

 `template<typename C>`
    `void print2nd(const C& container)`
    `{`
        `if(container.size()>=2)`
        `{`
            `C::const_iterator iter(container.begin());`
            `++iter;`
            `int valaue=*iter;`
            `std::cout<<value;`
        `}`
    `}`

现在解释一下两个local变量iter和value。iter类型为C::const_iterator,实际是什么必须取决于template参数C。template内出现的名称如果依赖于某个template参数，称这个名称为从属名称（dependent names）。如果从属名称在class内呈嵌套状，称之为嵌套从属名称（nested dependent name）。C::const_iterator就是这样的嵌套从属名称（nested dependent name），且指涉某类型。value是int类型，不依赖template参数，称之为非从属名称（non-dependent names）。

嵌套从属名称有可能导致解析（parsing）困难，例如在print2dn这样做

`template<typename C>`
`void print2nd(const C& container)`
`{`
    `C::const_iterator* x;`
    `……`
`}`

表面上看是声明了一个local变量x，x类型是只需C::const_iterator的指针，这是因为我们知道C::const_iterator是个类型。如果不是这样呢？例如，C内有个static成员变量碰巧命名为const_iterator，或x是个global变量；这样的话就是一个相乘动作：const::const_iterator乘以x。编写C++解析器的人必须操心这样的输入。

在知道C是什么之前，不能确定C::cont_iterator是否为一个类型。当编译器解析template print2nd时，不知道C是什么东西。C++有个规则可以解析这个歧义状态：如果解析器在template中遭遇到一个嵌套从属名称，它便假设这个明白不是个类型，除非你告诉它是。所以**在缺省情况下，嵌套从属名称不是类型**。这个规则还有个例外，稍后再提。

再来看看print2nd，C::const_iterator iter只有在C::const_iterator是个类型时才合理，但是C++缺省认为它不是。

要矫正这个形式，我们必须告诉C++C::const_iterator是个类型，在其前面放置关键字typename即可

 `template<typename C>`
    `void print2nd(const C& container)`
    `{`
        `if(container.size()>=2)`
        `{`
            **`typename C::const_iterator iter(container.begin());`**
            `++iter;`
            `int valaue=*iter;`
            `std::cout<<value;`
        `}`
    `}`

任何时候当你想要在tempolate中指涉一个嵌套从属类型名称，就必须在其前一一个位置放上关键字typename

“typename必须作为嵌套从属类型名称的前缀词”这一规则的例外是：typename不可以出现在base class list内的嵌套从属类型名称之前，也不可以在member initialization list（成员初值列）中作为base class修饰符。

`template<typename T>`
`class Derived: public Base<T>::Nested{ //base class list中不允许typename`
`public:`
    `explicit Derived(int x): Base<T>::Nested(x)  //mem init.list中不允许typename`
    `{`
        `typename Base<T>::Nested temp; //嵌套从属名称类型`
        `……  //既不在base class list中，也不再mem init list中，作为base class修饰符要加上typename`
    `}`
    `……`
`};`

再来看一个typename例子。一个function template接受一个迭代器，我们打算为该迭代器指涉的对象做一份local复件（副本）temp

`template<typename IterT>`
`void workWithIterator(IterT iter)`
`{`
    `typename::std::iterator_traits<IterT>::value_type temp(*iter);`
    `……`
`}`

typename::std::iterator_traits<IterT>::value_type是标准traits class（**条款**47）的一种运用。std::iterator_traits<IterT>::value_type是个嵌套从属类名称，所以在其前面放置typename。如果使用std::iterator_traits<IterT>::value_type感觉太长不习惯，可以考虑建立一个typedef，对于traits成员名称，普遍的习惯是设定typedef名称代表某个traits成员名称。

​    `template<typename IterT>`
​    `void workWithIterator(IterT iter)`
​    `{`
​        `typedef typename::std::iterator_traits<IterT>::value_type  value_type;`
​        `value_type temp(*iter);`
​        `……`
​    `}`

最后提一下，typename相关规则在不同编译器上有不同的实践。这意味着typename和嵌套从属名称之间的互动，也许会在移植性方面带来头疼的问题。

**总结（typename的双重意义）**

1. 声明template参数时，前缀关键字class和typename可以互换。
2. 使用关键字typename标识嵌套从属类型名称；但不得在base class list（基类列）或member initialization list（成员初值列）内以它作为base class修饰符。

## 43. 学习模板化基类内的名称

模板化的类作为基类时，有哪些要注意的地方。以一个例子说明，假设现在编写一个发送信息到不同公司的程序，信息要么译成密码，要么就是原始文字，在编译期间来决定哪一家公司发送至哪一家公司，采用template手法：

`class CompanyA{`
`public:`
    `void sendCleartext(const std::string& msg);`
    `void sendEncryted(const std::string& msg);`
    `……`
`};`
`class CompanyB{`
`public:`
    `void sendCleartext(const std::string& msg);`
    `void sendEncryted(const std::string& msg);`
    `……`
`};`
`……//还有一些公司`

`class MsgInfo{……};//用来保存信息，以备将来产生信息`

`template<typename Company>`
`class MsgSender{`
`public:`
    `……//构造析构等函数`
    `void sendClear(const MsgInfo& info)`
    `{`
        `std::string msg;`
        `//根据info产生信息`
        `Company c;`
        `c.sendCleartext(msg);`
    `}`
    `void sendSecart(const MsgInfo& info)`
    `{……}`
`};`

上面这个做法行得通，但是如果要在每次送出信息时记录日志，可以派生出derived class，加上记录的日志

`template<typename Company>`
`class LoggingMsgSender: public MsgSender<Company>`
`{`
`public:`
    `……//析构构造等`
    `void SendClearMsg(const MsgInfo& info)`
    `{`
        `//发送前的信息写到log`
        `sendClear(info);//调用base class函数，这段代码无法通过编译`
        `//传送后信息写到log`
    `}`
`};`

在派生类中，sendClearMsg和base class中的sendClear不同，这是个好设计，避免遮掩继承而得的名称，也避免了重新定义一个继承而得non-virtual函数。

但是上述代码编译时不能通过，因为编译器看不到sendClear函数。我们能看到sendClear的确在base class中，编译器却看不到，为什么？

因为当编译器遇到class template LoggingMsgSender定义式时，不知道它继承什么样的class，因为MsgSender中的Company是个参数，在LoggingMsgSender被具体化之前，无法确切知道它是什么，自然而然就不知道class MsgSender是什么，也就不知道它是否有个sendClear函数。（备注，在g++中可以使用参数 -fpermissive参数来通过编译，但是不建议使用）

为了让问题更具体化，假设现在有个class CompanyZ坚持使用加密通讯

`class CompanyZ{`
`pubic:`
    `void sendEncryted(const std::sting& msg);`
    `……`
`};`

CompanyZ没有sendClear函数，一般性的MsgSender template对CompanyZ并不合适，这时我们可以针对CompanyZ产生一个MsgSender特化版

**`template<>`**
**`class MsgSender<CompanyZ>{`   //全特化**
`public:`
    `void sendSecret(const MsgInfo& infof)`
    `{……}`
    `……`
`}；`

开头的template<>表示是特化版的MsgSender template，在template实参是CompanyZ时被使用。这就是所谓的模板全特化（total template specialization）：template MsgSender针对类型CompanyZ特化了，且是全面性特化，即一旦类型参数为CompanyZ，没有其他template参数可供变化了。

再来看一下刚刚的LoggingMsgSender

`template<typename Company>`
`class LoggingMsgSender: public MsgSender<Company>`
`{`
`public:`
    `……//析构构造等`
    `void SendClearMsg(const MsgInfo& info)`
    `{`
        `//发送前的信息写到log`
        `sendClear(info);//如实Company是CompanyZ，那么这个函数不存在`
        `//传送后信息写到log`
    `}`
`};`

如果Company=CompanyZ，那么sendClear函数就不存在。**C++拒绝调用这个函数是因为它知道base class template可能被特化，而那个特化版本可能不提供和一般性template相同的接口（可能在全特化版本中被调用的某一接口被删除了）**。所以它往往拒绝在templatized base classes（模板化基类）中寻找继承而来的名称。**当我们从Object Oriented C++跨进到Template C++时，继承不像以前那样畅行无阻了**。

现在应该讨论一下怎么解决上面不能通过编译的问题了。我们必须有某种办法令C++“不进入templatized base class观察”的行为失效。有一下三种办法

1、在base class函数调用动作之前加上“this->”

`template<typename Company>`
`class LoggingMsgSender: public MsgSender<Company>`
`{`
`public:`
    `……//析构构造等`
    `void SendClearMsg(const MsgInfo& info)`
    `{`
        `//发送前的信息写到log`
        **`this->sendClear(info);`**
        `//传送后信息写到log`
    `}`
`};`

2、使用using声明式，有点类似条款33。

`template<typename Company>`
`class LoggingMsgSender: public MsgSender<Company>`
`{`
`public:`
    `……//析构构造等`
    **`using MsgSender<Company>::sendClear;//告诉编译器，假设sendClear位于base class内`**
    `void SendClearMsg(const MsgInfo& info)`
    `{`
        `//发送前的信息写到log`
        `sendClear(info);//可以编译通过，假设sendClear将被继承`
        `//传送后信息写到log`
    `}`
`};`

（补充一下，这里情况和**条款**33不同，这里不是将被掩盖的base class名称带入一个derived class作用域内，而是编译器不进入base class作用域内查找，通过using告诉编译器，请它去查找。若写一个与该函数名称相同的函数，会与base class进行重载）

3、明白指出被调用的函数位于base class内

`template<typename Company>`
`class LoggingMsgSender: public MsgSender<Company>`
`{`
`public:`
    `……//析构构造等`
    `void SendClearMsg(const MsgInfo& info)`
    `{`
        `//发送前的信息写到log`
        **`MsgSender<Company>::sendClear(info);//可以编译通过，假设sendClear被继承`**
        `//传送后信息写到log`
    `}`
`};`

这种做法使用了明确资格修饰符（explicit qualification），这将会关闭virtual绑定行为。

回头再看上面三种做法，原理相同：**对编译器承诺“base class template的任何特化版本都将支持其一般（泛化）版本所提供的接口”**。这样的承诺是编译器在解析（parse）像LoggingMsgSender这样的derived class template时需要的。但如果这个承诺稍后没有兑现，编译器还会给事实一个公道。例如，稍后源码内：

`LoggingMsgSender<CompanyZ> zMsgSender;`
`MsgInfo msgData;`
`zMsgSender.sendClearMsg(msgData);//错误，无法编译通过`

在调用sendClearMsg这个点上，编译器直到base class是个template特化版本，且它直到这个特化版不提供sendClearMsg函数。

总结一下，本条款讨论的是，面对指涉base class member之无效references，编译器的诊断时间可能发生在早期（当解析derived class template定义时），也可能发生在晚期（当template被特定值template实参具体化时）。C++宁愿较早诊断，这也是为什么当base classes从template具体化时，它假设对那些base classes的内容毫无所悉的缘故。

**总结**

可在derived class templates内通过this->指涉base class templates内的成员名称，或藉由一个明白写出base class资格修饰符完成。

## 44. 将与参数无关的代码抽离templates

Templates可以节省时间和避免代码重复。对于类似的classes或functions，可以写一个class template或function template，让编译器来做剩余的事。这样做，有时候会导致代码膨胀（code bloat）：其二进制码带着重复（或几乎重复）的代码、数据，或者两者都有，但这时候源代码看起来可能很整齐。所以你需要知道如何避免这样的二进制浮夸？

先来学习一个名词：共性与变性分析（commonality and variability analysis）。比较容易理解。例如，你在编写几个函数，会用到相同作用的代码；这时候你往往将相同代码搬到一个新函数中，给其他几个函数调用。同理，如果编写某个class，其中某些部分和另外几个class相同，这时候你不会重复编写这些相同部分，只需把共同部分搬到新class中去即可，去使用继承或复合，让原先的classes取用这些共同特性，原classes的互异部分（变异部分）仍然留在原位置不动。

编写templates时，也要做相同分析，避免重复。non-template代码中重复十分明确：你可以看到两个函数或classes之间有所重复。但是在template代码中，重复是隐晦的，因为只有一份template源码。

例如，你打算在为尺寸固定的正方矩阵编写一个template，该矩阵有个支持逆矩阵运算的函数

 t`emplate<typename T, std::size_t n>//T为数据类型，n为矩阵大小`
    `class SquareMatrix{`
    `public:`
        `……`
        `void invert();//求逆运算`
    `};`

​    `SquareMatrix<double,5> sm1;`
​    `sm1.invert();//调用SquareMatrix<double,5>::invert`
​    `SquareMatrix<double,10> sm2;`
​    `sm2.invert();//调用SquareMatrix<double,10>::invert`

上面会具体化两份invert。这两份函数几乎完全相同（除了一个操作5 * 5矩阵，一个操作10 * 10）。这就是代码膨胀的一个典型例子。

上面两个函数除了操作矩阵大小不同外，其他相同。这时可以为其建立一个带数值的函数，而不是重复代码。于是有了对SquareMatrix的第一份修改：

 `template<typename T>`
    `class SquareMatrixBase{`
    `protected:`
        `void invert(std::size_t matrixSize);`
        `……`
    `};`

​    `template<typename T, std::size_t n>`
​    `class SquareMatrix:private SquareMatrixBase<T>{`
​    `private:`
​        `using SquareMatrixBase<T>::invert();//编码遮掩base中的invert`
​    `public:`
​    `……`
​        `void invert()//求逆运算`
​            `{`
​                `this->invsert(n);//稍后解释为什么用this`
​            `}`
​    `};`

SquareMatrixBase::invert只是企图避免derived classes代码重复，所以它以protected替换public。这个函数使用this->，因为模板化基类内的函数名称会被derived classes掩盖（条款**43）。注意，SquareMatrixBase和SquareMatrix之间继承关系是private，这说明base class是为了帮助derived classes实现，两者不是is-a关系**。

现在还有一个问题，SquareMatrixBase::invert操作的数据在哪？它在参数中直到矩阵大小，但是矩阵数据derived class才知道。derived class和base class如何联络？一个做法是可以为SquareMatrixBase::invert添加一个参数（例如一个指针）。这个行得通，但是考虑到其他因素（例如，SquareMatrixBase内还有其他函数，也要操作这些数据），可以把这个指针添加到SquareMatrixBase类中。

`template<typename T>`
`class SquareMatrixBase{`
`protected:`
    `SquareMatirxBase(std::size_t n,T* pMem)`
    `:size(n), pData(pMem){}`
    `void setDataPtr(T* ptr) {pData=ptr;}`
    `……`
`private:`
    `std::size_t size;`
    `T* pData;`
`};`

`template<typename T, std::size_t n>`
`class SquareMatrix:private SquareMatrixBase<T>{`
`public:`
    `SquareMatrix()`
    `:SquareMatrixBase<T>(n, data){}`
    `……`
`private:`
    `T data[n*n];`
`};`

这种类型的对象不需要动态分配内存，但是对象自身可能非常大。另一个做法是把矩阵数据放到heap

`template<typename T, std::size_t n>`
`class SquareMatrix:private SquareMatrixBase<T>{`
`public:`
    `SquareMatrix()`
    `:SquareMatrixBase<T>(n, 0),`
    `pData(new T[n*n])`
    `{this->setDataPtr(pData.get());}`
    `……`
`private:`
    `boost::scoped_array<T> pData;`
`};`

这样以来，类型相同的derived classes会共享base class。例如，SquareMatrix

## 45. 运用成员函数模板接受所有兼容类型

在模板中，具体化模板参数后的类不会因为具体化类型而存在派生关系。真实指针支持隐式转换（implitic conversions）：derived class指针可以隐式转换为base class指针，指向non-const对象的指针可以转换为指向const对象的指针，等等。例如：

`class Top{……}；`
`class Middle: public Top{……};`
`class Bottom:public Middle{……};`

`Top* pt1=new Middle;//Middle* 转换为Top*`
`Top* pt2=new Bottom;//Bottom* 转换为Top*`
`const Top* pct2=pt1;//Top* 转换为const Top*`

如果使用模板定义智能指针，上面的转换就有点麻烦了

`template<typename T>`
`class SmartPrt{`
`public:`
    `explicit SmartPtr(T* realPtr);`
    `……`
`};`
`SmartPtr<Top> pt1=SmartPtr<Middle>(new Middle);  //SmartPtr<Middle>转换为SmartPtr<Top>`
`SmartPrt<Top> pt2=SmartPrt<Bottom>(new Bottom);`
`SmartPrt<const Top> pct2=pt1;`

同一个template的不同具体化之间不存在什么关系，即使具体化的两个类型之间有继承、派生关系。编译器把SmartPtr<Middle>和SmartPtr<Top>视为完全不同两种类型的classes。为了让上面代码编译通过，获得SmartPtr classes之间的转换能力，必须明确的把它们编写出来。

### Templates和泛型编程

要想实现转换，可以**在智能指针的构造函数中完成**

但是如果派生类有继续派生，那么构造函数又要添加，这显然不合理。因此，我们需要的不是简简单单为SmartPtr写构造函数，而是**编一个构造模板**。这么的模板是所谓的**member function template**（简称member templates），作用是为class生成函数

t`emplate<typename T>`
`class SmartPrt{`
`public:`
    **`template<typename U>`**
    **`SmartPtr(const SmartPrt<U>& other);//member template，为了生成copy cotr`**
    `……`
`};`

以上代码意思是，对任何类型T和任何类型U，可以根据SmartPrt<U>生成一个SmartPtr<T>。这一类构造函数根据对象U创建对象T，而U和T是同一个Template的不同实例，我们称这种函数为**泛化构造函数**。

上面的泛化构造函数没有声明为explicit，因为转换可能是隐式的。因为原始指针之间的转换（如derived class指针转为base class指针）是隐式转换，无需明白写出转型动作，所以让智能指针效仿这种行为也属合理。

这个为SmartPtr而写的泛化构造函数提供的东西比我们需要的更多。我们希望根据一个SmartPtr创建一个Smartprt，却不希望根据一个SmartPtr创建一个SmartPtr，因为对于public继承来说是矛盾的。

上述代码并不完整，在SmartPtr没有实现copy cotr。假设SmartPtr像auto_ptr和tr1::shared_ptr一样，提供get成员函数，返回智能指针对象，那么就可以在构造模板中约束转换行为

`template<typaname T>`
`class SmartPtr{`
`public:`
    `template<typename U>`
    `SmartPrt(const SmartPrt<U>& other)`
    `:heldPrt(other.get()){};`
    `T* get() const{return heldPrt;}`
    `……`
`private:`
    `T* heldPrt;`
`};`

 在上述代码中，存一个隐式转换：将U* 转换为 T*，这限制了转换行为。

member function templates作用不仅仅在于构造函数，还有一个重要作用是支持赋值操作。例如TR1的shared_ptr支持所有来自兼容之内置指针、tr1::shared_ptrs、auto_ptrs和tr1::weak_ptrs的构造行为，以及来自上述各物（tr1::weak_ptr除外）的赋值操作。来看一下TR1规范中关于tr1::shared_ptr的一份摘录

`template<class T>`
`class shared_ptr{`
`public:`
    `template<class Y>`
        `explicit shared_ptr(Y* p);`
    `template<class Y>`
        `shared_ptr(shared_ptr<Y> const& r);`
    `template<class Y>`
        `explicit shared_ptr(weak_ptr<Y> const& r);`
    `template<class Y>`
        `explicit shared_ptr(auto_ptr<Y> const& r);`
    `template<class Y>`
        `shared_ptr& operator=(shared_ptr<Y> const& r);`
    `template<class Y>`
        `shared_ptr& operator=(auto_ptr<Y> const& r);`
    `……`
`};`

上面除了泛化copy构造函数外，其他构造函数都是explicit，表示shared_ptr类型隐式转换被允许，但是从其他智能指针隐式转换为shared_ptr不被允许。

member function templates并不改变语言基本规则，和编译器产生copy构造函数以及copy assignment不冲突。tr1:shared_ptr声明了一个泛化的copy构造函数，如果T和Y相同，泛化的copy构造函数会被具体化为正常的copy构造函数。编译器会暗自为tr1::shared_ptr生成一个copy构造函数？还是当tr1::shared_ptr对象根据另一个同类型的tr1::shared_ptr对象展开构造行为时，编译器会将泛化的copy构造函数模板具体化呢？

member templates没有改变语言规则，如果程序需要一个copy构造函数，你却没有声明它，编译器就会替你生成。在class内声明泛化copy构造函数并不阻止编译器生成它们自己的copy构造函数（non-template）。如果想要控制copy构造函数的方方面面，就要声明正常的copy构造函数。相同的规则也适用于赋值assignment操作。

总结
- **请使用member function templates（成员函数模板）生成“可接受所有兼容类型”的函数**。
- 如果声明member templates用于泛化copy构造函数或泛化assignment操作，还是要声明正常的copy构造函数和copy assignment操作符。


## 46. 需要类型转换时请为模板定义非成员函数

**条款** 24提到过为什么non-member函数才有能力“在所有实参身上实施隐式类型转换”，本条款接着那个Rational例子来讲，把Rational class模板化

`template<typename T>`
`class Rational{`
`public:`
    `Rational(const T& numerator=0,const T& denominator=1);`
    `const T numerator() const;`
    `const T denominator() const;`
    `……`
`};`
`template<typename T>`
`const Rational<T> operator*(const Rational<T>& lhs,const Rational<T>& rhs)
{……};
Rational<int> oneHalf(1,2);
Rational<int> result=oneHalf*2;//错误，无法通过编译`

非模板的例子可以通过编译，但是模板化的例子就不行。在条款24，编译器知道我们尝试调用什么函数（就是接受两个Rational参数那个operator ），但是这里编译器不知道。编译器试图想什么函数被命名为operator * 的template具体化出来，它们知道自己可以具体化某个operator * 并接受两个Rational参数的函数，但为完成这一具体化行动，必须先算出T是什么。问题是它们没这个能耐。

看一下这个例子，编译器怎么推导T。本例中类型参数分别是Rational和int。operator* 的第一个参数被声明为Rational，传递给operator* 的第一实参(oneHalf)正类型正是Rational，所以T一定是int。operator* 的第二个参数类型被声明为Rational，但传递给 operator* 的第二个实参类型是int，编译器如何推算出T？或许你期望编译器使用Rational的non-explicit构造函数将2转换为Rational，进而推导出T为int，但它不这么做，因为**在template实参推导过程中从不将隐式类型转换考虑在内**。隐式转换在函数调用过程中的确被使用，但是在能够调用一个函数之前，首先要知道那个函数的存在；为了知道存在，必须先为相关的function template推导出参数类型（然后才可以将适当的函数具体化出来）。但是在template实参推导过程中不考虑通过构造函数发生的隐式类型转换。

现在解决编译器在template实参推导方面遇到的挑战，可以使用template class内的friend函数，因为template class内的**friend声明式可以指涉某个特定的函数**。也就是说class Rational可以说明operator* 是它的friend函数。class templates并不依赖template实参推导（后者只施行于function templates身上），所以编译器总是能够在class Rational具体化时得知T。所以令Rational class声明适当的operator*为friend函数，可以简化整个问题。

​    `template<typename T>`
​    `class Rational{`
​    `public:`
​        `……`
​        `friend const Rational operator*(const Rational& lhs,const Rational& rhs);  //声明
​    };`
​    `template<typename T>`
​    `const Rational<T> operator*(const Rational<T>& lhs,const Rational<T>& rhs)  {……};  //定义`

这时候对operator* 的混合调用可以通过编译了。oneHalf被声明为一个Rational，class Rational被具体化出来，而作为过程的一部分，friend函数operator* (接受Rational参数）也就自动声明出来。后者身为一个函数而非函数模板，因此编译器在调用它的时候使用隐式转换（将int转换为Rational），所以混合调用可以通过编译。虽然通过编译，但是还会有链接问题，这个稍后再谈。先来看一下Rational内声明operator *的语法。

在一个class template内，template名称可被用来作为template和其参数的简略表达方式，所以在Rational内，我们可以把Rational<int> 简写为Rational。如果像下面这样写，一样有效

`template<typename T>`
`class Rational{`
`public:`
    `……`
    `friend const Rational operator*(const Rational<T>& lhs,const Rational<T>& rhs);//声明`
`};`

现在回头看一下刚刚说的链接的问题。虽然编译器直到我们调用的函数是接受Rational<int> 的那个operator * ，但是这个函数只有声明，没有定义。我们本来想让此class外部的operator * 提供定义式，但是这样行不通。如果我们自己声明了一个函数（Rational template内的作为），就有责任定义那个函数。如果没有定义，链接器就找不到它。一个最简单的办法就是将operator * 的定义合并到其声明内：

t`emplate<typename T>`
`class Rational{`
`public:`
    `……`
    `friend const Rational operator*(const Rational& lhs,const Rational& rhs);//声明+定义
    {
        return Rational（lhs.numerator()*rhs.numerator(),lhs.denominator()*rhs.denominator());`
    `}`
`};`

**这个技术虽然使用了friend，却与传统的friend用途“访问class的non-public成员”不同。为了让类型转换可能发生与所有实参身上，我们需要一个non-member函数（条款24）；为了让这个函数被自动具体化，我们需要将它声明在class内部；而在class内部声明non-member函数的唯一办法就是让它成为一个friend。**

定义在class内部的函数都是inline函数，包括像operator * 这样的friend函数。为了将inline声明带来的冲击最小化，可以让operator * 调用定义在class外部的辅助函数。


`template<typename T> class Rational;   //forward decelarion`
`template<typename T>`
`const Rational<T> doMultiply(const Rational<T>& lhs,const Rational<T>& rhs);`
`template<typename T>`
`class Rational{`
`public:`
    `……`
    `friend const Rational operator*(const Rational& lhs,const Rational& rhs);//声明+定义`
    `{`
        `return doMultiply(lhs,rhs);`
    `}`
`};`

许多编译器会强迫你把template定义式放到头文件，所以有时你需要在头文件定义doMultiply

doMultiply是个template，自然不支持混合乘法，其实也没必要支持。它只是被operator * 调用，operator * 支持了混合乘法。

**总结**

**当编写一个class template时，它所提供之“与此template相关的”函数支持“所有参数之隐式类型转换”时，请将那些函数定义为class template内部的friend函数。**

## 47. 请使用traits class表现类型信息

**当函数，类或者一些封装的通用算法中的某些部分会因为数据类型不同而导致处理或逻辑不同（而我们又不希望因为数据类型的差异而修改算法本身的封装时），traits会是一种很好的解决方案。**

STL主要由容器、迭代器和算法的templates构成，也包括若干工具性templates，其中有一个advance用来将迭代器移动某个给定距离：

`template<typename IterT, typename DistT>`
`void advance(IterT& iter, DistT d);  //d大于零，迭代器向前移动d位，小于零则向后移动d位`

表面上看，只是iterate+=d的动作，但是迭代器有5种，只有random access（随机访问）迭代器才支持+=操作。其他类型没这么大威力，只有反复++和--才行。

STL源码中关于迭代器的部分可以参考这里。这里也回顾一下这5中迭代器。

```
1.input迭代器，它是read only，只能读取它指向的对象，且只能读取一次。它只能向前移动，一次一步。它模仿指向输入文件的阅读指针（read pointer）；C++程序中的istream_iterators就是这类的代表。
2.output迭代器，和input迭代器相反，它是write only。它也是只能向前移动，一次一步，且只能涂写一次它指向的对象，它模仿指向输出文件的涂写指针（write pointer）；ostream_iterators是这一类代表。
3.forward迭代器。这个迭代器派生自input迭代器，所以有input迭代器的所有功能。并且他可以读写指向的对象一次以上。
4.bidirectional迭代器继承自forward迭代器，它的功能还包含向后移动。STL中的list、set、multiset、map、和multimap迭代器就是这一类迭代器。
5.random access迭代器继承自bidirectional迭代器。它厉害的地方在于可以向前或向后跳跃任意距离，这点类似原始指针，内置指针就可以当做random access迭代器使用。vector、deque和string的迭代器就是这类。

```



这5中分类，C++标准程序库提供专属卷标结构（tag struct）加以确认：

    struct input_iterator_tag {};  
    struct output_iterator_tag {};  
    struct forward_iterator_tag : public input_iterator_tag {};  
    struct bidirectional_iterator_tag : public forward_iterator_tag {};  
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

这些迭代器之间的继承关系都是有效的is-a关系

在了解了迭代器类型后，我们该去实现advance函数了。实现要高效，对于random access迭代器来说，前进d距离要一步完成，而其他类型则需要反复前进或后退

    template<typename Iter, typename DistT>
    void advance(IteT& iter,DistT d)
    {
        if(iter is a random access iterator)
            iter+=d;
        else
        {
            if(d>=0)
                while(d--) ++iter;
            else 
                while(d++) --iter;
        }
    }

在上面实现中要判断iter是否为random access迭代器，即要知道IterT类型是否为random access类型。这就需要Traits,它允许我们在编译期间获取某些类型信息。

**Traits**是一种技术，是C++程序员共同遵守的协议。这个技术要求之一就是，它对内置类型和自定义类型表现的一样好。traits必须能够施行于内置类型，意味着“类型内的嵌套信息”这种东西出局了，因为我们无法将信息嵌套于原始指针内。所以类型的traits信息必须位于类型自身之外。标准技术是把它放进一个template及其一个或多个特化版本中。这样的templates在STL中有若干个，其中针对迭代器的为iterator_traits:

    template<typename IterT>//用来处理迭代器分类
    struct iterator_traits;

虽然iterator_traits是个struct，往往称作traits classes。其运作方式是，**针对每一个类型IterT，在struct iterator_traits内声明某个typedef命名为iterator_category（迭代器类别），用来确认IterT的迭代器分类**。

iterator_traits以两个部分实现上述所言。1、它要求用户自定义的迭代器嵌套一个typedef，名为iterator_category，用来确认是哪个卷标结构（tag struct），例如deque和list

```
template<typename T>
    class deque{
    public:
        class iterator{
        public:
            typedef random_access_iterator_tag iterator_category;
            ……
        };
        ……
    };
    
template<typename T>
class list{
public:
    class iterator{
    public:
        typedef bidirectional_iterator_tag iterator_category;
        ……
    };
    ……
};

template<typename IterT>   //IterT的iterator_category就是用来表现IterT说自己是什么
struct iterator_traits{
    //typedef typename的使用，见**条款**42
    typedef typename IterT::iterator_category iterator_category;
    ……
};
```

这样对用户自定义类型行得通，但是对指针行不通，指针也是迭代器，但是指针不能嵌套typedef。下面就是iterator_traits的第2部分了，专门用来支持指针。

为了支持指针迭代器，iterator_traits特别针对类型提供一个偏特化版本（partial template specialization）。

    template<typename IterT>
    struct iterator_traits<IterT*>//针对内置指针
    {
        typedef random_access iterator_tag iterator_category;
        ……
    };

现在可以直到实现一个traits class步骤了

    1.确认若干我们希望将来可取得的类型相关信息。对于迭代器来说，就是可以取得其分类。
    2.为该信息选择一个名称。对于迭代器是iterator_category。
    3.提供一个template和一组特化版本，内含你希望支持的类型和相关信息。

现在可以实现一下advance了
    

```
template<typename IterT, typename DistT>
    void advance(IterT& iter,DisT d)
    {
        if(typeid(typename std::iterator_traits<IterT>::iterator_category)==
        typeid(std::random_access_iterator_tag))
        ……
    }
```

虽然逻辑是正确，但并非是我们想要的，抛开编译问题，还有一个更根本的问题：IterT类型在编译期间获知，所以iterator_traits::iterator_category在编译期间确定。但是if语句却是在运行期间核定。可以在编译期间完成的事情推到运行期间，这不仅浪费时间，还造成执行文件膨胀。

要在编译期间确定，可以使用重载。重载是在编译期间确定的，编译器会找到最匹配的函数来调用

    template<typename IterT, typename DisT>
    void doAdvance(IterT& iter, Dist d, std::random_access_iterator_tag)
    {
        iter+=d;
    }
    template<typename IterT, typename DisT>
    void doAdvance(IterT& iter, Dist d, std::bidirectional_iterator_tag)
    {
        if(d>=0)
        while(d--) ++iter;
    else 
        while(d++) --iter;
    }
    template<typename IterT, typename DisT>
    void doAdvance(IterT& iter, Dist d, std::input_iterator_tag)
    {
        if(d<0)
        throw std::out_of_range("Negative distance");
        while(d++) --iter;
    }
    
    template<typename IterT,typename DistT>
    void advance(IterT& iter,DistT d)
    {
        doAdvance(iter,d,typename::std::iterator_traits<IterT>::iterator_category();
    }

因为forward_iterator_tag继承自input_iterator_tag，所以input_iterator_tag版本的函数可以处理forward迭代器，这是因为public继承是is-a关系。

现在来总结一下如何使用traits class

    1.建立一组重载函数或函数模板（例如doAdvance），彼此间差异只在于各自的traits参数。每个函数实现与之接受的traits信息像匹配。
    2.建立一个控制函数或函数模板（例如advance），调用上面的函数并传递traits class信息。

Traits广泛应用在STL，除了上面所说的iterator_traits，还有char_traits用来保存字符类型相关信息，numeric_limits用来保存数值类型相关信息。

TR1（**条款**54）导入许多新的traits classes用来提供类型信息，例如is_fundamental判断T是否是内置类型，is_array判断T是否为数组，is_base_of


**请记住：**

1. Traits class使得”类型相关信息“在编译器可用，它们以templates和”tempates特化“完成实现
2. 整合重载技术后，traits clas有可能在编译期对类型执行if-else测试

## 48. 认识template元编程

Template metaprogramming(TMP,模板元编程）是编写template-based C++程序并执行于编译期的过程。元编程就是编写直接生成或操纵程序的程序。template metaprogramming是用C++写的模板程序，编译器编译出具体化的过程。也就是说，**TMP程序执行后，从templates具体化出来C++源码，不再是模板了。**

TMP有两个作用，一是它让某些事更容易。例如编写STL容器，使用模板，可以存放任何类型元素。二是将执行在运行期的某些工作转移到了编译期。还有一个结果是使用TMP的C++程序可能在其他方面更高效：较小的可执行文件、较短的运行期、较少的内存需求。但是将运行期的工作转移到了编译期，编译期可能变长了。

再看一下条款 47中的advance伪码

```
template<typename Iter, typename DistT>
    void advance(IteT& iter,DistT d)
    {
        if(iter is a random access iterator)
            iter+=d;
        else
        {
            if(d>=0)
                while(d--) ++iter;
            else 
                while(d++) --iter;
        }
    }
```

可以使用typeid让判断iter类型的伪码运行

```
 template<typename Iter, typename DistT>
    void advance(IteT& iter,DistT d)
    {
        if(typeid(typename std::iterator_traits<IterT>::iterator_category)
        ==typeid(std::random_access_iterator_tag))
            iter+=d;
        else
        {
            if(d>=0)
                while(d--) ++iter;
            else 
                while(d++) --iter;
        }
    }
```

typeid-based解法效率比traits解法低(使用if语句而不是函数重载），因为在此方案中，1. 类型测试发生在运行期而不是编译期，2. 运行期类型测试代码在（或被连接于）可执行文件中。这个例子可以说明TMP比正常的C++程序更高效，因为traits解法就是TMP。（traits引发“编译期发生于类型身上的if-else计算“）

一些东西在TMP比在正常的C++更容易，advance提供一个好例子。advance的typeid-based实现方式可能导致编译期问题

    std::list<int>::iterator iter;
    ……
    advance(iter,10);
    void advance(std::list<int>::iterator& iter,int d)
    {
        if(typeid(typename std::iterator_traits<std::list<int>::iterator>::iterator_category)
        ==typeid(std::random_access_iterator_tag))
            iter+=d;//错误
        else
        {
            if(d>=0)
                while(d--) ++iter;
            else 
                while(d++) --iter;
        }
    }

在+=这个操作符上是错误调用。因为list::iterator不支持+=，它是bidirectional迭代器。我们知道不会执行+=那一行，因为typeid那一行总是不相等；但是编译期要确保所有源码都有效，即使是不会执行的代码。与之相对的是traits-based TMP解法，其针对不同类型执行不同代码，被拆分为不同的函数，每个函数所使用的操作（操作符）都可实行于该函数所对应的类型。

TMP已被证明是个图灵完全机器，意思是说它的威力足以计算任何事物。可以使用TMP声明变量、执行循环（递归实现）、编写调用函数……。有时候这会和正常C++对应物看起来很是不同，例如条款 47展示的TMP if-else是由templas和其特化具体表现出来。不过那是汇编语言级的TMP。针对TMP设计的程序库（例如Boost’s MPL，**条款**55）提供更高级的语法。

为了再次认识下事物在TMP中如何运作，来看下循环。TMP没有真正循环，循环由递归（recursion）完成。TMP递归甚至不是正常的递归，因为TMP递归不涉及递归函数调用，而是涉及递归模板化（recursive template instantiation）。

TMP的起手程序是在编译期计算阶乘。TMP的阶乘运输示范如何通过“递归模板具体化“实现循环，以及如何在TMP中创建和使用变量

    template<unsigned n>
    struct Factorial{
        enum {value=n*Factorial<n-1>::value};
    };
    template<>
    struct Factorial<0>{ //特殊情况，Factorial<0>的值是1
        enum {value=1};
    };

有了这个template metaprogram，只要指涉Factorial::value就可以得到n阶乘值。循环发生在template具体化Factorial内部指涉另一个template具体化Factorial之时。特殊情况的template特化版本Factorial<0>是递归的结束。

每个Factorial template具体化都是一个struct，每个struct都声明一个名字为value的TMP变量，用来保存当前计算所获得的阶乘值。TMP以递归模板具体化取代循环，每个具体化有自己一份value，每个value有其循环内适当值。

你可以这样使用Factorial

```
std::cout<<Factorial<5>::value;
```

用Factorial示范TMP就像用hello world示范编程语言一样。为了领悟TMP之所以值得学习，就要先对它能够达成什么目标有一个比较好的理解。下面举三个例子：

    1.确保量度单位正确。使用TMP就可以确保在编译期所有量度单位的组合都正确。
    2.优化矩阵运算。
    3.可以生成客户定制之设计模式（custom design pattern）实现品


TMP目前还不完全成熟，语法不直观，支持的工具还不充分。但TMP对难以或甚至不可能于运行期实现出来的行为表现能力很吸引人。虽然TMP不会成为主流，但是会成为一些程序员（特别是程序库的开发人员）的主要粮食。

**总结**

1. Template metaprogramming（TMP，模板元编程）可将工作由运行期移到编译期，因而得以**实现早期错误侦测和更高的执行效率。**
2. TMP可被用来生成“基于政策选择组合”（based on combinations of policy choices）的客户定制代码，也可用来避免生成对某些特殊类型并不适合的代码。

## 49. 了解new-hardler的行为

C++内存是由程序员手动管理的，不像Java或.net有垃圾回收机制。**C++内存管理主要是分配例程和归还例程**（allocation and deallocation routines），即operator new和operator delete，还有一个配角new-handler。当涉及到数组时，上面提到的operator new和operator delete就会变为operator new[]和operator delete[]。

内存管理在多线程环境下更为复杂，因为heap是一个可被改动的全局性资源，多个线程访问同一个资源会有race conditions（竞速状态）。使用static类型变量更要注意。如果没有适当同步控制（synchronization），使用无锁（lock-free）算法或精心防止并发访问（concurrent access）时，可能会破坏heap数据结构。

最后要注意一点，STL容器使用的heap是由容器所拥有的分配器对象（allocator objects）管理，不是用new和delete直接管理。

当operator new无法满足某一内存分配时，就会throw一次。以前它会返回NULL指针，现在某些旧式编译器也还这么做。

**在operator new抛出异常以前，会先调用一个客户指定的错误处理函数：new-handler。**（这不是全部，operator new更复杂，见**条款**51）。客户用set_new_handler来指定这个“用以处理内存不足”的函数，这是声明于new的标准程序库函数

```
namespace std{
        typedef void(*new_handler)();
        new_handler set_new_handler(new_handler p) throw();
    }
```

```
func() throw(type);  //只能抛出type这种类型的异常

func() throw();  //不会抛出异常

func() throw(...);  //可以抛出任何类型的异常
```

new_handler是个函数指针，该函数没有参数也不返回任何东西。set_new_handler是设置一个new_handler并返回一个new_handler函数，返回的new_handler是指向set_new_handler被调用前正在执行的那个new-handler函数。后面的throw是一份异常明细，表示该函数不抛出异常。可以这样使用

    //当operator new无法分配足够的内存时，该被调用的函数
    void outOfMem()
    {
        std::cerr<<"Unable to satisfy request for memoryn";
        std::abort();
    }
    int main()
    {
        std::set_new_handler(outOfMem);
        int *pBigDataArray=new int[100000000L];
        ……
    }

如果operator new无法为100000000个整数分配足够空间，outOfMem会被调用。

当operator new无法满足内存申请时，它会不断调用new-handler，直到找到足够内存。反复调用的代码在条款51讨论。这里先说一下，设计良好的new-handler必须做好以下事情;

    1. 让更多内存可被使用。这样可以造成operator new内的下一次内存分配动作可能成功。一个做法是，程序一开始就分配一大块内存，当new-handler第一次被调用时将它释放。
    2. 安装另一个new-handler。当前的new-handler无法取得更多内存时，或许它知道哪个new-handler有此能力。
    如果真如此，目前这个new-handler就可以安装另外那个new-handler来替换自己。
    3. 卸除new-handler，也就是将null指针传给set_new_handler，一旦没有安装任何new-handler，operator new在内存分配不成功时便抛出异常。
    3. 抛出bad_alloc(或派生自bad_alloc)的异常。这样的异常不会被operator new捕捉，因此不会被传播到内存索求处。
    4. 不返回。通常abort或exit。

这些选择会让你在实现new-handler函数时拥有很大弹性

有时候，我们希望以不同的方式处理内存分配失败的情况，你希望分配物属于那个class而定。例如

    class X{
    public:
        static void outOfMemory();
        ……
    };
    class Y{
    public:
        static void outOfMemory();
        ……
    };
    X* p1=new X;//分配不成功，调用X::outOfMemory
    Y* p2=new Y;//分配不成功，调用Y::outOfMemory

C++并不支持class专属的new-handler，但是我们自己可以实现这种行为。令每一个class提供自己的set_new_handler和operator new即可。

现在打算处理Widget class内存分配失败的情况。首先要有一个operator new无法为Widget分配足够内存时的调用函数，即new_handler函数

```
 class Widget{
    public:
        static std::new_handler set_new_handler(std::new_handler p) throw();
        static void* operator new(std::size_t size) throw(std::bad_alloc);
    private:
        static std::new_handler currentHandler;
    };
    
std::new_handler Widget::currentHandler=0;  //类外初始化
std::new_handler Widget::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler=currentHandler;
    currentHandler=p;
    reutrn oldHandler;
}
```

Widget的operator new做以下事情：

1. 调用标准set_new_handler，告知Widget错误处理函数。这会将Widget的new-handler安装为global new-handler。
2. 调用global operator new，如果失败，global operator new会调用Widget的new-handler，因为第一步。如果global operator new最终无法分配足够内存，会抛出一个bad_alloc异常。这时Widget的operator new要恢复原本的global new-handler，之后在传播异常。
3. 如果global operator new调用成功，Widget的operator new会返回一个指针，指向分配的内存。Widget析构函数会管理global new-handler，它会将Widget’s operator new被调用前的那个global new-handler恢复回来。

下面是一个资源管理类，里面自由基础性的RAII（资源获取初始化）操作，在构造过程中获得一笔资源，并在析构过程中释还

```
class NewHandlerHolder{
public:
    explicit NewHandlerHolder(std::new_handler nh)
    :handlere(nh){}
    ~NewHandlerHolder()
    { std::set_new_handler(handler); }
private:
    std::new_handler handler;
    NewHandlerHolder&(const NewHandlerHolder&);//防止copying
    NewHandlerHolder& operator=(const NewHandlerHolder&);
};
```

这使得Widget’s operator new的实现变得简单

```
void* Widget::operator new(std::size_t size) throw(std::bad_alloc)
{
    NewHandlerHolder h(std::set_new_handler(currentHandler));//安装Widget的new-handler
    return ::operator new(size);
}
```

Widget客户应该类似这样使用其new-handling

    void outOfMem();
    Widget::set_new_handler(outOfMem);//设定outOfmem为Widget的new-handling函数
    Widget* pw1=new Widget;//内存分配失败，则调用outOfMEM
    std::string* ps=new std::string;//内存分配失败则调用global new-handling（如果有）
    Widget::set_new_handler(0);//设定Widget专属new-handling为null
    Widget* pw2=new Widget;//内存分配失败则立刻抛出异常

实现这个方案的class代码基本相同，用个基类base class加以复用是个好的方法。可以用个template base class，如此以来每个derived class将获得实体互异的class data复件。这个base class让其derived class继承它获取set_new_handler和operator new，template部分确保每一个derived class获得一个实体互异的currentHandler成员变量。

    template<typename T>
    class NewHandlerSupport{
    public:
        static std::new_handler set_new_handler（std::new_handler p) throw();
        static void* operator new(std::size_t size) throw(std::bad_alloc);
        ……
        private:
        static std::new_handler currentHandler;
    };
    template<typename T> std::new_handler
    NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
    {
        std::new_handler oldHandler=currentHandler;
        currentHandler=p;
        return oldHandler;
    }
    
    template<typename T> void* NewHandlerSupport<T>::operator new(std::size_t size)
    throw(std::bad_alloc)
    {
        NewHandlerHolder h(std::set_new_handler(currentHandler);
        return ::operator new(size);
    }
    //将每一个currentHandler初始化为null
    template<typename T>
    std::new_handler NewHandlerSupport<T>::currentHandler=0;
    //有了这个class template，为Widget添加set_new_handler就容易了
    class Widget:public NewHandlerSupport<Widget>{
    ……
    };
在template base class中，从未使用类型T。因为currentHandler是static类型，使用模板的话会是每个class都有自己的currentHandler。如果使用多重继承，要注意**条款**40所提到的内容。

C++中operator new分配失败抛出异常bad_alloc，但是旧标准是返回null指针。旧标准这个形式为nothrow形式。

```
class Widget{};
Widget* pw1=new Widget;//分配失败，抛出bad_alloc
if(pw1==null)//判断是否分配成功。但是这个测试失败
Widget* pw2=new(std::nothrow)Widget;//分配失败，返回null
if(pw2==null)//可以侦测
```

new(std::nothrow) Widget发生两件事，第一分配内存给Widget对象，如果失败返回null指针。第二，如果成功，调用Widget的构造函数，但是在这个构造函数做什么，nothrow new并不知情，有可能再次开辟内存。如果在构造函数使用operator new开辟内存，那么还是有可能抛出异常并传播。使用nothrow new只能保证operator new不抛出异常，不能保证像new(std::nothrow) Widget这样的表达式不抛出异常。所以，并没有运用nothrow的需要。

总结

    1.set_new_handler允许客户指定一个函数，在内存分配无法获得满足时被调用。
    2.nothrow new是一个颇为局限的工具，因为它只适用于内存分配；后继的构造函数调用还是有可能抛出异常。
## 50. 了解new和delete的合理替换时机

有人会想要替换掉编译器提供的operator new或operator delete，理由是：

1. **用来检测运用上的错误**。如果“new所得的内存，delete掉却不幸失败”，会导致内存泄漏。如果“在new所得内存多次delete"会导致不确定行为。使用编译器提供的operator new和operator delete不能检测上述行为。如果operator new持有一个动态分配所得地址 ，operator delete则将内存从中移走，这样就能呢检测上述错误用法。如果编程错误，可能在分配内存的之前区域或之后区域写入数据；这时可以**自己定义operator new分配超额内存，在多出部分写上特定byte patterns（即签名，signature），自己定义operator delete检测签名是否更改**。
2. **为了强化效能**。operator new和operator delete如果开辟大内存、小内存，持续这样做会造成内存碎片，这最终可能会导致无法满足大区快内存需求，即使有足够但分散的小区块自由内存。使用自己定制的operator new和operator delete可以避免这样的问题。针对特定的需求，有时还可以提升性能。
3. **为收集使用上的统计数据**。在定制operator new和operator delete之前，应该首先了解软件如何使用动态内存。分配区块如何分布？寿命如何？它们是FIFO先进先出还是LIFO后进先出，或随机分配和归还？软件在不同执行阶段有不同的分配归还形态吗？任何时刻使用的最大动态分配量是多少？自己定义的operator new和operator delete可以轻松收集到这些信息。

写个**定制的operator new和operator delete**并不难。例如，写个global operator new，用于检测在分配区块的后面或前面写入数据。下面是个初步版本，有小错误，后面在完善。

    static const int signature=0xDEADBEEF;
    typedef unsigned char Byte;
    //下面代码有些小错误
    void* operator new(std::size_t size) throw(std::bad_alloc)
    {
        using namespace std;
        size_t realSize=size+2*sizeof(int);  //增加大小，塞入两个sinature
        void* pMem=malloc(realSize);  //无类型指针，可以指向任何类型
        if(!pMem) throw bad_alloc();
    
        //将signarure写入内存最前后最后
        *(static_cast<int*>(pMem))=signarure;
        *(reinterpret_cast<int*>(static_cast<Byte*>(pMem)+realSize-sizeof(int)))=signature;
    	return static_cast<Byte*>(pMem)+sizeof(int);
    }

暂且忽略之前所说的operator new内应该有个循环，反复调用new-handling。来说一下另外一个主题：对齐（alignment）。

许多计算机体系结构要求**特定的类型必须放在特定的内存地址上**。例如可能是指针的地址必须是4的倍数（four-byte aligned）或double的地址是8的倍数（eight-byte aligned）。没有这些约束可能会导致运行期硬件异常。有些体系结构要求没这么严格，没有字节对齐不会导运行效率低下。

C++要求所有operator new返回的指针都有适当的对齐（取决于数据类型）。malloc就是在这样的要求下工作。所以令operator new返回一个得自malloc的指针是安全的。但是上面实现中，我们偏移了一个int的大小，就不能保证其安全了。例如，如果返回double指针，就不是8字节对齐了。

像对齐这类技术细节，可以区分内存管理器的质量。写一个能够运行的内存管理器并不难，难的是让它总是能够高效优良的运作。一般来说，若非必要，不要去写内存管理器。

很多时候也是非必要的。有些编译器已经在它们的内存管理函数中切换至调试状态（enable debugging）和志记状态（logging）。许多平台上有商业产品可以代替编译器自带的内存管理器，可以用它们来提高机能和改善效率。

另外一个选择是开源领域中的内存管理器。它们对许多平台都可以用。Boost程序库（条款 55）的Pool就是这样的一个分配器，它对常见的分配大量小内存很有帮助。一些小型开源内存分配器大多都不完整，缺少移植、线程安全、对齐等考虑。

本条款是在探讨**何时需要在全局性的或class专属的基础上合理替换掉缺省的new和delete**，前面说到了3点。这里继续。

    1.为了增加分配和归还的速度。使用定制的针对特定类型对象的分配器，可以提高效率。例如，Boost提供的Pool程序库便是。如果在单线程程序中，你的编译器所带的内存管理具备线程安全，你可以写个不具备线程安全的分配器而大幅度改善速度。
    2.为了降低缺省内存管理器带来的空间额外开销。泛用型分配器往往（虽然并非总是）不只比定制型慢，还使用更多空间，因为它们常常在每一个分配区块上招引某些额外开销。针对小型对象开放的分配器，例如Boost库的Pool，本质上消除了这样的额外开销。
    3.为了弥补缺省分配器的非最佳对齐（suboptimal alignment）。X86体系结构上的double访问最快–如果它们是8-byte对齐。但是编译器自带的operator new并不保证分配double是8-byte对齐。
    为了将相关对象成簇集中。如果特定的某个数据结构往往被一起使用，我们希望在处理这些数据时将“内存页错误”（page faults）的频率降至最低，那么为此数据结构创建另一个heap就有意义，这样就可以将它们成簇集中到尽可能少的内存也上。
    4.为了获得非传统的行为。有时候我们需要做operator new和delete没做的事。例如，在归还内存时将其数据覆盖为0，以此增加应用程序的数据安全。

总结

    有许多理由需要写个自定的new和delete，包括改善效能、对heap运用错误进行调试、收集heap使用信息。
## 51. 编写new和delete时要固守常规

条款 50已经说明为什么要写自己的operator new和operator delete，本条款解释在**编写时遵循什么守则**。

从operator new开始。operator new必须返回正确的值，内存不足时必须调用new-handling函数，要有对付零内存需求的准备，避免不慎掩盖正常形式的new–这比较偏近class接口的要求而非实现要求。正常形式的new描述于条款 52。

operator new如果申请内存成功，就返回指向那块内存的指针，失败则遵循条款 49描述，抛出bad_alloc异常。它实际上不止一次尝试内存分配，在每次失败后都调用new-handling函数。这里假设new-handling函数能做某些动作将一些内存释放出来。只有指向new-hangling函数的指针为，才会抛出异常。C++规定，即使客户要求0 byte，operator new也要返回一个合法指针。下面是个non-member operator new的伪码（pseudocode）：

    void* operator new(std::size_t size) throw(std::bad_alloc)
    {
        using namespace std;
        if(size==0)//处理0-byte申请
            size=1;
    	while(true){
    	//尝试分配size bytes;
    	if(分配成功）
    		return 指向分配得来的内存;
    	//分配失败，找到当前的new-handling函数
    	new_handler globalHandler=set_new_handler(0);
    	set_new_handler(globalHandler);
    	if(globalHandler)  
    		(*globalHandler)();
    	else  
    		throw std::bad_alloc();
    }
如果在多线程环境下，还需要某种锁机制，以便处理new-handling函数背后的global数据结构。

上面包含一个死循环，退出此死循环唯一办法就是内存分配成功（假设new-handling不为null），所以new-handling函数做的事是：让更多内存可用、安装另一个new-handler、卸载new-handler、抛出bad_alloc异常（或其派生类），或承认失败直接return。

上面的operator new成员函数可能会被derived classes继承。注意分配内存大小size，它是函数接收的实参。条款50提到，定制内存分配器往往是为了特定的class对象，以此来优化，不是为了该class的derived classes。

```
class Base{
public:
    static void* operator new(std::size_t size) throw(std::bad_alloc)
    ……
};
class Derived:public Base
{……};//假设为重新定义operator new
Derived* p=new Derived;//这里调用了Base::operator new
```

如果是class专属的operator new，应该改为这样：

```
void* Base::operator new(std::size_t size) throw(std::bad_alloc)
{
    if(size!=sizeof(Base))
        return ::operator new(size);//使用标准的operator new
    ……
}
```

如果你打算控制class专属版的arrays内存分配，那么需要实现operator new[]。编写operator new[]时，唯一要做的一件事就是分配一块未加工的内存（raw memory），因为你无法对array之内迄今尚未存在的元素对象做任何事。甚至我们无法知道这个array含有多少个元素对象。可能你不知道每个对象多大，因为base class的operator new[]有可能经由继承被调用，将内存分配给derived class对象的array使用。

所以不能再Base::operator new[]中假设每个元素对象大小是sizeof(Base)，这样就是说你不能假设array元素个数是（bytes申请数/sizeof(Base))。此外，传递给operator new[]的size_t参数，其值有可能比将辈填对象的内存大一些，因为条款 16提过，动态分配的arrays可能包含额外空间用来存放元素个数。

operator delete情况就简单很多，但是要记住，C++保证删除指针永远安全。下面是non-member operator delete的伪码（pseudocode）：

```
void operator delete(void* rawMemory) throw()
{
    if(rawMemory==0) return;

    归还rawMemory所指内存;

}
```

这个函数的member版本也很简单，只需多加一个检查删除数量。

```
void Base::operator delete(void rawMemory, std::size_t size) throw()
{
    if(rawMemory==0) return;
    if(size!=sizeof(Base)){
        ::operator delete(rawMemory);
        return ;
    }
    归还rawMemory所指内存;
    return ;
}

```

如果即将删除的对象派生自某个base class而后者没有virtual析构函数，那么C++传给operator delete的size_t数值可能不正确。

总结

    operator new应该内涵死循环，并在其中尝试分配内存，如果它无法满足内存需求，就该调用new-handler。它也应该有能力处理0bytes申请。class专属版本的还应该处理“比正确大小更大的（错误）申请”。
    operator delete应该在收到指针时不做任何事。class专属版本则还应该处理“比正确大小更大的（错误）申请”。




## 52. 写了placement new也写placement delete

placement new和placement delete是C++经常用到但是却不常见的两个操作符。当我们使用new创建一个对象时     

```
Widget* pw=new Widget;
```

这里共有两个函数被调用，第一个函数就是operator new，用以分配内存，第二个是Widget的default构造函数。如果第一个函数调用成功，但是第二个函数调用失败，这时需要释放第一步开辟的内存，否则就造成了内存泄露。这个时候，客户没有能力去归还内存，因为如果Widget构造函数抛出异常，那么pw尚未被赋值，客户手中的指针还没有指向开辟的内存。释放内存的任务落到了C++运行期系统身上。

运行期系统会调用第一个函数operator new所对应的operator delete版本。因为operator delete可能有多个版本。正常的operator new和对应的operator delete

    void *operator new(std::size_t) throw(std::bad_alloc);
    void operator delete(void* rawMemory) throw();  //global作用域中的正常签名式
    void operator delete(void* rawMemory , std::size_t size) throw();  //class作用域中典型的签名式

如果使用正常的operator new和operator delete，运行期系统可以找到如何释放new开辟内存的delete函数。但是如果使用非正常形式的operator new，究竟使用哪个delete就会有问题了。

那个例子来说明。假设编写一个class专属的operator new，要接接收一个ostream，用来志记相关分配信息，同时又写了一个正常形式的class专属operator delete：

    class Widget{
    public:
        ……
        static void* operator new(std::size_t size, std::ostream& logStream)//非正常形式的new
            throw(std::bad_alloc);
        static void operator delete(void* pMemory, std::size_t size)//正常的class专属delete
            throw();
        ……
    };

这个设计有问题，在讨论问题签，先说明若干术语。**operator new接受的参数除了必有的size_t之外还有其他，这便是placement new**。所以上述的operator new是个placement版本。众多placement new版本中，有一个特别有用的是“接受一个指针指向对象该被构造之处”，这个operator new形式如下

```
void* operator new(std::size_t, void* pMemory) throw(); //placement new
```

这个版本的new已被纳入C++标准程序库，只要你#include<new>就能使用它。这个placement new（参数为size_t和指向被构造处的指针）是最早的版本，实际上这正是这个函数的命名规则：一个特定位置上的new。当人们谈到placement new时，大多时候谈的是这一特定版本，即还有额外实参void*。有一点比较普遍，术语placement new意味着带有额外参数的new，因为另一个术语placement delete直接派生自它。

现在再来看一下Widget class的声明式，这个class会引起内存泄露。例如动态创建一个Widget时将相关信息分配信息志记(logs)于cerr:

```
Widget* pw=new (std:cerr) Widget;//调用operator new，并传递cerr作为ostream实参
```

如果内存分配成功，但是Widget构造函数抛出异常，运行期系统要释放operator new开辟的内存。但是运行期系统不知道真正被调用的operator new如何运作。运行期系统寻找参数个数和类型都与operator new相同的operator delete。这里对应的operator delete为：

```
void operator delete(void*, std::ostream&) throw();
```

和operator new的placement版本类似，接收额外参数的operator delete成为placement delete。上面Widget没有placement版本的operator delete，所以运行期系统不知道如何释放operator new开辟的内存。所以Widget class应该定义如下：

```
 class Widget{
    public:
        ……
        static void* operator new(std::size_t size, std::ostream& logStream) throw(std::bad_alloc);
        static void operator delete(void* pMemory) throw();
        static void operator delete(void* pMemory, std::ostream& logStream) throw();
    };
    Widget* pw=new (std:cerr) Widget;//调用operator new，并传递cerr作为ostream实参
```

这样如果Widget构造函数抛出异常，就会调用对应版本的placement delete。但是如果没有异常，这样调用

```
delete pw;
```

就会调用正常版本的operator delete。placement delete只有在placement new调用构造函数抛出异常时才会被调用。

需要注意的是，因为成员函数的名称会掩盖其外围作用域中相同名称的函数（**条款**33），所以要小心避免class专属的new掩盖客户希望调用的new。例如，你有一个base class，其中声明唯一一个placement 

    operator new
    class Base{
    public:
        ……
        static void* operator new(std::size_t size, std::ostream& logStream) throw(std::bad_alloc);//会掩盖global new
        ……
    };
    Base* pb=new Base;//错误，因为正常形式的operator new被掩盖
    Base* pb1=new (std::cerr) Base;//调用Base的placement new

在derived class的operator new会掩盖继承而来的operator new和global版本的new

    class Derived: public Base{
    public:
    	……
        static void* operator new(std::size_t size) throw(std::bad_alloc);//重新声明正常形式的new
    };
    Derived* pd=new (std::clog) Derived;//错误，因为Base的placement new被掩盖了
    Derived* pd1=new Derived;//正确

在缺省情况下，C++在global作用域内提供以下形式的operator new：

```
void* operator(std::size_t) throw(std::bad_alloc);//normal new
void* operator(std::size_t, void*) throw();//placement new
void* operator(std::size_t, const std::nothrow_t&) throw();//nothrow new
```

在class内声明任何形式的operator new都会掩盖上面这些标准形式。对于每一个可用的operator new，要确保提供了对应形式的operator delete。

一个简单的做法是建立一个base class，内含所有正常形式的new和delete

    class StadardNewDeleteForms{
    public:
        //normal
        static void* operator new(std::size_t size) throw(std::bad_alloc)
        {return ::operator new(size);}
        static void operator delete(void* pMemory) throw()
        {::operator delete(pMemory);}
        //placement
        static void* operator new(std::size_t size, void* ptr) throw(std::bad_alloc)
        {return ::operator new(size, ptr);}
        static void operator delete(void* pMemory, void* ptr) throw()
        {::operator delete(pMemory, ptr);}
        //nothrow
        static void* operator new(std::size_t size, const std::nothrow_t& nt) throw(std::bad_alloc)
        {return ::operator new(size,nt);}
        static void operator delete(void* pMemory,const std::nothrow_t&) throw()
        {::operator delete(pMemory);}
    };

如果想以自定义方式扩充标准形式，可以使用继承机制和using声明

```
class Widget: public StandardNewDeleteForms{
    public:
        //让这些形式可见
        using StandardNewDeleteForms::operator new;
        using StandardNewDeleteForms::operator delete;
        //添加自己定义的
        static void* operator new(std::size_t size, std::ostream& logStream) throw(std:;bad_alloc);
        static void operator detele(std::size_t size, std::ostream& logStream) throw();
    };
```

**总结**

```
1.当编写一个placement operator new时，也要编写对应版本的placement operator delete。否则就可能造成隐蔽的内存泄露。
2.当声明了placement new和placement delete时，就会掩盖正常版本。
```

## 53. 不要忽视编译者的警告

许多程序员习惯性的忽略编译器的警告。或许它们认为，如果问题很严重，那么编译器应该给一个错误而不是警告。这种想法在C++非常有害，以一个例子来说明：

```
class B{
    public:
        virtual void f() const;
    };
    class D: public B{
    public:
        virtual void f();
    };
```

这里希望D::f重新定义virtual函数B::F，但是D中却没有声明为const。编译器不会报错，可能会给如下警告

```
warning: D::f() hides virtual B::f()
```

你或许会想，D::f()遮掩了B::F，这正是我们想做的事。但是编译器在试图告诉你B::f并没有在D中重新声明，而是被遮掩了。如果忽略这个警告，比如导致错误行为。

从编译器的警告中获得经验，学会了解警告信息和它们看起来的意义并不十分相同。在最高警告级别下没有任何警告的程序是最理想的，但是对警告有了深刻理解，可以选择忽略某些警告信息。但是一定记住在忽略这个警告之前，一定要了解它的真实意图。

警告信息和编译器相关，不同的编译器有不同的警告标准。例如上面代码中的函数遮掩在另一个编译器编译，可能没有任何警告。

总结

    1.严肃对待编译器发出的警告信息。努力在你的编译器最高警告级别下做到无任何警告。
    2.不要过度依赖编译器的报警能力，因为不同编译器对待事情的态度不相同。一段有警告的代码，移植到另一个编译器上，可能没有任何警告。
## 54. 让自己熟悉包括TR1在内的标准程序库

本书中的C++规范是在1998年制定的。现在C++11，即C++0x已经制定，且C++14在讨论中。本条款讨论内容是关于Boost库的，当时没有制定新标准，一些语言上的新特性以技术报告内容程序给大家，TR1代表Technical Report 1。

现在来看一下C++98加如的标准程序库有哪些：

    STL，即Standard Template Library标准模板库。
    iostreams，包含用户自定义缓冲功能、国际化I/O，以及先定义好的对象cin、cout、cerr和clog。
    国家化支持，包括多区域能力。
    数值处理，包括复数模板（complex）和纯数值数值（valarray）。
    异常阶层体系（exception hierarchy）。
    C89标准程序库。

TR1详细叙述了14个新组件（components，即程序机能单位），放在命名空间std::tr1内。下面展示一些TR1组件实例：

    智能指针
    tr1::function，可以表示任何callable entity（可调用物，即任何函数或函数对象），只要签名复合目标。
    tr1::bind，它能做STL绑定器（binders）bind1st和bind2nd所做的每一件事，且更多。

把TR1组件划分为2组，第一组提供彼此互不相干的机能：

    Hash table，可以用来实现set、map等。
    正则表达式（Regular expression），包括以正则表达式为基础的字符串查找和替换等。
    Tuples（变量组），这是标准程序库中的pair template的新一代制品。
    tr1::array，本质是一个支持成员函数begin和end的数组。
    tr1::mem_fn，这是个语句构造上与成员函数指针（member function pointer）一致的东西。
    tr1::reference_wrapper，一个让reference的行为更像对象的设施。
    随机数（random number）生成工具，它大大超越了rand。
    数学特殊函数。
    C99兼容扩充。

第二组TR1组件由更精巧的template编程技术构成

    Type traits，一组traits classes（**条款**47），用以提供类型的编译期信息。
    tr1::result_of,这是个template，用来推导函数调用的返回类型。

总结

    1.C++的标准程序的主要机能由STL、iostream、locales组成。并包含C99标准程序库。
    2.TR1添加了智能指针、一般化函数指针、hash-based容器、正则表达式以及另外10个组件的支持。
    3.TR1只是一份规范，为获得TR1提供的好处，需要一个实物，例如Boost。
## 55. 让自己熟悉Boost

**Boost库是一个高质量、源码开发、平台独立、编译器独立的程序库**。Boost是一个C++开发者集结的社群，其网址为          http://boost.org

Boost有着其他C++组织没有的影响里。1、Boost有C++标准委员会创建，其目标为：作为一个可被加入标准C++之各种功能的测试场。2、它接纳程序的过程，以公开进行的同僚（public peer review）为基础。

下面介绍Boost程序库的类目

    字符串与文本处理
    容器
    函数对象和高级编程
    泛型编程
    模板编程
    数学和数值
    正确性与测试
    数据结构
    语言间的支持
    内存杂项

总结

    Boost是个社群，也是一个网址。致力于免费、源码开发、同僚复审的C++程序库开发。Boost在C++标准化过程中扮演深具影响里的角色。
    Boost提供许多TR1组件实现品，以及其他许多程序库。

















































