## 构造函数需要什么参数？

"Func 就是具体干什么, arg is parameter
"Func 其实就是一个task
"定义一个新的task类？
template <typename Func, typenaem Args>
stage(Func func, Args arg);
模板参数只是存在构造函数上，并没有放到类上
是否应该用std::function
stage(std::function<>())

## 是否要保存这个func参数以及他的参数？ 如何保存?

## 如何将传进来的函数包装一层后给thread？

## 如何解决thread的参数问题？
" std::bind?


