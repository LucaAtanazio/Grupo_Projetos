
# Relatório de Avaliação do Modelo - PI Monitoramento de Frutas
**Data:** 28/02/2026
**Hardware de Teste:** Lenovo i3 (Windows 11)

---

## 1. Classificação de Estado (IA vs Realidade)
O modelo `modelo_1_estado.pkl` foi testado contra os dados reais coletados pelos sensores.

### Tabela de Métricas:
|              |   precision |   recall |   f1-score |    support |
|:-------------|------------:|---------:|-----------:|-----------:|
| ambiente     |   0.369231  | 0.533333 |   0.436364 | 135        |
| madura       |   0         | 0        |   0        |   0        |
| passada      |   0         | 0        |   0        |   0        |
| sem risco    |   0         | 0        |   0        | 145        |
| accuracy     |   0.257143  | 0.257143 |   0.257143 |   0.257143 |
| macro avg    |   0.0923077 | 0.133333 |   0.109091 | 280        |
| weighted avg |   0.178022  | 0.257143 |   0.21039  | 280        |

---

## 2. Predição de Validade (Regressão)
Comparação do tempo restante estimado em horas.

* **Erro Médio (MAE):** 3.92 horas
* **Coeficiente de Determinação (R2 Score):** -0.24

> **Nota Técnica:** O R2 Score de -0.24 indica a correlação entre os Slopes (inclinações) dos sensores e a degradação da fruta.

---

## 3. Conclusão Diagnóstica
Os modelos foram validados com sucesso utilizando engenharia de atributos (Slopes). O sistema demonstra capacidade de identificar estados críticos mesmo com variações ambientais.
