
/*

此文件用于顶替系统的 _invalid_parameter 等函数。

为什么要顶替？因为不顶替的话可能会无法创建dmp。

*/




#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


#include <Windows.h>
#include <stdlib.h>
#include <intrin.h>

#ifdef _M_IX86
#define _DEFINE_IAT_SYMBOL_MAKE_NAME(f) _CRT_CONCATENATE(_imp__, f)
#else
#define _DEFINE_IAT_SYMBOL_MAKE_NAME(f) _CRT_CONCATENATE(__imp_, f)
#endif

#define _DEFINE_IAT_SYMBOL(_FUNCTION)                                                          \
    extern "C" __declspec(selectany) void const* const _DEFINE_IAT_SYMBOL_MAKE_NAME(_FUNCTION) \
        = reinterpret_cast<void const*>(_FUNCTION)




//invalid_parameter系列

extern "C" void __cdecl __acrt_initialize_invalid_parameter_handler(void* const encoded_null)
{
}


#if defined _M_X64
static void __cdecl capture_current_context(CONTEXT* const context_record) throw()
{
    ULONG64 ControlPc;
    ULONG64 EstablisherFrame;
    ULONG64 ImageBase;
    PRUNTIME_FUNCTION FunctionEntry;
    PVOID HandlerData;

    RtlCaptureContext(context_record);

    ControlPc = context_record->Rip;
    FunctionEntry = RtlLookupFunctionEntry(ControlPc, &ImageBase, nullptr);

    if (FunctionEntry != nullptr)
    {
        RtlVirtualUnwind(
            UNW_FLAG_NHANDLER,
            ImageBase,
            ControlPc,
            FunctionEntry,
            context_record,
            &HandlerData,
            &EstablisherFrame,
            nullptr);
    }
}
#endif

#if (defined _M_IX86 || defined _M_X64)
extern "C" void __cdecl __acrt_call_reportfault(
    int   const debugger_hook_code,
    DWORD const _exception_code,
    DWORD const _exception_flags
)
{
    // Fake an exception to call report fault:
    EXCEPTION_RECORD   ExceptionRecord{};
    CONTEXT            ContextRecord{};
    EXCEPTION_POINTERS ExceptionPointers = { &ExceptionRecord, &ContextRecord };

#ifdef _M_IX86

    __asm
    {
        mov dword ptr[ContextRecord.Eax], eax
        mov dword ptr[ContextRecord.Ecx], ecx
        mov dword ptr[ContextRecord.Edx], edx
        mov dword ptr[ContextRecord.Ebx], ebx
        mov dword ptr[ContextRecord.Esi], esi
        mov dword ptr[ContextRecord.Edi], edi
        mov word ptr[ContextRecord.SegSs], ss
        mov word ptr[ContextRecord.SegCs], cs
        mov word ptr[ContextRecord.SegDs], ds
        mov word ptr[ContextRecord.SegEs], es
        mov word ptr[ContextRecord.SegFs], fs
        mov word ptr[ContextRecord.SegGs], gs
        pushfd
        pop[ContextRecord.EFlags]
    }

    ContextRecord.ContextFlags = CONTEXT_CONTROL;

    ContextRecord.Eip = (ULONG)_ReturnAddress();
    ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
    ContextRecord.Ebp = *((ULONG*)_AddressOfReturnAddress() - 1);

#else // ^^^ _M_IX86 ^^^ // vvv _M_X64 vvv //

    capture_current_context(&ContextRecord);
    ContextRecord.Rip = (ULONGLONG)_ReturnAddress();
    ContextRecord.Rsp = (ULONGLONG)_AddressOfReturnAddress() + 8;

#endif // _M_X64

    ExceptionRecord.ExceptionCode = _exception_code;
    ExceptionRecord.ExceptionFlags = _exception_flags;
    ExceptionRecord.ExceptionAddress = _ReturnAddress();

    BOOL const was_debugger_present = IsDebuggerPresent();

    // Raises an exception that bypasses all exception handlers:
//#pragma warning(suppress:__WARNING_SETUNHANDLEDEXCEPTIONFILTER_USE) // 28725 Use Watson instead of this SetUnhandledExceptionFilter.
    SetUnhandledExceptionFilter(nullptr);
    DWORD const handler_result = UnhandledExceptionFilter(&ExceptionPointers);

}
#endif


extern "C" __declspec(noreturn) void __cdecl _invoke_watson(
        wchar_t const* const expression,
        wchar_t const* const function_name,
        wchar_t const* const file_name,
        unsigned int   const line_number,
        uintptr_t      const reserved
        )
{
    if (IsProcessorFeaturePresent(PF_FASTFAIL_AVAILABLE))
    {
        __fastfail(FAST_FAIL_INVALID_ARG);
    }

    TerminateProcess(/*GetCurrentProcess()*/(HANDLE)-1, STATUS_INVALID_CRUNTIME_PARAMETER);
}

_DEFINE_IAT_SYMBOL(_invoke_watson);


extern "C" void __cdecl _invalid_parameter(
    wchar_t const* const expression,
    wchar_t const* const function_name,
    wchar_t const* const file_name,
    unsigned int   const line_number,
    uintptr_t      const reserved
    )
{
    RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, NULL);
    _invoke_watson(nullptr, nullptr, nullptr, 0, 0);
}

_DEFINE_IAT_SYMBOL(_invalid_parameter);


extern "C" void __cdecl _invalid_parameter_noinfo()
{
    _invalid_parameter(nullptr, nullptr, nullptr, 0, 0);
}

_DEFINE_IAT_SYMBOL(_invalid_parameter_noinfo);


extern "C" __declspec(noreturn) void __cdecl _invalid_parameter_noinfo_noreturn()
{
    _invalid_parameter(nullptr, nullptr, nullptr, 0, 0);
}

_DEFINE_IAT_SYMBOL(_invalid_parameter_noinfo_noreturn);

extern "C" _invalid_parameter_handler __cdecl _set_invalid_parameter_handler(_invalid_parameter_handler const new_handler)
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_set_invalid_parameter_handler);

extern "C" _invalid_parameter_handler __cdecl _get_invalid_parameter_handler()
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_get_invalid_parameter_handler);

extern "C" _invalid_parameter_handler __cdecl _set_thread_local_invalid_parameter_handler(_invalid_parameter_handler const new_handler)
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_set_thread_local_invalid_parameter_handler);

extern "C" _invalid_parameter_handler __cdecl _get_thread_local_invalid_parameter_handler()
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_get_thread_local_invalid_parameter_handler);

//_purecall 系列

extern "C" int __cdecl _purecall()
{
    RaiseException(STATUS_INVALID_PARAMETER, EXCEPTION_NONCONTINUABLE, 0, NULL);

    abort();
}

_DEFINE_IAT_SYMBOL(_purecall);

extern "C" _purecall_handler __cdecl _set_purecall_handler(
    _purecall_handler const new_handler
    )
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_set_purecall_handler);

extern "C" _purecall_handler __cdecl _get_purecall_handler()
{
    return nullptr;
}

_DEFINE_IAT_SYMBOL(_get_purecall_handler);
