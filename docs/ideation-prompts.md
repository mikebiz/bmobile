# Initial Prompt:
This initial prompt is to kick-off the collaboration to migrate the Windows service to C++20.  It will be a process of analysis, design, refactoring, and migration.  \
As we begin the journey, I want to set some initial guidance to help develop a baseline.

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
This Windows Service was created in 2006/2007.  While it was needed to provide a specific purpose to solve a business problem, it evolved into what amounts \
to a network packet dissemination or broadcast service based on the notion of "Publishing Points", "Publishers", and "Subscribers".  For this first migration, we \
will focus solely on the network routing aspects and not the logic to manage the publishing points, publishers or subscribers.

A Publishing Point can contain 1 or more publishers and 1 or more subscribers.  One of the basic concepts this will support is a routing service. 

The functionality of the service, at a high-level, is pretty simple; it ingests network packets, determines what publishing point they are destined to, pulls the list \
of subscribers for the publishing point from an in-memory routing table, and then pushes the packets to them.  For the most part, the network packets are opaque \
to the server with the exception being the first section or bytes of the packet.

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






