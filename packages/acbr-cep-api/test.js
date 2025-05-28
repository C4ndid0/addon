// test_cep.js
const ACBrCEP = require('./index');
const fs = require('fs');
const path = require('path');

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


testCEP();
