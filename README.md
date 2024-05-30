dll 导出及动态调用

导出：
    普通函数导出
        extern "C" __declspec(dllexport) int __stdcall add(int a, int b);

    class 导出
        导出接口类为抽象类，析构函数为虚函数
        并添加createExportObj和destroyExportObj函数，作为生成和销毁对象接口
        实际导出类继承接口类，接口实现修改（接口不变时）不影响调用
        eg:
            ExportInterface 和 ExportImpl

调用：
    需要包含导出函数头文件或导出抽象类头文件
    LoadLibrary 加载dll
    GetProcAddress 获取导出函数地址
    FreedLibrary 卸载dll
    
    eg:
        普通函数 call.cpp 中 使用DllProxyPtr,只支持C风格接口
        调用导出class时，在ExportProxy(继承自 ExportProxyBaseTemplate)中实现接口调用，只用关心实际导出类中的接口调用，不用关心类的初始化和释放

