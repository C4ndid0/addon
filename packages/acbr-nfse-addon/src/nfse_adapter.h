#ifndef NFSE_ADAPTER_H
#define NFSE_ADAPTER_H

#include <napi.h>
#include <string>

class NFSeAdapter : public Napi::ObjectWrap<NFSeAdapter> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NFSeAdapter(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  // ACBrLibNFSe methods
  Napi::Value Initialize(const Napi::CallbackInfo& info);
  Napi::Value Finalize(const Napi::CallbackInfo& info);
  Napi::Value LoadConfig(const Napi::CallbackInfo& info);
  
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

  // Internal helper methods
  std::string CheckResult(int ret);
  void HandleError(const std::string& error);

  bool initialized;
  std::string lastError;
};

#endif // NFSE_ADAPTER_H 