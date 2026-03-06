# IoT de Baixo Custo para Redução de Perdas de FLV em Recipientes Dedicados
## Sistema de Monitoramento e Análise Preditiva de Amadurecimento de Frutos Climatéricos

## Visão Geral do Projeto

Este projeto implementa a arquitetura proposta no artigo científico desenvolvido na **Faculdade SENAI Fatesg**. O foco é mitigar a ineficiência operacional no setor de Hortifrúti (FLV), que registra perdas de **5,83%** (base 2024), através de um sistema ciber-físico que monitora a liberação de compostos voláteis e variáveis ambientais.

O sistema foca em **frutos climatéricos**, que apresentam pico respiratório e liberação acelerada de Etileno ($C_2H_4$) durante a maturação. O monitoramento contínuo permite identificar o ponto de transição entre o estado "Maduro" e "Risco" (degradação).

### Frutas de Estudo (Casos Climatéricos):
* **Banana** 
* **Tomate** 

## Metodologia e Tecnologias Aplicadas

### Camada de Aquisição (Edge Computing)
#### O nó sensor é baseado no microcontrolador ESP32, utilizando um protocolo de amostragem rigoroso para garantir a fidelidade do sinal analógico:

* **Processamento de Sinal:** Implementação de Filtro de Média Móvel Exponencial (EMA) para filtragem de ruído térmico e flutuações elétricas.

  * *Rigor Amostral*: O filtro mantém a flutuação do sinal bruto abaixo de 100 pontos ADC.

* **Sensores:** 
  * * **MQ-3**: Atua como proxy para detecção de gases voláteis de degradação.

  * **DHT22**: Monitoramento termohigrométrico (Pino 19).

* **Baseline Dinâmica**: Calibração baseada no tipo de fruta (ex: Tomate baseline ~1300 ADC).

### Camada de Inteligência (IA e Analytics)
#### Os dados são processados por modelos de Machine Learning treinados para predição de Shelf-life (Vida Útil).

* **Modelo de Regressão**: Random Forest Regressor para estimar o `tempo_restante` (MAE de 6.67 horas no modelo otimizado com 10 inputs).

* **Modelo de Classificação**: Random Forest Classifier para rotulagem automática entre: `Sem Risco`, `Madura`, `Alerta` e `Risco`.

### Stack Tecnológica
* **Linguagens**: `C++` (Arduino IDE), `Python 3.10`.

* **Frameworks**: `Flask` (Backend), `Scikit-Learn` (ML), `SocketIO` (Real-time).

* **Banco de Dados**: `MongoDB` (Persistência de séries temporais).

### Estrutura do Projeto

├──.venv/
├──.vscode/
├──data/
├──esp32/DADOS/
├──modelos/
├──static/
├──.env example
├──.gitignore
├──README.MD
├──app.py
├──config.h.example
├──machine.py
├──machinelearning.py
├──modelo_1_estado.pkl
├──modelo_2_tempo_restante.pkl
├──modelo_4_identificacao_fruta.pkl
├──modelo_extra_alerta.pkl
├──requirements.txt

## Instalação e Configuração
### Requisitos de Hardware
* **ESP32 DevKit V1**
* **Sensor MQ-3** (Aquecido previamente por 24h para estabilização)
* **Sensor DHT11**

### Configuração do Ambiente
* **Workstation**: Rodar o `app.py` e treinamento de modelos 

* **Variáveis de Ambiente**: Configure o arquivo `.env` com as credenciais do MongoDB e a `API_KEY` (deve coincidir com a definida no `config.h` do firmware).

## Resultados Obtidos
### Conforme documentado no relatório de validação e no artigo:

* A elevação de **4°C** na temperatura ambiente resultou em um aumento de **50%** na emissão de gases voláteis detectados.

* O sistema demonstrou capacidade de gerar alertas de "Venda Rápida" com antecedência mínima de 12 horas antes da perda total do produto.

## Contribuições

Este projeto foi desenvolvido pelos aludas do curso de *Inteligência Artificial* do 2° semestre na *SENAI FATESG* como trabalho de conclusão das disciplinas *(Internet das coisas / Projeto de Extensão Integrador)*.

* [@eopabro](https://github.com/eopabro) - Pablo Henrique: **Desenvolvimento e Implementação do Código Embarcado** (`monitoramentoHF.ino`), **Elaboração Conceitual do Projeto**, e **Pesquisa de Campo**.
* [@LucaAtanazio](https://github.com/LucaAtanazio) - Luca Atanazio: **Arquitetura e Apresentação do Projeto**, **Gerenciamento de Recursos e Aquisição de Hardware**, **Elaboração Conceitual do Projeto**, e **Pesquisa de Campo**.
---
### **Orientação Acadêmica**

O projeto foi concluído sob a direção dos professores:
* [Prof. Willgnner Ferreira Santos](https://github.com/Willgnner-Santos) *(Projeto de Extensão Integrador)*
* [Prof. Alisson Rodrigues](https://www.linkedin.com/in/alissonralves1/) *(Internet da Coisas)*

> **Licença:** MIT License [FACULDADE DE TEC SENAI DE DESENV.GERENCIAL / Uso Acadêmico]

