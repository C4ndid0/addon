#include "cep_adapter.h"
#include <stdexcept>
#include <vector>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <unordered_map>

namespace {
    void* libHandleCEP = nullptr;

    // Function types
    using CEP_Inicializar_Func = int (*)(const char*, const char*);
    using CEP_Finalizar_Func = int (*)();
    using CEP_BuscarPorCEP_Func = int (*)(const char*, char*, int);
    using CEP_UltimoRetorno_Func = int (*)(char*, int);

    // Function pointers
    struct LibFunctions {
        CEP_Inicializar_Func inicializar = nullptr;
        CEP_Finalizar_Func finalizar = nullptr;
        CEP_BuscarPorCEP_Func buscarPorCEP = nullptr;
        CEP_UltimoRetorno_Func ultimoRetorno = nullptr;
    } libFuncs;

    // Error handling
    std::string getErrorMessage(int errorCode) {
        static const std::unordered_map<int, std::string> errorMessages = {
            {-10, "Erro de conexão com o webservice. Verifique sua conexão com a internet e configurações de SSL/Proxy."},
            {-1, "Erro de inicialização da biblioteca."},
            {-2, "CEP inválido ou não encontrado."},
            {-5, "Erro de timeout na conexão."}
        };

        auto it = errorMessages.find(errorCode);
        if (it != errorMessages.end()) {
            return it->second;
        }

        try {
            return cepUltimoRetorno();
        } catch(...) {
            return "Erro desconhecido ao consultar CEP (código " + std::to_string(errorCode) + ")";
        }
    }

    std::string getLibraryPath() {
#ifdef _WIN32
        return "./lib/Windows/ACBrCEP64.dll";
#else
        return "./lib/Linux/libacbrcep64.so";
#endif
    }

    void* loadLibrary(const std::string& path) {
#ifdef _WIN32
        return LoadLibraryA(path.c_str());
#else
        return dlopen(path.c_str(), RTLD_LAZY);
#endif
    }

    void* getSymbol(void* handle, const char* name) {
#ifdef _WIN32
        return (void*)GetProcAddress((HMODULE)handle, name);
#else
        return dlsym(handle, name);
#endif
    }

    std::string getLoadError() {
#ifdef _WIN32
        DWORD error = GetLastError();
        char* msgBuf = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&msgBuf,
            0,
            NULL
        );
        std::string errorMsg = msgBuf ? msgBuf : "Erro desconhecido";
        LocalFree(msgBuf);
        return errorMsg;
#else
        const char* error = dlerror();
        return error ? error : "motivo desconhecido";
#endif
    }

    void loadLibraryFunctions() {
        if (libHandleCEP) return;

        std::string libPath = getLibraryPath();
        libHandleCEP = loadLibrary(libPath);
        
        if (!libHandleCEP) {
            throw std::runtime_error(
                "Erro ao carregar biblioteca (" + libPath + "): " + 
                getLoadError()
            );
        }

        // Load all functions
        libFuncs.inicializar = (CEP_Inicializar_Func)getSymbol(libHandleCEP, "CEP_Inicializar");
        libFuncs.finalizar = (CEP_Finalizar_Func)getSymbol(libHandleCEP, "CEP_Finalizar");
        libFuncs.buscarPorCEP = (CEP_BuscarPorCEP_Func)getSymbol(libHandleCEP, "CEP_BuscarPorCEP");
        libFuncs.ultimoRetorno = (CEP_UltimoRetorno_Func)getSymbol(libHandleCEP, "CEP_UltimoRetorno");

        if (!libFuncs.inicializar || !libFuncs.finalizar || 
            !libFuncs.buscarPorCEP || !libFuncs.ultimoRetorno) {
            throw std::runtime_error(
                "Erro ao resolver funções da biblioteca: " + 
                getLoadError()
            );
        }
    }

    std::string sanitizeCEP(const std::string& cep) {
        std::string result;
        result.reserve(8);
        for (char c : cep) {
            if (std::isdigit(c)) {
                result += c;
            }
        }
        return result;
    }
} // anonymous namespace

int cepInicializar(const std::string& arqConfig, const std::string& chaveCrypt) {
    loadLibraryFunctions();
    return libFuncs.inicializar(
        arqConfig.empty() ? "" : arqConfig.c_str(),
        chaveCrypt.empty() ? "" : chaveCrypt.c_str()
    );
}

int cepFinalizar() {
    return libFuncs.finalizar ? libFuncs.finalizar() : -1;
}

std::string cepUltimoRetorno() {
    if (!libFuncs.ultimoRetorno) {
        return "Função CEP_UltimoRetorno não disponível";
    }

    std::vector<char> buffer(1024);
    int ret = libFuncs.ultimoRetorno(buffer.data(), buffer.size());
    
    if (ret > static_cast<int>(buffer.size())) {
        buffer.resize(ret);
        libFuncs.ultimoRetorno(buffer.data(), buffer.size());
    }
    
    return std::string(buffer.data());
}

std::string cepBuscarPorCEP(const std::string& cep) {
    if (!libFuncs.buscarPorCEP) {
        throw std::runtime_error("ACBrLibCEP não inicializada");
    }

    std::string cepNumerico = sanitizeCEP(cep);
    if (cepNumerico.length() != 8) {
        throw std::runtime_error("CEP deve conter 8 dígitos numéricos");
    }

    std::vector<char> buffer(2000);
    int ret = libFuncs.buscarPorCEP(cepNumerico.c_str(), buffer.data(), buffer.size());
    
    if (ret < 0) {
        throw std::runtime_error("Erro CEP_BuscarPorCEP: " + getErrorMessage(ret));
    }
    
    if (ret > static_cast<int>(buffer.size())) {
        buffer.resize(ret);
        ret = libFuncs.buscarPorCEP(cepNumerico.c_str(), buffer.data(), buffer.size());
        if (ret < 0) {
            throw std::runtime_error("Erro CEP_BuscarPorCEP (2ª chamada): " + getErrorMessage(ret));
        }
    }
    
    return std::string(buffer.data());
}
