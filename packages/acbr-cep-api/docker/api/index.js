const { initializeCEP, consultCep, finalizeCEP } = require('@acbr/cep-addon');
const path = require('path');

class ACBrCEP {
  constructor(configPath = '') {
    this.initialized = false;
    this.configPath = configPath || path.join(process.cwd(), 'acbr.ini');
  }

  initialize(chaveCrypt = '') {
    if (this.initialized) {
      return;
    }
    initializeCEP(this.configPath, chaveCrypt);
    this.initialized = true;
  }

  consultarCEP(cep) {
    if (!this.initialized) {
      this.initialize();
    }

    try {
      const result = consultCep(cep);
      return JSON.parse(result);
    } catch (error) {
      throw new Error(`Erro ao consultar CEP: ${error.message}`);
    }
  }

  finalizar() {
    if (this.initialized) {
      finalizeCEP();
      this.initialized = false;
    }
  }
}

module.exports = ACBrCEP; 