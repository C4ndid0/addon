const NFSe = require('./');

async function testNFSe() {
    const nfse = new NFSe();

    try {
        // Initialize
        console.log('Initializing NFSe...');
        await nfse.initialize('acbr.ini');

        // Create RPS
        console.log('Creating RPS...');
        const rpsData = {
            xml: `<?xml version="1.0" encoding="UTF-8"?>
            <RPS>
                <InfRps>
                    <IdentificacaoRps>
                        <Numero>1</Numero>
                        <Serie>1</Serie>
                        <Tipo>1</Tipo>
                    </IdentificacaoRps>
                    <DataEmissao>2024-03-19</DataEmissao>
                    <NaturezaOperacao>1</NaturezaOperacao>
                    <RegimeEspecialTributacao>1</RegimeEspecialTributacao>
                    <OptanteSimplesNacional>1</OptanteSimplesNacional>
                    <IncentivadorCultural>2</IncentivadorCultural>
                    <Status>1</Status>
                    <Servico>
                        <Valores>
                            <ValorServicos>1000.00</ValorServicos>
                            <ValorDeducoes>0.00</ValorDeducoes>
                            <ValorPis>0.00</ValorPis>
                            <ValorCofins>0.00</ValorCofins>
                            <ValorInss>0.00</ValorInss>
                            <ValorIr>0.00</ValorIr>
                            <ValorCsll>0.00</ValorCsll>
                            <IssRetido>2</IssRetido>
                            <ValorIss>20.00</ValorIss>
                            <OutrasRetencoes>0.00</OutrasRetencoes>
                            <BaseCalculo>1000.00</BaseCalculo>
                            <Aliquota>0.02</Aliquota>
                            <ValorLiquidoNfse>980.00</ValorLiquidoNfse>
                        </Valores>
                        <ItemListaServico>1401</ItemListaServico>
                        <CodigoCnae>6201500</CodigoCnae>
                        <Discriminacao>Desenvolvimento de software</Discriminacao>
                        <CodigoMunicipio>3550308</CodigoMunicipio>
                    </Servico>
                    <Prestador>
                        <Cnpj>12345678000199</Cnpj>
                        <InscricaoMunicipal>12345</InscricaoMunicipal>
                    </Prestador>
                    <Tomador>
                        <IdentificacaoTomador>
                            <CpfCnpj>
                                <Cnpj>98765432000199</Cnpj>
                            </CpfCnpj>
                        </IdentificacaoTomador>
                        <RazaoSocial>Empresa Teste</RazaoSocial>
                        <Endereco>
                            <Endereco>Rua Teste</Endereco>
                            <Numero>123</Numero>
                            <Bairro>Centro</Bairro>
                            <CodigoMunicipio>3550308</CodigoMunicipio>
                            <Uf>SP</Uf>
                            <Cep>01001000</Cep>
                        </Endereco>
                        <Contato>
                            <Telefone>1199999999</Telefone>
                            <Email>teste@teste.com</Email>
                        </Contato>
                    </Tomador>
                </InfRps>
            </RPS>`
        };
        await nfse.createRPS(rpsData);

        // Sign RPS
        console.log('Signing RPS...');
        await nfse.signRPS();

        // Validate RPS
        console.log('Validating RPS...');
        await nfse.validateRPS();

        // Transmit RPS
        console.log('Transmitting RPS...');
        await nfse.transmitRPS(1, false, true, 30000);

        // Get response
        console.log('Getting response...');
        const response = await nfse.getLastResponse();
        console.log('Response:', response);

        // Consult NFSe by RPS
        console.log('Consulting NFSe by RPS...');
        await nfse.consultNFSeByRPS('1', '1', '1');
        const consultResponse = await nfse.getLastResponse();
        console.log('Consult Response:', consultResponse);

    } catch (error) {
        console.error('Error:', error);
    } finally {
        // Finalize
        console.log('Finalizing NFSe...');
        await nfse.finalize();
    }
}

testNFSe().catch(console.error); 