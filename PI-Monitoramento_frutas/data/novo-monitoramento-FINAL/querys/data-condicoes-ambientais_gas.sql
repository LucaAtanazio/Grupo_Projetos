SELECT 
    estado_real,
    ROUND(AVG(temperatura), 2) AS media_temp,
    ROUND(STDDEV(temperatura), 2) AS desvio_temp,
    ROUND(AVG(umidade_ar), 2) AS media_umidade,
    ROUND(AVG(mq3_raw), 0) AS media_gas_raw,
    ROUND(STDDEV(mq3_raw), 0) AS desvio_gas
FROM monitoramento_frutas
GROUP BY estado_real
ORDER BY media_gas_raw ASC;