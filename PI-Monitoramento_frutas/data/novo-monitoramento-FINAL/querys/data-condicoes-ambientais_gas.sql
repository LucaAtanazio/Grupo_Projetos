SELECT 
    estado_real, 
    ROUND(AVG(temperatura), 2) as media_temp, 
    ROUND(AVG(umidade_ar), 2) as media_umidade,
    ROUND(AVG(mq3_raw), 0) as media_gas_raw
FROM monitoramento_frutas
GROUP BY estado_real
ORDER BY media_gas_raw ASC;