#ifndef CEP_ADAPTER_H
#define CEP_ADAPTER_H

#include <string>

/**
 * @brief Inicializa a biblioteca ACBrLibCEP
 * @param arqConfig Caminho do arquivo de configuração (opcional)
 * @param chaveCrypt Chave de criptografia (opcional)
 * @return 0 em caso de sucesso, código de erro negativo em caso de falha
 * @throws std::runtime_error se a biblioteca não puder ser carregada
 */
int cepInicializar(const std::string& arqConfig = "", const std::string& chaveCrypt = "");

/**
 * @brief Finaliza a biblioteca ACBrLibCEP
 * @return 0 em caso de sucesso, código de erro negativo em caso de falha
 */
int cepFinalizar();

/**
 * @brief Consulta um CEP e retorna os dados do endereço
 * @param cep CEP a ser consultado (apenas números)
 * @return String JSON com os dados do endereço
 * @throws std::runtime_error em caso de erro na consulta
 */
std::string cepBuscarPorCEP(const std::string& cep);

/**
 * @brief Obtém a última mensagem de erro da biblioteca
 * @return String contendo a mensagem de erro
 */
std::string cepUltimoRetorno();

#endif // CEP_ADAPTER_H
