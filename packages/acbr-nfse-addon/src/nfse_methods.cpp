// RPS Operations
Napi::Value NFSeAdapter::CreateRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Object expected for RPS data").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object rpsData = info[0].As<Napi::Object>();
    std::string xmlContent = rpsData.Get("xml").As<Napi::String>();

    int ret = NFSe_CarregarXML(xmlContent.c_str());
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to create RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::AddRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Object expected for RPS data").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object rpsData = info[0].As<Napi::Object>();
    std::string xmlContent = rpsData.Get("xml").As<Napi::String>();

    int ret = NFSe_CarregarXML(xmlContent.c_str());
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to add RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::SignRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int ret = NFSe_Assinar();
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to sign RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::ValidateRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int ret = NFSe_Validar();
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to validate RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::TransmitRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 4) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    int batch = info[0].As<Napi::Number>();
    bool print = info[1].As<Napi::Boolean>();
    bool sync = info[2].As<Napi::Boolean>();
    int timeout = info[3].As<Napi::Number>();

    int ret = NFSe_Enviar(batch, print, sync, timeout);
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to transmit RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

// NFSe Operations
Napi::Value NFSeAdapter::ConsultNFSeByRPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string number = info[0].As<Napi::String>();
    std::string series = info[1].As<Napi::String>();
    std::string type = info[2].As<Napi::String>();

    int ret = NFSe_ConsultarNFSeporRps(number.c_str(), series.c_str(), type.c_str(), false);
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to consult NFSe by RPS: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::ConsultNFSeByNumber(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected for NFSe number").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string number = info[0].As<Napi::String>();

    int ret = NFSe_ConsultarNFSePorNumero(number.c_str(), false);
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to consult NFSe by number: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::ConsultNFSeByPeriod(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string startDate = info[0].As<Napi::String>();
    std::string endDate = info[1].As<Napi::String>();

    int ret = NFSe_ConsultarNFSePorPeriodo(startDate.c_str(), endDate.c_str(), false);
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to consult NFSe by period: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::CancelNFSe(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string number = info[0].As<Napi::String>();
    std::string reason = info[1].As<Napi::String>();
    std::string verificationCode = info[2].As<Napi::String>();

    int ret = NFSe_CancelarNFSe(number.c_str(), reason.c_str(), verificationCode.c_str());
    if (ret < 0) {
        std::string error = CheckResult(ret);
        Napi::Error::New(env, "Failed to cancel NFSe: " + error).ThrowAsJavaScriptException();
        return env.Null();
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::GetLastResponse(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    char buffer[8192];
    int bufferLen = sizeof(buffer);

    if (NFSe_UltimoRetorno(buffer, bufferLen) < 0) {
        return env.Null();
    }

    return Napi::String::New(env, buffer);
}

// Configuration methods
Napi::Value NFSeAdapter::SetMunicipality(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected for municipality code").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string code = info[0].As<Napi::String>();
    // Update municipality code in config
    // This would typically involve updating the INI file or using a specific ACBr function

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::SetProvider(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
        Napi::TypeError::New(env, "Object expected for provider data").ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object providerData = info[0].As<Napi::Object>();
    // Update provider settings in config
    // This would typically involve updating the INI file or using a specific ACBr function

    return Napi::Boolean::New(env, true);
}

Napi::Value NFSeAdapter::SetEnvironment(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Number expected for environment").ThrowAsJavaScriptException();
        return env.Null();
    }

    int environment = info[0].As<Napi::Number>();
    // Update environment setting in config
    // This would typically involve updating the INI file or using a specific ACBr function

    return Napi::Boolean::New(env, true);
} 