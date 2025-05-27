// test_cep.js
const path = require('path');
const https = require('https');
const fs = require('fs');
const { initializeCEP, consultCep, finalizeCEP } = require('./build/Release/acbr_cep.node');

// Configurações HTTPS para ViaCEP
const VIACEP_CONFIG = {
  hostname: 'viacep.com.br',
  port: 443,
  method: 'GET',
  rejectUnauthorized: false,
  ca: fs.readFileSync('/etc/ssl/certs/ca-certificates.crt'),
  minVersion: 'TLSv1.2',
  maxVersion: 'TLSv1.2',
  ciphers: 'HIGH:!aNULL:!MD5'
};

async function consultViaCEP(cep) {
  return new Promise((resolve, reject) => {
    const options = {
      ...VIACEP_CONFIG,
      path: `/ws/${cep}/json/`
    };

    const req = https.request(options, (res) => {
      let data = '';
      res.on('data', chunk => data += chunk);
      res.on('end', () => {
        try {
          const jsonData = JSON.parse(data);
          if (jsonData.erro) {
            reject(new Error('CEP não encontrado'));
          } else {
            resolve(jsonData);
          }
        } catch (error) {
          reject(new Error(`Erro ao processar resposta do ViaCEP: ${error.message}`));
        }
      });
    });

    req.on('error', error => {
      reject(new Error(`Erro na requisição ao ViaCEP: ${error.message}`));
    });

    req.end();
  });
}

async function testCEP() {
  try {
    // Inicialização
    const configPath = path.join(process.cwd(), 'acbr.ini');
    console.log('Inicializando ACBrLibCEP com:', configPath);
    const initResult = initializeCEP(configPath, '');
    console.log('Status:', initResult);

    // Teste com CEP válido
    const validCep = '01001000';
    console.log(`\nTestando CEP válido: ${validCep}`);
    const response = await consultViaCEP(validCep);
    console.log('Resposta:', JSON.stringify(response, null, 2));
    
    const processedResult = consultCep(validCep, JSON.stringify(response));
    console.log('Resultado processado:', processedResult);

    // Teste com CEP inválido
    const invalidCep = '00000000';
    console.log(`\nTestando CEP inválido: ${invalidCep}`);
    try {
      await consultViaCEP(invalidCep);
    } catch (error) {
      console.log('Erro esperado:', error.message);
    }

  } catch (error) {
    console.error('Erro:', error.message);
  } finally {
    const finResult = finalizeCEP();
    console.log('\nFinalização:', finResult);
  }
}

testCEP();
