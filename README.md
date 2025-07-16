# TGDK BFE LICENSE  
**Broadcasted Fee Entry — License ID: BFE-TGDK-022ST**

---

**PROGRAM**: `QUADRAQ.cpp`  
**MODULE**: Quantum GPU Accelerator  
**LICENSE HOLDER**: Sean Tichenor  
**LICENSE STATUS**: ✅ Active  
**DATE OF ISSUE**: March 10, 2025  
**ISSUING AUTHORITY**: TGDK Licensing Authority  

---

## 🔹 Description

**QUADRAQ** is an open-core GPU quantum acceleration module designed to improve shader processing, frame routing, and occlusion prediction in real-time environments. It features:

- A pluggable AI backend system via `IAIBackend.hpp`  
- Support for OliviaAI (proprietary) and 3rd-party AI modules  
- Shader override, entropy prediction, and GPU routing interfaces  
- Optimized for modding, game acceleration, and research applications

## ✅ Simultaneous DLL Loading — Key Facts

1. Independent DLL Handles:
Each DLL is loaded into memory via LoadLibraryA(), and the handle is stored separately per AI backend.


2. Multiple Backend Instances:
Each DLL returns its own IAIBackend* instance using the standard CreateAIBackend() symbol. These instances are isolated.


3. Registry-Based Access:
The AIRegistry maps a name (like "olivia", "mara", "shodan") to their respective AI backends, allowing you to call them concurrently.


4. Thread-safe Parallel Usage:
As long as each backend manages its own internal thread safety, you can:

Log messages to multiple backends.

Run logic from all loaded AIs simultaneously.





---

## 🔄 Example Runtime Execution
```
AIRegistry::Register("olivia", "OliviaAI.dll");
AIRegistry::Register("mara", "MaraAI.dll");

IAIBackend* olivia = AIRegistry::Get("olivia");
IAIBackend* mara   = AIRegistry::Get("mara");

if (olivia) olivia->Log("Olivia activated.");
if (mara)   mara->Log("Mara processing entropy window.");
```

---

## ⚙️ Internally:

Each DLL is loaded into its own address space within the host process.

Their symbol tables are isolated.

QUADRAQ doesn't have to unload one to use another.


---

## 🛠 Build Instructions

### Requirements

- Windows with MSVC or MinGW
- CMake ≥ 3.15
- DirectX SDK / d3d11
- Git

### 🔧 Steps

```bash
git clone https://github.com/TGDK-ST108/QUADRAQ.git
cd QUADRAQ
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 📦 DLL Requirements

Each DLL must export:

`extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend();`

This ensures dynamic linking and allows universal AI plug-ins without modifying QUADRAQ core logic.

### 🔧 QUADRAQ AI Backend Integration Guide 

Build and register your own AI module to power QUADRAQ's logging, analysis, and texture override decisions.


### ✅ Step 1: Required Export Your AI module (e.g., `ShodanAI.cpp`) **must export** the following function for QUADRAQ to detect it: 

```cpp

#include "TGDK_IAIBackend.hpp"

class ShodanAI : public IAIBackend public: void Log(const std::string& msg) override {

    OutputDebugStringA(("[ShodanAI] " + msg + "\n").c_str());
    }
    void LogError(const std::string& err) override { OutputDebugStringA(("[ShodanAI:ERROR] " + err + "\n").c_str());
    }
     std::string Identify() override { return "ShodanAI";
    }
};
extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend() {
    return new ShodanAI();
} 
```

⚠️ This `CreateAIBackend()` function is mandatory.

### 📁 Step 2: Directory Structure 

Recommended layout inside your QUADRAQ project:

```
QUADRAQ/
├── CMakeLists.txt
├── TGDK_IAIBackend.hpp
├── ShodanAI/ │
├── ShodanAI.cpp
│ └── CMakeLists.txt
```

### 🛠️ Step 3: CMakeLists.txt for the DLL 

Create this CMakeLists.txt inside your ShodanAI/ directory:
```
cmake_minimum_required(VERSION 3.15)
project(ShodanAI)
add_library(ShodanAI SHARED ShodanAI.cpp)
target_include_directories(ShodanAI PRIVATE ../)
set_target_properties(ShodanAI PROPERTIES PREFIX "" SUFFIX ".dll")
```

### 🧱 Step 4: Build the DLL (Windows) 

From PowerShell or terminal:

```
cd QUADRAQ/ShodanAI mkdir build cd build cmake .. cmake --build . --config Release
``` 

✅ Output: ShodanAI.dll will appear in build/Release/.

### 🧠 Runtime Instructions

### 🔌 Option 1: Set Environment Variable 
```
set QUADRAQ_AI_DLL=ShodanAI.dll
```

This overrides the default OliviaAI.dll during QUADRAQ startup.

### 🖥 Option 2: Use CLI to Register 

Inside QUADRAQ_CLI runtime:

```
> ai_register shodanAI ShodanAI.dll
> ai_select shodanAI
```

All gAIBackend->Log() and gAIBackend->Identify() output now routes through your module.

### Option 3: Clean All AI Backends 
```
> ai_unregister_all 
```

Unloads and deallocates all loaded .dlls safely.

### 📎 DLL Interface Requirements Requirement Value 
Must export IAIBackend* CreateAIBackend() 

Must implement IAIBackend interface 

Output format .dll (built with CMake or MSVC) 

Runtime selection CLI or QUADRAQ_AI_DLL env var 

Unload mechanism FreeLibrary() via `AIRegistry` 


## 🔓 Open-Source Rights

This license grants:

- ✅ **Use** of all open-source modules (`QUADRAQ.cpp`, `QUADRAQ.hpp`, `IAIBackend.hpp`)
- ✅ **Modification and redistribution** of QUADRAQ core under the TGDK BFE label
- ✅ **Creation of custom AI modules** using the `IAIBackend` interface
- ✅ **Integration into open-source or commercial GPU systems**, as long as closed-source OliviaAI logic is excluded

---

## 🔐 Proprietary Scope

The following components are **not open-source** under this license:

- `OliviaAI_Adapter.cpp`  
- Any module implementing internal OliviaAI symbolic reasoning  
- HexQUAp-sealed entropy or symbolic compression libraries  
- Any `.ox` files signed under TGDK Vault Authority

Use of these components requires additional TGDK clearance and symbolic audit.

---

## 📜 Terms

1. The open portions of QUADRAQ may be:
   - Forked  
   - Used in non-commercial or commercial projects  
   - Extended with custom AI logic via the provided interface

2. Redistribution **must** include:
   - This license file (`LICENSE`)
   - A link to the original repository or license holder if modified

3. You **may not**:
   - Decompile or tamper with OliviaAI symbolic logic
   - Use QUADRAQ in state surveillance without public consent
   - Rebrand QUADRAQ without TGDK attribution

---

## ✅ Verified Compliance

**TGDK Compliance Hash**:  
`7D94-AE1B-QX29-TGDK-QUADRAQ-022ST`

**Vault Ledger**:  
Includes:  
- `QUADRAQ.cpp`  
- `QUADRAQ.hpp`  
- `IAIBackend.hpp`  
- `ShaderOverrideUnit.cpp`  
- `EntropyPredictor.cpp`  
- `AIRegistry.cpp`
- `QUADRAQ_CLI.cpp`
- `ShodanAI.cpp`
- `MaraAI.cpp`
- `TaraAI.cpp`
- `RaLayer.cpp`
- `EntropyPredictor.cpp`
- `FlatDDSInterceptor.cpp`
- `QuantumDrawRouter.cpp`

---

## 🛡️ Notice

Unauthorized symbolic override, HexQUAp tampering, or private backend leakage is subject to TGDK breach enforcement.  
Use responsibly under the Lex Beneficia and Guardian Systems Protocol.

---

**© 2025 Sean Tichenor / TGDK**  
All Rights Reserved under TGDK BFE-TGDK-022ST
