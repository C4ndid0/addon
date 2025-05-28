#ifndef NFSE_METHODS_H
#define NFSE_METHODS_H

#include <napi.h>
#include "nfse_adapter.h"

// Function declarations for NFSe methods
namespace NFSeMethods {
    // RPS Operations
    Napi::Value CreateRPS(const Napi::CallbackInfo& info);
    Napi::Value AddRPS(const Napi::CallbackInfo& info);
    Napi::Value SignRPS(const Napi::CallbackInfo& info);
    Napi::Value ValidateRPS(const Napi::CallbackInfo& info);
    Napi::Value TransmitRPS(const Napi::CallbackInfo& info);

    // NFSe Operations
    Napi::Value ConsultNFSeByRPS(const Napi::CallbackInfo& info);
    Napi::Value ConsultNFSeByNumber(const Napi::CallbackInfo& info);
    Napi::Value ConsultNFSeByPeriod(const Napi::CallbackInfo& info);
    Napi::Value CancelNFSe(const Napi::CallbackInfo& info);

    // Utility methods
    Napi::Value GetLastResponse(const Napi::CallbackInfo& info);
    Napi::Value SetMunicipality(const Napi::CallbackInfo& info);
    Napi::Value SetProvider(const Napi::CallbackInfo& info);
    Napi::Value SetEnvironment(const Napi::CallbackInfo& info);
}

#endif // NFSE_METHODS_H 