#include <napi.h>
#include "cep_adapter.h"

namespace {
    // Helper to validate string arguments
    std::string getStringArg(const Napi::CallbackInfo& info, size_t index, const char* defaultValue = "") {
        if (info.Length() > index && info[index].IsString()) {
            return info[index].As<Napi::String>();
        }
        return defaultValue;
    }

    // Helper to throw JavaScript errors
    void throwError(const Napi::Env& env, const std::string& message) {
        Napi::Error::New(env, message).ThrowAsJavaScriptException();
    }
}

// initializeCEP(arqConfig?: string, chaveCrypt?: string) → string
Napi::Value InitializeCEP(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        std::string arqConfig = getStringArg(info, 0);
        std::string chaveCrypt = getStringArg(info, 1);
        
        int ret = cepInicializar(arqConfig, chaveCrypt);
        if (ret != 0) {
            throwError(env, "CEP_Inicializar falhou: " + cepUltimoRetorno());
            return env.Null();
        }
        return Napi::String::New(env, "OK");
    } catch (const std::exception& e) {
        throwError(env, e.what());
        return env.Null();
    }
}

// finalizeCEP() → string
Napi::Value FinalizeCEP(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        int ret = cepFinalizar();
        if (ret != 0) {
            throwError(env, "CEP_Finalizar falhou: " + cepUltimoRetorno());
            return env.Null();
        }
        return Napi::String::New(env, "OK");
    } catch (const std::exception& e) {
        throwError(env, e.what());
        return env.Null();
    }
}

// consultCep(cep: string) → string (JSON ou exceção)
Napi::Value ConsultCep(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    try {
        if (info.Length() < 2) {
            throwError(env, "Argumentos insuficientes. Esperado: cep, responseData");
            return env.Null();
        }

        if (!info[0].IsString() || !info[1].IsString()) {
            throwError(env, "Argumentos inválidos. Esperado: string, string");
            return env.Null();
        }

        std::string cep = info[0].As<Napi::String>();
        std::string responseData = info[1].As<Napi::String>();
        
        return Napi::String::New(env, cepBuscarPorCEP(cep));
    } catch (const std::exception& e) {
        throwError(env, e.what());
        return env.Null();
    }
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    exports.Set("initializeCEP", Napi::Function::New(env, InitializeCEP));
    exports.Set("finalizeCEP", Napi::Function::New(env, FinalizeCEP));
    exports.Set("consultCep", Napi::Function::New(env, ConsultCep));
    return exports;
}

NODE_API_MODULE(acbr_cep, InitAll)
