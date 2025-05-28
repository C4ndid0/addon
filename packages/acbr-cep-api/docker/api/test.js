// test_cep.js
const ACBrCEP = require('./index');
const fs = require('fs');
const path = require('path');
const https = require('https');

async function testSSLConnection() {
  return new Promise((resolve, reject) => {
    const options = {
      hostname: 'viacep.com.br',
      port: 443,
      path: '/ws/01001000/json/',
      method: 'GET',
      rejectUnauthorized: true,
      ca: fs.readFileSync('/etc/ssl/certs/ca-certificates.crt')
    };

    const req = https.request(options, (res) => {
      console.log('SSL Test Response:', {
        statusCode: res.statusCode,
        headers: res.headers
      });
      resolve(true);
    });

    req.on('error', (e) => {
      console.error('SSL Test Error:', e);
      reject(e);
    });

    req.end();
  });
}

async function testCEP() {
  const cep = new ACBrCEP();

  try {
    // Verificar ambiente
    console.log('Verificando ambiente...');
    console.log('Diretório atual:', process.cwd());
    console.log('Conteúdo do diretório lib/Linux:');
    console.log(fs.readdirSync(path.join(process.cwd(), 'lib/Linux')));
    console.log('Variáveis de ambiente SSL:');
    Object.keys(process.env).filter(key => key.includes('SSL')).forEach(key => {
      console.log(`${key}=${process.env[key]}`);
    });

    // Testar conexão SSL
    console.log('\nTestando conexão SSL direta...');
    await testSSLConnection();
    console.log('Teste de conexão SSL bem sucedido!');

    // Inicialização
    console.log('\nInicializando ACBrLibCEP...');
    cep.initialize();
    console.log('ACBrLibCEP inicializada com sucesso!');

    // Teste com CEP válido
    const validCep = '01001000';
    console.log(`\nTestando CEP válido: ${validCep}`);
    const endereco = cep.consultarCEP(validCep);
    console.log('Resposta:', endereco);

    // Teste com CEP inválido
    const invalidCep = '00000000';
    console.log(`\nTestando CEP inválido: ${invalidCep}`);
    try {
      cep.consultarCEP(invalidCep);
    } catch (error) {
      console.log('Erro esperado:', error.message);
    }

  } catch (error) {
    console.error('Erro:', error);
    console.error('Stack:', error.stack);
  } finally {
    console.log('\nFinalizando ACBrLibCEP...');
    try {
      cep.finalizar();
      console.log('ACBrLibCEP finalizada!');
    } catch (error) {
      console.error('Erro ao finalizar:', error);
    }
  }
}

// Configurar ambiente
process.env.OPENSSL_CONF = '/etc/ssl/openssl.cnf';
process.env.SSL_CERT_FILE = '/etc/ssl/certs/ca-certificates.crt';
process.env.SSL_CERT_DIR = '/etc/ssl/certs';
process.env.LD_LIBRARY_PATH = path.join(process.cwd(), 'lib/Linux') + 
  (process.env.LD_LIBRARY_PATH ? `:${process.env.LD_LIBRARY_PATH}` : '');

testCEP();
