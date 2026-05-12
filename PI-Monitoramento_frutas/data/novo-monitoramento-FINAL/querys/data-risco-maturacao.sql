SELECT
    tipo_fruta,
    mq3_raw,
    temperatura,
    umidade_ar,
    validade,
    estado_real,
    estado_previsto,
    data_registro
FROM monitoramento_frutas
WHERE estado_real = 'Risco de Perda'
ORDER BY mq3_raw DESC
LIMIT 10;