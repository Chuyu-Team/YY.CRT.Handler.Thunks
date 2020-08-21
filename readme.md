
# YY.CRT.Handler.Thunks

## 1. YY.CRT.Handler.Thunks
Windows 8开始支持了fastfail，于此同时微软CRT原版大量的异常处理（比如说_invalid_parameter）默认情况也将直接调用fastfail退出程序，这使得捕获某些异常状态的dmp变得更加困难。
因为SetUnhandledExceptionFilter没有机会处理这些信息。


[ [YY交流群 633710173](https://shang.qq.com/wpa/qunwpa?idkey=21d51d8ad1d77b99ea9544b399e080ec347ca6a1bc04267fb59cebf22644a42a) ]

### 1.1. 原理
通过链接时Hook，代替微软CRT中的_invalid_parameter、_purecall等函数。仅针对模块自身有效，不会产生污染。

> 使用本库后 _set_invalid_parameter_handler、_set_purecall_handler 之类的操作会直接无效！

### 1.2. Thunks列表
* _invalid_parameter
* _purecall


## 2. 使用YY.CRT.Handler.Thunks

### 2.1. 通过NuGet引用（推荐）
1. 项目右键 - 管理 NuGet 程序包。
2. 在搜索框中输入YY.CRT.Handler.Thunks，然后点击安装。
3. 重新编译代码。

### 2.2. 传统方式引用
1. 下载[YY.CRT.Handler.Thunks-Binary](https://github.com/Chuyu-Team/YY.CRT.Handler.Thunks/releases)，然后解压到你的工程目录。<br/>
2. 【链接器】-【输入】-【附加依赖项】，添加`objs\$(PlatformShortName)\YY.CRT.Handler.Thunks.obj`。<br/>
3. 重新编译代码。


## 3. YY-Guard兼容性
### 3.1. 支持的编译器
全平台ABI兼容。
* 支持的平台工具集：Visual Studio 2012、Visual Studio 2013、Visual Studio 2015、Visual Studio 2017以及Visual Studio 2019。
* 所有运行库模式均支持（比如：`/MD`、`/MT`、`/MDd`、`/MTd`）。

> Visual Studio 2012之前的工具集由于不会使用fastfail，因此用了本库那也是白用。


## 更改日志
[更改日志](https://github.com/Chuyu-Team/YY.CRT.Handler.Thunks/wiki)
