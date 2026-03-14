# Redução de custos hospitalares através da prevenção direcionada por dados 

</p>
<p align="center">
O projeto consiste em um ecossistema de monitoramento e predição baseado em IoT de baixo custo para antecipar surtos de dengue. Utilizando sensores ESP32 e modelos de Deep Learning (LSTM), o sistema identifica anomalias térmicas que precedem a proliferação do mosquito, permitindo uma antecipação de 14 dias em relação aos métodos clínicos tradicionais. Nesta nova fase, o foco expande-se para a viabilidade comercial B2G, convertendo inteligência de dados em economia real para o setor público através da gestão otimizada de recursos hospitalares e logística de agentes de saúde. 
</p>

# Objetivo geral

Desenvolver uma plataforma preditiva de governança em saúde que utilize inteligência artificial e IoT para reduzir custos hospitalares e otimizar a resposta do Estado a crises epidemiológicas.

# Objetivo Específicos

* **Implementação de Borda:** Consolidar a rede de sensores ESP32 (R$ 8,50/unidade) para monitoramento térmico contínuo.
* **Engenharia de Dados Híbrida:** Estruturar um pipeline que combine a velocidade do MongoDB para telemetria e a integridade do PostgreSQL para dados de auditoria e custos hospitalares.
* **Treinamento de Alta Performance:** Utilizar aceleração via CUDA para processamento paralelo de grandes volumes de dados históricos e em tempo real.
* **Visualização Estratégica:** Desenvolver dashboards em Power BI com mapas de calor e indicadores de ROI para apoio à decisão governamental.
* **Edge AI:** Implementar o modelo quantized LSTM diretamente no microcontrolador para alertas locais.

# Justificativa

Os sistemas atuais são reativos, gerando atrasos de cerca de 14 dias nas notificações. Nossa solução ataca a raiz ambiental do problema com um custo 65% menor que alternativas de mercado. Ao prever o surto com F1-score de 0,82, o governo deixa de gastar com internações emergenciais e passa a investir na prevenção precisa, alocando insumos e equipes de forma cirúrgica, o que garante a sustentabilidade financeira do SUS e a eficiência no modelo B2G

# Fundamentação Teórica

O projeto fundamenta-se na correlação entre variações térmicas e o ciclo biológico do Aedes aegypti. Utilizamos o modelo LSTM (Long Short-Term Memory) por sua capacidade superior em capturar dependências temporais em séries sazonais, superando métodos clássicos como ARIMA em 21%. A detecção de outliers é garantida pelo algoritmo Isolation Forest. Academicamente, o trabalho integra conceitos de Computação Paralela (treinamento via GPU), Data Mining (análise de grafos para propagação entre bairros) e Ética em IA (conformidade com a Resolução CNS 585/2022 e LGPD).

# Metodologia

Para a coleta de dados, serão usados sensores DHT22 que realizam leituras a cada 5 minutos. Os dados são ingeridos via API Flask e armazenados no MongoDB. A mineração e treinamento teŕa uso de Python com bibliotecas aceleradas por CUDA para o modelo LSTM (2 camadas ocultas de 64 unidades). A expansão em Grafos teŕa aplicação de Graph Mining para modelar a conectividade entre bairros e prever a direção do contágio. Análise Financeira com cruzamento dos dados epidemiológicos com o custo de insumos hospitalares no PostgreSQL para gerar o cálculo de economia prevista.

# Entregáveis

* Modelo de IA: Script de treinamento otimizado para GPU e versão quantizada para borda
* Dashboard Executivo com painel em Power BI com indicadores de risco e alocação de recursos.
* Plano de Negócios B2G: Relatório técnico de viabilidade econômica e impacto social (ROI).

# Recursos Necessários

* Hardware:
  * Nós Sensores: Microcontroladores ESP32-WROOM-32, sensores de temperatura DHT22, baterias de Li-ion (2000 mAh) e cases para proteção ambiental.
* Software e Bibliotecas:
  * Linguagens: C++ (Firmware ESP32) e Python (IA e Backend).
  * IA/ML: TensorFlow/PyTorch (com suporte a CUDA), Scikit-learn (Isolation Forest) e bibliotecas para Quantization (Edge AI).
  * Bancos de Dados: MongoDB (Dados brutos de telemetria) e PostgreSQL (Dados estruturados de custos e auditoria).
  * Visualização: Power BI Desktop para criação de mapas de calor e indicadores de alocação de recursos.
* Infraestrutura de Rede: Broker MQTT para comunicação segura entre os nós e a API de ingestão Flask.

# Equipe

Luca Atanazio, Luciano Sales, Valdez, Santiago, Wassil

# Riscos e Desafios

A precisão de Hardware sofre o erro de $\pm0,5^{\circ}C$ do sensor DHT22 pode limitar a detecção de variações térmicas sutis em climas muito estáveis. Falsos Positivos e Ética, onde a taxa de 8,5% de falsos positivos (geralmente por ilhas de calor urbanas) exige um rigoroso protocolo de governança para evitar pânico desnecessário na população. Além disso, a conectividade tem ependência de redes estáveis para a transmissão em tempo real, desafio que pretendemos mitigar com a implementação de alertas via Edge AI local. O modelo treinado com dados de Goiânia pode precisar de recalibração para outras regiões devido a variações no comportamento do vetor Aedes aegypti.

# Considerações finais

O projeto do semestre passado evoluiu de uma prova de conceito acadêmica para uma solução robusta de inteligência governamental (B2G). Ao unir a capacidade preditiva do modelo LSTM (F1-score 0,82) com uma arquitetura de dados voltada para a gestão de custos hospitalares, entregamos uma ferramenta que antecipa surtos em 14 dias. A integração das disciplinas de Computação Paralela, Empreendedorismo, Data Mining e Ética garante que o sistema seja não apenas tecnicamente superior, mas também financeiramente viável e socialmente responsável, permitindo que o setor público atue de forma proativa na preservação de vidas e recursos.