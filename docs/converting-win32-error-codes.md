### Converting Win32 error codes to modern C++ exceptions

Win32 services traditionally return **BOOL + `GetLastError()`** or **HRESULT**.
In C++20 you can raise *typed* failures instead, so callers write `try / catch` instead of `if (!call) …`.
Below is a battle-tested pattern used inside Microsoft (WIL), C++/WinRT, and many OSS projects.

---

## 1 Pick an exception type

| Option                                                      | When to pick it                                                               | How it’s created                                         |
| ----------------------------------------------------------- | ----------------------------------------------------------------------------- | -------------------------------------------------------- |
| **`std::system_error`**                                     | Portable, no extra headers; integrates with `<system_error>` utilities.       | `std::system_error{ code, std::system_category(), msg }` |
| **`wil::ResultException`** (Windows Implementation Library) | You already use WIL macros such as `THROW_IF_FAILED`; richer context strings. | Produced automatically by WIL. ([GitHub][1])             |
| **Custom domain-specific class**                            | You need extra payload (filename, handle, etc.).                              | Derive from `std::runtime_error` and store members.      |

For most services **`std::system_error` is enough**: the numeric value and `system_category()` preserve the original Win32 code, and you get human text via `what()`.

---

## 2 Two tiny helper functions

```cpp
// -------------  win32_error.h  -----------------
#pragma once
#include <system_error>
#include <windows.h>

inline void throw_last_error(char const* msg)
{
    DWORD e = ::GetLastError();
    throw std::system_error{ static_cast<int>(e),
                             std::system_category(), msg };
}

inline void throw_if_failed(HRESULT hr, char const* msg)
{
    if (FAILED(hr))
        throw std::system_error{ static_cast<int>(hr),
                                 std::system_category(), msg };
}
```

*Why this is legal*: the Standard says `std::system_category()` is the correct category for “native OS” errors; Win32 error codes and HRESULTs fit that bill. ([Stack Overflow][2])

---

## 3 Wrap every Win32 call

```cpp
#include "win32_error.h"

HANDLE open_file(std::wstring_view path)
{
    HANDLE h = ::CreateFileW(path.data(), GENERIC_READ, FILE_SHARE_READ,
                             nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE)
        throw_last_error("CreateFileW");
    return h;
}

ComPtr<IMFMediaSession> create_session()
{
    ComPtr<IMFMediaSession> s;
    throw_if_failed(::MFCreateMediaSession(nullptr, &s),
                    "MFCreateMediaSession");
    return s;
}
```

Now callers do:

```cpp
try {
    auto h = open_file(LR"(C:\config.ini)");
    …                             // success path
} catch (const std::system_error& ex) {
    LOG_ERROR("{} (code {})", ex.what(), ex.code().value());
}
```

---

## 4 Automated helpers (Windows Implementation Library — WIL)

If you add **`#include <wil/result.h>`** you get ready-made macros:

```cpp
THROW_IF_FAILED(hr);                // for HRESULT
THROW_LAST_ERROR_IF_FALSE(expr);    // for BOOL + GetLastError
THROW_LAST_ERROR();                 // always throws with current error
```

These produce `wil::ResultException`, already carrying the function, file, line and formatted error text.
The macros are header-only; no runtime dependency other than the STL. ([Microsoft Learn][3])

---

## 5 Design guidelines for a Windows service

| Guideline                                                                                                                                                        | Rationale |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------- |
| **Convert immediately** after the API call. Never propagate raw error codes through your own code.                                                               |           |
| **Keep fast paths zero-cost** – exceptions are only constructed on failure, so the hot success path is just a branch.                                            |           |
| **Use RAII** (`wil::unique_handle`, `std::unique_ptr` + custom deleter) so resources are released when exceptions unwind.                                        |           |
| **Don’t cross DLL boundaries with C++ exceptions** unless you control both sides and use the same CRT; otherwise translate back into HRESULT at the boundary.    |           |
| **Compile with `/EHsc`** to make sure Structured Exception Handling (SEH) does not leak into C++ exceptions by accident.                                         |           |
| **Log ex.code() and ex.what()**; never reinterpret the numeric code yourself—pass it to `FormatMessage()` if you need the Windows string. ([Microsoft Learn][4]) |           |

---

## 6 Advanced: wrapping SEH

Some low-level WinAPI (e.g., `ReadProcessMemory`) can raise an *SEH* exception.
If required, install a translator once:

```cpp
#include <excpt.h>
#include <eh.h>

void seh_to_cpp(unsigned, _EXCEPTION_POINTERS*)
{
    throw std::system_error{ ERROR_UNHANDLED_EXCEPTION,
                             std::system_category(),
                             "SEH raised" };
}

int main()
{
    _set_se_translator(seh_to_cpp);
    …
}
```

But do this only when you truly need it; most service code never sees SEH.

---

### TL;DR

1. **Call Win32 API.**
2. **If it fails, create `std::system_error` (or WIL’s `THROW_*` macro) immediately.**
3. **Let normal C++ `try / catch` flow handle the rest.**

This keeps your new C++20 service idiomatic, testable, and free from manual `if (FAILED(hr))` sprawl while preserving every bit of Win32 diagnostic information.

[1]: https://github.com/microsoft/wil/blob/master/include/wil/result_macros.h?utm_source=chatgpt.com "wil/include/wil/result_macros.h at master · microsoft/wil - GitHub"
[2]: https://stackoverflow.com/questions/15854930/how-do-you-use-stdsystem-error-with-getlasterror?utm_source=chatgpt.com "How do you use std::system_error with GetLastError? - Stack Overflow"
[3]: https://learn.microsoft.com/en-us/virtualization/api/hcs/reference/tutorial?utm_source=chatgpt.com "Quick Start | Microsoft Learn"
[4]: https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-?utm_source=chatgpt.com "System Error Codes (0-499) (WinError.h) - Win32 apps"
