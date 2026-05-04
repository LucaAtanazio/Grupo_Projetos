SELECT tipo_fruta, mq3_tensao, estado_real, data_registro
FROM monitoramento_frutas
ORDER BY mq3_tensao DESC
LIMIT 10;