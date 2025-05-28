const { NFSeAdapter } = require('./build/Release/acbr_nfse.node');

class NFSe {
    constructor() {
        this._adapter = new NFSeAdapter();
    }

    initialize(configPath) {
        return this._adapter.initialize(configPath);
    }

    finalize() {
        return this._adapter.finalize();
    }

    loadConfig(configPath) {
        return this._adapter.loadConfig(configPath);
    }

    createRPS(rpsData) {
        return this._adapter.createRPS(rpsData);
    }

    addRPS(rpsData) {
        return this._adapter.addRPS(rpsData);
    }

    signRPS() {
        return this._adapter.signRPS();
    }

    validateRPS() {
        return this._adapter.validateRPS();
    }

    transmitRPS(batch = 1, print = false, sync = true, timeout = 30000) {
        return this._adapter.transmitRPS(batch, print, sync, timeout);
    }

    consultNFSeByRPS(rpsNumber, rpsSeries, rpsType) {
        return this._adapter.consultNFSeByRPS(rpsNumber, rpsSeries, rpsType);
    }

    consultNFSeByNumber(nfseNumber) {
        return this._adapter.consultNFSeByNumber(nfseNumber);
    }

    consultNFSeByPeriod(startDate, endDate) {
        return this._adapter.consultNFSeByPeriod(startDate, endDate);
    }

    cancelNFSe(nfseNumber, reason, verificationCode) {
        return this._adapter.cancelNFSe(nfseNumber, reason, verificationCode);
    }

    getLastResponse() {
        return this._adapter.getLastResponse();
    }

    setMunicipality(municipalityCode) {
        return this._adapter.setMunicipality(municipalityCode);
    }

    setProvider(providerData) {
        return this._adapter.setProvider(providerData);
    }

    setEnvironment(environment) {
        return this._adapter.setEnvironment(environment);
    }
}

module.exports = NFSe; 