#include <node_api.h>
#include <napi.h>
#include "nfse_adapter.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// Function type definitions for ACBrLibNFSe
typedef int (*NFSE_Inicializar)(const char* eArqConfig, const char* eChaveCrypt);
typedef int (*NFSE_Finalizar)();
typedef int (*NFSE_Nome)(char* buffer, int bufferSize);
typedef int (*NFSE_Versao)(char* buffer, int bufferSize);
typedef int (*NFSE_UltimoRetorno)(char* buffer, int bufferSize);
typedef int (*NFSE_ConfigLer)(const char* eArqConfig);
typedef int (*NFSE_ConfigGravar)(const char* eArqConfig);
typedef int (*NFSE_CarregarXML)(const char* eArquivoOuXML);
typedef int (*NFSE_CarregarINI)(const char* eArquivoOuINI);
typedef int (*NFSE_ObterXml)(int AIndex, char* buffer, int bufferSize);
typedef int (*NFSE_GravarXml)(int AIndex, const char* eNomeArquivo, const char* ePathArquivo);
typedef int (*NFSE_LimparLista)();
typedef int (*NFSE_Assinar)();
typedef int (*NFSE_Validar)();
typedef int (*NFSE_Enviar)(int aLote, bool imprimir, bool sincrono, int timeout);
typedef int (*NFSE_ConsultarLoteRps)(const char* eProcotolo, bool aImprimir);
typedef int (*NFSE_ConsultarNFSeporRps)(const char* eNumeroRps, const char* eSerie, const char* eTipo, bool aImprimir);
typedef int (*NFSE_ConsultarNFSePorNumero)(const char* eNumero, bool aImprimir);
typedef int (*NFSE_ConsultarNFSePorPeriodo)(const char* eDataInicial, const char* eDataFinal, bool aImprimir);
typedef int (*NFSE_CancelarNFSe)(const char* eNumero, const char* eMotivo, const char* eCodigoVerificacao);

// Static function pointers
static NFSE_Inicializar NFSe_Inicializar = nullptr;
static NFSE_Finalizar NFSe_Finalizar = nullptr;
static NFSE_Nome NFSe_Nome = nullptr;
static NFSE_Versao NFSe_Versao = nullptr;
static NFSE_UltimoRetorno NFSe_UltimoRetorno = nullptr;
static NFSE_ConfigLer NFSe_ConfigLer = nullptr;
static NFSE_ConfigGravar NFSe_ConfigGravar = nullptr;
static NFSE_CarregarXML NFSe_CarregarXML = nullptr;
static NFSE_CarregarINI NFSe_CarregarINI = nullptr;
static NFSE_ObterXml NFSe_ObterXml = nullptr;
static NFSE_GravarXml NFSe_GravarXml = nullptr;
static NFSE_LimparLista NFSe_LimparLista = nullptr;
static NFSE_Assinar NFSe_Assinar = nullptr;
static NFSE_Validar NFSe_Validar = nullptr;
static NFSE_Enviar NFSe_Enviar = nullptr;
static NFSE_ConsultarLoteRps NFSe_ConsultarLoteRps = nullptr;
static NFSE_ConsultarNFSeporRps NFSe_ConsultarNFSeporRps = nullptr;
static NFSE_ConsultarNFSePorNumero NFSe_ConsultarNFSePorNumero = nullptr;
static NFSE_ConsultarNFSePorPeriodo NFSe_ConsultarNFSePorPeriodo = nullptr;
static NFSE_CancelarNFSe NFSe_CancelarNFSe = nullptr;

#ifdef _WIN32
static HMODULE hLib = nullptr;
#else
static void* hLib = nullptr;
#endif

Napi::FunctionReference NFSeAdapter::constructor;

Napi::Object NFSeAdapter::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "NFSeAdapter", {
        InstanceMethod("initialize", &NFSeAdapter::Initialize),
        InstanceMethod("finalize", &NFSeAdapter::Finalize),
        InstanceMethod("loadConfig", &NFSeAdapter::LoadConfig),
        InstanceMethod("createRPS", &NFSeAdapter::CreateRPS),
        InstanceMethod("addRPS", &NFSeAdapter::AddRPS),
        InstanceMethod("signRPS", &NFSeAdapter::SignRPS),
        InstanceMethod("validateRPS", &NFSeAdapter::ValidateRPS),
        InstanceMethod("transmitRPS", &NFSeAdapter::TransmitRPS),
        InstanceMethod("consultNFSeByRPS", &NFSeAdapter::ConsultNFSeByRPS),
        InstanceMethod("consultNFSeByNumber", &NFSeAdapter::ConsultNFSeByNumber),
        InstanceMethod("consultNFSeByPeriod", &NFSeAdapter::ConsultNFSeByPeriod),
        InstanceMethod("cancelNFSe", &NFSeAdapter::CancelNFSe),
        InstanceMethod("getLastResponse", &NFSeAdapter::GetLastResponse),
        InstanceMethod("setMunicipality", &NFSeAdapter::SetMunicipality),
        InstanceMethod("setProvider", &NFSeAdapter::SetProvider),
        InstanceMethod("setEnvironment", &NFSeAdapter::SetEnvironment),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("NFSeAdapter", func);
    return exports;
}

NFSeAdapter::NFSeAdapter(const Napi::CallbackInfo& info) 
    : Napi::ObjectWrap<NFSeAdapter>(info), initialized(false) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
}

Napi::Value NFSeAdapter::Initialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected for config path").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string configPath = info[0].As<Napi::String>().Utf8Value();
    
#ifdef _WIN32
    hLib = LoadLibrary("ACBrNFSe64.dll");
#else
    hLib = dlopen("./lib/Linux/libacbrnfse64.so", RTLD_LAZY);
#endif

    if (!hLib) {
        Napi::Error::New(env, "Failed to load ACBrNFSe library").ThrowAsJavaScriptException();
        return env.Null();
    }

#ifdef _WIN32
    #define LOAD_FUNCTION(type, name) name = (type)GetProcAddress(hLib, #name)
#else
    #define LOAD_FUNCTION(type, name) name = (type)dlsym(hLib, #name)
#endif

    LOAD_FUNCTION(NFSE_Inicializar, NFSe_Inicializar);
    LOAD_FUNCTION(NFSE_Finalizar, NFSe_Finalizar);
    LOAD_FUNCTION(NFSE_Nome, NFSe_Nome);
    LOAD_FUNCTION(NFSE_Versao, NFSe_Versao);
    LOAD_FUNCTION(NFSE_UltimoRetorno, NFSe_UltimoRetorno);
    LOAD_FUNCTION(NFSE_ConfigLer, NFSe_ConfigLer);
    LOAD_FUNCTION(NFSE_ConfigGravar, NFSe_ConfigGravar);
    LOAD_FUNCTION(NFSE_CarregarXML, NFSe_CarregarXML);
    LOAD_FUNCTION(NFSE_CarregarINI, NFSe_CarregarINI);
    LOAD_FUNCTION(NFSE_ObterXml, NFSe_ObterXml);
    LOAD_FUNCTION(NFSE_GravarXml, NFSe_GravarXml);
    LOAD_FUNCTION(NFSE_LimparLista, NFSe_LimparLista);
    LOAD_FUNCTION(NFSE_Assinar, NFSe_Assinar);
    LOAD_FUNCTION(NFSE_Validar, NFSe_Validar);
    LOAD_FUNCTION(NFSE_Enviar, NFSe_Enviar);
    LOAD_FUNCTION(NFSE_ConsultarLoteRps, NFSe_ConsultarLoteRps);
    LOAD_FUNCTION(NFSE_ConsultarNFSeporRps, NFSe_ConsultarNFSeporRps);
    LOAD_FUNCTION(NFSE_ConsultarNFSePorNumero, NFSe_ConsultarNFSePorNumero);
    LOAD_FUNCTION(NFSE_ConsultarNFSePorPeriodo, NFSe_ConsultarNFSePorPeriodo);
    LOAD_FUNCTION(NFSE_CancelarNFSe, NFSe_CancelarNFSe);

#undef LOAD_FUNCTION

    if (!NFSe_Inicializar || !NFSe_Finalizar) {
        Napi::Error::New(env, "Failed to load ACBrNFSe functions").ThrowAsJavaScriptException();
        return env.Null();
    }

    int ret = NFSe_Inicializar(configPath.c_str(), 0);
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to initialize ACBrNFSe: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    initialized = true;
    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::Finalize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (!initialized) {
        return Napi::Boolean::New(env, true);
    }

    int ret = NFSe_Finalizar();
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to finalize ACBrNFSe: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

#ifdef _WIN32
    if (hLib) FreeLibrary(hLib);
#else
    if (hLib) dlclose(hLib);
#endif

    initialized = false;
    return Napi::Boolean::New(env, true);
}

std::string NFSeAdapter::CheckResult(int ret) {
    if (ret >= 0) return "";

    char buffer[1024];
    int bufferLen = sizeof(buffer);
    
    if (NFSe_UltimoRetorno) {
        NFSe_UltimoRetorno(buffer, bufferLen);
        return std::string(buffer);
    }
    
    return "Unknown error";
}

void NFSeAdapter::HandleError(const std::string& error) {
    lastError = error;
}

// Implement other methods with proper error handling and parameter validation
Napi::Value NFSeAdapter::LoadConfig(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected for config path").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string configPath = info[0].As<Napi::String>().Utf8Value();
    int ret = NFSe_ConfigLer(configPath.c_str());
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to load config: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

// Implement remaining methods similarly...
Napi::Value NFSeAdapter::CreateRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::AddRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::SignRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::ValidateRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::TransmitRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::ConsultNFSeByRPS(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::ConsultNFSeByNumber(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::ConsultNFSeByPeriod(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::CancelNFSe(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::GetLastResponse(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::SetMunicipality(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::SetProvider(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
}

Napi::Value NFSeAdapter::SetEnvironment(const Napi::CallbackInfo& info) {
    // Implementation pending
    return info.Env().Null();
} 