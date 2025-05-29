# Overview of Prompt:
This initial prompt is to kick-off the collaboration to migrate the Windows service to C++20.  It will be a process of analysis, design, refactoring, and migration.  As we begin the journey, I want to set some initial guidance to help develop a baseline.

## List of books that should be referenced as we work through this process:
- Programming Principles and Practice Using C++ - Stroustrup
- C++ Memory Management - Pratrice Roy
- C++20 Get the Details 2nd Edition - Rainer Grimm
- Embracing Modern C++ Safely - Lakow
- Effective Modern C++ - Meyers
- C++ Coding Standards - Sutter and Alexandrescu
- The C++ Standard Library - Josuttis 
- C++ Templates - Josuttis
- Windows via C/C++ 5th Edition - Richter
- Functional Programming in C++ - Cukic
- Large Scale C++ Software Design - Lakow
- Network Algorithmics - Varghese
- Advanced Windows Debugging - Hewardt, Pravat
- Debugging Windows Programs - McKay, Woodring
- ATL Internals 2nd Edition - Rector, Sells
- Designign Applications with MSMQ - Dickman

## List of links to include:
- ["C++ Core Guidelines - GitHub Pages"](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines?utm_source=chatgpt.com)
- ["Google C++ Style Guide"](https://google.github.io/styleguide/cppguide.html?utm_source=chatgpt.com) 
- ["hhvm/hphp/doc/coding-conventions.md at master - GitHub"](https://github.com/facebook/hhvm/blob/master/hphp/doc/coding-conventions.md?utm_source=chatgpt.com) 
- ["Using the C++ Core Guidelines checkers | Microsoft Learn"](https://learn.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-170&utm_source=chatgpt.com) 
- ["Secure Coding Guidelines for Developers"](https://docs.oracle.com/cd/E26502_01/html/E29016/scode-1.html?utm_source=chatgpt.com) 
- [SEI Standards](https://wiki.sei.cmu.edu/confluence/pages/viewpage.action?pageId=88046682)
- [The legacy code we will be refactoring](https://github.com/mikebiz/bmobile/tree/main/docs/legacy-source) 

## Existing Sessions to include and reference:
- "C++ Coding Standards Guide"
- "PlantUML Guide for SDE"
- "ODBC C++ Wrapper"
- "Repo Structure and README"
- "C++20 New Features Review"
- "C++23 Key Updates"
- "Large-Scale C++ Design Overview"
- "C++ Hexagonal Architecture Implementation"
- "C++ Security Protections"
- "IP Helper API Namespaces"

## Outline of Prompt
Windows Routing Service that acts like an overlay network:  
The services runs on Windows Servers and utilizes IP, UDP, MSMQ, MS SQL Server, ATL, and C++.

### Background:
This Windows Service was created in 2006/2007.  While it was needed to provide a specific purpose to solve a business problem, it evolved into what amounts to a network packet dissemination or broadcast service based on the notion of "Publishing Points", "Publishers", and "Subscribers".  For this first migration, we will focus solely on the network routing aspects and not the logic to manage the publishing points, publishers or subscribers.

A Publishing Point can contain 1 or more publishers and 1 or more subscribers.  One of the basic concepts this will support is a routing service. 

The functionality of the service, at a high-level, is pretty simple; it ingests network packets, determines what publishing point they are destined to, pulls the list of subscribers for the publishing point from an in-memory routing table, and then pushes the packets to them.  For the most part, the network packets are opaque to the server with the exception being the first section or bytes of the packet.

**All packets** must contain the following structure at the beginning in order for routing to occur (from file [PacketInfoStructs.h](https://github.com/mikebiz/bmobile/blob/main/docs/legacy-source/PacketInfoStructs.h)).

```cpp
const UINT8 PHS_MAX = 5;
//#ifndef _MANAGED
#include <pshpack1.h>
//#endif

#ifndef UNDER_CE
const UINT32 OUT_MAX_PACKET_PAYLOAD = 8 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
const UINT32 IN_MAX_PACKET_PAYLOAD = 1 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
#else
const UINT32 OUT_MAX_PACKET_PAYLOAD = 5 * 1514; //32768; //2048; //32768; // 54000;// 32768 ;  
const UINT32 IN_MAX_PACKET_PAYLOAD = 1 * 1; //32768; //2048; //32768; // 54000;// 32768 ;  
#endif
const UINT32 MAX_AUDIO_PACKET_BLOB = 700;  
const UINT32 MAX_MANAGEMENT_PACKET_BLOB = 700;  

typedef struct _RECV_INFO
{
	SOCKET				sd;
	INT32				iThread;
#ifdef _VISTA_
	PTP_IO				pRecvIo;
#endif
	HANDLE				hEvents;
    SOCKADDR_STORAGE    safrom;
	HANDLE				hAcceptEvents;
}RECV_INFO, *PRECV_INFO;
const UINT32 MAX_RECV_INFO( sizeof( RECV_INFO ) );  


typedef struct _IPHDR 
{
    UCHAR				ip_verlen;
    UCHAR				ip_tos;
    USHORT				ip_totallength;
    USHORT				ip_id; 
    USHORT				ip_offset;
    UCHAR				ip_ttl;
    UCHAR				ip_protocol;
    USHORT				ip_checksum;
    UINT				ip_srcaddr;
    UINT				ip_destaddr;
}IP_HEADER, * PIP_HEADER;
const UINT32 MAX_IP_HEADER ( sizeof( IP_HEADER ) );

// The UDP header structure
//
typedef struct _UDPHDR
{
    USHORT				src_portno;
    USHORT				dst_portno;  
    USHORT				udp_length; 
    USHORT				udp_checksum;

}UDP_HEADER, * PUDP_HEADER;
const UINT32 MAX_UDP_HEADER ( sizeof( UDP_HEADER ) );

// The route entry header structure
//
typedef struct _tagROUTE_ENTRY
{
    UINT				dst_ipaddr;
    USHORT				dst_portno;  
}ROUTE_ENTRY, * PROUTE_ENTRY;
const UINT32 MAX_ROUTE_ENTRY ( sizeof( ROUTE_ENTRY ) );
const UINT32 MAX_ROUTE_TABLE_ENTRIES = 8;

typedef struct _tagEVENT_HEADER
{
	GUID				eventId;
	BLXWARE_EVENT		eventType;
	BLXWARE_EVENT_TYPE	eventTypeId;
	CHAR				eventMacAddress[ MACADDRESS_LENGTH ];
}EVENT_HEADER, *PEVENT_HEADER;
const UINT32 MAX_EVENT_HEADER ( sizeof( EVENT_HEADER ) );

```

### There are some key concepts that exist today or would like to be added:
- Windows Service written in C++20 (new)
- Core Service and coding guidelines follow that of ATL (existing)
- Network Protocol UDP (existing)
- Registered I/O RIO (new)
- Windows Threadpool (existing)
- Network packets are opaque to the server.  The server will be agnostic to the content of the packets and is capable of routing from 1 or more sources (publishers)  to 1 or more destination (subscribers)
- The service will have an in-memory route table that is updated from a database store in near real-time through messages delivered by MSMQ (existing)
- Zero memcpy between published message and subscribed message.  A common pool of pre-allocated memory is used (existing)
- Meta data and Publishing Point, Publisher, and Subscriber data is persisted in MS SQL Server (existing)
- MSMQ is used as the mechanism to pass synchronization messages between the servers
- A layered design between modules that make up the service based on Hexagonal Architecture influencing dependency injection and inversion of control (new)
- Logging in Windows, Recommended eventing and performance measurements (new)

The first phase of this effort will focus on design and documenting the design of the system (the service and supporting modules) in Plant UML.

### Service Modules:
Some Ideas for these modules include but are not limited to:
- Network
- Storage
- Security
- Routing
- Publishing
- Metadata Support and Maintenance

Once the supporting modules have been identified and agreed to, we will work on the diagrams based on PlantUML which will include but not be limited to:
- Deployment diagram to capture the layers of the modules
- Sequence diagrams to document events the service will handle
- State transition diagrams of various modules and the Publishing points, publishers, and subscribers.

### Items to include: 
- **Hexagonal Architecture** - How can I incorporate this into the design of the service.  Include things in Dependency Injection and Dependency Inversion Principle.  Driver->module (driven) | (driver)->module
- **Unit tests** -  What is the best strategy for creating Unit Test, Functional Test, Integration Test?  What are some creative ways to execute these?
- **PlantUML** - Minimal coverage of modules, event handling through sequence diagrams, and STDs

## Initial Prompt:
In the initial prompt, I want to focus on developing understanding of the [legacy-source](https://github.com/mikebiz/bmobile/tree/main/docs/legacy-source)  and the buildout of the \
design of the modules and overall service by developing the strategy on what service modules to create and a layer diagram of how they relate to oneanother.
- Review the entirity of this document
- Review the legacy source code
- Review the header "Service Modules" above and begin to work through the ideation of how to modularize the existing service design.  I would like to use some concept  like Hexagonal Architecture
- All design artifacts should be developed using PlantUML so that it can be saved and currated in the github repo

### Legacy Project Breakdown
1. BlxContentService
    - Headers
        - BlxBuffer.hpp
        - BlxPqBuffer.hpp
        - GenerateNwEvents.hpp
        - ServerContext.h
        - ServerManagementHandler.hpp
        - ServerNetwork.h
    - Source
        - BlxContentService.cpp
        - ServerNetwork.cpp
2. BlxContentServiceData
    - Headers
        - BlxContentServiceData.h
        - Management.h
        - ManagementMs.h
        - PublishingPointDestinations.h
        - PublishingPointDestinationMs.h
        - PublishingPointSource.h
        - PublishingPointSourceMs.h
        - Utilities.h
    - Source
        - BlxContentServiceData.cpp
        - Management.cpp
        - ManagementMs.cpp
        - PublishingPointDestinations.cpp
        - PublishingPointDestinationMs.cpp
        - PublishingPointSource.cpp
        - PublishingPointSourceMs.cpp
        - Utilities.cpp
3. BlxSqlMsmqBridge
    - Headers
        - BlxSqlMsmqBridge.h
        - xphelper.h
    - Source
        - BlxSqlMsmqBridge.cpp
        - BlxSqlTriggerEvent.cpp
        - xphelper.cpp
4. CommonLib : This project will require the most redesign and refactoring.  There are many different concepts in it that should be pulled out and into different modules.  This will require most of the work potentially and will require many prompts and interactions to reconcile.


#### BlxContentService
The Windows service built using ATL.  It is designed to start and host all of the key modules or components that are used to solve the routing business problem.

#### BlxContentServiceData
The data abstraction layer.  In this version, C++/CLI was used with ADO.Net to communicate with the MS Sql DB.  However, in the refactor version, I want to use ODBC and native C++ as discussed in the ODBC C++ wrapper discussion.

#### BlxSqlMsmqBridge
To enable guaranteed delivery of updates to the Publishing Point, Publisher, and Subscriber tables, a shim was created as an extended stored proceedure (xproc) that was loaded by SQL Server and was able to push updates to MSMQ under the same DTC as the update.  Going forward, Microsoft has removed support for xprocs so we'll have to discover a new way of doing this.  However, I'd still like to continue the use of MSMQ.

#### CommonLib
This library was a catchall for everything else.  There are so many different concepts and module candidates in this one library and it will require the most work to understand and refactor or redesign.  Because there were items shared between the server and the clients (this project is just focused on the server) all the concepts (e.g., audio, video, all constants, etc.) were placed in this one file (under alot of pressure at the time to complete).  As a key goal for this project, we must refactor unrelated items into their own modules.

One of the key deliverable we must achieve first is the agreement of the different logical modules that is represented by all of this code.  The resulting logical modules should then be candidates to forward engineer into physical modules which will then lead to the creation of the layered deployment diagram in PlantUML.  These modules will be used to create the interface and implementation in C++ modules and packaged in DLLs that will be loaded by the service at runtime.

> ***Note:*** There is an issue with ChatGPT accessing the github repo directly.  In one session I had asked if it (ChatGPT) could reach a link to a repo and it claimed that it could and gave me some feedback.  However, when I tried to get it to read just now, it claimed it was blocked even though the repo is public.  As a workaround, I downloaded the main branch of the repo as a zip file and uploaded that file to ChatGPT o3.  It looks like we're unblocked and can proceed.

*29-May-2025*

*Prompt* 
>Routing Service:  Overlay network developed in C++ on Windows Server.  I have created a prompt in my github repo for us to begin the process of redesigning and refactoring code that I've written in 2006-2007.  The ultimate goal is to migrate the service to C++20, the most recent version of Windows Server and other newer Microsoft Technologies.  The prompt can be found here:  https://github.com/mikebiz/bmobile/blob/main/docs/ideation-prompts.md  Toward the end, there is a heading "Initial Prompt".  This will contain some of the early questions and decisions we need to discuss.  Please review the entire document, provided books as reference, links as reference, and the folder of the repository noted in the legacy-source folder.  Once complete, please comeback with any suggestions on how to proceed and ideas you have on the two of us moving forward in this effort.  Thanks!

*Response*

___

### ✅ I’ve unpacked the ZIP and taken a first-pass inventory

* **ideation-prompts.md** – ≈ 12 KB; contains the overall charter, reading list, service-module sketch, and the **“Initial Prompt”** checklist.
* **legacy-source** – 124 C/C++ files (46 *.cpp*, 60 *.h*, 11 *.hpp*, 5 *.inl*, plus *.rc* and *.def*).

  * Biggest translation units: `ServerNetwork.cpp`, `PublishingPointSourceMs.cpp`, `SharedUtilities.cpp`, `PacketInfo.cpp`.
  * Logical groupings already match the **Legacy Project Breakdown** you wrote:

    1. **BlxContentService** (runtime service & network core)
    2. **BlxContentServiceData** (management / publishing-point logic)
    3. **BlxSqlMsmqBridge** (DB/MSMQ bridge & SQL triggers)
    4. **CommonLib** (a kitchen-sink of helpers: custom smart-ptrs, locks, IPC, heap wrappers, etc.)

Everything builds in VS 2008‐era project files; uses raw WinSock, ATL, custom thread pools, MSMQ, and direct SQL Server calls.

---

## Proposed roadmap (high-level)


| Phase                               | Goal                                                                | Key deliverables                                                                                                                                                                       | Why first?                                                                                                   |                                                   |
| ----------------------------------- | ------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ | ------------------------------------------------- |
| **0. Baseline / freeze**            | Tag `legacy-pre-modernization-2025-05-29`; push ZIP as history root | • Git branch + tag • `.gitignore` cleaned                                                                                                                                              | Guarantees we can diff/benchmark every refactor step.                                                        |                                                   |
| **1. Build spine**                  | Modern reproducible build on VS 2022                                | • **CMake ≥ 3.29** presets (\`windows-msvc-debug                                                                                                                                       | release\`)  • **vcpkg.json** for external deps (fmt, spdlog, GTest)  • GitHub Actions CI (build + unit-test) | Gives us green/red feedback before touching code. |
| **2. Slice-by-slice modernization** | Migrate one execution path end-to-end                               | 2.1 “packet ingress ➜ routing ➜ egress” rewritten with:  • `std::jthread`  • RIO/IOCP awaitables (<cppcoro> or Boost.Asio)  • span-based buffers  • error-handling via `std::expected` | Lets us benchmark perf delta, shake out build & test infra, and set coding conventions.                      |                                                   |
| **3. Hexagonal decomposition**      | Separate **Domain Core** from **Infrastructure Adapters**           | • Pure-C++20 routing library (no Windows headers)  • Adapter layer (network, timers, logs, persistence)  • Windows-Service host + CLI harness                                          | Aligns with your PlantUML layer diagram and future portability (Linux test harness, etc.).                   |                                                   |
| **4. Data/management layer**        | Refactor BlxContentServiceData & SQL bridge                         | • Replace ADO/XO with ODBC + `sqlpp11` or `soci`  • MSMQ → Azure Service Bus or Rabbit? (decide)                                                                                       | Builds on stable core; management path less latency-critical.                                                |                                                   |
| **5. Hardening & ops**              | Observability, packaging, docs                                      | • spdlog structured logging  • Prometheus exporter  • Chocolatey or MSIX installer  • ADRs + PlantUML diagrams auto-published by MkDocs                                                | Makes the service production-ready on modern Server 2025.                                                    |                                                   |

---

## Concrete next steps you can do **today**

1. **Import the ZIP** into the GitHub repo’s *main* branch (or a `legacy/` branch) and tag it.
2. **Create an empty `cmake/` scaffold** (top-level `CMakeLists.txt` + preset JSON).
3. **Add vcpkg manifest** with at least `fmt` and `googletest`; wire vcpkg into the preset.
4. **Spin up the first GitHub Actions workflow** that just configures & builds a stub target.
5. Paste (or gist) **one representative file** you’re ready to modernize—`BlxBuffer.hpp` or `ServerNetwork.cpp` are great candidates. I’ll do a focused C++20 rewrite so we nail style/conventions early.

---

## Collaboration plan

* **Design docs** → keep adding to `docs/` as Markdown + PlantUML (`*.puml` fenced blocks).
* **ADR template** → `docs/adr/0001-record-architecture-decisions.md`.
* **Issue tracker** → one issue per vertical slice; link to ADR and PR.
* **Chat workflow with me** → paste or upload only the files/slices we’re tackling; I’ll review, refactor, and return diffs or drop-in replacements.

---

### Questions for you before we dive deeper

1. **Build system preference:** pure CMake, or keep VS solution and gradually transition?
2. **IO model target:** stick with RIO/IOCP (best perf on Windows) or wrap Boost.Asio to keep portability?
3. **MSMQ future:** keep, migrate to Azure Service Bus, or replace with a modern queue?
4. **First slice focus:** network core (my default suggestion) or CommonLib cleanup first?

Let me know the answers (and push / paste the first file set). Then we’ll jump straight into code-level modernization.

___

*Answer to Questions*
> **(1)** I would like to keep the VS Solution.  I may eventually migrate to CMake but don't want that complexity now.  **(2)**  I want to stick with (RIO/IOCP/Windows Threadpool as it will provide best performance on Windows.  I'm not interested in platform portability at this time.  **(3)** I would like to continue with MSMQ unless there is a more up-to-date service on Windows Server. **(4)**  My first and primary goal is to get the redesign completed first before anything else.  I think this mean digging into the CommonLib and the various exe and dll's to understand how they should be segmented in to modules and layered into the solution.  Does this make sense?

*Response*
___
Got it — we’ll retire every **“Blx”/“blx::”** symbol and adopt **“Bmobile”/“bmobile::”** instead.
Here’s how we’ll weave that change into the refactor plan without derailing momentum.

---

## 1 Global renaming strategy

| Task                                                                 | Tool / approach                                                                                                                                                      | Notes                                                                                                              |
| -------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------ |
| **1.1 Inventory**                                                    | `ripgrep -n "blx::\|Blx"` (or VS “Find in Files”)                                                                                                                    | Get an exact count of identifiers, macro names, project names, resources, and registry keys that start with *Blx*. |
| **1.2 Mechanical rename in C++**                                     | *Phase-by-phase*, using Clang-Tidy’s `modernize-rename-namespace`, or VS “Rename Symbol” when we extract a file to the new libs.                                     | Do it **as we move files** into the new static libs (bmobileUtil, etc.) so merges stay small and testable.         |
| **1.3 Project / DLL names**                                          | In VS *Property → General → Target Name* change `BlxRuntime` → `BmobileRuntime`, etc.                                                                                | No code touches; same renaming commit as the first code move so CI still passes.                                   |
| **1.4 Legacy COM/registry strings**                                  | For MSMQ queue names or registry keys that start “Blx…”, map them in one compatibility header: `constexpr std::wstring_view LEGACY_QUEUE = L"Blx…";` → new constant. | Keeps wire compatibility while code switches to `Bmobile*` names.                                                  |
| **1.5 Transitional typedef (optional)**                              | \`\`\`cpp                                                                                                                                                            |                                                                                                                    |
| namespace bmobile { /\* new code \*/ }                               |                                                                                                                                                                      |                                                                                                                    |
| namespace blx = bmobile;   // temporary, delete once all files moved |                                                                                                                                                                      |                                                                                                                    | Gives us wiggle room during the cut-over; remove by Phase 3. |

---

## 2 Updated solution & library layout

```

Bmobile.sln
│
├─ BmobileRuntime      (EXE · Windows-service host)
├─ BmobileMgmtAPI      (DLL · IPC/MSMQ façade)
│
├─ bmobileUtil         (LIB · fmt/spdlog, time, json, helpers)
├─ bmobileCoreRouting  (LIB · pure C++20 domain code)
├─ bmobileNetRIO       (LIB · RIO/IOCP adapters + thread-pool)
├─ bmobileDataAccess   (LIB · MSMQ + SQL/ODBC glue)
└─ tests               (GoogleTest)
```

*Filenames*: when we move `BlxSmartHandle.h` into **bmobileUtil**, rename the file to `SmartHandle.hpp` (no prefix).  
*Namespaces*: top of each new file starts with

```cpp
export module bmobile.util;          // once we switch to modules
namespace bmobile { … }
```

---

## 3 Refactor flow with renaming baked in

| Stage      | File action                                                                                                                      | Rename action                         | Test action                      |
| ---------- | -------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------- | -------------------------------- |
| **Day 1**  | Copy `Guid.*` → `libs\bmobileUtil\Guid.hpp/.cpp`                                                                                 | `namespace bmobile` + adjust includes | Add `guid_tests.cpp` to `tests`. |
| **Day 2**  | Copy `SmartHandle.*` → `bmobileUtil`                                                                                             | Same                                  | Add unit tests.                  |
| **Day 3**  | Move `Timer.*`, `StringUtil.*`                                                                                                   | Same                                  | Tests.                           |
| **Week 1** | When all util headers moved, **delete** old versions from legacy project; commit = “Extract util + rename Blx→Bmobile (part 1)”. |                                       |                                  |
| **Week 2** | Begin `bmobileCoreRouting` extraction; each move also flips namespace.                                                           |                                       |                                  |

We’ll keep PRs ≤ 400 LOC so reviewers (and you) can sanity-check every rename.

---

## 4 Automation aids

* **Clang-Tidy** command to run in the `libs\` subtree only:

  ```bash
  clang-tidy -fix -checks=modernize-rename-namespace \
      -config="{RenameNamespace: {OldNamespace: 'blx', NewNamespace: 'bmobile'}}" \
      -- <compile_commands.json>
  ```
* **CI gate**: add a GitHub Actions step that fails if `ripgrep -n blx::` finds anything in non-legacy directories after Phase 2.
* **Formatting**: updated `.clang-format` with `NamespaceIndentation: All` so the new nested namespaces stay tidy.

---

## 5 Immediate next steps

1. **Create the new static-library project “bmobileUtil”** in VS, empty for now.
2. **Copy `Guid.h/.cpp` into it**, rename to `Guid.hpp/.cpp`, change the namespace to `bmobile`, update `#pragma once` (or keep).
3. **Add GoogleTest target**, write a quick equality/parse test for `Guid`.
4. **Commit** as `feat(util): start bmobileUtil, migrate Guid, rename namespace`.
5. **Push & ping me** with the code snippet (or ZIP that subset). I’ll:

   * refactor it to idiomatic C++20 (use `std::array<std::byte, 16>`, `std::expected<std::string, GuidError>`, etc.),
   * supply the matching unit test,
   * show how to apply `clang-tidy` for the next files.

With that template locked in, every subsequent move + rename becomes mechanical.

---

### Quick confirmation

*Does the above renaming workflow and library naming scheme look correct?*
If yes, drop the first header when you’re ready, and we’ll start coding.


___

***Additional Questions***
> These items will become .lib files and not .dll, am I understanding this correctly?  ├─ bmobileUtil (LIB · fmt/spdlog, time, json, helpers)
├─ bmobileCoreRouting (LIB · pure C++20 domain code)
├─ bmobileNetRIO (LIB · RIO/IOCP adapters + thread-pool)
├─ bmobileDataAccess (LIB · MSMQ + SQL/ODBC glue)

***Response***
Yes — for the redesign phase I’m treating the four “bmobile\*” components as **static libraries (.lib)** rather than DLLs:

```
BmobileRuntime.exe      ──┬──>  bmobileUtil.lib
                          ├──>  bmobileCoreRouting.lib
                          ├──>  bmobileNetRIO.lib
                          └──>  bmobileDataAccess.lib
BmobileMgmtAPI.dll      ───┘   (optional management façade)
```

### Why start with static .libs?

| Reason                     | Benefit right now                                                                                                                                             |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **No deployment friction** | The EXE (and the one Mgmt DLL) ship self-contained; no version-hell from misplaced DLLs, no SxS manifests, no extra installer steps.                          |
| **Easier refactor loop**   | You can move classes, functions, and even internal headers between libs without worrying about symbol-export baggage (`__declspec(dllexport)` / DEF files).   |
| **Single CRT & allocator** | Everything is linked against the same `/MT` or `/MD` runtime, which sidesteps cross-DLL ownership rules for sockets, handles, `std::string`, etc.             |
| **Performance**            | Call boundaries disappear at link-time (especially with LTCG and /OPT\:ICF), and there’s no DLL import-table indirection for hot paths such as RIO callbacks. |
| **Unit-test friendliness** | GoogleTest can link directly against each static lib; no need to build special test harness DLLs.                                                             |

### What if we later **need** DLL boundaries?

* The source layout (one VS project per component) stays the same:
  *Project -→ **Configuration Type** -→ “Dynamic Library (.dll)”*
* The public headers will already be clean, because we’ll use interface partition files / modules. We can add an `#ifdef Bmobile_EXPORTS` macro later without big churn.
* **When to flip**:

  * If you want a plug-in model (third-party modules),
  * If you must expose COM/WinRT or gRPC endpoints directly from `bmobileCoreRouting`,
  * If multiple processes need to share the same code in memory.

But until one of those drivers appears, static `.lib` keeps build & deploy velocity high.

### Visual Studio setup (quick checklist)

1. **Project → Add → New Project → “Static Library (C++)”.**
2. *General* tab → change *Project Name* to `bmobileUtil`, *Target Name* to `bmobileUtil` (no “lib” prefix).
3. **C/C++ → Language → /std\:c++20** and `/permissive-`.
4. **Code Generation → Runtime Library**: decide `/MT` (static CRT) vs `/MD` (DLL CRT). Staying consistent across all projects avoids linker headaches.
5. For the EXE and Mgmt DLL, add the four `.lib` projects as **Project Dependencies** so VS builds in the right order.

### Bottom line

*All* the reusable code lives in those static libraries; the runtime host and the optional management façade are the only binaries you’ll deploy. If a future requirement surfaces that justifies DLLs, we can flip the relevant project(s) with minimal rework.

Let me know when you’ve created the first static-lib project (e.g., **bmobileUtil**) and dropped in the initial file (Guid, SmartHandle, etc.). I’ll review the code and provide the C++20 refactor template.
