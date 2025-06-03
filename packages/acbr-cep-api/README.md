# Consulta de CEP com Node.js e ACBrLibCEP

Este projeto implementa uma solução para consulta de CEP utilizando Node.js e ACBrLibCEP, com suporte a ambiente Linux através de containerização Docker.


## Instalação

1. Clone o repositório:
```bash
git clone <seu-repositorio>
cd <seu-repositorio>
```

2. Construa a imagem Docker:
```bash
docker build -t acbr-cep-test .
```

## Uso

### Com Docker

Execute o container:
```bash
docker run --network host acbr-cep-test
```

### Desenvolvimento Local

1. Instale as dependências:
```bash
npm install
```

2. Execute os testes:
```bash
node test.js
```

## Estrutura do Projeto

- `test.js`: Implementação Node.js da consulta de CEP
- `cep_adapter.cpp`: Addon nativo para integração com ACBrLibCEP
- `acbr.ini`: Arquivo de configuração da ACBrLibCEP
- `Dockerfile`: Configuração do container Docker

## Configuração

### Arquivo acbr.ini

O arquivo `acbr.ini` contém as configurações da ACBrLibCEP. Principais seções:

```ini
[CEP]
ArquivoLog=logs/acbr_cep.log
LogNivel=4
SSLType=1
...

[SSL]
SSLType=1
HttpLib=OpenSSL
...
```

## Funcionalidades

- Consulta de CEP individual
- Validação de CEPs
- Tratamento de erros
- Logs detalhados
- Suporte a SSL/TLS

## Logs

Os logs são armazenados em:
- `logs/acbr_cep.log`: Logs da ACBrLibCEP
- `logs/debug.log`: Logs de debug da aplicação

## Troubleshooting

### Problemas Comuns

1. Erro de SSL:
   - Verifique se os certificados estão atualizados
   - Confirme as configurações SSL no `acbr.ini`

2. Erro de conexão:
   - Verifique a conectividade com internet
   - Confirme as configurações de proxy (se houver)

## Contribuição

1. Faça um fork do projeto
2. Crie uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -am 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Crie um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.

## Suporte

Para suporte, abra uma issue no repositório ou contate a equipe de desenvolvimento. 
