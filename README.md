# clox

学习编译器做的一个练习。

## 介绍

clox 的名称和语法来自 [craftinginterpreters](https://craftinginterpreters.com/)，该书是我见过写得最容易理解的编译原理书籍，在此感谢作者。

[craftinginterpreters](https://craftinginterpreters.com/) 的 interpreter 是用 JAVA 写的，我用 C 重新写了，并做了如下改进：

* 1. 支持 break/continue。
* 2. 支持|, ||, &. &&, ^, ~, <<, >>, +=, -=, *=, /=等操作。
* 3. 支持字符串块。
* 4. 支持原生扩展函数。
* 5. 增加更多数据类型 uint8/uint16/uint32/uint64/int8/int16/int32/int64/float/double

> 目前只是实现了 tree walk 版本 interpreter。虚拟机版本暂未实现。

## 示例

```js
class Animal {
  init(name) {
    this.name = name
  }

  speak(msg) {
    print(this.name, ":", msg)
  }
}

class Dog < Animal {
  init(name) {
    super.init(name)
  }
}

var a = Dog("WangCai")
a.speak("hello");
```

## 编译

```
scons
```

## 运行示例


```
./bin/ilox @demos/class2.lox
```

