# @acbr/cep-addon

Node.js addon nativo para ACBrLibCEP - Consulta de CEP usando a biblioteca ACBr.

## Instalação

```bash
npm install @acbr/cep-addon
```

## Requisitos

- Node.js ≥ 20.0.0
- Sistema Operacional:
  - Windows 64-bit
  - Linux 64-bit

### Dependências do Sistema

#### Windows
- Visual C++ Redistributable 2015-2022

#### Linux
- GTK2
- OpenSSL
- Outras dependências listadas no Dockerfile

## Uso

```javascript
const { initializeCEP, consultCep, finalizeCEP } = require('@acbr/cep-addon');

// Inicializar a biblioteca
const configPath = 'caminho/para/acbr.ini';
initializeCEP(configPath, '');

try {
  // Consultar CEP
  const cep = '01001000';
  const enderecoJson = consultCep(cep);
  console.log('Endereço:', JSON.parse(enderecoJson));
} finally {
  // Finalizar a biblioteca
  finalizeCEP();
}
```

## API

### initializeCEP(configPath?: string, chaveCrypt?: string) → string

Inicializa a biblioteca ACBrLibCEP.

- `configPath`: Caminho para o arquivo de configuração (opcional)
- `chaveCrypt`: Chave de criptografia (opcional)
- Retorna: "OK" em caso de sucesso
- Lança exceção em caso de erro

### consultCep(cep: string) → string

Consulta um CEP e retorna os dados do endereço.

- `cep`: CEP a ser consultado (apenas números)
- Retorna: String JSON com os dados do endereço
- Lança exceção em caso de erro

### finalizeCEP() → string

Finaliza a biblioteca ACBrLibCEP.

- Retorna: "OK" em caso de sucesso
- Lança exceção em caso de erro

## Configuração

O arquivo `acbr.ini` deve estar acessível e configurado corretamente. Exemplo de configuração:

```ini
[CEP]
ArquivoLog=logs/acbr_cep.log
LogNivel=4
SSLType=1

[SSL]
SSLType=1
HttpLib=OpenSSL
```

## Estrutura de Arquivos

O addon espera encontrar as bibliotecas nativas nos seguintes caminhos:

- Windows: `./lib/Windows/ACBrCEP64.dll`
- Linux: `./lib/Linux/libacbrcep64.so`

Certifique-se de que a biblioteca correspondente ao seu sistema operacional está presente no local correto. 